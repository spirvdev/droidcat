#pragma once

#include <types.h>

typedef enum whiskey_event {
    WHISKEY_EVENT_SUCCESS,
    WHISKEY_EVENT_INFO,
    WHISKEY_EVENT_WARNING,
    WHISKEY_EVENT_ERROR,
    WHISKEY_EVENT_ASSERT,
    WHISKEY_EVENT_DEBUG,
    WHISKEY_EVENT_BUG,
    WHISKEY_EVENT_FATAL
} whiskey_event_e;

typedef enum whiskey_trap {
    WHISKEY_TRAP_INITIALIZING_SYSTEM,
    WHISKEY_TRAP_ASSERTION_WAS_OCCURRED
} whiskey_trap_e;

typedef i32 (*event_callback_t)(whiskey_trap_e wh_id, droidcat_ctx_t* droidcat_ctx, const void* msg_buf, const u64 len);

struct whiskey_ctx {
    event_callback_t event_back;
};

typedef struct whiskey_location {
    u64 reg_instruction_pointer;
    const char* cruntime_filename;
    i32 cruntime_sourceline;
} whiskey_location_t;

typedef struct whiskey_attr {

    bool enable_event_treating;
    event_callback_t attr_event_callback;

} whiskey_attr_t;
