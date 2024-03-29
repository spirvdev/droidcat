#include <module_interface/contract.h>

static module_contract_t    g_casper3_contract;
static backend_comm_t*      g_backend_node = 0;

const module_contract_t* casper3_init(backend_comm_t* backend_ctx)
{
    g_backend_node = backend_ctx;
    return (const module_contract_t*)&g_casper3_contract;
}

void casper3_deinit() {}

void casper3_test(bool is_accepted)
{
    if (g_backend_node && is_accepted == true)
        return;
}

static module_contract_t g_casper3_contract = {
    .module_name                = "Casper3",
    .module_version             = "1a01",
    .module_init                = casper3_init,
    .module_deinit              = casper3_deinit,
    .module_confirmation        = casper3_test
};
