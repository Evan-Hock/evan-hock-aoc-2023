#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <ranges>

bool adjacent_symbol(std::string_view prev, std::string_view next, int pos);

bool issymbol(char c);

int main() {
    std::ifstream input{"input.txt"};
    std::string curr, next;
    std::getline(std::getline(input, curr), next);
    if (!input) {
        next = std::string(curr.size(), '.');
    }
    
    std::string prev(curr.size(), '.');
    
    std::uintmax_t out = 0;
    
    bool die = bool(input);
    while (true) {
        // for each position in the current string
        for (int i = 0; i < curr.size(); ) {
            std::string number_string;
            
            // skip past non-symbol non-digit characters
            while (i < curr.size() && !std::isdigit(curr[i])) {
                ++i;
            }
            
            if (i >= curr.size()) {
                break;
            }
            
            // extract the number
            int j = i;
            while (i < curr.size() && std::isdigit(curr[i])) {
                number_string.push_back(curr[i++]);
            }
            
            // determine if it is a part number
            if (
                   issymbol(curr[std::max(0, j - 1)]) // if the character before the number string is a symbol
                || issymbol(curr[std::min(curr.size() - 1, j + number_string.size())]) // or the character after
                || std::ranges::any_of(std::views::iota(j, int(j + number_string.size())), std::bind_front(adjacent_symbol, prev, next))
            ) {
                out += std::stoi(number_string);
                continue;
            }
        }
        
        if (die) {
            break;
        }
        
        prev = std::move(curr);
        curr = std::move(next);
        if (!std::getline(input, next)) {
            next = std::string(curr.size(), '.');
            die = true;
        } 
    }
    
    std::cout << out << std::endl;
}

bool adjacent_symbol(std::string_view prev, std::string_view next, int pos) {
    std::array<std::string_view, 2> views = { prev, next };
    return std::any_of(views.cbegin(), views.cend(),
                       [pos] (std::string_view sv) {
                           return issymbol(sv[std::max(0, pos - 1)]) || issymbol(sv[pos]) || issymbol(sv[std::min(int(sv.size() - 1), pos + 1)]);
                       });
}

bool issymbol(char c) {
    return c != '.' && !std::isdigit(c);
}
