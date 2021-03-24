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
        case SDL_TEXTEDITING: {
            SDL_TextEditingEvent *textEditingEvent = reinterpret_cast<SDL_TextEditingEvent *>(event);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", textEditingEvent->windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                int length;
                for(length = 0; length < SDL_TEXTEDITINGEVENT_TEXT_SIZE && textEditingEvent->text[length] != 0; ++length);
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "text"))
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, value, v8::String::NewFromOneByte(isolate, reinterpret_cast<const uint8_t *>(textEditingEvent->text), v8::NewStringType::kNormal, length))
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, value, JS_PROPERTY_ATTRIBUTE_CONSTANT));
            }
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "start", textEditingEvent->start, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "length", textEditingEvent->length, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_TEXTINPUT: {
            SDL_TextInputEvent *textInputEvent = reinterpret_cast<SDL_TextInputEvent *>(event);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", textInputEvent->windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                int length;
                for(length = 0; length < SDL_TEXTINPUTEVENT_TEXT_SIZE && textInputEvent->text[length] != 0; ++length);
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "text"))
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, value, v8::String::NewFromOneByte(isolate, reinterpret_cast<const uint8_t *>(textEditingEvent->text), v8::NewStringType::kNormal, length))
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, value, JS_PROPERTY_ATTRIBUTE_CONSTANT));
            }
            break;
        }
        case SDL_MOUSEMOTION: {
            SDL_MouseMotionEvent *mouseMotionEvent = reinterpret_cast<SDL_MouseMotionEvent *>(event);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", mouseMotionEvent->windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", mouseMotionEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "state", mouseMotionEvent->state, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", mouseMotionEvent->x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", mouseMotionEvent->y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "xrel", mouseMotionEvent->xrel, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "yrel", mouseMotionEvent->yrel, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            SDL_MouseButtonEvent *mouseButtonEvent = reinterpret_cast<SDL_MouseButtonEvent *>(event);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", mouseButtonEvent->windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", mouseButtonEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "button", mouseButtonEvent->button, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "state", mouseButtonEvent->state, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "clicks", mouseButtonEvent->clicks, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", mouseButtonEvent->padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", mouseButtonEvent->x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", mouseButtonEvent->y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_MOUSEWHEEL: {
            SDL_MouseWheelEvent *mouseWheelEvent = reinterpret_cast<SDL_MouseWheelEvent *>(event);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", mouseWheelEvent->windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", mouseWheelEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", mouseWheelEvent->x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", mouseWheelEvent->y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "direction", mouseWheelEvent->direction, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_JOYAXISMOTION: {
            SDL_JoyAxisEvent *joyAxisEvent = reinterpret_cast<SDL_JoyAxisEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", joyAxisEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "axis", joyAxisEvent->axis, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", joyAxisEvent->padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", joyAxisEvent->padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", joyAxisEvent->padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "value", joyAxisEvent->value, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding4", joyAxisEvent->padding4, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_JOYBALLMOTION: {
            SDL_JoyBallEvent *joyBallEvent = reinterpret_cast<SDL_JoyBallEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", joyBallEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "ball", joyBallEvent->ball, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", joyBallEvent->padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", joyBallEvent->padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", joyBallEvent->padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "xrel", joyBallEvent->xrel, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "yrel", joyBallEvent->yrel, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_JOYHATMOTION: {
            SDL_JoyHatEvent *joyHatEvent = reinterpret_cast<SDL_JoyHatEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", joyHatEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "hat", joyHatEvent->hat, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "value", joyHatEvent->value, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", joyHatEvent->padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", joyHatEvent->padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP: {
            SDL_JoyButtonEvent *joyButtonEvent = reinterpret_cast<SDL_JoyButtonEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", joyButtonEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "button", joyButtonEvent->button, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "state", joyButtonEvent->state, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", joyButtonEvent->padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", joyButtonEvent->padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED: {
            SDL_JoyDeviceEvent *joyDeviceEvent = reinterpret_cast<SDL_JoyDeviceEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", joyDeviceEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_CONTROLLERAXISMOTION: {
            SDL_ControllerAxisEvent *sdlEvent = reinterpret_cast<SDL_ControllerAxisEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", sdlEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "axis", sdlEvent->axis, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", sdlEvent->padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", sdlEvent->padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", sdlEvent->padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "value", sdlEvent->value, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding4", sdlEvent->padding4, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP: {
            SDL_ControllerButtonEvent *sdlEvent = reinterpret_cast<SDL_ControllerButtonEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", sdlEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "button", sdlEvent->button, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "state", sdlEvent->state, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", sdlEvent->padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", sdlEvent->padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
        case SDL_CONTROLLERDEVICEREMAPPED: {
            SDL_ControllerDeviceEvent *sdlEvent = reinterpret_cast<SDL_ControllerDeviceEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", sdlEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_CONTROLLERTOUCHPADDOWN:
        case SDL_CONTROLLERTOUCHPADMOTION:
        case SDL_CONTROLLERTOUCHPADUP: {
            SDL_ControllerTouchpadEvent *sdlEvent = reinterpret_cast<SDL_ControllerTouchpadEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", sdlEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "touchpad", sdlEvent->touchpad, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "finger", sdlEvent->finger, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", sdlEvent->x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", sdlEvent->y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "pressure", sdlEvent->pressure, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_CONTROLLERSENSORUPDATE: {
            SDL_ControllerSensorEvent *sdlEvent = reinterpret_cast<SDL_ControllerSensorEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", sdlEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "sensor", sdlEvent->sensor, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                v8::Local<v8::Value> values[3];
                for(int i = 0; i < 3; ++i) {
                    values[i] = v8::Number::New(isolate, sdlEvent->data[i]);
                }
                v8::Local<v8::Array> value = v8::Array::New(isolate, values, 3);
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), value->SetIntegrityLevel(context, v8::IntegrityLevel::kFrozen));
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "data"));
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, value, JS_PROPERTY_ATTRIBUTE_CONSTANT));
            }
            break;
        }
        case SDL_AUDIODEVICEADDED:
        case SDL_AUDIODEVICEREMOVED: {
            SDL_AudioDeviceEvent *sdlEvent = reinterpret_cast<SDL_AudioDeviceEvent *>(event);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", sdlEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "iscapture", sdlEvent->iscapture, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", sdlEvent->padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", sdlEvent->padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", sdlEvent->padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_FINGERMOTION:
        case SDL_FINGERDOWN:
        case SDL_FINGERUP: {
            SDL_TouchFingerEvent *sdlEvent = reinterpret_cast<SDL_TouchFingerEvent *>(event);
            JS_DEFINE_INT64_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "touchId", sdlEvent->touchId, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT64_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "fingerId", sdlEvent->fingerId, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", sdlEvent->x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", sdlEvent->y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "dx", sdlEvent->dx, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "dy", sdlEvent->dy, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "pressure", sdlEvent->pressure, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", sdlEvent->windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_MULTIGESTURE: {
            SDL_MultiGestureEvent *sdlEvent = reinterpret_cast<SDL_MultiGestureEvent *>(event);
            JS_DEFINE_INT64_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "touchId", sdlEvent->touchId, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "dTheta", sdlEvent->dTheta, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "dDist", sdlEvent->dDist, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", sdlEvent->x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", sdlEvent->y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "numFingers", sdlEvent->numFingers, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding", sdlEvent->padding, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_DOLLARGESTURE:
        case SDL_DOLLARRECORD: {
            SDL_DollarGestureEvent *sdlEvent = reinterpret_cast<SDL_DollarGestureEvent *>(event);
            JS_DEFINE_INT64_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "touchId", sdlEvent->touchId, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT64_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "gestureId", sdlEvent->gestureId, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "numFingers", sdlEvent->numFingers, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "error", sdlEvent->error, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", sdlEvent->x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", sdlEvent->y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_DROPBEGIN:
        case SDL_DROPFILE:
        case SDL_DROPTEXT:
        case SDL_DROPCOMPLETE: {
            SDL_DropEvent *sdlEvent = reinterpret_cast<SDL_DropEvent *>(event);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", sdlEvent->windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "file"));
                if(sdlEvent->file == nullptr) {
                    JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, v8::Null(isolate), JS_PROPERTY_ATTRIBUTE_CONSTANT));
                } else {
                    JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, strFile, ToString(context, const_cast<const char*>(sdlEvent->file)));
                    JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, strFile, JS_PROPERTY_ATTRIBUTE_CONSTANT));
                }
            }
            break;
        }
        case SDL_SENSORUPDATE: {
            SDL_SensorEvent *sdlEvent = reinterpret_cast<SDL_SensorEvent *>(event);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", sdlEvent->which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                v8::Local<v8::Value> values[6];
                for(int i = 0; i < 6; ++i) {
                    values[i] = v8::Number::New(isolate, sdlEvent->data[i]);
                }
                v8::Local<v8::Array> value = v8::Array::New(isolate, values, 3);
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), value->SetIntegrityLevel(context, v8::IntegrityLevel::kFrozen));
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "data"));
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, value, JS_PROPERTY_ATTRIBUTE_CONSTANT));
            }
            break;
        }
    }
    return jsEvent;
}
