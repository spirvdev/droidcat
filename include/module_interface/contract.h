#pragma once

#include <backend_comm.h>

#include <stdbool.h>

typedef struct module_contract {
    const char*     module_name;
    const char*     module_version;

    const struct    module_contract* (*module_init)(backend_comm_t* backend_module_ctx);
    void            (*module_deinit)(void);
    void            (*module_confirmation)(bool accepted);
} module_contract_t;
