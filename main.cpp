#include <iostream>
#include <ctime>

int main() {
    std::time_t start = std::time(nullptr);
    volatile double d;

    // some time-consuming operation
    for (int n=0; n<10000; ++n) {
        for (int m=0; m<100000; ++m);
    }

    std::cout << "Wall time passed: "
              << std::difftime(std::time(nullptr), start) << " s.\n";
    return 0;
}