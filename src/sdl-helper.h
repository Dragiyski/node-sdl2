#ifndef NODE_SDL2_SDL_HELPER_H
#define NODE_SDL2_SDL_HELPER_H

#define JS_SDL2_THROW_ERROR(bailout, context, errorCode) \
    { \
        const char *errorMessage = SDL_GetError(); \
        JS_EXECUTE_RETURN_HANDLE(bailout, v8::String, message, ToString((context), errorMessage)); \
        v8::Local<v8::Object> error = v8::Exception::Error(message).As<v8::Object>(); \
        { \
            JS_EXECUTE_RETURN_HANDLE(bailout, v8::String, name, ToString((context), "code")); \
            v8::Local<v8::Integer> value = v8::Integer::New(scope.GetIsolate(), (errorCode)); \
            JS_EXECUTE_IGNORE(bailout, error->DefineOwnProperty((context), name, value, static_cast<v8::PropertyAttribute>(v8::DontEnum | v8::DontDelete | v8::ReadOnly))); \
        } \
        { \
            JS_EXECUTE_RETURN_HANDLE(bailout, v8::String, name, ToString((context), "sdl2")); \
            JS_EXECUTE_IGNORE(bailout, error->DefineOwnProperty((context), name, v8::True(scope.GetIsolate()), static_cast<v8::PropertyAttribute>(v8::DontEnum | v8::DontDelete | v8::ReadOnly))); \
        } \
        (context)->GetIsolate()->ThrowException(error); \
        return bailout; \
    }

#define JS_SDL2_THROW_IF_ERROR(bailout, context) \
    { \
        const char *errorMessage = SDL_GetError(); \
        if (errorMessage != nullptr && strlen(errorMessage) > 0) { \
            JS_EXECUTE_RETURN_HANDLE(bailout, v8::String, message, ToString((context), errorMessage)); \
            v8::Local<v8::Object> error = v8::Exception::Error(message).As<v8::Object>(); \
            { \
                JS_EXECUTE_RETURN_HANDLE(bailout, v8::String, name, ToString((context), "sdl2")); \
                JS_EXECUTE_IGNORE(bailout, error->DefineOwnProperty((context), name, v8::True(scope.GetIsolate()), static_cast<v8::PropertyAttribute>(v8::DontEnum | v8::DontDelete | v8::ReadOnly))); \
            } \
            (context)->GetIsolate()->ThrowException(error); \
            return bailout; \
        } \
    }

#define JS_SDL2_DEFINE_ENUM_UNSIGNED(bailout, context, target, group, flag) \
    {\
        v8::Local<v8::Integer> value = v8::Integer::NewFromUnsigned(scope.GetIsolate(), flag); \
        constexpr auto groupLength = const_string::length(group##_const) + 1; \
        constexpr auto flagName = const_string::substr<groupLength>(#flag ##_const); \
        JS_EXECUTE_RETURN_HANDLE(bailout, v8::String, name, ToString(context, const_string::to_string(flagName))); \
        JS_EXECUTE_IGNORE(VOID_NOTHING, target->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::ReadOnly))); \
    }

#define JS_SDL2_EXPORT_FUNCTION(bailout, js_prefix, sdl_prefix, function, functionLength) \
    { \
        constexpr auto jsPrefixLength = const_string::length(js_prefix##_const); \
        constexpr auto functionName = const_string::substr<jsPrefixLength>(#function ##_const); \
        constexpr auto sdlName = const_string::concat(sdl_prefix##_const, functionName); \
        JS_EXECUTE_RETURN_HANDLE(bailout, v8::String, name, ToString(context, const_string::to_string(sdlName))); \
        JS_EXECUTE_RETURN_HANDLE(bailout, v8::Function, value, v8::Function::New(context, function, exports, functionLength, v8::ConstructorBehavior::kThrow)); \
        JS_EXECUTE_IGNORE(bailout, exports->DefineOwnProperty(context, name, value, static_cast<v8::PropertyAttribute>(v8::DontDelete | v8::DontEnum | v8::ReadOnly))); \
    }

#endif //NODE_SDL2_SDL_HELPER_H
