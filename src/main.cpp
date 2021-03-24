#include <node.h>

#include "initialization-and-shutdown.hpp"
#include "hints.hpp"
#include "window.hpp"

NODE_MODULE_INIT() {
    v8::HandleScope scope(context->GetIsolate());
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_INIT"));
        v8::Local<v8::Object> value = v8::Object::New(scope.GetIsolate(), v8::Null(scope.GetIsolate()), nullptr, nullptr, 0);
        JS_EXECUTE_IGNORE(NOTHING, node_sdl2::load_SDL2_INIT(context, value));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly)));
    }
    JS_EXECUTE_IGNORE(NOTHING, node_sdl2::export_SDL2_Init(context, exports));
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_HINT_PRIORITY"));
        v8::Local<v8::Object> value = v8::Object::New(scope.GetIsolate(), v8::Null(scope.GetIsolate()), nullptr, nullptr, 0);
        JS_EXECUTE_IGNORE(NOTHING, node_sdl2::load_SDL_HINT_PRIORITY(context, value));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly)));
    }
    JS_EXECUTE_IGNORE(NOTHING, node_sdl2::export_SDL2_Hint(context, exports));
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_WINDOW"));
        v8::Local<v8::Object> value = v8::Object::New(scope.GetIsolate(), v8::Null(scope.GetIsolate()), nullptr, nullptr, 0);
        JS_EXECUTE_IGNORE(NOTHING, node_sdl2::Window::load_SDL_WINDOW(context, value));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly)));
    }
    {
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::String, name, ToString(context, u8"SDL_Window"));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::FunctionTemplate, classTemplate, node_sdl2::Window::Template(scope.GetIsolate()));
        JS_EXECUTE_RETURN_HANDLE(NOTHING, v8::Function, classFunction, classTemplate->GetFunction(context));
        JS_EXECUTE_IGNORE(NOTHING, exports->DefineOwnProperty(context, name, classFunction, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly)));
    }
    JS_EXECUTE_IGNORE(NOTHING, node_sdl2::Window::export_SDL2_Window(context, exports));
}