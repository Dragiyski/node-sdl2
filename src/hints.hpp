#ifndef NODE_SDL2_HINTS_HPP
#define NODE_SDL2_HINTS_HPP

#include <v8.h>

namespace node_sdl2 {
    v8::Maybe<void> load_SDL_HINT_PRIORITY(v8::Local<v8::Context> context, v8::Local<v8::Object> target);
    v8::Maybe<void> export_SDL2_Hint(v8::Local<v8::Context> context, v8::Local<v8::Object> exports);
    void js_SetHint(const v8::FunctionCallbackInfo<v8::Value> &info);
    void js_SetHintWithPriority(const v8::FunctionCallbackInfo<v8::Value> &info);
    void js_GetHint(const v8::FunctionCallbackInfo<v8::Value> &info);
    void js_GetHintBoolean(const v8::FunctionCallbackInfo<v8::Value> &info);
    void js_SetHintCallback(const v8::FunctionCallbackInfo<v8::Value> &info);
    void js_RemoveHintCallback(const v8::FunctionCallbackInfo<v8::Value> &info);
    void js_ClearHints(const v8::FunctionCallbackInfo<v8::Value> &info);
}

#endif //NODE_SDL2_HINTS_HPP
