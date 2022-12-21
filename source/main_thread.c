
#include <stdint.h>

typedef int32_t i32;

static const char* const gs_STATE_0000 = "Doing nothing (Initializing)";
static const char* const gs_STATE_0010 = "Loading all resources";

static const char* gs_STATES_XX[] = {
    gs_STATE_0000, gs_STATE_0010
};

typedef struct droidcat_status {
    i32 state_value;
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

i32 main()
{

}
