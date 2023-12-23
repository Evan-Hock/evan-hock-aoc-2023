#include <fstream>
#include <iostream>
#include <cstdint>

inline int digit_to_int(char digit);

// day 1 part 1
// find the sum of the first and last digits on each line of the input file
int main() {
    std::uintmax_t out = 0;
    
    {
        std::string line;
        while (std::getline(std::cin, line)) {
            static const char* digits = "0123456789";
            std::size_t first = line.find_first_of(digits),
                        last = line.find_last_of(digits);
                 
            out += 10 * digit_to_int(line[first]) + digit_to_int(line[last]);
        }
    }
    
    std::cout << out << '\n';
}

int digit_to_int(char digit) {
    return digit - '0';
}
