#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "sgx_urts.h"
#include "Enclave_u.h"

// Enclave ID
sgx_enclave_id_t global_eid = 0;

/**
 * Initialize an SGX enclave.
 * @return error state
 */
int initialize_enclave(void) {
    sgx_status_t state = SGX_ERROR_UNEXPECTED;

    char enclavefile[256];
    if (getcwd(enclavefile, sizeof(enclavefile)) == NULL) {
        return -1;
    }
    strcat(enclavefile, "/enclave.signed.so");

    state = sgx_create_enclave(enclavefile, SGX_DEBUG_FLAG, NULL, NULL, &global_eid, NULL);

    if (state != SGX_SUCCESS) {
        printf("Can not create enclave, error-code: %d\n", state);
        return -1;
    }

    return 0;
}

/**
 * Main application.
 */
int SGX_CDECL main() {
    int random_number;
    if (initialize_enclave() < 0) {
        return -1;
    }
    sgx_status_t state = ecall_gen_random_number(global_eid, &random_number);
    if (state != SGX_SUCCESS) {
        printf("Can not run enclave call, error-code: %d\n", state);
        return -1;
    } else {
        printf("Random Number: %d\n", random_number);
    }
    sgx_destroy_enclave(global_eid);   
    return 0;
}
