#include <iostream>
#include <fstream>

int main() {
    std::ofstream os;
    os.open("test5.txt", std::ios_base::app);
    size_t number_of_test_numbers = 400;
    int range = 100;
    for (size_t i = 0; i < number_of_test_numbers; i++) {
        os << (rand() % range - range/2);
        os << " ";
    }
    std::cout << RAND_MAX << std::endl;
    os.close();

    return 0;
}