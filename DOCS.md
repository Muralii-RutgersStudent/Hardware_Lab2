# ECE 455/565 (Spring 2026) — Lab 2: Introduction to Intel SGX

This lab covers:

1. Installing Intel SGX SDK and running SGX in **simulation mode** on Ubuntu 20.04/22.04.
2. Running an SGX “hello world” example.
3. Implementing a secure random number generator inside an enclave.
4. **Optional bonus:** building a digital wallet using SGX.

**Required OS:** Ubuntu 20.04 or 22.04  
**Code package:** `lab_sgx.zip` (course-provided)

If you have questions, post them on the course Piazza.

---

## 1) Install SGX in simulation mode

### Step 1: Build Intel SGX SDK installer

Install dependencies:

```bash
sudo apt-get install build-essential ocaml ocamlbuild automake autoconf libtool wget python-is-python3 libssl-dev git cmake perl
```

Create and enter working folder:

```bash
mkdir sgx_lab && cd sgx_lab
```

Clone SGX source and prepare:

```bash
git clone https://github.com/intel/linux-sgx.git
cd linux-sgx && make preparation
```

Ubuntu-specific mitigation tools step:

- **Ubuntu 22.04:** skip this step.
- **Ubuntu 20.04:** copy mitigation tools and ensure executables are available:

```bash
sudo cp external/toolset/ubuntu20.04/* /usr/local/bin
which ar as ld objcopy objdump ranlib
```

Build SDK installer:

```bash
make sdk_install_pkg
```

Installer output is under:

```text
linux/installer/bin/
```

Expected installer name pattern:

```text
sgx_linux_x64_sdk_${version}.bin
```

### Step 2: Install Intel SGX SDK installer

Install required tools:

```bash
sudo apt-get install build-essential python-is-python3
```

Run installer (from `linux/installer/bin`), installing to `sgx_lab/`:

```bash
cd linux/installer/bin
./sgx_linux_x64_sdk_${version}.bin --prefix ../../../../
```

After successful install, source the SGX environment:

```bash
source ${sgx-sdk-install-path}/environment
```

> You must re-run the `source .../environment` command in every new terminal.

---

## 2) Run the SGX “hello world” example (simulation mode)

**Submission:** save terminal output as `Lab1_HelloEnclave.txt`.

Project location: `Lab1_HelloEnclave/`

- `App/`: untrusted app code
- `Enclave/`: enclave code
- `Enclave/Enclave.edl`: ECALL/OCALL declarations

Build in simulation mode:

```bash
cd Lab1_HelloEnclave
make SGX_MODE=SIM
```

Successful build message includes:

```text
The project has been built in debug simulation mode.
```

Run:

```bash
./app
```

Expected output:

```text
Hello world
```

If compilation fails, use the troubleshooting section from course material.

---

## 3) Implement a random number generator with SGX

**Submission:** save terminal output as `Lab2_WriteEnclave.txt`.

Folder: `Lab2_WriteEnclave/`

Baseline C++ RNG (`rng.cpp`) uses:

```cpp
int gen_random_number() {
   srand((unsigned) time(NULL));
   int random = rand();
   return random;
}
```

Compile/run baseline:

```bash
g++ -o rng ./rng.cpp
./rng
```

To make RNG secure, implement SGX enclave RNG in `SampleEnclave/`:

### 3.1 Define ECALL in EDL

File: `Lab2_WriteEnclave/SampleEnclave/Enclave/Enclave.edl`

```edl
enclave {
    from "sgx_tstdc.edl" import *;

    trusted {
        public int ecall_gen_random_number();
    };

    untrusted {
        /* define OCALLs if needed */
    };
};
```

### 3.2 Implement ECALL in enclave

File: `Lab2_WriteEnclave/SampleEnclave/Enclave/Enclave.cpp`

```cpp
#include "sgx_trts.h"
#include "Enclave_t.h"

int ecall_gen_random_number() {
   unsigned int random_number = 0;
   sgx_status_t status = sgx_read_rand((unsigned char*)&random_number, sizeof(random_number));
   if (status != SGX_SUCCESS) {
       return -1;
   }
   return (int)random_number;
}
```

### 3.3 Call ECALL from untrusted app

File: `Lab2_WriteEnclave/SampleEnclave/App/App.cpp` (`main`)

```cpp
int SGX_CDECL main() {
    int random_number;
    initialize_enclave();
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
```

Build and run in simulation mode:

```bash
make SGX_MODE=SIM
./secure_rng
```

Expected output:

```text
Random Number: <some integer>
```

If compilation fails, use the troubleshooting section from course material.

---

## 4) Optional Bonus — Digital Wallet with SGX

**Submission requirements:**

1. Save compile/run terminal output as `Bonus_DigitalWallet.txt`.
2. Submit your program plus `Bonus_DigitalWallet.txt`.

Implement an SGX-protected wallet with these requirements:

- Initial balance is **1000**, stored securely in enclave.
- Maintain enclave `transactions` structure logging every action:
  - `Deposit: <amount>`
  - `Pay: <amount>`
- Implement four ECALLs:
  - `deposit(amount)`: increase balance and log.
  - `pay(amount)`: decrease balance and log.
  - `view_balance()`: use OCALL to print current balance.
  - `check_transactions()`: use OCALL to print most recent 3 transactions (or all if fewer than 3).
- In untrusted `App.cpp`, call all four ECALLs to test functionality.

---

## Notes

- Always source SGX environment in a new terminal:

```bash
source ${sgx-sdk-install-path}/environment
```

- Use `SGX_MODE=SIM` for simulation mode builds.
