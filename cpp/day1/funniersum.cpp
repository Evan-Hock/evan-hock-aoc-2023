#include <algorithm>
#include <array>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <span>
#include <string_view>

const std::array<const char*, 9> number_strings = { "one", "two", "three", "four", "five", "six", "seven", "eight", "nine" };
const char* const digits = "0123456789";

inline int digit_to_int(char digit);

enum class converter_type {
    front,
    back,
};

template<converter_type Type>
int convert(std::string_view sv);

// day 1 part 2
// number words count as digits too
int main() {
    std::ifstream input{"input.txt"};
    
    std::uintmax_t out = 0;
    
    {
        std::string line;
        while (std::getline(input, line)) {
            using enum converter_type;
            std::string_view lv{line};
            out += 10 * convert<front>(lv) + convert<back>(lv);
        }
    }
    
    std::cout << out << '\n';
}

int digit_to_int(char digit) {
    return digit - '0';
}


template<converter_type Type = converter_type::front>
struct converter_functions {
    static std::size_t number_string_finder(std::string_view sv, const char* number_string) {
        return sv.find(number_string);
    }
    
    static std::size_t digit_finder(std::string_view sv) {
        return sv.find_first_of(digits);
    }
    
    static const std::size_t& digit_location_finder(const std::size_t& digit, std::span<std::size_t, number_strings.size()> locations) {
        return std::min(digit, *std::min_element(locations.begin(), locations.end()));
    }
};

template<>
struct converter_functions<converter_type::back> {
    static std::size_t number_string_finder(std::string_view sv, const char* number_string) {
        return sv.rfind(number_string);
    }
    
    static std::size_t digit_finder(std::string_view sv) {
        return sv.find_last_of(digits);
    }
    
    static const std::size_t& digit_location_finder(const std::size_t& digit, std::span<std::size_t, number_strings.size()> locations) {
        const auto npos_smallest_comparison = [] (std::size_t x, std::size_t y) -> bool {
            if (x == std::string::npos) {
                return y != std::string::npos;
            }
            
            if (y == std::string::npos) {
                return false;
            }
            
            return x < y;
        };
        
        return std::max(digit, *std::max_element(locations.begin(), locations.end(), npos_smallest_comparison), npos_smallest_comparison);
    }
};

template<converter_type Type>
int convert(std::string_view sv) {
    std::array<std::size_t, number_strings.size()> string_locations;
    std::transform(number_strings.cbegin(), number_strings.cend(), string_locations.begin(),
                   [sv] (const char* number_string) {
                        return converter_functions<Type>::number_string_finder(sv, number_string);
                   }
    );
    
    std::size_t digit = converter_functions<Type>::digit_finder(sv);
    const std::size_t& digit_location = converter_functions<Type>::digit_location_finder(digit, string_locations);
    return &digit_location == &digit ? digit_to_int(sv[digit]) : &digit_location - string_locations.data() + 1;
}
