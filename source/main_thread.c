
#include <types.h>
#include <heapctrl.h>

#include <string.h>
#include <math.h>

#include <whiskey/release.h>

static const char* const gs_STATE_0000 = "Doing nothing (Invalid)";
static const char* const gs_STATE_0001 = "Initializing everything (Turning ready in some milliseconds)";
static const char* const gs_STATE_0010 = "Loading all resources";

static const char* gs_STATES_XX[] = {
    gs_STATE_0000, gs_STATE_0001, gs_STATE_0010
};

i32 droidcat_init(droidcat_ctx_t* droidcat_ctx)
{
    if (droidcat_ctx == NULL || droidcat_ctx->running_status != NULL) return -1;

    droidcat_ctx->running_status    = (droidcat_status_t*)  dccalloc(1, sizeof(droidcat_ctx_t));
    droidcat_ctx->user_cmd_options  = (user_options_t*)     dccalloc(1, sizeof(user_options_t));
    droidcat_ctx->backend_manager   = (backend_ctx_t*)      dccalloc(1, sizeof(backend_ctx_t ));
    droidcat_ctx->whiskey_server    = (whiskey_ctx_t*)      dccalloc(1, sizeof(whiskey_ctx_t));

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
    dcfree(droidcat_ctx->whiskey_server);

    return 0;
}

static const i32 gs_droidcat_version = 0x0011000; // 0.0.1a000

bool tooling_hex_to_str(char byte_arr[2], u8 bvalue, bool half)
{
    if (byte_arr == NULL) return false;
    if (bvalue == 0x00)
    {
        if (half == true)
            *(u8*)byte_arr = 0x30;
        else
            *(u16*)byte_arr = 0x3030;
        return true;
    }

    static const char* const hex_values = "0123456789abcdef";
    *byte_arr++ = hex_values[bvalue & 0xf];
    
    if (half == true) return true;

    *byte_arr = hex_values[bvalue & 0xf0];

    return true;
}

bool tooling_dec_to_str(u8 byte_arr[], u8 asize, i32 bvalue)
{
    if (byte_arr == NULL) return false;
    if (bvalue == 0)
    {
        *(u16*)byte_arr = 0x30;
        return true;
    }
    // We can know what much times the do-while loop will run with this trick!
    i32 final = (i32)(ceil(log10(bvalue) + 1));
    if ((final * sizeof(char)) >= asize)
        return false;
    
    // Putting a final invalid byte
    u8* arr_ptr = &byte_arr[asize - 1];

    if (bvalue > 0) goto end;
    
    final[arr_ptr] = '-';
    
    if (asize + 1 > final)
        (final + 1)[byte_arr] = '\0'; 
    goto loop;

    end:
    *arr_ptr-- = '\0';
    loop:

    do {
        *arr_ptr-- = '0' + (bvalue % 10);
        bvalue /= 10;
    } while (bvalue != 0);
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

    #define DEC_CONV(inc_dex, output, intbuf, size, value)\
    do\
    {\
        if (tooling_dec_to_str(intbuf, size, value) == false)\
            return false;\
        *(u16*)(inc_dex++ + output) = *(u16*)intbuf;\
    } while(0)

    for (i32 ptr_mask = 8; ptr_mask >= 0; ptr_mask -= 4)
    {
        DEC_CONV(seat, over, (u8*)inter, sizeof(inter), mask_value >> ptr_mask & 0xf);
        if (ptr_mask != 0)
            *(seat++ + over) = '.';
    }

    *(seat++ + over) = '-';
    mask_value = 0xffff & ver;

    //static const char mask_version[] = {'?', 'a', 'b', 'R', '?'};
    //*(seat++ + over) = mask_version[mask_value>>12 & 0xf];

    switch (mask_value>>12 & 0xa)
    {
    case 0x1: *(seat + over) = 'a'; seat++; break;
    case 0x2: *(seat + over) = 'b'; seat++; break;
    case 0x3: *(seat + over) = 'R'; seat++; break;
    default:  *(seat + over) = '?'; seat++; break;
    }

    for (i32 ptr_mask = 8; ptr_mask >= 0; ptr_mask -= 4)
        DEC_CONV(seat, over, (u8*)inter, sizeof(inter), mask_value >> ptr_mask & 0xf);
    
    #undef DEC_CONV

    sover[over] = '\0';
    return seat;
}

void welcome_display(droidcat_ctx_t* droidcat_ctx)
{
    char version_buffer[0x10];
    const bool verok = tooling_version_to_str(version_buffer, sizeof(version_buffer), gs_droidcat_version);

    whiskey_log_assert(droidcat_ctx, verok == false, "Invalid version string!");

    whiskey_log_info(droidcat_ctx, "Welcome to droidcat version (%s) compiled with ...\n", version_buffer);
}

void goodbye_display(const droidcat_ctx_t* droidcat_ctx) {}

static const char* const gs_MAIN_INV_CTX_0001 = "Can't allocate the droidcat main context, quitting now!";
static const char* const gs_MAIN_INV_CTX_0002 = "Can't initialize the main context, we can't continue from here!";

i32 main()
{
    droidcat_ctx_t* droidcat_ctx = (droidcat_ctx_t*)dccalloc(1, sizeof(droidcat_ctx_t));
    
    if (droidcat_ctx == NULL)
    {
        whiskey_log_fatal(NULL, "%s", gs_MAIN_INV_CTX_0001);
        return -1;
    }

    const i32 initret = droidcat_init(droidcat_ctx);

    if (initret != 0)
    {
        whiskey_log_fatal(NULL, "%s", gs_MAIN_INV_CTX_0002);
        dcfree(droidcat_ctx);

        return -1;
    }

    welcome_display(droidcat_ctx);

    goodbye_display(droidcat_ctx);

    droidcat_deinit(droidcat_ctx);
    dcfree(droidcat_ctx);

    return 0;
}
