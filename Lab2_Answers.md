# ECE 455/565 (Spring 2026) — Lab 2 Answers (Write-up)

## Q1) How do I install Intel SGX SDK in simulation mode?

Use Ubuntu 20.04/22.04.  
If you are on macOS, use Docker (simulation mode only).

### Commands
```bash
sudo apt-get install build-essential ocaml ocamlbuild automake autoconf libtool wget python-is-python3 libssl-dev git cmake perl
mkdir sgx_lab && cd sgx_lab
git clone https://github.com/intel/linux-sgx.git
cd linux-sgx && make preparation
```

For Ubuntu 20.04 only:
```bash
sudo cp external/toolset/ubuntu20.04/* /usr/local/bin
which ar as ld objcopy objdump ranlib
```

Build and install SDK:
```bash
make sdk_install_pkg
cd linux/installer/bin
./sgx_linux_x64_sdk_${version}.bin --prefix ../../../../
source ${sgx-sdk-install-path}/environment
```

**Important:** run `source .../environment` in every new terminal.

---

## Q2) How do I run the SGX Hello World in simulation mode?

### Commands
```bash
cd Lab1_HelloEnclave
make SGX_MODE=SIM
./app
```

### Expected output
```text
The project has been built in debug simulation mode.
Hello world
```

### Submission
Save terminal output to:
```text
Lab1_HelloEnclave.txt
```

---

## Q3) How do I implement SGX secure random number generation?

### 1) Define ECALL in `Enclave.edl`
```edl
enclave {
    from "sgx_tstdc.edl" import *;
    trusted {
        public int ecall_gen_random_number();
    };
    untrusted { };
};
```

### 2) Implement ECALL in `Enclave.cpp`
```cpp
#include "sgx_trts.h"
#include "Enclave_t.h"

int ecall_gen_random_number() {
    unsigned int random_number = 0;
    sgx_status_t status = sgx_read_rand((unsigned char*)&random_number, sizeof(random_number));
    if (status != SGX_SUCCESS) return -1;
    return (int)random_number;
}
```

### 3) Call ECALL from `App.cpp`
```cpp
int SGX_CDECL main() {
    int random_number;
    initialize_enclave();
    sgx_status_t state = ecall_gen_random_number(global_eid, &random_number);
    if (state != SGX_SUCCESS) {
        printf("Can not run enclave call, error-code: %d\n", state);
        return -1;
    }
    printf("Random Number: %d\n", random_number);
    sgx_destroy_enclave(global_eid);
    return 0;
}
```

### Build and run
```bash
make SGX_MODE=SIM
./secure_rng
```

### Expected output
```text
Random Number: <integer>
```

### Submission
Save terminal output to:
```text
Lab2_WriteEnclave.txt
```

---

## Q4) Bonus: How should the Digital Wallet be implemented?

### Required enclave state
1. `balance` initialized to `1000`
2. `transactions` log stored in enclave

### Required ECALLs
1. `deposit(amount)` → increase balance and log `Deposit: amount`
2. `pay(amount)` → decrease balance and log `Pay: amount`
3. `view_balance()` → use OCALL to print balance
4. `check_transactions()` → use OCALL to print latest 3 transactions (or all if fewer)

### Testing requirement
From untrusted `App.cpp`, call all four ECALLs.

### Submission
1. Program source
2. Terminal output file:
```text
Bonus_DigitalWallet.txt
```

---

## Docker note (for your current Mac setup)

- Use Docker for **simulation mode** screenshots and execution.
- Docker on Mac is **not** for true SGX hardware mode.
- Hardware mode requires SGX-capable hardware + native Linux SGX driver/runtime.
