#include <iostream>

using namespace std;

int gen_random_number() {
    srand((unsigned) time(NULL));
    int random = rand();
    return random;
}

int main() {
    int random_number;
    random_number = gen_random_number();

    printf("Random Number: %d\n", random_number);

    return 0;
}