#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef uint8_t     u8;
typedef uint16_t    u16;
typedef int32_t     i32;
typedef uint32_t    u32;
typedef int32_t     u64;

typedef struct droidcat_status {
    i32 state_value;
    const char* state_message;
} droidcat_status_t;

typedef struct user_options {
} user_options_t;

typedef struct backend_ctx {
} backend_ctx_t;

typedef struct droidcat_ctx {
    droidcat_status_t*  running_status;

    user_options_t*     user_cmd_options;
    backend_ctx_t*      backend_manager;

} droidcat_ctx_t;
