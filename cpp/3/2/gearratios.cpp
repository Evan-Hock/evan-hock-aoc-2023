#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <optional>
#include <numeric>
#include <set>
#include <sstream>
#include <string_view>

std::set<int> find_adjacent_numbers(std::string_view prev, std::string_view curr, std::string_view next, int gear_idx);

std::optional<std::size_t> find_earliest_number(std::string_view sv, int pos);

// day 3 part 2
// find the sums of all gear ratios
int main() {
    std::string curr, next;
    std::getline(std::getline(std::cin, curr), next);
    std::string prev(curr.size(), '.');
    
    std::uintmax_t out = 0;
    
    bool die = !std::cin;
    while (true) {
        for (int gear_idx = 0; gear_idx < curr.size(); ++gear_idx) {
            if ((gear_idx = curr.find('*', gear_idx)) == curr.npos) {
                break;
            }
            
            std::set<int> adjacent_numbers = find_adjacent_numbers(prev, curr, next, gear_idx);
            if (adjacent_numbers.size() == 2) {
                out += std::accumulate(adjacent_numbers.cbegin(), adjacent_numbers.cend(), 1, std::multiplies<>{});
            }
        }
        
        if (die) {
            break;
        }
        
        prev = std::move(curr);
        curr = std::move(next);
        if (!std::getline(std::cin, next)) {
            next = std::string(curr.size(), '.');
            die = true;
        } 
    }
    
    std::cout << out << std::endl;
}

std::set<int> find_adjacent_numbers(std::string_view prev, std::string_view curr, std::string_view next, int gear_idx) {
    std::array<std::string_view, 3> views = { prev, curr, next };
    std::array<std::set<std::size_t>, views.size()> number_positions;
    std::transform(views.cbegin(), views.cend(), number_positions.begin(),
                   [gear_idx] (std::string_view sv) {
                       std::set<size_t> out;
                       for (int i = std::max(0, gear_idx - 1); i <= std::min(int(sv.size() - 1), gear_idx + 1); ++i) {
                           if (std::optional<std::size_t> idx = find_earliest_number(sv, i)) {
                               out.insert(idx.value());
                           }
                       }
                       
                       return out;
                   });
                   
    std::set<int> out;
    for (std::size_t i = 0; i < views.size(); ++i) {
        for (std::size_t pos : number_positions[i]) {
            out.insert(std::stoi(std::string{views[i].substr(pos)}));
        }
    }
    
    return out;
}

std::optional<std::size_t> find_earliest_number(std::string_view sv, int pos) {
    if (!std::isdigit(sv[pos])) {
        return std::nullopt;
    }
    
    do {
        --pos;
    } while (pos >= 0 && std::isdigit(sv[pos]));
    
    return ++pos;
}
