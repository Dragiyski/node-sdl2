#ifndef NODE_SDL2_INITIALIZATION_AND_SHUTDOWN_HPP
#define NODE_SDL2_INITIALIZATION_AND_SHUTDOWN_HPP

#include <node.h>
#include "js-helper.h"

namespace node_sdl2 {
    v8::Maybe<void> load_SDL2_INIT(v8::Local<v8::Context> context, v8::Local<v8::Object> target);
    v8::Maybe<void> export_SDL2_Init(v8::Local<v8::Context> context, v8::Local<v8::Object> exports);
    void js_Init(const v8::FunctionCallbackInfo<v8::Value> &info);
    void js_InitSubSystem(const v8::FunctionCallbackInfo<v8::Value> &info);
    void js_WasInit(const v8::FunctionCallbackInfo<v8::Value> &info);
    void js_Quit(const v8::FunctionCallbackInfo<v8::Value> &info);
    void js_QuitSubSystem(const v8::FunctionCallbackInfo<v8::Value> &info);
}

#endif //NODE_SDL2_INITIALIZATION_AND_SHUTDOWN_HPP
