#include <cstdint>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <optional>
#include <set>
#include <string>
#include <vector>

struct lotto_t {
    friend std::istream& operator>>(std::istream& is, lotto_t& out);
    
    std::set<int> winning_numbers, scratched_numbers;
    
  private:
    static std::set<int> get_lotto_numbers(std::istream& input);
};

template<class T>
std::set<T> intersect(const std::set<T>& x, const std::set<T>& y);

std::uintmax_t score(const std::vector<lotto_t>& lottery);

template<class CharT, class Traits = std::char_traits<CharT>>
std::basic_istream<CharT, Traits>& ignore_until(CharT delim, std::basic_istream<CharT, Traits>& is);

// day 4 part 2
// sum the scores of lottery games based on this criteria
// every lotto card allows you to win an additional |w ∩ s| lotto cards
// where w and s are the winning numbers and scratched numbers
int main() {
    std::vector<lotto_t> lottery;

    do {
        lotto_t lotto;
        std::cin >> lotto;
        lottery.push_back(std::move(lotto));
    } while (!std::cin.eof());
    
    std::cout << score(lottery) << std::endl;
}

std::istream& operator>>(std::istream& input, lotto_t& out) {
    out.winning_numbers = out.get_lotto_numbers(ignore_until(':', input));
    out.scratched_numbers = out.get_lotto_numbers(ignore_until('|', input));
    return input;
}


std::set<int> lotto_t::get_lotto_numbers(std::istream& input) {
    std::set<int> out;
    do {
        int x;
        if (input >> x) {
            out.insert(x);
        }
    } while (input);
    
    input.clear(input.rdstate() & ~input.failbit);
    
    return out;
}

template<class T>
std::set<T> intersect(const std::set<T>& x, const std::set<T>& y) {
    std::set<T> out;
    for (const T& e : x) {
        if (y.contains(e)) {
            out.insert(e);
        }
    }
    
    return out;
}

std::uintmax_t score(const std::vector<lotto_t>& lottery) {
    std::vector<std::optional<std::size_t>> memo(lottery.size());
    std::function<std::uintmax_t(std::vector<lotto_t>::const_iterator)> score_recurse =
        [&] (std::vector<lotto_t>::const_iterator it) -> std::uintmax_t {
            std::optional<std::size_t>& memo_ref = memo[it - lottery.cbegin()];
            if (memo_ref) {
                return memo_ref.value();
            }
            
            std::size_t nwins = intersect(it->winning_numbers, it->scratched_numbers).size();
            std::uintmax_t out = nwins;
            for (std::size_t i = 1; i <= nwins; ++i) {
                out += score_recurse(it + i);
            }
            
            return (memo_ref = out).value();
        };
    
    std::uintmax_t out = lottery.size();
    for (auto lotto = lottery.cbegin(); lotto != lottery.cend(); ++lotto) {
        out += score_recurse(lotto);
    }
    
    return out;
}

template<class CharT, class Traits>
std::basic_istream<CharT, Traits>& ignore_until(CharT delim, std::basic_istream<CharT, Traits>& is) {
    return is.ignore(std::numeric_limits<std::streamsize>::max(), delim);
}
