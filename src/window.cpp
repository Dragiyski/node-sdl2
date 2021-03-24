#include "window.hpp"
#include "js-helper.h"
#include "sdl-helper.h"
#include "string-helper.h"

namespace node_sdl2 {

    std::map<v8::Isolate *, v8::Eternal<v8::FunctionTemplate>> Window::template_map;
    std::recursive_mutex Window::template_map_mutex;

    v8::Maybe<void> Window::load_SDL_WINDOW(v8::Local<v8::Context> context, v8::Local<v8::Object> target) {
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

    v8::Maybe<void> Window::export_SDL2_Window(v8::Local<v8::Context> context, v8::Local<v8::Object> exports) {
        using const_string::operator""_const;
        v8::Isolate *isolate = context->GetIsolate();
        v8::HandleScope scope(isolate);
        JS_SDL2_EXPORT_FUNCTION(VOID_NOTHING, "js_", "SDL_", js_CreateWindow, 6);
        JS_SDL2_EXPORT_FUNCTION(VOID_NOTHING, "js_", "SDL_", js_DeleteWindow, 1);
        JS_SDL2_EXPORT_FUNCTION(VOID_NOTHING, "js_", "SDL_", js_IsWindow, 1);
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
        v8::Local<v8::FunctionTemplate> constructor = v8::FunctionTemplate::New(isolate, js_Constructor);
        JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::FunctionTemplate), v8::String, class_name, ToString(context, "SDL_Window"));
        constructor->SetClassName(class_name);
        constructor->InstanceTemplate()->SetInternalFieldCount(1);

        template_map.insert(
            std::pair<
                v8::Isolate *,
                v8::Eternal<v8::FunctionTemplate>>(std::piecewise_construct, std::make_tuple(isolate), std::make_tuple(isolate, constructor)));
        return scope.Escape(constructor);
    }

    void Window::js_CreateWindow(const v8::FunctionCallbackInfo<v8::Value> &info) {
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
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::FunctionTemplate, constructorTemplate, Template(scope.GetIsolate()));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, constructor, constructorTemplate->GetFunction(context));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Object, jsWindow, constructor->NewInstance(context));
        SDL_ClearError();
        SDL_Window *sdlWindow = SDL_CreateWindow(*title, x, y, w, h, flags);
        auto error = SDL_GetError();
        JS_SDL2_THROW_IF_ERROR(NOTHING, context);
        Window *window = new Window(context, jsWindow, sdlWindow);
        jsWindow->SetAlignedPointerInInternalField(0, window);
        info.GetReturnValue().Set(jsWindow);
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

    void Window::js_DeleteWindow(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();

        if (info.Length() < 1) {
            JS_THROW_INVALID_ARG_COUNT(info, 1);
        }
        if (!info[0]->IsObject()) {
            JS_THROW_INVALID_ARG_TYPE(info, 0, "[object SDL_Window]");
        }
        v8::Local<v8::Object> jsThis = info[0].As<v8::Object>();
        if (jsThis->InternalFieldCount() < 1) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::FunctionTemplate, constructor, Template(scope.GetIsolate()));
            if (!constructor->HasInstance(jsThis)) {
                JS_THROW_INVALID_ARG_TYPE(info, 0, "[object SDL_Window]");
            } else {
                JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToDetailString(context, "Object ", jsThis, " is not valid SDL_Window"));
            }
        }
        Window *window = reinterpret_cast<Window *>(jsThis->GetAlignedPointerFromInternalField(0));
        if (window->m_internal != nullptr) {
            SDL_ClearError();
            SDL_DestroyWindow(window->m_internal);
            JS_SDL2_THROW_IF_ERROR(NOTHING, context);
            window->m_internal = nullptr;
        }
    }

    void Window::js_IsWindow(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());

        info.GetReturnValue().Set(false);
        if (info.Length() < 1) {
            return;
        }
        if (!info[0]->IsObject()) {
            return;
        }
        v8::Local<v8::Object> jsThis = info[0].As<v8::Object>();
        if(jsThis->InternalFieldCount() < 1) {
            return;
        }
        Window *window = reinterpret_cast<Window *>(jsThis->GetAlignedPointerFromInternalField(0));
        info.GetReturnValue().Set(window->m_internal != nullptr);
    }

    void Window::js_Constructor(const v8::FunctionCallbackInfo<v8::Value> &info) {
        v8::HandleScope scope(info.GetIsolate());
        v8::Local<v8::Context> context = scope.GetIsolate()->GetCurrentContext();

        if(!info.IsConstructCall()) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToString(context, "Class constructor SDL_Window cannot be invoked without 'new'"));
            scope.GetIsolate()->ThrowException(v8::Exception::TypeError(message));
            return;
        }

        if(info.Holder()->InternalFieldCount() < 1) {
            JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, message, ToString(context, "Illegal constructor"));
            scope.GetIsolate()->ThrowException(v8::Exception::TypeError(message));
            return;
        }

        Window *window = new Window(context, info.Holder(), nullptr);
        info.Holder()->SetAlignedPointerInInternalField(0, window);
        info.GetReturnValue().Set(info.This());
    }
}