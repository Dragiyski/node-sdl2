#ifndef NODE_SDL2_ADDON_H
#define NODE_SDL2_ADDON_H

#define NAPI_VALUE_CALL(env, call) \
  do { \
    napi_status status = (call); \
    if (status != napi_ok) { \
      bool is_pending; \
      napi_is_exception_pending((env), &is_pending); \
      if (!is_pending) { \
        const napi_extended_error_info* error_info = NULL; \
        napi_get_last_error_info((env), &error_info); \
        const char *message = \
          (error_info != NULL && error_info->error_message != NULL) \
          ? error_info->error_message \
          : "empty error message"; \
        napi_throw_error((env), NULL, message); \
      } \
      return NULL; \
    } \
  } while(0)

#define NAPI_STATUS_CALL(env, call)  \
  do { \
    napi_status = (call); \
    if (status != napi_ok) { \
      bool is_pending; \
      napi_is_exception_pending((env), &is_pending); \
      if (!is_pending) { \
        const napi_extended_error_info* error_info = NULL; \
        napi_get_last_error_info((env), &error_info); \
        const char *message = \
          (error_info != NULL && error_info->error_message != NULL) \
          ? error_info->error_message \
          : "empty error message"; \
        napi_throw_error((env), NULL, message); \
      } \
      return status; \
    } \
  } while(0)

#endif //NODE_SDL2_ADDON_H
