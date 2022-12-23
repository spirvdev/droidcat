
#include <types.h>
#include <heapctrl.h>

#include <string.h>

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

static const i32 gs_droidcat_version = 0x0011000; // 0.0.1

bool tooling_hex_to_str(char byte_arr[2], u8 bvalue, bool half)
{
    if (byte_arr == NULL) return false;
    if (bvalue == 0x00)
    {
        if (half == false)
            *(u16*)byte_arr = 0x3030;
        else
            *(u16*)byte_arr = 0x30;
        return true;
    }

    static const char* const hex_values = "0123456789abcdef";
    *byte_arr++ = hex_values[bvalue & 0xf];
    
    if (half == true) return true;

    *byte_arr = hex_values[bvalue & 0xf0];

    return true;
}

i32 tooling_version_to_str(char over[], u64 sover, const i32 ver)
{
    if (ver == 0)           return strncpy(over, "0.0.0", sover) != NULL ? 0 : -1;
    if (ver & 0xf0000000)   return -1;
    if (sover < 12)         return -1;

    u64 seat = 0;
    char inter[2];

    u16 mask_value = 0x0fff & ver>>16;

    for (i32 ptr_mask = 8; ptr_mask >= 0; ptr_mask -= 4)
    {
        tooling_hex_to_str(inter, mask_value >> ptr_mask, true);
        *(u16*)(seat++ + over) = *(u16*)inter;
        if (ptr_mask != 0)
            *(seat++ + over) = '.';
    }

    *(seat++ + over) = '-';
    mask_value = 0xffff & ver;

    switch (mask_value & 0xf000)
    {
    case 0x1000: *(seat + over) = 'a'; seat++; break;
    case 0x2000: *(seat + over) = 'b'; seat++; break;
    case 0x3000: *(seat + over) = 'R'; seat++; break;
    default:     *(seat + over) = '?'; seat++; break;
    }

    for (i32 ptr_mask = 8; ptr_mask >= 0; ptr_mask -= 4)
    {
        tooling_hex_to_str(inter, mask_value >> ptr_mask, true);
        *(u16*)(seat++ + over) = *(u16*)inter;
    }

    sover[over] = '\0';
    return seat;
}

void welcome_display(const droidcat_ctx_t* droidcat_ctx)
{
    char version_buffer[0x10];
    tooling_version_to_str(version_buffer, 0x10, gs_droidcat_version);
    
}

void goodbye_display(const droidcat_ctx_t* droidcat_ctx)
{

}

i32 main()
{
    droidcat_ctx_t* droidcat_proc = (droidcat_ctx_t*)dccalloc(1, sizeof(droidcat_ctx_t));
    
    if (droidcat_proc == NULL) {}

    droidcat_init(droidcat_proc);

    welcome_display(droidcat_proc);

    goodbye_display(droidcat_proc);

    droidcat_deinit(droidcat_proc);

    dcfree(droidcat_proc);

    return 0;
}
