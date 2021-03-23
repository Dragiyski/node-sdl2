#include "window.hpp"
#include "js-helper.h"
#include "sdl-helper.h"
#include "string-helper.h"

namespace node_sdl2 {

    std::map<v8::Isolate *, v8::Eternal<v8::FunctionTemplate>> Window::template_map;
    std::recursive_mutex Window::template_map_mutex;

    v8::Maybe<void> Window::load_SDL2_WINDOW(v8::Local<v8::Context> context, v8::Local<v8::Object> target) {
        using const_string::operator ""_const;
        v8::HandleScope scope(context->GetIsolate());
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_FULLSCREEN);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_FULLSCREEN_DESKTOP);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_OPENGL);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_VULKAN);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_SHOWN);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_HIDDEN);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_BORDERLESS);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_RESIZABLE);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_MINIMIZED);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_MAXIMIZED);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_INPUT_GRABBED);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_INPUT_FOCUS);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_MOUSE_FOCUS);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_FOREIGN);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_ALLOW_HIGHDPI);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_MOUSE_CAPTURE);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_ALWAYS_ON_TOP);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_SKIP_TASKBAR);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_UTILITY);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_TOOLTIP);
        JS_SDL2_DEFINE_ENUM_UNSIGNED(VOID_NOTHING, context, target, "SDL_WINDOW", SDL_WINDOW_POPUP_MENU);
        return v8::JustVoid();
    }

    v8::MaybeLocal<v8::FunctionTemplate> Window::Template(v8::Isolate *isolate) {
        v8::EscapableHandleScope scope(isolate);
        std::scoped_lock lock(template_map_mutex);

        auto template_iterator = template_map.find(isolate);
        if (template_iterator != template_map.end()) {
            return scope.Escape(template_iterator->second.Get(isolate));
        }

        if (!isolate->InContext()) {
            return JS_NOTHING(v8::FunctionTemplate);
        }

        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        v8::Local<v8::FunctionTemplate> constructor = v8::FunctionTemplate::New(isolate);
        JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::FunctionTemplate), v8::String, class_name, ToString(context, "SDL_Window"));
        constructor->SetClassName(class_name);
        constructor->InstanceTemplate()->SetInternalFieldCount(1);

        {
            JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::FunctionTemplate), v8::String, name, ToString(context, "createWindow"));
            v8::Local<v8::FunctionTemplate> method = v8::FunctionTemplate::New(isolate, CreateWindow, v8::Local<v8::Value>(), v8::Local<v8::Signature>(), 6, v8::ConstructorBehavior::kThrow);
            constructor->Set(name, method, static_cast<v8::PropertyAttribute>(v8::DontEnum | v8::DontDelete | v8::ReadOnly));
        }
        {
            JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::FunctionTemplate), v8::String, name, ToString(context, "deleteWindow"));
            v8::Local<v8::FunctionTemplate> method = v8::FunctionTemplate::New(isolate, DeleteWindow, v8::Local<v8::Value>(), v8::Local<v8::Signature>(), 1, v8::ConstructorBehavior::kThrow);
            constructor->Set(name, method, static_cast<v8::PropertyAttribute>(v8::DontEnum | v8::DontDelete | v8::ReadOnly));
        }
        {
            JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::FunctionTemplate), v8::String, name, ToString(context, "isWindow"));
            v8::Local<v8::FunctionTemplate> method = v8::FunctionTemplate::New(isolate, IsWindow, v8::Local<v8::Value>(), v8::Local<v8::Signature>(), 1, v8::ConstructorBehavior::kThrow);
            constructor->Set(name, method, static_cast<v8::PropertyAttribute>(v8::DontEnum | v8::DontDelete | v8::ReadOnly));
        }

        // TODO: Initialize the function and/or prototype templates here...

        template_map.insert(
            std::pair<
                v8::Isolate *,
                v8::Eternal<v8::FunctionTemplate>>(std::piecewise_construct, std::make_tuple(isolate), std::make_tuple(isolate, constructor)));
        return scope.Escape(constructor);
    }

    void Window::CreateWindow(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();
        if (info.Length() < 6) {
            JS_THROW_INVALID_ARG_COUNT(info, 6);
        }
        if (!info[0]->IsString()) {
            JS_THROW_INVALID_ARG_TYPE(info, 0, "string");
        }
        for (int i = 1; i < 5; ++i) {
            if (!info[i]->IsInt32()) {
                JS_THROW_INVALID_ARG_TYPE(info, i, "integer");
            }
        }
        if (!info[5]->IsUint32()) {
            JS_THROW_INVALID_ARG_TYPE(info, 5, "unsigned integer");
        }
        v8::String::Utf8Value title(scope.GetIsolate(), info[0]);
        JS_EXECUTE_RETURN(NOTHING, int, x, info[1]->Int32Value(context));
        JS_EXECUTE_RETURN(NOTHING, int, y, info[2]->Int32Value(context));
        JS_EXECUTE_RETURN(NOTHING, int, w, info[3]->Int32Value(context));
        JS_EXECUTE_RETURN(NOTHING, int, h, info[4]->Int32Value(context));
        JS_EXECUTE_RETURN(NOTHING, Uint32, flags, info[5]->Uint32Value(context));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Object, jsThis, info.This().As<v8::Function>()->NewInstance(context));
        SDL_ClearError();
        SDL_Window *sdlWindow = SDL_CreateWindow(*title, x, y, w, h, flags);
        auto error = SDL_GetError();
        JS_SDL2_THROW_IF_ERROR(NOTHING, context);
        Window *window = new Window(context, jsThis, sdlWindow);
        jsThis->SetAlignedPointerInInternalField(0, window);
        info.GetReturnValue().Set(jsThis);
    }

    Window::Window(v8::Local<v8::Context> context, v8::Local<v8::Object> jsThis, SDL_Window *sdlWindow) :
        m_this(context->GetIsolate(), jsThis),
        m_internal(sdlWindow) {
        m_this.SetWeak(this, GCFirstPass, v8::WeakCallbackType::kParameter);
    }

    Window::~Window() {
        if (m_internal != nullptr) {
            SDL_DestroyWindow(m_internal);
            m_internal = nullptr;
        }
    }

    void Window::GCFirstPass(const v8::WeakCallbackInfo<Window> &info) {
        info.GetParameter()->m_this.Reset();
        info.SetSecondPassCallback(GCSecondPass);
    }

    void Window::GCSecondPass(const v8::WeakCallbackInfo<Window> &info) {
        delete info.GetParameter();
    }

    void Window::DeleteWindow(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();

        if (info.Length() < 1) {
            JS_THROW_INVALID_ARG_COUNT(info, 1);
        }
        if (!info[0]->IsObject()) {
            JS_THROW_INVALID_ARG_TYPE(info, 0, "[object SDL_Window]");
        }
        v8::Local<v8::Object> jsThis = info[0].As<v8::Object>();
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::FunctionTemplate, constructorTemplate, Template(scope.GetIsolate()));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, constructor, constructorTemplate->GetFunction(context));
        JS_EXECUTE_RETURN(NOTHING, bool, isInstanceOf, jsThis->InstanceOf(context, constructor));
        if (!isInstanceOf) {
            JS_THROW_INVALID_ARG_TYPE(info, 0, "[object SDL_Window]");
        }
        while (jsThis->InternalFieldCount() < 1) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Value, valuePrototype, jsThis->GetPrototype());
            if (!valuePrototype->IsObject()) {
                JS_THROW_INVALID_ARG_TYPE(info, 0, "[object SDL_Window]");
            }
            jsThis = valuePrototype.As<v8::Object>();
        }
        Window *window = reinterpret_cast<Window *>(jsThis->GetAlignedPointerFromInternalField(0));
        if (window->m_internal != nullptr) {
            SDL_ClearError();
            SDL_DestroyWindow(window->m_internal);
            JS_SDL2_THROW_IF_ERROR(NOTHING, context);
            window->m_internal = nullptr;
        }
    }

    void Window::IsWindow(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();

        info.GetReturnValue().Set(false);
        if (info.Length() < 1) {
            return;
        }
        if (!info[0]->IsObject()) {
            return;
        }
        v8::Local<v8::Object> jsThis = info[0].As<v8::Object>();
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::FunctionTemplate, constructorTemplate, Template(scope.GetIsolate()));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, constructor, constructorTemplate->GetFunction(context));
        JS_EXECUTE_RETURN(NOTHING, bool, isInstanceOf, jsThis->InstanceOf(context, constructor));
        if (!isInstanceOf) {
            return;
        }
        while (jsThis->InternalFieldCount() < 1) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Value, valuePrototype, jsThis->GetPrototype());
            if (!valuePrototype->IsObject()) {
                return;
            }
            jsThis = valuePrototype.As<v8::Object>();
        }
        Window *window = reinterpret_cast<Window *>(jsThis->GetAlignedPointerFromInternalField(0));
        info.GetReturnValue().Set(window->m_internal != nullptr);
    }
}