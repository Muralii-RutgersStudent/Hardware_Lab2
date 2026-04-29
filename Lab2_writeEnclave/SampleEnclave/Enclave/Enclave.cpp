#include "sgx_trts.h"
#include "Enclave_t.h"

/** Securely generate a random number inside the enclave. **/
int ecall_gen_random_number() {
   unsigned int random_number = 0;
   sgx_status_t status = sgx_read_rand((unsigned char*)&random_number, sizeof(random_number));
   if(status != SGX_SUCCESS) {
       return -1;
   }
   return (int)random_number;
}
