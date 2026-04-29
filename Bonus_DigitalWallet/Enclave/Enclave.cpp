#include "sgx_trts.h"
#include "Enclave_t.h"
#include <string.h>

#define MAX_TRANSACTIONS 100

typedef struct {
    char type[10];
    int amount;
} transaction_t;

int current_balance = 1000;
transaction_t transactions[MAX_TRANSACTIONS];
int transaction_count = 0;

void log_transaction(const char* type, int amount) {
    if (transaction_count < MAX_TRANSACTIONS) {
        strncpy(transactions[transaction_count].type, type, sizeof(transactions[transaction_count].type));
        transactions[transaction_count].amount = amount;
        transaction_count++;
    }
}

void ecall_deposit(int amount) {
    current_balance += amount;
    log_transaction("Deposit", amount);
}

void ecall_pay(int amount) {
    if (current_balance >= amount) {
        current_balance -= amount;
        log_transaction("Pay", amount);
    } else {
        ocall_print_string("Insufficient funds!\n");
    }
}

void ecall_view_balance() {
    ocall_print_balance(current_balance);
}

void ecall_view_transactions() {
    int start = (transaction_count > 3) ? (transaction_count - 3) : 0;
    for (int i = transaction_count - 1; i >= start; i--) {
        ocall_print_transaction(transactions[i].type, transactions[i].amount);
    }
}
