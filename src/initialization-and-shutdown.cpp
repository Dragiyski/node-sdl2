#include "initialization-and-shutdown.hpp"
#include <SDL.h>
#include "string-helper.h"
#include "sdl-helper.h"
#include "js-helper.h"

namespace node_sdl2 {
    v8::Maybe<void> load_SDL2_INIT(v8::Local<v8::Context> context, v8::Local<v8::Object> target) {
        using const_string::operator""_const;
        v8::HandleScope scope(context->GetIsolate());
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_INIT", SDL_INIT_TIMER);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_INIT", SDL_INIT_AUDIO);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_INIT", SDL_INIT_VIDEO);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_INIT", SDL_INIT_JOYSTICK);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_INIT", SDL_INIT_HAPTIC);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_INIT", SDL_INIT_GAMECONTROLLER);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_INIT", SDL_INIT_EVENTS);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_INIT", SDL_INIT_SENSOR);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_INIT", SDL_INIT_EVERYTHING);
        return v8::JustVoid();
    }

    v8::Maybe<void> export_SDL2_Init(v8::Local<v8::Context> context, v8::Local<v8::Object> exports) {
        using const_string::operator""_const;
        v8::Isolate *isolate = context->GetIsolate();
        v8::HandleScope scope(isolate);
        JS_SDL2_EXPORT_FUNCTION(VOID_NOTHING, "js_", "SDL_", js_Init, 1);
        JS_SDL2_EXPORT_FUNCTION(VOID_NOTHING, "js_", "SDL_", js_InitSubSystem, 1);
        JS_SDL2_EXPORT_FUNCTION(VOID_NOTHING, "js_", "SDL_", js_Quit, 0);
        JS_SDL2_EXPORT_FUNCTION(VOID_NOTHING, "js_", "SDL_", js_QuitSubSystem, 1);
        JS_SDL2_EXPORT_FUNCTION(VOID_NOTHING, "js_", "SDL_", js_WasInit, 1);
        return v8::JustVoid();
    }

    void js_Init(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        if (info.Length() < 1) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToDetailString(context, "Expected 1 argument, ", info.Length(), " given"));
            v8::Local<v8::Value> error = v8::Exception::TypeError(message);
            scope.GetIsolate()->ThrowException(error);
            return;
        }
        v8::Local<v8::Value> valueFlags = info[0];
        if (!valueFlags->IsUint32()) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToDetailString(context, "Argument 1 must be 32-bit integer"));
            v8::Local<v8::Value> error = v8::Exception::TypeError(message);
            scope.GetIsolate()->ThrowException(error);
            return;
        }
        JS_EXECUTE_RETURN(NOTHING, uint32_t, flags, valueFlags.As<v8::Uint32>()->Uint32Value(context));
        int errorCode = SDL_Init(flags);
        if (errorCode != 0) {
            JS_SDL2_THROW_ERROR(NOTHING, context, errorCode);
        }
    }

    void js_InitSubSystem(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        if (info.Length() < 1) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToDetailString(context, "Expected 1 argument, ", info.Length(), " given"));
            v8::Local<v8::Value> error = v8::Exception::TypeError(message);
            scope.GetIsolate()->ThrowException(error);
            return;
        }
        v8::Local<v8::Value> valueFlags = info[0];
        if (!valueFlags->IsUint32()) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToDetailString(context, "Argument 1 must be 32-bit integer"));
            v8::Local<v8::Value> error = v8::Exception::TypeError(message);
            scope.GetIsolate()->ThrowException(error);
            return;
        }
        JS_EXECUTE_RETURN(NOTHING, uint32_t, flags, valueFlags.As<v8::Uint32>()->Uint32Value(context));
        int errorCode = SDL_InitSubSystem(flags);
        if (errorCode != 0) {
            JS_SDL2_THROW_ERROR(NOTHING, context, errorCode);
        }
    }

    void js_WasInit(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        if (info.Length() < 1) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToDetailString(context, "Expected 1 argument, ", info.Length(), " given"));
            v8::Local<v8::Value> error = v8::Exception::TypeError(message);
            scope.GetIsolate()->ThrowException(error);
            return;
        }
        v8::Local<v8::Value> valueFlags = info[0];
        if (!valueFlags->IsUint32()) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToDetailString(context, "Argument 1 must be 32-bit integer"));
            v8::Local<v8::Value> error = v8::Exception::TypeError(message);
            scope.GetIsolate()->ThrowException(error);
            return;
        }
        JS_EXECUTE_RETURN(NOTHING, uint32_t, flags, valueFlags.As<v8::Uint32>()->Uint32Value(context));
        uint32_t result = SDL_WasInit(flags);
        info.GetReturnValue().Set(result);
    }

    void js_Quit(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        SDL_Quit();
    }

    void js_QuitSubSystem(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        info.GetReturnValue().SetUndefined();
        if (info.Length() < 1) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToDetailString(context, "Expected 1 argument, ", info.Length(), " given"));
            v8::Local<v8::Value> error = v8::Exception::TypeError(message);
            scope.GetIsolate()->ThrowException(error);
            return;
        }
        v8::Local<v8::Value> valueFlags = info[0];
        if (!valueFlags->IsUint32()) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToDetailString(context, "Argument 1 must be 32-bit integer"));
            v8::Local<v8::Value> error = v8::Exception::TypeError(message);
            scope.GetIsolate()->ThrowException(error);
            return;
        }
        JS_EXECUTE_RETURN(NOTHING, uint32_t, flags, valueFlags.As<v8::Uint32>()->Uint32Value(context));
        SDL_QuitSubSystem(flags);
    }
}