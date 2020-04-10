#include <iostream>


int main() {
    int i;
    char c [] = {'-','1','2', '3'};
    sscanf(c,"%d" , &i);
    std::cout << i << std::endl;
    return 0;
}