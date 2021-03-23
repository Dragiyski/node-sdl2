#include "hints.hpp"
#include "js-helper.h"
#include "sdl-helper.h"
#include "string-helper.h"
#include <SDL.h>
#include <SDL_hints.h>
#include <map>
#include <mutex>
#include <v8-util.h>

namespace node_sdl2 {
    v8::Maybe<void> load_SDL_HINT_PRIORITY(v8::Local<v8::Context> context, v8::Local<v8::Object> target) {
        v8::HandleScope scope(context->GetIsolate());
        using const_string::operator""_const;
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_HINT", SDL_HINT_DEFAULT);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_HINT", SDL_HINT_NORMAL);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_HINT", SDL_HINT_OVERRIDE);
        return v8::JustVoid();
    }

    void js_SetHint(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        if (info.Length() < 2) {
            JS_THROW_INVALID_ARG_COUNT(info, 2);
        }
        if (!info[0]->IsString()) {
            JS_THROW_INVALID_ARG_TYPE(info, 0, "string");
        }
        if (!info[1]->IsString()) {
            JS_THROW_INVALID_ARG_TYPE(info, 1, "string");
        }
        v8::String::Utf8Value name(scope.GetIsolate(), info[0]);
        v8::String::Utf8Value value(scope.GetIsolate(), info[1]);
        SDL_ClearError();
        auto result = static_cast<bool>(SDL_SetHint(*name, *value));
        JS_SDL2_THROW_IF_ERROR(NOTHING, context);
        info.GetReturnValue().Set(result);
    }

    void js_SetHintWithPriority(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        if (info.Length() < 3) {
            JS_THROW_INVALID_ARG_COUNT(info, 3);
        }
        if (!info[0]->IsString()) {
            JS_THROW_INVALID_ARG_TYPE(info, 0, "string");
        }
        if (!info[1]->IsString()) {
            JS_THROW_INVALID_ARG_TYPE(info, 1, "string");
        }
        if (!info[2]->IsInt32()) {
            JS_THROW_INVALID_ARG_TYPE(info, 2, "int32");
        }
        v8::String::Utf8Value name(scope.GetIsolate(), info[0]);
        v8::String::Utf8Value value(scope.GetIsolate(), info[1]);
        JS_EXECUTE_RETURN(NOTHING, int32_t, priority, info[2]->Int32Value(context));
        SDL_ClearError();
        auto result = static_cast<bool>(SDL_SetHintWithPriority(*name, *value, static_cast<SDL_HintPriority>(priority)));
        JS_SDL2_THROW_IF_ERROR(NOTHING, context);
        info.GetReturnValue().Set(result);
    }

    void js_GetHint(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        if (info.Length() < 1) {
            JS_THROW_INVALID_ARG_COUNT(info, 1);
        }
        if (!info[0]->IsString()) {
            JS_THROW_INVALID_ARG_TYPE(info, 0, "string");
        }
        v8::String::Utf8Value name(scope.GetIsolate(), info[0]);
        SDL_ClearError();
        const char *result = SDL_GetHint(*name);
        JS_SDL2_THROW_IF_ERROR(NOTHING, context);
        if (result != nullptr) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, jsResult, ToString(context, result));
            info.GetReturnValue().Set(jsResult);
        }
    }

    void js_GetHintBoolean(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        if (info.Length() < 2) {
            JS_THROW_INVALID_ARG_COUNT(info, 2);
        }
        if (!info[0]->IsString()) {
            JS_THROW_INVALID_ARG_TYPE(info, 0, "string");
        }
        if (!info[1]->IsBoolean()) {
            JS_THROW_INVALID_ARG_TYPE(info, 1, "boolean");
        }
        v8::String::Utf8Value name(scope.GetIsolate(), info[0]);
        auto defaultValue = static_cast<SDL_bool>(info[1]->BooleanValue(scope.GetIsolate()));
        SDL_ClearError();
        auto result = static_cast<bool>(SDL_GetHintBoolean(*name, defaultValue));
        JS_SDL2_THROW_IF_ERROR(NOTHING, context);
        info.GetReturnValue().Set(result);
    }

    // Hints and callbacks...
    // Unfortunately, SDL has SDL_ClearHints, which clear the hints;
    // Therefore we cannot have authoritative point in the callback;
    // The only useful non-authoritative pointer in the callback is the v8::Isolate *
    // so we create a map from the isolate to the specified function.
    // Note: this will allow only a zero or one function per isolate, handling
    // multiple callbacks must be handled in JavaScript.
    namespace {
        std::map<v8::Isolate *, std::unique_ptr<v8::StdGlobalValueMap<std::string, v8::Function>>> isolate_callback_map;
        std::recursive_mutex isolate_callback_map_mutex;

        void js_boundMicrotask(const v8::FunctionCallbackInfo<v8::Value> &info) {
            v8::HandleScope scope(info.GetIsolate());
            v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
            v8::Local<v8::Object> container = info.Data().As<v8::Object>();
            v8::Local<v8::Function> callback;
            v8::Local<v8::Value> arguments[2];
            {
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, "function"));
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Value, value, container->Get(context, name));
                callback = value.As<v8::Function>();
            }
            {
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, "arguments"));
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Value, value, container->Get(context, name));
                v8::Local<v8::Array> args = value.As<v8::Array>();
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Value, arg0, args->Get(context, 0));
                arguments[0] = arg0;
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Value, arg1, args->Get(context, 1));
                arguments[1] = arg1;
            }
            JS_EXECUTE_IGNORE_HANDLE(NOTHING, callback->Call(callback->CreationContext(), v8::Undefined(scope.GetIsolate()), 2, arguments));
        }

        void OnHintChange(void *userdata, const char *name, const char *oldValue, const char *newValue) {
            auto isolate = reinterpret_cast<v8::Isolate *>(userdata);
            if (isolate->IsDead() || isolate->IsExecutionTerminating()) {
                {
                    std::scoped_lock lockMap(isolate_callback_map_mutex);
                    auto it = isolate_callback_map.find(isolate);
                    if (V8_LIKELY(it != isolate_callback_map.end())) {
                        it->second->Clear();
                        isolate_callback_map.erase(it);
                    }
                }
                // Unfortunately, v8::StdGlobalValueMap is not iterable (yet)
                // So we remove only the current hint callback (further calls will remove the rest).
                SDL_DelHintCallback(name, OnHintChange, userdata);
                return;
            }
            v8::Locker lock(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope scope(isolate);
            v8::Local<v8::Function> callback;
            {
                std::scoped_lock lockMap(isolate_callback_map_mutex);
                auto it = isolate_callback_map.find(isolate);
                if (V8_LIKELY(it != isolate_callback_map.end())) {
                    std::string stringName(name);
                    if (it->second->Contains(stringName)) {
                        callback = it->second->Get(stringName);
                    } else if (it->second->Size() == 0) {
                        isolate_callback_map.erase(it);
                    }
                }
            }
            if (V8_UNLIKELY(callback.IsEmpty())) {
                SDL_DelHintCallback(name, OnHintChange, userdata);
                return;
            }
            v8::Local<v8::Value> args[2];
            v8::Local<v8::Context> context = callback->CreationContext();
            if (oldValue != nullptr) {
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, value, ToString(context, oldValue));
                args[0] = value;
            } else {
                args[0] = v8::Null(isolate);
            }
            if (newValue != nullptr) {
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, value, ToString(context, newValue));
                args[1] = value;
            } else {
                args[1] = v8::Null(isolate);
            }
            // We should not call the JS from here, this thread is not suitable for the task.
            // Instead we should schedule a microtask. Unfortunately, there is no way to add microtask with arguments (for the moment).
            // Instead we create "bound" function (there is not v8::Function::Bind...) to call the callback.
            v8::Local<v8::Object> container = v8::Object::New(isolate);
            v8::Local<v8::Array> arguments = v8::Array::New(isolate, args, 2);
            {
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, "function"));
                JS_EXECUTE_IGNORE(NOTHING, container->Set(context, name, callback));
            }
            {
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, "arguments"));
                JS_EXECUTE_IGNORE(NOTHING, container->Set(context, name, arguments));
            }
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, bound, v8::Function::New(context, js_boundMicrotask, container, 0, v8::ConstructorBehavior::kThrow));
            isolate->EnqueueMicrotask(bound);
        }
    }

    void js_SetHintCallback(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        if (info.Length() < 2) {
            JS_THROW_INVALID_ARG_COUNT(info, 2);
        }
        if (!info[0]->IsString()) {
            JS_THROW_INVALID_ARG_TYPE(info, 0, "string");
        }
        if (!info[1]->IsFunction()) {
            JS_THROW_INVALID_ARG_TYPE(info, 1, "function");
        }
        v8::String::Utf8Value name(scope.GetIsolate(), info[0]);
        v8::Local<v8::Function> callback = info[1].As<v8::Function>();
        v8::Isolate *isolate = scope.GetIsolate();
        v8::Local<v8::Function> previous;
        {
            std::scoped_lock lockMap(isolate_callback_map_mutex);
            auto it = isolate_callback_map.find(isolate);
            if (it == isolate_callback_map.end()) {
                it = isolate_callback_map.insert(std::make_pair(isolate, std::make_unique<decltype(isolate_callback_map)::mapped_type::element_type>(isolate))).first;
            }
            v8::Global<v8::Function> globalPrevious = it->second->Set(*name, callback);
            if (!globalPrevious.IsEmpty()) {
                previous = v8::Local<v8::Function>::New(isolate, globalPrevious);
            }
        }
        if (previous.IsEmpty()) {
            SDL_ClearError();
            SDL_AddHintCallback(*name, OnHintChange, isolate);
            JS_SDL2_THROW_IF_ERROR(NOTHING, context);
        } else {
            info.GetReturnValue().Set(previous);
        }
    }

    void js_RemoveHintCallback(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        if (info.Length() < 1) {
            JS_THROW_INVALID_ARG_COUNT(info, 1);
        }
        if (!info[0]->IsString()) {
            JS_THROW_INVALID_ARG_TYPE(info, 0, "string");
        }
        v8::String::Utf8Value name(scope.GetIsolate(), info[0]);
        v8::Isolate *isolate = scope.GetIsolate();
        v8::Local<v8::Function> previous;
        {
            std::scoped_lock lockMap(isolate_callback_map_mutex);
            auto it = isolate_callback_map.find(isolate);
            if (it != isolate_callback_map.end()) {
                v8::Global<v8::Function> globalPrevious = it->second->Remove(*name);
                if (!globalPrevious.IsEmpty()) {
                    previous = v8::Local<v8::Function>::New(isolate, globalPrevious);
                }
                if (it->second->Size() == 0) {
                    isolate_callback_map.erase(it);
                }
            }
        }
        SDL_ClearError();
        SDL_DelHintCallback(*name, OnHintChange, isolate);
        if (!previous.IsEmpty()) {
            info.GetReturnValue().Set(previous);
        }
    }

    void js_ClearHints(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        {
            std::scoped_lock lockMap(isolate_callback_map_mutex);
            isolate_callback_map.clear();
        }
        SDL_ClearHints();
    }
}
