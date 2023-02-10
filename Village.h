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


    // Constructor for the Building class
    Building(const std::string& type, int level) :
            type(type), level(level) {}
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
    int movement = 3;


    // Constructor for the MarchingTroops class
    MarchingTroops(const std::string& type, int health, int attack, int carryingCapacity, int marchingSpeed, int marchTime, int destinationX, int destinationY, int amount, int movement):
                    Troop(type, health, attack, carryingCapacity, marchingSpeed, amount), Resource(type, amount), marchTime(marchTime), destinationX(destinationX), destinationY(destinationY), isMarching(true), movement(movement){}
};

class Village;


// Full definition of the Player class
class Player {
public:
    std::string name;  // The name of the player
    std::vector<Troop> troops;
    std::vector<Resource> resources;
    std::vector<Building> buildings;
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
    //std::vector<std::pair<Player *, std::vector<Troop>>> incomingAttacks_;


    // Default constructor for the Village class
    Village() : x(0), y(0), health(0), owner(nullptr) {}

    // Constructor for the Village class with arguments
    Village(int x, int y, int health, Player *owner) :
            x(x), y(y), health(health), owner(owner) {}

    bool trainTroop(const std::string &type) {

        int cost = 0;
        int carryingCapacity = 0;
        int attack = 0;
        int marchingSpeed = 0;

        if (type == "Archer") {
            cost = 10;
            carryingCapacity = 2;
            attack = 5;
            marchingSpeed = 2;
        } else if (type == "Knight") {
            cost = 20;
            carryingCapacity = 5;
            attack = 10;
            marchingSpeed = 6;
        } else if (type == "Wizard") {
            cost = 30;
            carryingCapacity = 10;
            attack = 15;
            marchingSpeed = 4;
        } else {
            return false;  // Invalid troop type
        }


        //Check if the village has enough resources to train the troop

        int food = 0;
        for (const Resource &resource: resources) {
            if (resource.type == "Food") {
                food += resource.amount;
            }
        }

        if (food < cost) {
            std::cout << "You don't have enough resources \n";
            return false;  // Return false if the village doesn't have enough resources
        }

        // Check if the troop type already exists
        auto iter = std::find_if(troops.begin(), troops.end(), [&](const Troop &r) { return r.type == type; });
        if (iter != troops.end()) {
            // If the troop type already exists, increment its amount by 2
            iter->amount ++;

        }

        for (auto &resource: resources) {
            if (resource.type == "Food") {
                resource.amount -= cost;
                break;
            }
        }

        std::cout << "A " << type << " was added successfully to your troop." << std::endl;
        return true;

    }

    // Attack the given village with the given troops
    std::vector<int> attack(Village &targetVillage, std::vector<Troop> &troops) {
        //int totalAttackPower = 0;
        int archersAttack = 0;
        int knightsAttack = 0;
        int wizardsAttack = 0;
        int attackPower = 0;
        int defensePower = 0;

        for (const Troop &troop: troops) {

            if (troop.type == "Archer") {
                archersAttack = troop.amount;
            } else if (troop.type == "Knight") {
                knightsAttack = troop.amount;
            } else if (troop.type == "Wizard") {
                wizardsAttack = troop.amount;
            }

        }
        attackPower = (archersAttack*5)+(knightsAttack*10)+(wizardsAttack*15);

        //int totalDefensePower = 0;
        int archersDefense = 0;
        int knightsDefense = 0;
        int wizardsDefense = 0;
        for (const Troop &troop: targetVillage.troops) {

            if (troop.type == "Archer") {
                archersDefense = troop.amount;
            } else if (troop.type == "Knight") {
                knightsDefense = troop.amount;;
            } else if (troop.type == "Wizard") {
                wizardsDefense = troop.amount;;
            }
        }
        defensePower = (archersDefense*5)+(knightsDefense*10)+(wizardsDefense*15);

        if (attackPower > defensePower) {
            std::cout << "Attacker wins!" << std::endl;

            //defense troops all killed
            archersDefense = 0;
            knightsDefense = 0;
            wizardsDefense = 0;



            while(archersAttack!=0){
                if(defensePower==0){
                    break;
                }
                defensePower -= 5;
                archersAttack--;
            }
            while(knightsAttack!=0){
                if(defensePower==0){
                    break;
                }
                defensePower -= 10;
                if(defensePower<0){
                    break;
                }
                knightsAttack--;
            }
            while(wizardsAttack!=0){
                if(defensePower==0){
                    break;
                }
                defensePower -= 15;
                if(defensePower<0){
                    break;
                }
                wizardsAttack--;
            }

            int totalCarry=0;
            int foodAmount =0;
            int woodAmount =0;
            int goldAmount =0;

            int foodTaken =0;
            int woodTaken =0;
            int goldTaken =0;

            totalCarry= archersAttack*2+knightsAttack*5+wizardsAttack*10;
            for (const Resource &resource : targetVillage.resources) {
                if (resource.type == "Food") {
                    foodAmount = resource.amount;

                }
                if (resource.type == "Wood") {
                    woodAmount = resource.amount;

                }
                if (resource.type == "Gold") {
                    goldAmount = resource.amount;

                }
            }

            while(totalCarry>0){
                if(foodAmount>0){
                    foodAmount--;
                    foodTaken++;
                    totalCarry--;
                    continue;
                }
                if(woodAmount>0){
                    woodAmount--;
                    woodTaken++;
                    totalCarry--;
                    continue;
                }
                if(goldAmount>0){
                    goldAmount--;
                    goldTaken++;
                    totalCarry--;
                    continue;
                }
            }



            std::cout << "Archers " << archersAttack<< std::endl;
            std::cout << "Knights " << knightsAttack<<std::endl;
            std::cout << "Wizards " << wizardsAttack<<std::endl;
            std::cout << "Food Taken " << foodTaken<< std::endl;
            std::cout << "Wood Taken " << woodTaken<<std::endl;
            std::cout << "Gold Taken " << goldTaken<<std::endl;
            std::vector<int> arr={goldTaken, foodTaken, woodTaken, archersAttack, knightsAttack, wizardsAttack, 1};
            return arr;
            //updateResourcesAndTroops(goldTaken, foodTaken, woodTaken, archersAttack, knightsAttack, wizardsAttack, 1);
            //targetVillage.updateResourcesAndTroops(goldTaken, foodTaken, woodTaken, archersDefense, knightsDefense, wizardsDefense, 2);


        } else {
            std::cout << "Defender wins!" << std::endl;

            //defense troops all killed
            archersAttack = 0;
            knightsAttack = 0;
            wizardsAttack = 0;



            while(archersDefense!=0){
                if(attackPower==0){
                    break;
                }
                attackPower -= 5;
                archersDefense--;
            }
            while(knightsDefense!=0){
                if(attackPower==0){
                    break;
                }
                attackPower -= 10;
                if(attackPower<0){
                    break;
                }
                knightsDefense--;
            }
            while(wizardsDefense!=0){
                if(attackPower==0){
                    break;
                }
                attackPower -= 15;
                if(attackPower<0){
                    break;
                }
                wizardsDefense--;
            }

            std::cout << "Defence Archers " << archersDefense<< std::endl;
            std::cout << "Defence Knights " << knightsDefense<<std::endl;
            std::cout << "Defence Wizards " << wizardsDefense<<std::endl;
            //targetVillage.updateResourcesAndTroops(0, 0, 0, archersDefense, knightsDefense, wizardsDefense, 2);
            std::vector<int> arr={0, 0, 0, archersDefense, knightsDefense, wizardsDefense, 2};
            return arr;
        }


    }

    void updateResourcesAndTroops(int gold, int food, int wood, int archersKilled, int knightsKilled, int wizardsKilled, int movement) {
        for (auto &marchingTroops: marching) {
            for (auto &resource: marchingTroops.resources) {
                if (resource.type == "Gold") {
                    resource.amount += gold;
                } else if (resource.type == "Food") {
                    resource.amount += food;
                } else if (resource.type == "Wood") {
                    resource.amount += wood;
                }
            }
            for (auto &troop: marchingTroops.troops) {
                if (troop.type == "Archer") {
                    troop.amount += archersKilled;
                } else if (troop.type == "Knight") {
                    troop.amount += knightsKilled;
                } else if (troop.type == "Wizard") {
                    troop.amount += wizardsKilled;
                }
            }
            marchingTroops.movement = movement;
        }
    }

    // Earn resources according to the village's resource-generating buildings
    void earnResources() {
        // Check if the village has any resource-generating buildings
        if (buildings.empty()) {
            return;  // No resource-generating buildings, so don't earn any resources
        }

        // Earn resources according to the village's resource-generating buildings
        for (const Building &building: buildings) {
            if (building.type == "Farm") {
                addResource("Food", 20 * building.level);
            }
            if (building.type == "Gold Mine") {
                addResource("Gold", 10 * building.level);
            }
            if (building.type == "Lumber Mill") {
                // Add 1 wood resource to the village's resources
                addResource("Wood", 15 * building.level);
            }
        }
    }

    void addResource(const std::string &type, int amount) {
        auto iter = std::find_if(resources.begin(), resources.end(), [&](const Resource &r) { return r.type == type; });
        if (iter != resources.end()) {
            iter->amount += amount;
        } else {
            resources.push_back(Resource(type, amount));
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
            Building newBuilding(buildingType, 1);
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
    std::vector<std::vector<Village *>> cells;

// Map from village pointers to the coordinates of the villages on the map
    std::map<Village *, std::pair<int, int>> villageLocations;

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
    void addVillage(Village *village, int x, int y) {
        cells[x][y] = village;
        villageLocations[village] = {x, y};
    }

// Remove the given village from the map
    void removeVillage(Village *village) {
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
        for (const auto &[village, coord]: villageLocations) {
            locations.emplace_back(coord);
        }
        return locations;
    }

};

class AI {
public:
    Village *village;
    std::mt19937 gen;
    std::uniform_int_distribution<> dist;
    std::random_device rd;

    //AI(Village* village) : village(village), gen(rd()), dist(0, MAP_WIDTH*MAP_HEIGHT-1) {}
    AI(Village *village) : village(village) {}


    Village *selectRandomVillage() {
        int villageIndex = dist(gen);
        return map_->cells[villageIndex % MAP_WIDTH][villageIndex / MAP_WIDTH];
    }

    void takeTurn() {

        // Earn resources
        village->earnResources();


        int ready = 0;
        std::vector<int> arr;
        do {
            // Decide on an action
            int action = dist(gen) % 3;

            if (action == 0) {
                std::cout<<"AI is attacking\n";
                // Attack a random village
                Village *targetVillage = selectRandomVillage();
                if (targetVillage != nullptr && targetVillage->owner != village->owner) {


                    arr = village->attack(*targetVillage, village->troops);
                    for (int i = 0; i < 7; i++) {
                        std::cout << arr[i] << " ";
                    }
                    if(arr[6]==1){
                        village->updateResourcesAndTroops(arr[0],arr[1],arr[2],arr[3],arr[4],arr[5],arr[6]);
                        targetVillage->updateResourcesAndTroops(arr[0],arr[1],arr[2],0,0,0,2);

                    }
                    if(arr[6]==2){
                        village->updateResourcesAndTroops(0,0,0,0,0,0,1);
                        targetVillage->updateResourcesAndTroops(arr[0],arr[1],arr[2],arr[3],arr[4],arr[5],2);

                    }
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
                    std::cout<<"AI is training a "<<troopType<<"\n";
                    village->trainTroop(troopType);
                    ready = 1;
                }
            } else if (action == 2) {
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
                    std::cout<<"AI is building a "<<buildingType<<"\n";
                    ready = 1;
                } else if (buildingType == "Gold Mine" && gold >= 15 && wood >= 20 && food >= 10) {
                    village->buildOrUpgradeBuilding(buildingType);
                    std::cout<<"AI is building a "<<buildingType<<"\n";
                    ready = 1;
                } else if (buildingType == "Lumber Mill" && gold >= 15 && wood >= 20 && food >= 10) {
                    village->buildOrUpgradeBuilding(buildingType);
                    std::cout<<"AI is building a "<<buildingType<<"\n";
                    ready = 1;
                }

            }


        } while (ready != 1);
    }

private:
    Map *map_;
};


#endif //CPS2004_OOP_TASK1_VILLAGE_H
