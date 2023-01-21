#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
const int MAP_WIDTH = 50; // The width of the map (in grid cells)
const int MAP_HEIGHT = 50; // The height of the map (in grid cells)
#include "Village.h";
std::random_device rd;



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
    std::vector<Village> villages;
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

    bool turn=false;
    bool correct =false;
    int playerChoice;
    int buildingChoice;
    int troopChoice;

    int foodAmount;
    int goldAmount;
    int woodAmount;

    int archerAmount;
    int knightAmount;
    int wizardAmount;

    int farmLevel;
    int goldMineLevel;
    int lumberMillLevel;

    int numArchers;
    int numKnights;
    int numWizards;

    // Game loop
    bool gameIsRunning = true;
    while (gameIsRunning) {
        for (auto &player : players) {
            player->village->earnResources();
            //want to output all resources and troops;
            std::cout << "Village of "<< player;
            foodAmount = 0;
            goldAmount = 0;
            woodAmount = 0;
            archerAmount = 0;
            knightAmount = 0;
            wizardAmount = 0;
            farmLevel = 0;
            goldMineLevel = 0;
            lumberMillLevel = 0;


            for (const Resource &resource : player->resources) {
                if (resource.type == "Food") {
                    foodAmount = resource.amount;
                    break;
                }
            }

            for (const Resource &resource : player->resources) {
                if (resource.type == "Gold") {
                    goldAmount = resource.amount;
                    break;
                }
            }

            for (const Resource &resource : player->resources) {
                if (resource.type == "Wood") {
                    woodAmount = resource.amount;
                    break;
                }
            }

            for (const Troop& troop : player->village->troops) {
                if (troop.type == "Archers") {
                    archerAmount += troop.amount;
                }
            }

            for (const Troop& troop : player->village->troops) {
                if (troop.type == "Knights") {
                    knightAmount += troop.amount;
                }
            }

            for (const Troop& troop : player->village->troops) {
                if (troop.type == "Wizards") {
                    wizardAmount += troop.amount;
                }
            }


            for (const Building& building : player->village->buildings) {
                if (building.type == "Farm") {
                    farmLevel = building.level;
                    break;
                }
            }


            for (const Building& building : player->village->buildings) {
                if (building.type == "Gold Mine") {
                    goldMineLevel = building.level;
                    break;
                }
            }


            for (const Building& building : player->village->buildings) {
                if (building.type == "Lumber Mill") {
                    lumberMillLevel = building.level;
                    break;
                }
            }

            std::cout << "Total Food: "<<foodAmount;
            std::cout << "Total Gold: "<<goldAmount;
            std::cout << "Total Wood: "<<woodAmount;
            std::cout << "----------------------";
            std::cout << "Total Archers: "<<archerAmount;
            std::cout << "Total Knights: "<<knightAmount;
            std::cout << "Total Wizards: "<<wizardAmount;
            std::cout << "----------------------";
            std::cout << "Level of Farm: "<<farmLevel;
            std::cout << "Level of Gold Mine: "<<goldMineLevel;
            std::cout << "Level of Lumber MIll: "<<lumberMillLevel;
            std::cout << "######################\n\n";
            std::cout << "1. Build or Upgrade Buildings";
            std::cout << "2. Train Troops";
            std::cout << "3. Attack Village";
            std::cout << "4. Surrender Village";
            std::cout << "5. Pass Turn\n";
            do{
                std::cout << "Enter your choice: ";
                std::cin >> playerChoice;
                switch(playerChoice){
                    case 1: std::cout << "1. Farm";
                            std::cout << "2. Gold Mine ";
                            std::cout << "3. Lumber MIll ";

                            std::cout << "Which building you want to build or upgrade: ";
                            std::cin >> buildingChoice;

                            switch(buildingChoice){
                                case 1: turn = player->village->buildOrUpgradeBuilding("Farm");
                                        break;
                                case 2: turn = player->village->buildOrUpgradeBuilding("Gold Mine");
                                        break;
                                case 3: turn = player->village->buildOrUpgradeBuilding("Lumber Mill");
                                        break;
                                default: std::cout<< "Invalid Choice";
                            }
                            break;
                    case 2: std::cout << "1. Archer";
                            std::cout << "2. Knight ";
                            std::cout << "3. Wizard ";
                            std::cout << "What troop do you like to train: ";
                            std::cin >> troopChoice;

                            switch(troopChoice){
                                case 1: turn = player->village->trainTroop("Archer");
                                    break;
                                case 2: turn = player->village->trainTroop("Knight");
                                    break;
                                case 3: turn = player->village->trainTroop("Wizard");
                                    break;
                                default: std::cout<< "Invalid Choice";
                            }
                            break;

                    case 3: for (const auto &village : villages) {
                                std::cout << "Village at x: " << village.x << ", y: " << village.y << std::endl;
                            }

                            std::cout << "Select a village to attack by entering its x-coordinate: ";
                            int x;
                            std::cin >> x;
                            std::cout << "Select a village to attack by entering its y-coordinate: ";
                            int y;
                            std::cin >> y;

                            // Search for the village the player selected
                            Village *targetVillage = nullptr;
                            for (auto &village : villages) {
                                if (village.x == x && village.y == y) {
                                    targetVillage = &village;
                                    break;
                                }
                            }

                            // Make sure a valid village was selected
                            if (targetVillage == nullptr) {
                                std::cout << "Invalid village selected." << std::endl;

                            }

                            do{
                                std::cout << "Enter the number of archers you want to use for the attack: ";

                                std::cin >> numArchers;
                                if(numArchers > archerAmount){
                                    std::cout << "You don't have that amount of archers: ";
                                    numArchers = 0;
                                    correct =false;
                                }else{
                                    correct = true;
                                }
                            }while(correct == false);


                            do{
                                std::cout << "Enter the number of knights you want to use for the attack: ";

                                std::cin >> numKnights;
                                if(numKnights > knightAmount){
                                    std::cout << "You don't have that amount of knights: ";
                                    numKnights = 0;
                                    correct =false;
                                }else{
                                    correct = true;
                                }
                            }while(correct == false);

                            do{
                                std::cout << "Enter the number of wizards you want to use for the attack: ";

                                std::cin >> numWizards;
                                if(numWizards > wizardAmount){
                                    std::cout << "You don't have that amount of wizards: ";
                                    numWizards = 0;
                                    correct =false;
                                }else{
                                    correct = true;
                                }
                            }while(correct == false);

                            std::vector<Troop> troops;
                            for (int i = 0; i < numArchers; i++) {
                                troops.push_back(Troop("Archer", 100, 5, 2, 2, numArchers));
                            }
                            for (int i = 0; i < numWizards; i++) {
                                troops.push_back(Troop("Wizard", 100, 15, 10, 4, numWizards));
                            }
                            for (int i = 0; i < numKnights; i++) {
                                troops.push_back(Troop("Knight", 100, 10, 5, 6, numKnights));
                            }


                            // Now you can call the attack method
                            player->village.attack(*targetVillage, troops);
                            turn=true;
                            break;

                    case 4: map.removeVillage(player->village);
                            std::cout<< "Village of "<< player<<"has been distroyed";
                            turn=true;
                            break;

                    case 5: turn = true;
                            break;

                    default: std::cout<< "Invalid Choice";

                }
            }while(turn == false);
            turn = false;
        }
        for (auto aiPlayer : AIs) {
            aiPlayer->takeTurn();
        }

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

