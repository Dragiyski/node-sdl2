#include "event.hpp"
#include "js-helper.h"

v8::MaybeLocal<v8::Object> internal_makeEventObject(v8::Local<v8::Context> context, SDL_Event *event) {
    v8::Isolate *isolate = context->GetIsolate();
    v8::EscapableHandleScope scope(isolate);
    v8::Local<v8::Object> jsEvent = v8::Object::New(isolate, v8::Null(isolate), nullptr, nullptr, 0);
    JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "type", event->type, JS_PROPERTY_ATTRIBUTE_CONSTANT);
    JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "timestamp", event->common.timestamp, JS_PROPERTY_ATTRIBUTE_CONSTANT);

    switch (event->type) {
        case SDL_DISPLAYEVENT: {
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "display", event->display.display, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "event", event->display.event, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", event->display.padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", event->display.padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", event->display.padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "data1", event->display.data1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_WINDOWEVENT: {
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", event->window.windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "event", event->window.event, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", event->window.padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", event->window.padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", event->window.padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "data1", event->window.data1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "data2", event->window.data2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", event->key.windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "state", event->key.state, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "repeat", event->key.repeat, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", event->key.padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", event->key.padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            v8::Local<v8::Object> jsKeySym = v8::Object::New(isolate, v8::Null(isolate), nullptr, nullptr, 0);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsKeySym, "scancode", event->key.keysym.scancode, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsKeySym, "sym", event->key.keysym.sym, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsKeySym, "mod", event->key.keysym.mod, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsKeySym, "unused", event->key.keysym.unused, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "keysym"));
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, jsKeySym, JS_PROPERTY_ATTRIBUTE_CONSTANT));
            }
            break;
        }
        case SDL_TEXTEDITING: {
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", event->edit.windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                int length;
                for (length = 0; length < SDL_TEXTEDITINGEVENT_TEXT_SIZE && event->edit.text[length] != 0; ++length) {}
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "text"))
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, value, v8::String::NewFromOneByte(isolate, reinterpret_cast<const uint8_t *>(event->edit.text), v8::NewStringType::kNormal, length))
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, value, JS_PROPERTY_ATTRIBUTE_CONSTANT));
            }
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "start", event->edit.start, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "length", event->edit.length, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_TEXTINPUT: {
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", event->text.windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                int length;
                for (length = 0; length < SDL_TEXTINPUTEVENT_TEXT_SIZE && event->text.text[length] != 0; ++length) {}
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "text"))
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, value, v8::String::NewFromOneByte(isolate, reinterpret_cast<const uint8_t *>(event->edit.text), v8::NewStringType::kNormal, length))
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, value, JS_PROPERTY_ATTRIBUTE_CONSTANT));
            }
            break;
        }
        case SDL_MOUSEMOTION: {
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", event->motion.windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->motion.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "state", event->motion.state, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", event->motion.x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", event->motion.y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "xrel", event->motion.xrel, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "yrel", event->motion.yrel, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", event->button.windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->button.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "button", event->button.button, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "state", event->button.state, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "clicks", event->button.clicks, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", event->button.padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", event->button.x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", event->button.y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_MOUSEWHEEL: {
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", event->wheel.windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->wheel.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", event->wheel.x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", event->wheel.y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "direction", event->wheel.direction, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_JOYAXISMOTION: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->jaxis.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "axis", event->jaxis.axis, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", event->jaxis.padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", event->jaxis.padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", event->jaxis.padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "value", event->jaxis.value, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding4", event->jaxis.padding4, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_JOYBALLMOTION: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->jball.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "ball", event->jball.ball, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", event->jball.padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", event->jball.padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", event->jball.padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "xrel", event->jball.xrel, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "yrel", event->jball.yrel, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_JOYHATMOTION: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->jhat.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "hat", event->jhat.hat, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "value", event->jhat.value, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", event->jhat.padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", event->jhat.padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->jbutton.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "button", event->jbutton.button, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "state", event->jbutton.state, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", event->jbutton.padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", event->jbutton.padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->jdevice.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_CONTROLLERAXISMOTION: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->caxis.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "axis", event->caxis.axis, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", event->caxis.padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", event->caxis.padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", event->caxis.padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "value", event->caxis.value, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding4", event->caxis.padding4, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->cbutton.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "button", event->cbutton.button, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "state", event->cbutton.state, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", event->cbutton.padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", event->cbutton.padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
        case SDL_CONTROLLERDEVICEREMAPPED: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->cdevice.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_CONTROLLERTOUCHPADDOWN:
        case SDL_CONTROLLERTOUCHPADMOTION:
        case SDL_CONTROLLERTOUCHPADUP: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->ctouchpad.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "touchpad", event->ctouchpad.touchpad, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "finger", event->ctouchpad.finger, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", event->ctouchpad.x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", event->ctouchpad.y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "pressure", event->ctouchpad.pressure, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_CONTROLLERSENSORUPDATE: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->csensor.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "sensor", event->csensor.sensor, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                v8::Local<v8::Value> values[3];
                for (int i = 0; i < 3; ++i) {
                    values[i] = v8::Number::New(isolate, event->csensor.data[i]);
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
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->adevice.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "iscapture", event->adevice.iscapture, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding1", event->adevice.padding1, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding2", event->adevice.padding2, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding3", event->adevice.padding3, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_FINGERMOTION:
        case SDL_FINGERDOWN:
        case SDL_FINGERUP: {
            JS_DEFINE_INT64_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "touchId", event->tfinger.touchId, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT64_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "fingerId", event->tfinger.fingerId, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", event->tfinger.x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", event->tfinger.y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "dx", event->tfinger.dx, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "dy", event->tfinger.dy, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "pressure", event->tfinger.pressure, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", event->tfinger.windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_MULTIGESTURE: {
            JS_DEFINE_INT64_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "touchId", event->mgesture.touchId, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "dTheta", event->mgesture.dTheta, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "dDist", event->mgesture.dDist, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", event->mgesture.x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", event->mgesture.y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "numFingers", event->mgesture.numFingers, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "padding", event->mgesture.padding, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_DOLLARGESTURE:
        case SDL_DOLLARRECORD: {
            JS_DEFINE_INT64_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "touchId", event->dgesture.touchId, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_INT64_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "gestureId", event->dgesture.gestureId, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "numFingers", event->dgesture.numFingers, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "error", event->dgesture.error, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "x", event->dgesture.x, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            JS_DEFINE_FLOAT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "y", event->dgesture.y, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            break;
        }
        case SDL_DROPBEGIN:
        case SDL_DROPFILE:
        case SDL_DROPTEXT:
        case SDL_DROPCOMPLETE: {
            JS_DEFINE_UINT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "windowID", event->drop.windowID, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "file"));
                if (event->drop.file == nullptr) {
                    JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, v8::Null(isolate), JS_PROPERTY_ATTRIBUTE_CONSTANT));
                } else {
                    JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, strFile, ToString(context, const_cast<const char *>(event->drop.file)));
                    JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, strFile, JS_PROPERTY_ATTRIBUTE_CONSTANT));
                }
            }
            break;
        }
        case SDL_SENSORUPDATE: {
            JS_DEFINE_INT_ATTR(JS_NOTHING(v8::Object), context, jsEvent, "which", event->sensor.which, JS_PROPERTY_ATTRIBUTE_CONSTANT);
            {
                v8::Local<v8::Value> values[6];
                for (int i = 0; i < 6; ++i) {
                    values[i] = v8::Number::New(isolate, event->sensor.data[i]);
                }
                v8::Local<v8::Array> value = v8::Array::New(isolate, values, 3);
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), value->SetIntegrityLevel(context, v8::IntegrityLevel::kFrozen));
                JS_EXECUTE_RETURN_HANDLE(JS_NOTHING(v8::Object), v8::String, name, ToString(context, "data"));
                JS_EXECUTE_IGNORE(JS_NOTHING(v8::Object), jsEvent->DefineOwnProperty(context, name, value, JS_PROPERTY_ATTRIBUTE_CONSTANT));
            }
            break;
        }
    }
    return scope.Escape(jsEvent);
}

v8::Maybe<SDL_Event> internal_readEventObject(v8::Local<v8::Context> context, v8::Local<v8::Object> jsEvent) {
    v8::Isolate *isolate = context->GetIsolate();
    v8::HandleScope scope(isolate);
    SDL_Event event;
    JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "type", event.type);
    JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "timestamp", event.common.timestamp);
    switch (event.type) {
        case SDL_DISPLAYEVENT: {
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "display", event.display.display);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "event", event.display.event);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding1", event.display.padding1);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding2", event.display.padding2);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding3", event.display.padding3);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "data1", event.display.data1);
            break;
        }
        case SDL_WINDOWEVENT: {
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "windowID", event.window.windowID);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "event", event.window.event);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding1", event.window.padding1);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding2", event.window.padding2);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding3", event.window.padding3);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "data1", event.window.data1);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "data2", event.window.data2);
            break;
        }
        case SDL_KEYDOWN:
        case SDL_KEYUP: {
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "windowID", event.key.windowID);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "state", event.key.state);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "repeat", event.key.repeat);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding2", event.key.padding2);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding3", event.key.padding3);
            {
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, name, ToString(context, "keysym"));
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::Value, jsKeySymValue, jsEvent->Get(context, name));
                if (!jsKeySymValue->IsObject()) {
                    JS_THROW_INVALID_PROPERTY_TYPE(CPP_NOTHING(SDL_Event), name.As<v8::Value>(), "object");
                }
                v8::Local<v8::Object> jsKeySym = jsKeySymValue.As<v8::Object>();
                {
                    JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, name, ToString(context, "scancode"));
                    JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::Value, value, jsEvent->Get(context, name));
                    if (!value->IsUint32()) {
                        JS_THROW_INVALID_PROPERTY_TYPE(CPP_NOTHING(SDL_Event), name.As<v8::Value>(), "uint32_t");
                    }
                    JS_EXECUTE_RETURN(CPP_NOTHING(SDL_Event), uint32_t, primitive, value->Uint32Value(context));
                    event.key.keysym.scancode = static_cast<SDL_Scancode>(primitive);
                }
                JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "sym", event.key.keysym.sym);
                JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "mod", event.key.keysym.mod);
                JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "unused", event.key.keysym.unused);
            }
            break;
        }
        case SDL_TEXTEDITING: {
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "windowID", event.edit.windowID);
            {
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, name, ToString(context, "text"));
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::Value, value, jsEvent->Get(context, name));
                if (!value->IsString()) {
                    JS_THROW_INVALID_PROPERTY_TYPE(CPP_NOTHING(SDL_Event), name.As<v8::Value>(), "string");
                }
                if (value.As<v8::String>()->Length() >= SDL_TEXTEDITINGEVENT_TEXT_SIZE) {
                    JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, message, ToString(context, "Exceeded maximum allowed SDL_TEXTEDITING text size: ", SDL_TEXTEDITINGEVENT_TEXT_SIZE));
                    isolate->ThrowException(v8::Exception::RangeError(message));
                    return v8::Nothing<SDL_Event>();
                }
                v8::String::Utf8Value text(isolate, value);
                SDL_strlcpy(event.edit.text, *text, SDL_TEXTEDITINGEVENT_TEXT_SIZE);
            }
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "start", event.edit.start);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "length", event.edit.length);
            break;
        }
        case SDL_TEXTINPUT: {
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "windowID", event.text.windowID);
            {
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, name, ToString(context, "text"));
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::Value, value, jsEvent->Get(context, name));
                if (!value->IsString()) {
                    JS_THROW_INVALID_PROPERTY_TYPE(CPP_NOTHING(SDL_Event), name.As<v8::Value>(), "string");
                }
                if (value.As<v8::String>()->Length() >= SDL_TEXTINPUTEVENT_TEXT_SIZE) {
                    JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, message, ToString(context, "Exceeded maximum allowed SDL_TEXTEDITING text size: ", SDL_TEXTEDITINGEVENT_TEXT_SIZE));
                    isolate->ThrowException(v8::Exception::RangeError(message));
                    return v8::Nothing<SDL_Event>();
                }
                v8::String::Utf8Value text(isolate, value);
                SDL_strlcpy(event.edit.text, *text, SDL_TEXTINPUTEVENT_TEXT_SIZE);
            }
            break;
        }
        case SDL_MOUSEMOTION: {
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "windowID", event.motion.windowID);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.motion.which);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "state", event.motion.state);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "x", event.motion.x);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "y", event.motion.y);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "xrel", event.motion.xrel);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "yrel", event.motion.yrel);
            break;
        }
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP: {
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "windowID", event.button.windowID);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.button.which);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "button", event.button.button);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "state", event.button.state);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "clicks", event.button.clicks);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding1", event.button.padding1);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "x", event.button.x);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "y", event.button.y);
            break;
        }
        case SDL_MOUSEWHEEL: {
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "windowID", event.wheel.windowID);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.wheel.which);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "x", event.wheel.x);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "y", event.wheel.y);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "direction", event.wheel.direction);
            break;
        }
        case SDL_JOYAXISMOTION: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.jaxis.which);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "axis", event.jaxis.axis);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding1", event.jaxis.padding1);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding2", event.jaxis.padding2);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding3", event.jaxis.padding3);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "value", event.jaxis.value);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding4", event.jaxis.padding4);
            break;
        }
        case SDL_JOYBALLMOTION: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.jball.which);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "ball", event.jball.ball);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding1", event.jball.padding1);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding2", event.jball.padding2);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding3", event.jball.padding3);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "xrel", event.jball.xrel);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "yrel", event.jball.yrel);
            break;
        }
        case SDL_JOYHATMOTION: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.jhat.which);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "hat", event.jhat.hat);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "value", event.jhat.value);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding1", event.jhat.padding1);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding2", event.jhat.padding2);
            break;
        }
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.jbutton.which);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "button", event.jbutton.button);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "state", event.jbutton.state);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding1", event.jbutton.padding1);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding2", event.jbutton.padding2);
            break;
        }
        case SDL_JOYDEVICEADDED:
        case SDL_JOYDEVICEREMOVED: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.jdevice.which);
            break;
        }
        case SDL_CONTROLLERAXISMOTION: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.caxis.which);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "axis", event.caxis.axis);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding1", event.caxis.padding1);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding2", event.caxis.padding2);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding3", event.caxis.padding3);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "value", event.caxis.value);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding4", event.caxis.padding4);
            break;
        }
        case SDL_CONTROLLERBUTTONDOWN:
        case SDL_CONTROLLERBUTTONUP: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.cbutton.which);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "button", event.cbutton.button);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "state", event.cbutton.state);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding1", event.cbutton.padding1);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding2", event.cbutton.padding2);
            break;
        }
        case SDL_CONTROLLERDEVICEADDED:
        case SDL_CONTROLLERDEVICEREMOVED:
        case SDL_CONTROLLERDEVICEREMAPPED: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.cdevice.which);
            break;
        }
        case SDL_CONTROLLERTOUCHPADDOWN:
        case SDL_CONTROLLERTOUCHPADMOTION:
        case SDL_CONTROLLERTOUCHPADUP: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.ctouchpad.which);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "touchpad", event.ctouchpad.touchpad);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "finger", event.ctouchpad.finger);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "x", event.ctouchpad.x);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "y", event.ctouchpad.y);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "pressure", event.ctouchpad.pressure);
            break;
        }
        case SDL_CONTROLLERSENSORUPDATE: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.csensor.which);
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "sensor", event.csensor.sensor);
            {
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, name, ToString(context, "data"));
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::Value, value, jsEvent->Get(context, name));
                if (!value->IsArray()) {
                    JS_THROW_INVALID_PROPERTY_TYPE(CPP_NOTHING(SDL_Event), name.As<v8::Value>(), "[object Array]");
                }
                if (value.As<v8::Array>()->Length() != 3) {
                    JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, name, ToString(context, "Invalid array size, expected 3, got ", value.As<v8::Array>()->Length()));
                }
                v8::Local<v8::Array> array = value.As<v8::Array>();
                for (int i = 0; i < 3; ++i) {
                    JS_READ_INDEX_FLOAT(CPP_NOTHING(SDL_Event), context, array, i, event.csensor.data[i]);
                }
            }
            break;
        }
        case SDL_AUDIODEVICEADDED:
        case SDL_AUDIODEVICEREMOVED: {
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.adevice.which);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "iscapture", event.adevice.iscapture);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding1", event.adevice.padding1);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding2", event.adevice.padding2);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding3", event.adevice.padding3);
            break;
        }
        case SDL_FINGERMOTION:
        case SDL_FINGERDOWN:
        case SDL_FINGERUP: {
            JS_READ_INT64(CPP_NOTHING(SDL_Event), context, jsEvent, "touchId", event.tfinger.touchId);
            JS_READ_INT64(CPP_NOTHING(SDL_Event), context, jsEvent, "fingerId", event.tfinger.fingerId);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "x", event.tfinger.x);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "y", event.tfinger.y);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "dx", event.tfinger.dx);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "dy", event.tfinger.dy);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "pressure", event.tfinger.pressure);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "windowID", event.tfinger.windowID);
            break;
        }
        case SDL_MULTIGESTURE: {
            JS_READ_INT64(CPP_NOTHING(SDL_Event), context, jsEvent, "touchId", event.mgesture.touchId);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "dTheta", event.mgesture.dTheta);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "dDist", event.mgesture.dDist);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "x", event.mgesture.x);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "y", event.mgesture.y);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "numFingers", event.mgesture.numFingers);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "padding", event.mgesture.padding);
            break;
        }
        case SDL_DOLLARGESTURE:
        case SDL_DOLLARRECORD: {
            JS_READ_INT64(CPP_NOTHING(SDL_Event), context, jsEvent, "touchId", event.dgesture.touchId);
            JS_READ_INT64(CPP_NOTHING(SDL_Event), context, jsEvent, "gestureId", event.dgesture.gestureId);
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "numFingers", event.dgesture.numFingers);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "error", event.dgesture.error);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "x", event.dgesture.x);
            JS_READ_FLOAT(CPP_NOTHING(SDL_Event), context, jsEvent, "y", event.dgesture.y);
            break;
        }
        case SDL_DROPBEGIN:
        case SDL_DROPFILE:
        case SDL_DROPTEXT:
        case SDL_DROPCOMPLETE: {
            JS_READ_UINT(CPP_NOTHING(SDL_Event), context, jsEvent, "windowID", event.drop.windowID);
            {
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, name, ToString(context, "file"));
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::Value, value, jsEvent->Get(context, name));
                if (!value->IsString()) {
                    JS_THROW_INVALID_PROPERTY_TYPE(CPP_NOTHING(SDL_Event), name.As<v8::Value>(), "string");
                }
                v8::String::Utf8Value string(isolate, value);
                event.drop.file = SDL_strdup(*string);
            }
            break;
        }
        case SDL_SENSORUPDATE: {
            JS_READ_INT(CPP_NOTHING(SDL_Event), context, jsEvent, "which", event.sensor.which);
            {
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, name, ToString(context, "data"));
                JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::Value, value, jsEvent->Get(context, name));
                if (!value->IsArray()) {
                    JS_THROW_INVALID_PROPERTY_TYPE(CPP_NOTHING(SDL_Event), name.As<v8::Value>(), "[object Array]");
                }
                if (value.As<v8::Array>()->Length() != 6) {
                    JS_EXECUTE_RETURN_HANDLE(CPP_NOTHING(SDL_Event), v8::String, name, ToString(context, "Invalid array size, expected 6, got ", value.As<v8::Array>()->Length()));
                }
                v8::Local<v8::Array> array = value.As<v8::Array>();
                for (int i = 0; i < 6; ++i) {
                    JS_READ_INDEX_FLOAT(CPP_NOTHING(SDL_Event), context, array, i, event.sensor.data[i]);
                }
            }
            break;
        }
    }
}
