
#include <types.h>
#include <heapctrl.h>

static const char* const gs_STATE_0000 = "Doing nothing (Invalid)";
static const char* const gs_STATE_0001 = "Initializing everything (Turning ready in some milliseconds)";
static const char* const gs_STATE_0010 = "Loading all resources";

static const char* gs_STATES_XX[] = {
    gs_STATE_0000, gs_STATE_0001, gs_STATE_0010
};

i32 droidcat_init(droidcat_ctx_t* droidcat_ctx)
{
    if (droidcat_ctx == NULL || droidcat_ctx->running_status) return -1;

    droidcat_ctx->running_status    = (droidcat_status_t*)  dccalloc(1, sizeof(droidcat_ctx_t));
    droidcat_ctx->user_cmd_options  = (user_options_t*)     dccalloc(1, sizeof(user_options_t));
    droidcat_ctx->backend_manager   = (backend_ctx_t*)      dccalloc(1, sizeof(backend_ctx_t ));

    droidcat_status_t* status_ctx = droidcat_ctx->running_status;

    if (status_ctx == NULL) return -1;

    status_ctx->state_message = gs_STATES_XX[++status_ctx->state_value];

    return 0;
}

i32 droidcat_deinit(droidcat_ctx_t* droidcat_ctx)
{
    if (droidcat_ctx == NULL) return -1;
    
    dcfree(droidcat_ctx->running_status);
    dcfree(droidcat_ctx->user_cmd_options);
    dcfree(droidcat_ctx->backend_manager);

    return 0;
}

i32 main()
{
    droidcat_ctx_t* droidcat_proc = (droidcat_ctx_t*)dccalloc(1, sizeof(droidcat_ctx_t));
    
    if (droidcat_proc == NULL) {}

    droidcat_init(droidcat_proc);

    droidcat_deinit(droidcat_proc);

    dcfree(droidcat_proc);

    return 0;
}
