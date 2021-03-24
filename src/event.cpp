#include "event.hpp"
#include "js-helper.h"

v8::MaybeLocal<v8::Object> internal_makeEventObject(v8::Local<v8::Context> context, SDL_Event *event) {
    v8::Isolate *isolate = context->GetIsolate();
    v8::HandleScope scope(isolate);
    v8::Local<v8::Object> jsEvent = v8::Object::New(isolate, v8::Null(isolate), nullptr, nullptr, 0);
    SDL_CommonEvent *commonEvent = reinterpret_cast<SDL_CommonEvent *>(event);
    JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "type", event->type, JS_PROPERTY_ATTRIBUTE_CONSTANT);
    JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "timestamp", commonEvent->timestamp, JS_PROPERTY_ATTRIBUTE_CONSTANT);

    switch (event->type) {
        case SDL_DISPLAYEVENT: {
            SDL_DisplayEvent *displayEvent = reinterpret_cast<SDL_DisplayEvent *>(event);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "display", displayEvent->display, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "event", displayEvent->event, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", displayEvent->padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", displayEvent->padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", displayEvent->padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "data1", displayEvent->data1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_WINDOWEVENT: {
            SDL_WindowEvent *windowEvent = reinterpret_cast<SDL_WindowEvent *>(event);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", windowEvent->windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "event", windowEvent->event, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", windowEvent->padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", windowEvent->padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", windowEvent->padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "data1", windowEvent->data1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "data2", windowEvent->data2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            SDL_KeyboardEvent *keyboardEvent = reinterpret_cast<SDL_KeyboardEvent *>(event);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", keyboardEvent->windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "state", keyboardEvent->state, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "repeat", keyboardEvent->repeat, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", keyboardEvent->padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", keyboardEvent->padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            v8::Local<v8::Object> jsKeySym = v8::Object::New(isolate, v8::Null(isolate), nullptr, nullptr, 0);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsKeySym, "scancode", keyboardEvent->keysym.scancode, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsKeySym, "sym", keyboardEvent->keysym.sym, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsKeySym, "mod", keyboardEvent->keysym.mod, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsKeySym, "unused", keyboardEvent->keysym.unused, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "keysym"));
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, jsKeySym, JS_PROPERTY_ATTRIBUTE_CONSTANT));
            }
            break;
        }
    }
    return jsEvent;
}
