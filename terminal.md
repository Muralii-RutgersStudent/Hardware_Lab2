# SGX Lab Terminal Session

```bash
root@ubuntu:~/sgx_lab# source /root/sgxsdk/sgxsdk/environment
root@ubuntu:~/sgx_lab# cd Lab1_HelloEnclave
root@ubuntu:~/sgx_lab/Lab1_HelloEnclave# make clean && make SGX_MODE=SIM
GEN  =>  App/Enclave_u.c
CC   <=  App/Enclave_u.c
App/App.cpp: In function 'int initialize_enclave()':
App/App.cpp:174:16: warning: 'char* strncat(char*, const char*, size_t)' specified bound 1 equals source length [-Wstringop-overflow=]
CXX  <=  App/App.cpp
LINK =>  app
GEN  =>  Enclave/Enclave_t.c
CC   <=  Enclave/Enclave_t.c
CXX  <=  Enclave/Enclave.cpp
LINK =>  enclave.so
SIGN =>  enclave.signed.so
The project has been built in debug simulation mode.

root@ubuntu:~/sgx_lab/Lab1_HelloEnclave# ./app
Hello world

root@ubuntu:~/sgx_lab/Lab1_HelloEnclave# cd ../Lab2_writeEnclave/SampleEnclave
root@ubuntu:~/sgx_lab/Lab2_writeEnclave/SampleEnclave# make clean && make SGX_MODE=SIM
GEN  =>  App/Enclave_u.h
CC   <=  App/Enclave_u.c
CXX  <=  App/App.cpp
LINK =>  secure_rng
GEN  =>  Enclave/Enclave_t.h
CC   <=  Enclave/Enclave_t.c
CXX  <=  Enclave/Enclave.cpp
LINK =>  enclave.so
SIGN =>  enclave.signed.so
The project has been built in debug simulation mode.

root@ubuntu:~/sgx_lab/Lab2_writeEnclave/SampleEnclave# ./secure_rng
Random Number: 1842095311

root@ubuntu:~/sgx_lab/Lab2_writeEnclave/SampleEnclave# cd ../../Bonus_DigitalWallet
root@ubuntu:~/sgx_lab/Bonus_DigitalWallet# make clean && make SGX_MODE=SIM
GEN  =>  App/Enclave_u.h
CC   <=  App/Enclave_u.c
CXX  <=  App/App.cpp
LINK =>  digital_wallet
GEN  =>  Enclave/Enclave_t.h
CC   <=  Enclave/Enclave_t.c
CXX  <=  Enclave/Enclave.cpp
LINK =>  enclave.so
SIGN =>  enclave.signed.so
The project has been built in debug simulation mode.

root@ubuntu:~/sgx_lab/Bonus_DigitalWallet# ./digital_wallet
Starting Digital Wallet test...
Insufficient funds!

Viewing balance:
Current Balance: 1220

Viewing most recent 3 transactions:
Pay: 50
Deposit: 100
Pay: 30
root@ubuntu:~/sgx_lab/Bonus_DigitalWallet# 
```
