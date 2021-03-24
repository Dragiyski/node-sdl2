#ifndef NODE_SDL2_EVENT_HPP
#define NODE_SDL2_EVENT_HPP

#include <v8.h>
#include <SDL_events.h>

namespace node_sdl2 {
    v8::MaybeLocal<v8::Object> internal_makeEventObject(v8::Local<v8::Context> context, SDL_Event *event);
}

#endif //NODE_SDL2_EVENT_HPP
