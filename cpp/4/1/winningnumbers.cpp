#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <set>

std::set<int> get_lotto_numbers(std::istream& input);

// day 4 part 1
// sum the scores of lottery games based on this criteria
// s = 2 ^ (n - 1)
int main() {
    std::ifstream input{"../../input/day4.txt"};
    
    std::uintmax_t out;
    
    while (!input.eof()) {
        input.ignore(std::numeric_limits<std::streamsize>::max(), ':');
        
        std::set<int> winning_numbers = get_lotto_numbers(input);
        
        input.ignore(std::numeric_limits<std::streamsize>::max(), '|');
        
        std::set<int> scratched_numbers = get_lotto_numbers(input);
        
        int score = 0;
        for (int wn : winning_numbers) {
            if (scratched_numbers.contains(wn)) {
                score = score == 0 ? 1 : 2 * score;
            }
        }
        
        out += score;
    }
    
    std::cout << out << std::endl;
}

std::set<int> get_lotto_numbers(std::istream& input) {
    std::set<int> out;
    do {
        int x;
        if (input >> x) {
            out.insert(x);
        }
    } while (input);
    
    if (input.fail()) {
        input.clear(input.rdstate() & input.eofbit);
    }
    
    return out;
}
