
#include <whiskey/base.h>

#include <stdarg.h>

MAYBE_UNUSED i32 whiskey_log_va(MAYBE_UNUSED const whiskey_event_e event_id, MAYBE_UNUSED droidcat_ctx_t* droidcat_ctx, MAYBE_UNUSED const char* format, MAYBE_UNUSED va_list event_va)
{
    return 0;
}

i32 whiskey_log(MAYBE_UNUSED const whiskey_location_t* event_payload, MAYBE_UNUSED const whiskey_event_e event_id, MAYBE_UNUSED droidcat_ctx_t* droidcat_ctx, MAYBE_UNUSED const char* format, ...)
{
    return 0;
}
