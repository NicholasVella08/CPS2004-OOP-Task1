#include <vector>
#include <string>

class Village {
public:
    int x;
    int y;
    int health;
    int owner;
    std::vector<int> resources;
    std::vector<std::string> buildings;
};


class Map {
public:
    Map(int width, int height) : width_(width), height_(height) {
        // Initialize the map with empty villages
        map_.resize(width * height);
    }

    int width() const { return width_; }
    int height() const { return height_; }

    Village& findVillage(int x, int y) {
        return map_[y * width_ + x];
    }

    const Village& findVillage(int x, int y) const {
        return map_[y * width_ + x];
    }

private:
    int width_;
    int height_;
    std::vector<Village> map_;
};
