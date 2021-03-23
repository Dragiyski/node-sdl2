#include <node.h>

#include "initialization-and-shutdown.hpp"
#include "hints.hpp"
#include "window.hpp"

NODE_MODULE_INIT() {
    v8::HandleScope scope(context->GetIsolate());
    auto isolate = scope.GetIsolate();
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_INIT"));
        v8::Local<v8::Object> value = v8::Object::New(scope.GetIsolate(), v8::Null(scope.GetIsolate()), nullptr, nullptr, 0);
        JS_EXECUTE_IGNORE(NOTHING, node_sdl2::load_SDL2_INIT(context, value));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_WINDOW"));
        v8::Local<v8::Object> value = v8::Object::New(scope.GetIsolate(), v8::Null(scope.GetIsolate()), nullptr, nullptr, 0);
        JS_EXECUTE_IGNORE(NOTHING, node_sdl2::Window::load_SDL2_WINDOW(context, value));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_Init"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_Init, exports, 1, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_InitSubSystem"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_InitSubSystem, exports, 1, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_WasInit"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_WasInit, exports, 1, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_Quit"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_Quit, exports, 1, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_QuitSubSystem"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_QuitSubSystem, exports, 1, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_HINT_PRIORITY"));
        v8::Local<v8::Object> value = v8::Object::New(scope.GetIsolate(), v8::Null(scope.GetIsolate()), nullptr, nullptr, 0);
        JS_EXECUTE_IGNORE(NOTHING, node_sdl2::load_SDL_HINT_PRIORITY(context, value));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_SetHint"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_SetHint, exports, 2, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_SetHintWithPriority"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_SetHintWithPriority, exports, 3, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_GetHint"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_GetHint, exports, 1, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_GetHintBoolean"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_GetHintBoolean, exports, 1, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_SetHintCallback"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_SetHintCallback, exports, 2, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_RemoveHintCallback"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_RemoveHintCallback, exports, 2, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_ClearHints"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, value, v8::Function::New(context, node_sdl2::js_ClearHints, exports, 0, v8::ConstructorBehavior::kThrow));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_Window"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::FunctionTemplate, classTemplate, node_sdl2::Window::Template(scope.GetIsolate()));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, classFunction, classTemplate->GetFunction(context));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, classFunction, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
}