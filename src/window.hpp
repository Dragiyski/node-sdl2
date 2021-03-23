#ifndef NODE_SDL2_WINDOW_HPP
#define NODE_SDL2_WINDOW_HPP

#include <map>
#include <mutex>
#include <v8.h>
#include <SDL_video.h>

namespace node_sdl2 {

    class alignas(alignof(void*)) Window {
    public:
        static v8::MaybeLocal<v8::FunctionTemplate> Template(v8::Isolate *);
        static v8::Maybe<void> load_SDL2_WINDOW(v8::Local<v8::Context> context, v8::Local<v8::Object> target);

    private:
        static void CreateWindow(const v8::FunctionCallbackInfo<v8::Value> &info);
        static void DeleteWindow(const v8::FunctionCallbackInfo<v8::Value> &info);
        static void IsWindow(const v8::FunctionCallbackInfo<v8::Value> &info);
        static void GCFirstPass(const v8::WeakCallbackInfo<Window> &info);
        static void GCSecondPass(const v8::WeakCallbackInfo<Window> &info);

    private:
        Window(v8::Local<v8::Context> context, v8::Local<v8::Object> jsThis, SDL_Window *sdlWindow);
    public:
        virtual ~Window();

    private:
        static std::map<v8::Isolate *, v8::Eternal<v8::FunctionTemplate>> template_map;
        static std::recursive_mutex template_map_mutex;
        SDL_Window *m_internal;
        v8::Global<v8::Object> m_this;
    };

}


#endif //NODE_SDL2_WINDOW_HPP
