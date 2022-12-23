#pragma once

#include <module_interface/contract.h>
#include <backend_comm.h>
#include <shared.h>

typedef struct module_ctx {
    module_contract_t*  contract_from_module;
    backend_comm_t*     common_backend_part;

    shared_object_t*    module_handler;
} module_ctx;

typedef struct backend_ctx {

} backend_t;

