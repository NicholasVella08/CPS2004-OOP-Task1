#include <string>
#include <iostream>
#include <algorithm>
#include <vector>

class Player;  // Forward declaration of the Player class

// Full definition of the Troop class
class Troop {
public:
    std::string type;  // The type of the troop
    int health;  // The health of the troop
    int attack;  // The attack power of the troop
    int carryingCapacity;  // The carrying capacity of the troop (for resources)
    int marchingSpeed;  // The marching speed of the troop

    // Constructor for the Troop class
    Troop(const std::string& type, int health, int attack,
          int carryingCapacity, int marchingSpeed) :
            type(type), health(health), attack(attack),
            carryingCapacity(carryingCapacity), marchingSpeed(marchingSpeed) {}

};

class Village {
public:
    int x;  // The x-coordinate of the village on the map
    int y;  // The y-coordinate of the village on the map
    int health;  // The health of the village
    Player* owner;  // Pointer to the player who owns the village
    std::vector<std::string> resources;  // The resources that the village has available
    std::vector<std::string> buildings;  // The buildings in the village
    std::vector<Troop> troops;  // The troops stationed in the village

    // Default constructor for the Village class
    Village() : x(0), y(0), health(0), owner(nullptr) {}

    // Constructor for the Village class with arguments
    Village(int x, int y, int health, Player* owner) :
            x(x), y(y), health(health), owner(owner) {}

    Troop* trainTroop(const std::string& type) {
        // Check if the village has the resources to train the troop
        if ( std::find (resources.begin(), resources.end(), type) == resources.end()) {
            return nullptr;  // Return nullptr if the village doesn't have the resources
        }
        // Remove the training resources from the village's resources
        auto it = std::find(resources.begin(), resources.end(), type);
        resources.erase(it);

        // Create the new troop and add it to the village's troops
        Troop* newTroop = new Troop(type, 100, 10, 0, 10);
        troops.push_back(*newTroop);
        return newTroop;
    }

    // Attack the given village with the given troops
    void attack(Village& village, const std::vector<Troop>& troops) {
        // Calculate the total attack power of the attacking troops
        int totalAttackPower = 0;
        for (const Troop& troop : troops) {
            totalAttackPower += troop.attack;
        }

        // Reduce the health of the attacked target by the total attack power
        village.health = std::max(village.health - totalAttackPower, 0);

        // Check if the attack was successful (health of target village is zero or less)
        if (village.health <= 0) {
            // Attack was successful:
            // Transfer the ownership of the village to the attacker
            village.owner = owner;

            // Capture any resources in the village
            resources.insert(resources.end(), village.resources.begin(), village.resources.end());

            // Clear the resources of the captured village
            village.resources.clear();
        }
    }

    // Check if the village is under attack by enemy troops
    bool isUnderAttack() const;

    // Resolve any incoming attacks on the village
    void resolveAttacks();

    // Earn resources according to the village's resource-generating buildings
    void earnResources();



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

int main(){
    Village v;
    // Train a new troop of type "Archer"
    Troop* archer = v.trainTroop("Archer");
    return 0;
};
