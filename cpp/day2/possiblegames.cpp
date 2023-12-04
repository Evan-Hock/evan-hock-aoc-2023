#include <fstream>
#include <iostream>
#include <limits>
#include <map>
#include <sstream>

struct bag_t {
    int red, green, blue;
    
    bool exceeds(const bag_t& o) {
        return this->red > o.red || this->green > o.green || this->blue > o.blue;
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

const bag_t kCubeLimit{12, 13, 14};

// day 2 part 1
// output the sum of the IDs of which games are possible
int main() {
    std::ifstream input{"input.txt"};
    
    int out = 0;
    
    {
        for (int game_id = 1; input.ignore(std::numeric_limits<std::streamsize>::max(), ':'); ++game_id) {
            std::string line;
            std::getline(input, line);
            
            bool possible = true;
            std::istringstream liness{line};
            for (std::string cubeinfo; std::getline(liness, cubeinfo, ';'); ) {
                std::istringstream cubess{cubeinfo};
                bag_t bag;
                cubess >> bag;
                
                if (bag.exceeds(kCubeLimit)) {
                    possible = false;
                    break;
                }
            }
            
            if (possible) {
                out += game_id;
            }
        }
    }
    
    std::cout << out << std::endl;
}
