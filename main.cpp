#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <map>
#include <random>
const int MAP_WIDTH = 50; // The width of the map (in grid cells)
const int MAP_HEIGHT = 50; // The height of the map (in grid cells)
std::random_device rd;

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

    // Constructor for the Troop class
    Troop(const std::string& type, int health, int attack,
          int carryingCapacity, int marchingSpeed) :
            type(type), health(health), attack(attack),
            carryingCapacity(carryingCapacity), marchingSpeed(marchingSpeed) {}

};

class MarchingTroops : public Troop {
public:
    // Additional properties for marching troops
    int distance;  // The distance to the target village
    int timeRemaining;  // The time remaining until the troops reach the target village

    // Constructor for the MarchingTroops class
    MarchingTroops(const std::string& type, int health, int attack,
                   int carryingCapacity, int marchingSpeed, int distance, int timeRemaining) :
            Troop(type, health, attack, carryingCapacity, marchingSpeed),
            distance(distance), timeRemaining(timeRemaining) {}

    // Method for updating the time remaining for the troops to reach the target village
    void updateTimeRemaining() {
        timeRemaining--;
    }
};
// Full definition of the Player class
class Player {
public:
    std::string name;  // The name of the player
    std::vector<Troop> troops;
    std::vector<Resource> resources;
    // Constructor for the Player class
    Player(const std::string& name) : name(name) {}
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
    std::vector<std::pair<Player *, std::vector<Troop>>> incomingAttacks_;


    // Default constructor for the Village class
    Village() : x(0), y(0), health(0), owner(nullptr) {}

    // Constructor for the Village class with arguments
    Village(int x, int y, int health, Player *owner) :
            x(x), y(y), health(health), owner(owner) {}

    Troop *trainTroop(const std::string &type) {
        int cost = 0;
        int carryingCapacity = 0;
        int attack = 0;
        if (type == "Archer") {
            cost = 10;
            carryingCapacity = 2;
            attack = 5;
        } else if (type == "Knight") {
            cost = 20;
            carryingCapacity = 5;
            attack = 10;
        } else if (type == "Wizard") {
            cost = 30;
            carryingCapacity = 10;
            attack = 15;
        } else {
            return nullptr;  // Invalid troop type
        }

        // Check if the village has enough resources to train the troop
        int food = 0;
        for (const Resource &resource: resources) {
            if (resource.type == "Food") {
                food += resource.amount;
            }
        }
        if (food < cost) {
            return nullptr;  // Return nullptr if the village doesn't have enough resources
        }

        // Remove the training resources from the village's resources
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

        // Create the new troop and add it to the village's troops
        Troop *newTroop = new Troop(type, 100, attack, carryingCapacity, 10);
        troops.push_back(*newTroop);
        return newTroop;
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

        //Check if the attack was successful or not
        if (!troops.empty()) {
            // Attack was successful
            targetVillage.health -= totalAttackPower;
            int carryingCapacity = 0;
            for (const auto &troop: troops) {
                carryingCapacity += troop.carryingCapacity;
            }
            int goldTaken = 0;
            int foodTaken = 0;
            int woodTaken = 0;
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
    }

    void updateResourcesAndTroops(int gold, int food, int wood, int archersKilled, int knightsKilled, int wizardsKilled) {
        // Add the taken resources to the attacker's resources
        resources.push_back(Resource("Food", food));
        resources.push_back(Resource("gold", gold));
        resources.push_back(Resource("wood", wood));
        // Subtract the killed troops from the attacker's troops
        auto it = troops.begin();
        while (archersKilled > 0 && it != troops.end()) {
            if (it->type == "Archer") {
                it = troops.erase(it);
                archersKilled--;
            } else {
                it++;
            }
        }
        it = troops.begin();
        while (knightsKilled > 0 && it != troops.end()) {
            if (it->type == "Knight") {
                it = troops.erase(it);
                knightsKilled--;
            } else {
                it++;
            }
        }
        it = troops.begin();
        while (wizardsKilled > 0 && it != troops.end()) {
            if (it->type == "Wizard") {
                it = troops.erase(it);
                wizardsKilled--;
            } else {
                it++;
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

    void buildOrUpgradeBuilding(const std::string& buildingType) {
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
            return;
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
            return;
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

    AI(Village* village) : village(village), gen(rd()), dist(0, MAP_WIDTH*MAP_HEIGHT-1) {}

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

//int main() {
//    std::cout << "Enter number of players: ";
//    int numPlayers;
//    std::cin >> numPlayers;
//    std::cout << "Enter number of AIs: ";
//    int numAIs;
//    std::cin >> numAIs;
//
//    // Create a map and generate villages for players and AIs
//    Map map_;
//    std::vector<Player*> players;
//    std::vector<AI> ais;
//    int totalNumVillages = numPlayers + numAIs;
//    std::vector<std::pair<int, int>> villageLocations;
//    for (int i = 0; i < totalNumVillages; i++) {
//        int x = i % MAP_WIDTH;
//        int y = i / MAP_HEIGHT;
//        Village* village = new Village(x, y, 100, nullptr);
//        if (i < numPlayers) {
//            Player* player = new Player("Player " + std::to_string(i+1));
//            village->owner = player;
//            players.push_back(player);
//        }
//
//        villageLocations.push_back({x, y});
//        map_.addVillage(village, x, y);
//    }
//
//    // Main game loop
//    bool gameIsRunning = true;
//    while (gameIsRunning) {
//        // ...
//    }
//
//    return 0;
//}



int main() {

    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, MAP_WIDTH*MAP_HEIGHT-1);

    // Create a map object
    Map map;

    // Prompt the user for the number of players
    int numPlayers;
    std::cout << "Enter the number of players: ";
    std::cin >> numPlayers;

    // Prompt the user for the number of AIs
    int numAIs;
    std::cout << "Enter the number of AIs: ";
    std::cin >> numAIs;

    // Create the player and AI objects
    std::vector<Player*> players;
    std::vector<AI*> AIs;
    for (int i = 1; i <= numPlayers; i++) {
        std::string playerName;
        std::cout << "Enter the name of player " << i << ": ";
        std::cin >> playerName;
        players.push_back(new Player(playerName));
    }
    for (int i = 1; i <= numAIs; i++) {
        Village *village = new Village(i, i, 100, nullptr);
        AIs.push_back(new AI(village));
    }


// Place the villages on the map
    std::vector<std::pair<int, int>> villageLocations;
    for (int i = 0; i < numPlayers; i++) {
        int index = dist(gen);
        int x = index % MAP_WIDTH;
        int y = index / MAP_WIDTH;
        while (std::find(villageLocations.begin(), villageLocations.end(), std::make_pair(x, y)) != villageLocations.end()) {
            index = dist(gen);
            x = index % MAP_WIDTH;
            y = index / MAP_WIDTH;
        }
        Village* village = new Village(x, y, 100, players[i]);
        villageLocations.emplace_back(x, y);
        map.addVillage(village, x, y);
    }
    for (int i = 0; i < numAIs; i++) {
        int index = dist(gen);
        int x = index % MAP_WIDTH;
        int y = index / MAP_WIDTH;
        while (std::find(villageLocations.begin(), villageLocations.end(), std::make_pair(x, y)) != villageLocations.end()) {
            index = dist(gen);
            x = index % MAP_WIDTH;
            y = index / MAP_HEIGHT;
        }
        AIs[i]->village->x = x;
        AIs[i]->village->y = y;
        map.addVillage(AIs[i]->village, x, y);
        villageLocations.emplace_back(x, y);
    }



    // Game loop
    bool gameIsRunning = true;
    while (gameIsRunning) {
        // Code to update game state, handle player input, etc.
        // ...
    }

    // Clean up memory
    for (auto player : players) {
        delete player;
    }
    for (auto AI : AIs) {
        delete AI;
    }
    for (auto &[village, coord] : map.villageLocations) {
        delete village;
    }
    return 0;
}

