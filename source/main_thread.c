
#include <types.h>
#include <heapctrl.h>

#include <string.h>
#include <math.h>
#include <getopt.h>
#include <stdlib.h>

#include <core_backend.h>
#include <whiskey/release.h>

static const char* const gs_STATE_0000 = "Doing nothing (Invalid)";
static const char* const gs_STATE_0001 = "Initializing everything (Turning ready in some milliseconds)";
static const char* const gs_STATE_0010 = "Loading all resources";

static const char* gs_STATES_XX[] = {
    gs_STATE_0000, gs_STATE_0001, gs_STATE_0010
};

i32 user_parser_init(droidcat_ctx_t* droidcat_ctx)
{
    user_options_t* user_commands = droidcat_ctx->user_cmd_options;
    if (user_commands == NULL) return -1;

    // Applying default options configuration!
    user_commands->display_banner = true;
    return 0;
}

i32 user_parser_deinit(MAYBE_UNUSED const droidcat_ctx_t* droidcat_ctx) {return 0;}

#define OPT_INDEX_DISPLAY_BANNER    'B'
#define OPT_INDEX_VERBOSE_LEVEL     'V'

static const struct option long_options[] = {
    {"display-banner", required_argument,   NULL, OPT_INDEX_DISPLAY_BANNER  },
    {"verbose-level",  required_argument,   NULL, OPT_INDEX_VERBOSE_LEVEL   },
    {}
};

i32 user_parser_setup(droidcat_ctx_t* droidcat_ctx, i32 argc, char** argv)
{
    user_options_t* user_commands = droidcat_ctx->user_cmd_options;
    
    static const char* const short_getopt = "B:V:";
    i32 c;
    i32 long_index;

    while ((c = getopt_long(argc, argv, short_getopt, long_options, &long_index)) != -1)
    {
        switch (c) 
        {
        case OPT_INDEX_DISPLAY_BANNER: 
            user_commands->display_banner = (i32)strtoull(optarg, NULL, 10) != 0 ? true : false;
            break;
        case OPT_INDEX_VERBOSE_LEVEL:
            user_commands->verbosity = (i32)strtoull(optarg, NULL, 0);
            break;
        }
    }

    return 0;
}

i32 user_parser_sanitizer(droidcat_ctx_t* droidcat_ctx)
{
    user_options_t* user_commands = droidcat_ctx->user_cmd_options;

    if (user_commands->verbosity < 2)
    {
        if (user_commands->display_banner != false)
        {
            whiskey_log_warning(droidcat_ctx, "Program banner can't be displayed " 
                "because verbosity level is under level 2\n");
            user_commands->display_banner = false;
        }
    }

    return 0;
}


i32 session_init(droidcat_ctx_t* droidcat_ctx)
{
    i32 sess = 0;
    droidcat_status_t* status_session = droidcat_ctx->running_status;
    
    status_session->state_message = gs_STATES_XX[++status_session->state_value];
    
    sess = user_parser_init(droidcat_ctx);
    if (sess != 0)
    {
        whiskey_log_error(droidcat_ctx, "User parser at %p has failed to initializer\n", droidcat_ctx->user_cmd_options);
        return sess;
    }

    sess = backend_init(droidcat_ctx);
    if (sess != 0)
    {
        whiskey_log_error(droidcat_ctx, "Backend at %p has failed to initializer\n", droidcat_ctx->backend_manager);
        return sess;
    }

    whiskey_log_success(droidcat_ctx, "Droidcat session has successfully initialized!\n");

    return 0;
}

i32 session_deinit(droidcat_ctx_t* droidcat_ctx)
{
    if (droidcat_ctx == NULL) return -1;
    
    backend_deinit(droidcat_ctx);
    user_parser_deinit(droidcat_ctx);

    return 0;
}

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

MAYBE_UNUSED bool tooling_hex_to_str(char byte_arr[2], u8 bvalue, bool half)
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
    if (ver == 0)           return strncpy(over, "0.0.0-?000", sover) != NULL ? 0 : -1;
    if (ver & 0xf0000000)   return -1;
    if (sover < 12)         return -1;

    u64 seat = 0;
    char inter[2];

    u16 mask_value = 0x0fff & ver>>16;

    #define DEC_CONV(inc_dex, output, intbuf, size, value)\
    do\
    {\
        if (tooling_dec_to_str(intbuf, size, value) == false)\
            return -1;\
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
    const user_options_t* user_commands = droidcat_ctx->user_cmd_options;
    
    if (user_commands->display_banner == false) return;
    
    char version_buffer[0x10];
    const bool verok = tooling_version_to_str(version_buffer, sizeof(version_buffer), gs_droidcat_version);

    whiskey_log_assert(droidcat_ctx, verok == false, "Invalid version string!");

    whiskey_log_info(droidcat_ctx, "Welcome to droidcat version (%s) compiled with ...\n", version_buffer);
}

void goodbye_display(MAYBE_UNUSED const droidcat_ctx_t* droidcat_ctx) {}

static const char* const gs_MAIN_INV_CTX_0001 = "Can't allocate the droidcat main context, quitting now!";
static const char* const gs_MAIN_INV_CTX_0002 = "Can't initialize the main context, we can't continue from here!";

i32 main(i32 argc, char** argv)
{
    droidcat_ctx_t* droidcat_ctx = (droidcat_ctx_t*)dccalloc(1, sizeof(droidcat_ctx_t));
    
    if (droidcat_ctx == NULL)
    {
        whiskey_log_fatal(NULL, "%s\n", gs_MAIN_INV_CTX_0001);
        return -1;
    }

    const i32 initret = droidcat_init(droidcat_ctx);

    if (initret != 0)
    {
        whiskey_log_fatal(NULL, "%s\n", gs_MAIN_INV_CTX_0002);
        dcfree(droidcat_ctx);

        return -1;
    }

    session_init(droidcat_ctx);

    user_parser_setup(droidcat_ctx, argc, argv);
    user_parser_sanitizer(droidcat_ctx);

    welcome_display(droidcat_ctx);

    goodbye_display(droidcat_ctx);

    session_deinit(droidcat_ctx);
    droidcat_deinit(droidcat_ctx);

    dcfree(droidcat_ctx);

    return 0;
}
