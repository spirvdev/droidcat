#pragma once

#include <whiskey/base.h>
#include <stdarg.h>

i32 whiskey_log_va(const whiskey_event_e event_id, droidcat_ctx_t* droidcat_ctx, const char* format, va_list event_va);

i32 whiskey_log(const whiskey_location_t* event_payload, const whiskey_event_e event_id, droidcat_ctx_t* droidcat_ctx, const char* format, ...);

#define whiskey_log_LEVEL(level, droidcat_ctx, format, ...)\
    do\
    {\
        whiskey_location_t stack_local = {\
            .cruntime_filename = __FILE__,\
            .cruntime_sourceline = __LINE__\
        };\
        whiskey_log((const whiskey_location_t*)&stack_local, level, droidcat_ctx, format, ##__VA_ARGS__);\
    } while (0)

#define whiskey_log_info(droidcat_ctx, format, ...)\
    whiskey_log_LEVEL(WHISKEY_EVENT_INFO, droidcat_ctx, format, __VA_ARGS__)

#define whiskey_log_assert(droidcat_ctx, condition, message)\
    if (((condition)) == true)\
        whiskey_log_LEVEL(WHISKEY_EVENT_ASSERT, droidcat_ctx, message" Condition: (##condition)")

