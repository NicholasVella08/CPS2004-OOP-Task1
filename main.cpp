#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include "Village.h"
std::random_device rd;



int main() {

    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, MAP_WIDTH*MAP_HEIGHT-1);

    // Create a map object
    Map map;

    // Prompt the user for the number of players
    int numPlayers;
    do {
        std::cout << "Enter the number of players (at least one):\n ";
        std::cin >> numPlayers;
    }while(numPlayers<1);



    // Prompt the user for the number of AIs
    int numAIs;
    std::cout << "Enter the number of AIs: \n";
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

    for (int i = 0; i < numAIs; i++) {
        // Create a new village for the AI

        Village *village = new Village(i, i, 100, nullptr);
        // Create a new AI object and store it in the AIs vector
        AIs.push_back(new AI(village));

    }


    std::vector<std::pair<int, int>> villageLocations;

    std::vector<std::string> buildingTypes = {"Farm", "Gold Mine", "Lumber Mill"};
    std::vector<std::string> resourceTypes = {"Gold", "Wood", "Food"};

    for (int i = 0; i < numPlayers; i++) {

        int x, y;
        do {

            int index = dist(gen);

            x = index % MAP_WIDTH;

            y = index / MAP_WIDTH;

        } while (std::find(villageLocations.begin(), villageLocations.end(), std::make_pair(x, y)) != villageLocations.end());

        Village* village = new Village(x, y, 100, players[i]);

        players[i]->village = village;


        villageLocations.emplace_back(x, y);

        map.addVillage(village, x, y);


        players[i]->troops.emplace_back(Troop("Archer", 10, 5, 2, 2, 0));
        players[i]->troops.emplace_back(Troop("Knight", 10, 10, 5, 6, 0));
        players[i]->troops.emplace_back(Troop("Wizard", 10, 15, 10, 4, 0));

        //Initialize Buildings
        for (const std::string &buildingType: buildingTypes) {
            players[i]->buildings.emplace_back(Building(buildingType, 0));
        }


        //Initialize Resources
        for (const std::string &resourceType: resourceTypes) {
            players[i]->resources.emplace_back(Resource(resourceType, 10));
        }


    }


    //std::vector<std::pair<int, int>> villageLocations;
    std::cout << "start village mapping\n";
    for (int i = 0; i < numAIs; i++) {
        int x, y;
        do {
            int index = dist(gen);
            x = index % MAP_WIDTH;
            y = index / MAP_WIDTH;
        } while (std::find(villageLocations.begin(), villageLocations.end(), std::make_pair(x, y)) != villageLocations.end());
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

    int foodAmount = 0;
    int goldAmount = 0;
    int woodAmount = 0;

    int archerAmount;
    int knightAmount;
    int wizardAmount;

    int farmLevel;
    int goldMineLevel;
    int lumberMillLevel;

    int numArchers;
    int numKnights;
    int numWizards;

    Village *targetVillage = nullptr;
    std::vector<Troop> troops;


    // Game loop
    bool gameIsRunning = true;
    while (gameIsRunning) {
        std::cout << "start game\n";



        for (auto &player : players) {
            //friendly troops arrival



            std::cout << "player turn \n";

            //enemy troops arival




            if(player && player->village)
                player->village->earnResources();
                std::cout << "earn resource ";



            //want to output all resources and troops;
            std::cout << "Village of "<< player->name<<"\n";
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

            std::cout << "Total Food: "<<foodAmount;
            std::cout << "\nTotal Gold: "<<goldAmount;
            std::cout << "\nTotal Wood: "<<woodAmount<<"\n";

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


            std::cout << "----------------------\n";
            std::cout << "Total Archers: "<<archerAmount;
            std::cout << "\nTotal Knights: "<<knightAmount;
            std::cout << "\nTotal Wizards: "<<wizardAmount<<"\n";
            std::cout << "----------------------\n";
            std::cout << "\nLevel of Farm: "<<farmLevel;
            std::cout << "\nLevel of Gold Mine: "<<goldMineLevel;
            std::cout << "\nLevel of Lumber MIll: "<<lumberMillLevel;
            std::cout << "\n######################\n\n";
            do{

                std::cout << "1. Build or Upgrade Buildings\n";
                std::cout << "2. Train Troops\n";
                std::cout << "3. Attack Village\n";
                std::cout << "4. Surrender Village\n";
                std::cout << "5. Pass Turn\n\n";

                std::cout << "Enter your choice: \n";
                std::cin >> playerChoice;
                switch(playerChoice){
                    case 1: std::cout << "\n1. Farm\n";
                            std::cout << "2. Gold Mine \n";
                            std::cout << "3. Lumber MIll \n";

                            std::cout << "Which building you want to build or upgrade: \n";
                            std::cin >> buildingChoice;

                            switch(buildingChoice){
                                case 1: turn = player->village->buildOrUpgradeBuilding("Farm");
                                        break;
                                case 2: turn = player->village->buildOrUpgradeBuilding("Gold Mine");
                                        break;
                                case 3: turn = player->village->buildOrUpgradeBuilding("Lumber Mill");
                                        break;
                                default: std::cout<< "Invalid Choice\n";
                            }
                            break;
                    case 2: std::cout << "1. Archer\n";
                            std::cout << "2. Knight \n";
                            std::cout << "3. Wizard \n";
                            std::cout << "What troop do you like to train: \n";
                            std::cin >> troopChoice;

                            switch(troopChoice){
                                case 1: turn = player->village->trainTroop("Archer");
                                    break;
                                case 2: turn = player->village->trainTroop("Knight");
                                    break;
                                case 3: turn = player->village->trainTroop("Wizard");
                                    break;
                                default: std::cout<< "Invalid Choice\n";
                            }
                            break;

                    case 3: for (const auto &village : villages) {
                                std::cout << "Village at x: " << village.x << ", y: " << village.y << std::endl;
                            }

                            std::cout << "Select a village to attack by entering its x-coordinate: \n";
                            int x;
                            std::cin >> x;
                            std::cout << "Select a village to attack by entering its y-coordinate: \n";
                            int y;
                            std::cin >> y;

                            // Search for the village the player selected

                            for (auto &village : villages) {
                                if (village.x == x && village.y == y) {
                                    targetVillage = &village;
                                    break;
                                }
                            }

                            // Make sure a valid village was selected
                            if (targetVillage == nullptr) {
                                std::cout << "Invalid village selected.\n" << std::endl;

                            }

                            do{
                                std::cout << "Enter the number of archers you want to use for the attack: \n";

                                std::cin >> numArchers;
                                if(numArchers > archerAmount){
                                    std::cout << "You don't have that amount of archers: \n";
                                    numArchers = 0;
                                    correct =false;
                                }else{
                                    correct = true;
                                }
                            }while(correct == false);


                            do{
                                std::cout << "Enter the number of knights you want to use for the attack: \n";

                                std::cin >> numKnights;
                                if(numKnights > knightAmount){
                                    std::cout << "You don't have that amount of knights: \n";
                                    numKnights = 0;
                                    correct =false;
                                }else{
                                    correct = true;
                                }
                            }while(correct == false);

                            do{
                                std::cout << "Enter the number of wizards you want to use for the attack: \n";

                                std::cin >> numWizards;
                                if(numWizards > wizardAmount){
                                    std::cout << "You don't have that amount of wizards: \n";
                                    numWizards = 0;
                                    correct =false;
                                }else{
                                    correct = true;
                                }
                            }while(correct == false);


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
                            player->village->attack(*targetVillage, troops);
                            turn=true;
                            break;


                    case 4:
                            map.removeVillage(player->village);
                            std::cout<< "Village of "<< player<<"has been distroyed\n";
                            turn=true;
                            break;

                    case 5: turn = true;
                            break;

                    default: std::cout<< "Invalid Choice\n";
                             break;

                }
            }while(!turn);
            turn = false;
        }

        for (auto aiPlayer : AIs) {
            std::cout << "ai turn";
            aiPlayer->takeTurn();
        }

        // Check for win condition
        if (villages.size() == 1) {
            std::cout << "The owner of village " << villages[0].owner << " has won the game!" << std::endl;
            gameIsRunning = false;
        }

        //marching


        //end round


        //start round

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

