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

// OCALL implementations
void ocall_print_string(const char *str) {
    printf("%s", str);
}

void ocall_print_balance(int balance) {
    printf("Current Balance: %d\n", balance);
}

void ocall_print_transaction(const char *type, int amount) {
    printf("%s: %d\n", type, amount);
}

/**
 * Main application.
 */
int SGX_CDECL main() {
    if (initialize_enclave() < 0) {
        return -1;
    }

    printf("Starting Digital Wallet test...\n");

    ecall_deposit(global_eid, 200);
    ecall_pay(global_eid, 50);
    ecall_deposit(global_eid, 100);
    ecall_pay(global_eid, 30);
    ecall_pay(global_eid, 10000); // Test insufficient funds

    printf("\nViewing balance:\n");
    ecall_view_balance(global_eid);

    printf("\nViewing most recent 3 transactions:\n");
    ecall_view_transactions(global_eid);

    sgx_destroy_enclave(global_eid);   
    return 0;
}
