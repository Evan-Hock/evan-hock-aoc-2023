#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>

struct bag_t {
    int red, green, blue;
    
    int power() const {
        return this->red * this->green * this->blue;
    }
};

std::istream& operator>>(std::istream& is, bag_t& out) {
    std::map<std::string, int> cubes;
    
    for (int i = 0; i < 3 && is; ++i) {
        int ncubes;
        std::string color;
        is >> ncubes;
        std::getline(is >> std::ws, color, ',');
        cubes.insert({std::move(color), ncubes});
    }
    
    out.red = cubes["red"];
    out.green = cubes["green"];
    out.blue = cubes["blue"];
    
    return is;
}

// day 2 part 2
// find the minimum bag for each game to be possible
// output their "power", which is defined as the product of
// their cube totals
int main() {
    std::ifstream input{"../../input/day2.txt"};
    
    std::uintmax_t out = 0;
    
    for (int game_id = 1; input.ignore(std::numeric_limits<std::streamsize>::max(), ':'); ++game_id) {
        std::string line;
        std::getline(input, line);
        
        bag_t minimum_bag{std::numeric_limits<int>::min(), std::numeric_limits<int>::min(), std::numeric_limits<int>::min()};
        std::istringstream liness{line};
        for (std::string cubeinfo; std::getline(liness, cubeinfo, ';'); ) {
            std::istringstream cubess{cubeinfo};
            bag_t bag;
            cubess >> bag;
            minimum_bag.red = std::max(minimum_bag.red, bag.red);
            minimum_bag.green = std::max(minimum_bag.green, bag.green);
            minimum_bag.blue = std::max(minimum_bag.blue, bag.blue);
        }
        
        out += minimum_bag.power();
    }
    
    std::cout << out << std::endl;
}
