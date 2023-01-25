//
// Created by Nicholas Vella on 20/01/2023.
//

#ifndef CPS2004_OOP_TASK1_VILLAGE_H
#define CPS2004_OOP_TASK1_VILLAGE_H

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <random>
const int MAP_WIDTH = 50; // The width of the map (in grid cells)
const int MAP_HEIGHT = 50; // The height of the map (in grid cells)

class Resource {
public:
    std::string type; // The type of resource
    int amount; // The amount of the resource

// Constructor for the Resource class
    Resource(const std::string& type, int amount) : type(type), amount(amount) {}
};

class Building {
public:
    std::string type;  // The type of building (e.g. "Barracks", "Farm", etc.)
    int level;  // The level of the building (e.g. 1, 2, 3, etc.)
    int cost;  // The cost to build or upgrade the building

    // Constructor for the Building class
    Building(const std::string& type, int level, int cost) :
            type(type), level(level), cost(cost) {}
};

// Full definition of the Troop class
class Troop {
public:
    std::string type;  // The type of the troop
    int health;  // The health of the troop
    int attack;  // The attack power of the troop
    int carryingCapacity;  // The carrying capacity of the troop (for resources)
    int marchingSpeed;  // The marching speed of the troop
    int amount;

    // Constructor for the Troop class
    Troop(const std::string& type, int health, int attack,
          int carryingCapacity, int marchingSpeed,int amount) :
            type(type), health(health), attack(attack),
            carryingCapacity(carryingCapacity), marchingSpeed(marchingSpeed), amount(amount){}

};

class MarchingTroops : public Troop, public Resource {
public:
    int marchTime; // The time it takes for the troops to reach their destination
    int destinationX; // The x-coordinate of the destination
    int destinationY; // The y-coordinate of the destination
    bool isMarching; // Whether or not the troops are currently marching
    std::vector<Resource> resources;
    std::vector<Troop> troops;


    // Constructor for the MarchingTroops class
    MarchingTroops(const std::string& type, int health, int attack, int carryingCapacity, int marchingSpeed, int marchTime, int destinationX, int destinationY, int amount)
            : Troop(type, health, attack, carryingCapacity, marchingSpeed, amount), Resource(type, amount), marchTime(marchTime), destinationX(destinationX), destinationY(destinationY), isMarching(true){}
};

class Village;


// Full definition of the Player class
class Player {
public:
    std::string name;  // The name of the player
    std::vector<Troop> troops;
    std::vector<Resource> resources;
    Village* village;


    // Constructor for the Player class
    Player(const std::string& name) : name(name) {}
    Player(std::string name, Village* village) : name(name), village(village) {}

};

class Village {
public:
    int x;  // The x-coordinate of the village on the map
    int y;  // The y-coordinate of the village on the map
    int health;  // The health of the village
    Player *owner;  // Pointer to the player who owns the village
    std::vector<Resource> resources;  // The resources that the village has available
    std::vector<Building> buildings;  // The buildings in the village
    std::vector<Troop> troops;  // The troops stationed in the village
    std::vector<MarchingTroops> marching;
    std::vector<std::pair<Player *, std::vector<Troop>>> incomingAttacks_;


    // Default constructor for the Village class
    Village() : x(0), y(0), health(0), owner(nullptr) {}

    // Constructor for the Village class with arguments
    Village(int x, int y, int health, Player *owner) :
            x(x), y(y), health(health), owner(owner) {}

    bool trainTroop(const std::string &type) {
        int cost = 0;
        int carryingCapacity = 0;
        int attack = 0;
        int marchingSpeed=0;

        if (type == "Archer") {
            cost = 10;
            carryingCapacity = 2;
            attack = 5;
            marchingSpeed=2;
        } else if (type == "Knight") {
            cost = 20;
            carryingCapacity = 5;
            attack = 10;
            marchingSpeed=6;
        } else if (type == "Wizard") {
            cost = 30;
            carryingCapacity = 10;
            attack = 15;
            marchingSpeed=4;
        } else {
            return false;  // Invalid troop type
        }

        // Check if the village has enough resources to train the troop
        int food = 0;
        for (const Resource &resource: resources) {
            if (resource.type == "Food") {
                food += resource.amount;
            }
        }
        if (food < cost) {
            return false;  // Return nullptr if the village doesn't have enough resources
        }

        // check if the troop type already exist
        for (auto &troop: troops) {
            if (troop.type == type) {
                // remove the training resources
                for (auto it = resources.begin(); it != resources.end();) {
                    if (it->type == "Food") {
                        if (it->amount > cost) {
                            it->amount -= cost;
                        } else {
                            resources.erase(it);
                        }
                        break;
                    }
                }
                // add the amount of the troops
                std::cout << "A "<<type<<" was added successfully to your troop.";
                troop.amount++;
                return true;
            }
        }
        // If the troop type doesn't exist, remove the training resources from the village's resources and create a new troop
        for (auto it = resources.begin(); it != resources.end(); ++it) {
            if (it->type == "Food") {
                if (it->amount > cost) {
                    it->amount -= cost;
                } else {
                    resources.erase(it);
                }
                break;
            }
        }
        Troop *newTroop = new Troop(type, 100, attack, carryingCapacity, marchingSpeed, 1);
        troops.push_back(*newTroop);
        return true;
    }


    // Attack the given village with the given troops
    void attack(Village& targetVillage, std::vector<Troop>& troops) {
        int totalAttackPower = 0;
        int archersKilled = 0;
        int knightsKilled = 0;
        int wizardsKilled = 0;
        for (const Troop &troop: troops) {
            totalAttackPower += troop.attack;
            if (troop.type == "Archer") {
                archersKilled++;
            } else if (troop.type == "Knight") {
                knightsKilled++;
            } else if (troop.type == "Wizard") {
                wizardsKilled++;
            }
        }

        int totalDefensePower = 0;
        int defenderArchersKilled = 0;
        int defenderKnightsKilled = 0;
        int defenderWizardsKilled = 0;
        for (const Troop &troop: targetVillage.troops) {
            totalDefensePower += troop.attack;
            if (troop.type == "Archer") {
                defenderArchersKilled++;
            } else if (troop.type == "Knight") {
                defenderKnightsKilled++;
            } else if (troop.type == "Wizard") {
                defenderWizardsKilled++;
            }
        }

        // Kill troops from both sides until total attack power of the opposing army is reached
        int attackPower = 0;
        int defensePower = 0;
        for (auto attacker = troops.begin(); attacker != troops.end();) {
            if (attackPower >= totalDefensePower) {
                break;
            }
            attackPower += attacker->attack;
            ++attacker;
        }
        for (auto defender = targetVillage.troops.begin(); defender != targetVillage.troops.end();) {
            if (defensePower >= totalAttackPower) {
                break;
            }
            defensePower += defender->attack;
            ++defender;
        }
        int goldTaken = 0;
        int foodTaken = 0;
        int woodTaken = 0;
        //Check if the attack was successful or not
        if (!troops.empty()) {
            // Attack was successful
            targetVillage.health -= totalAttackPower;
            int carryingCapacity = 0;
            for (const auto &troop: troops) {
                carryingCapacity += troop.amount * troop.carryingCapacity;
            }

            for (auto it = targetVillage.resources.begin(); it != targetVillage.resources.end();) {
                if (carryingCapacity > 0) {
                    if (it->type == "gold") goldTaken += it->amount;
                    if (it->type == "food") foodTaken += it->amount;
                    if (it->type == "wood") woodTaken += it->amount;
                    carryingCapacity -= it->amount;
                    it = targetVillage.resources.erase(it);
                } else {
                    break;
                }
            }
            std::cout << "The attacker lost " << archersKilled << " archers, " << knightsKilled << " knights and "
                      << wizardsKilled << " wizards." << std::endl;
            std::cout << "The defender lost " << defenderArchersKilled << " archers, " << defenderKnightsKilled
                      << " knights and " << defenderWizardsKilled << " wizards." << std::endl;
            std::cout << "The attacker took " << goldTaken << " gold, " << foodTaken << " food and " << woodTaken
                      << " wood." << std::endl;
        }

        updateResourcesAndTroops(goldTaken, foodTaken, woodTaken, archersKilled, knightsKilled, wizardsKilled);
    }

    void updateResourcesAndTroops(int gold, int food, int wood, int archersKilled, int knightsKilled, int wizardsKilled) {
        for (auto &marchingTroops : marching) {
            for (auto &resource : marchingTroops.resources) {
                if (resource.type == "Gold") {
                    resource.amount += gold;
                } else if (resource.type == "Food") {
                    resource.amount += food;
                } else if (resource.type == "Wood") {
                    resource.amount += wood;
                }
            }
            for (auto &troop : marchingTroops.troops) {
                if (troop.type == "Archer") {
                    troop.amount += archersKilled;
                } else if (troop.type == "Knight") {
                    troop.amount += knightsKilled;
                } else if (troop.type == "Wizard") {
                    troop.amount += wizardsKilled;
                }
            }
        }
    }

    // Check if the village is under attack by enemy troops
    bool isUnderAttack() const {
        // Check if there are any incoming troops that have not yet arrive at the village
        for(const Troop& troop : troops) {
            if (troop.marchingSpeed > 0) {
                return true;  // There is an incoming troop, so the village is under attack
            }
        }
        return false;  // No incoming troops, so the village is not under attack
    }

    void resolveAttacks() {
        // Check if the village is under attack
        if (!isUnderAttack()) {
            return;  // No attacks to resolve
        }

        // Calculate the total attack power of the defending troops
        int totalDefendingPower = 0;
        for (const Troop& troop : troops) {
            totalDefendingPower += troop.attack;
        }

        // Loop through all incoming attacks and resolve them one by one
        for (const auto& attack : incomingAttacks_) {
            // Calculate the total attack power of the attacking troops
            int totalAttackingPower = 0;
            for (const Troop& troop : attack.second) {
                totalAttackingPower += troop.attack;
            }

            // Check if the attack was successful (defending power is less than attacking power)
            if (totalDefendingPower < totalAttackingPower) {
                // Attack was successful:
                // Transfer the ownership of the village to the attacker
                owner = attack.first;

                // Capture any resources in the village
                attack.first->resources.insert(attack.first->resources.end(), resources.begin(), resources.end());

                // Clear the resources of the captured village
                resources.clear();
            }

            // Reduce the health of the defending village by the total attacking power
            health = std::max(health - totalAttackingPower, 0);
        }

        // Clear all incoming attacks
        incomingAttacks_.clear();
    }

// Earn resources according to the village's resource-generating buildings
    void earnResources() {
        // Check if the village has any resource-generating buildings
        if (buildings.empty()) {
            return;  // No resource-generating buildings, so don't earn any resources
        }

        // Earn resources according to the village's resource-generating buildings
        for (const Building& building : buildings) {
            if (building.type == "Farm") {
                resources.push_back(Resource("Food", 20 * building.level));
            }
            if (building.type == "Gold Mine") {
                resources.push_back(Resource("Gold", 10 * building.level));
            }
            if (building.type == "Lumber Mill") {
                // Add 1 wood resource to the village's resources
                resources.push_back(Resource("Wood", 15 * building.level));
            }
        }
    }

    bool buildOrUpgradeBuilding(const std::string& buildingType) {
        // Check if the village has the resources to build or upgrade the building
        bool hasResources = true;
        int goldCost = 0;
        int woodCost = 0;
        int foodCost = 0;
        if (buildingType == "Farm") {
            goldCost = 10;
            woodCost = 20;
            foodCost = 15;
        } else if (buildingType == "Gold Mine") {
            goldCost = 15;
            woodCost = 20;
            foodCost = 10;
        } else if (buildingType == "Lumber Mill") {
            goldCost = 15;
            woodCost = 20;
            foodCost = 10;
        } else {
            std::cout << "Invalid building type." << std::endl;
            return false;
        }

        //Check if the village has the resources to build or upgrade the building
        for (const Resource &resource: resources) {
            if (resource.type == "Gold" && resource.amount < goldCost) {
                hasResources = false;
                break;
            }
            if (resource.type == "Wood" && resource.amount < woodCost) {
                hasResources = false;
                break;
            }
            if (resource.type == "Food" && resource.amount < foodCost) {
                hasResources = false;
                break;
            }
        }
        if (!hasResources) {
            std::cout << "The village does not have enough resources to build or upgrade this building." << std::endl;
            return false;
        }

        // Remove the building cost resources from the village's resources
        for (auto it = resources.begin(); it != resources.end(); ++it) {
            if (it->type == "Gold" && it->amount >= goldCost) {
                it->amount -= goldCost;
            }
            if (it->type == "Wood" && it->amount >= woodCost) {
                it->amount -= woodCost;
            }
            if (it->type == "Food" && it->amount >= foodCost) {
                it->amount -= foodCost;
            }
        }

        // Check if the village already has a building of the given type
        bool buildingExists = false;
        int buildingLevel = 0;
        for (Building &building: buildings) {
            if (building.type == buildingType) {
                buildingExists = true;
                buildingLevel = building.level;
                break;
            }
        }

        // If the building does not exist, add a new building of the given type to the village's buildings
        if (!buildingExists) {
            Building newBuilding(buildingType, 1, 0);
            buildings.push_back(newBuilding);
        } else {
            // Upgrade the building if it already exists
            for (Building &building: buildings) {
                if (building.type == buildingType) {
                    building.level += 1;
                    break;
                }
            }
        }
        std::cout << buildingType << " has been built or upgraded to level " << buildingLevel + 1;
        return true;
    }

};

// Full definition of the Map class
class Map {
public:
// The 2D grid of map cells (each containing a pointer to the village at that location, if any)
    std::vector<std::vector<Village*>> cells;

// Map from village pointers to the coordinates of the villages on the map
    std::map<Village*, std::pair<int, int>> villageLocations;

// Default constructor for the Map class
    Map() {
        // Initialize the map cells to be empty (i.e. no villages present)
        cells.resize(MAP_WIDTH);
        for (int i = 0; i < MAP_WIDTH; i++) {
            cells[i].resize(MAP_HEIGHT);
            for (int j = 0; j < MAP_HEIGHT; j++) {
                cells[i][j] = nullptr;
            }
        }
    }

// Add the given village to the map at the given coordinates
    void addVillage(Village* village, int x, int y) {
        cells[x][y] = village;
        villageLocations[village] = {x, y};
    }

// Remove the given village from the map
    void removeVillage(Village* village) {
        std::pair<int, int> villageLocation = villageLocations[village];
        cells[villageLocation.first][villageLocation.second] = nullptr;
        villageLocations.erase(village);
    }

// Check if the given coordinates are within the bounds of the map
    bool isValidLocation(int x, int y) const {
        return x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT;
    }

    std::vector<std::pair<int, int>> getVillageLocations() {
        std::vector<std::pair<int, int>> locations;
        for (const auto& [village, coord] : villageLocations) {
            locations.emplace_back(coord);
        }
        return locations;
    }

};

class AI {
public:
    Village* village;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
    std::random_device rd;

    //AI(Village* village) : village(village), gen(rd()), dist(0, MAP_WIDTH*MAP_HEIGHT-1) {}
    AI(Village *village) : village(village) { }



    Village* selectRandomVillage() {
        int villageIndex = dist(gen);
        return map_->cells[villageIndex % MAP_WIDTH][villageIndex / MAP_WIDTH];
    }

    void takeTurn() {
        // Earn resources
        village->earnResources();

        // Check if the village is under attack
        village->resolveAttacks();
        int ready = 0;
        do {
            // Decide on an action
            int action = dist(gen) % 3;
            if (action == 0) {
                // Attack a random village
                Village *targetVillage = selectRandomVillage();
                if (targetVillage != nullptr && targetVillage->owner != village->owner) {
                    village->attack(*selectRandomVillage(), village->troops);
                    ready = 1;
                }
            } else if (action == 1) {
                // Train a random type of troop
                std::vector<std::string> troopTypes = {"Archer", "Knight", "Wizard"};
                int troopTypeIndex = dist(gen) % troopTypes.size();
                std::string troopType = troopTypes[troopTypeIndex];

                int food = 0;
                for (const Resource &resource: village->resources) {
                    if (resource.type == "Food") {
                        food = resource.amount;
                        break;
                    }
                }

                int cost = 0;
                if (troopType == "Archer") {
                    cost = 10;
                } else if (troopType == "Knight") {
                    cost = 20;
                } else if (troopType == "Wizard") {
                    cost = 30;
                }

                if (food >= cost) {
                    village->trainTroop(troopType);
                    ready = 1;
                }
            }else if (action == 2) {
                // Build or upgrade a random type of building
                std::vector<std::string> buildingTypes = {"Farm", "Gold Mine", "Lumber Mill"};
                int buildingTypeIndex = dist(gen) % buildingTypes.size();
                std::string buildingType = buildingTypes[buildingTypeIndex];

                // check if the village has enough resources to build or upgrade the building
                int gold = 0, wood = 0, food = 0;
                for (const Resource &resource: village->resources) {
                    if (resource.type == "Gold") gold += resource.amount;
                    if (resource.type == "Wood") wood += resource.amount;
                    if (resource.type == "Food") food += resource.amount;
                }

                if (buildingType == "Farm" && gold >= 25 && wood >= 20 && food >= 15) {
                    village->buildOrUpgradeBuilding(buildingType);
                    ready = 1;
                } else if (buildingType == "Gold Mine" && gold >= 15 && wood >= 20 && food >= 10) {
                    village->buildOrUpgradeBuilding(buildingType);
                    ready = 1;
                }
                else if (buildingType == "Lumber Mill" && gold >= 15 && wood >= 20 && food >= 10) {
                    village->buildOrUpgradeBuilding(buildingType);
                    ready = 1;
                }

            }


        } while (ready == 1);
    }
private:
    Map* map_;
};




#endif //CPS2004_OOP_TASK1_VILLAGE_H
