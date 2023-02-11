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
    std::vector<MarchingTroops> marching;
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


        players[i]->village->troops.emplace_back(Troop("Archer", 10, 5, 2, 2, 0));
        players[i]->village->troops.emplace_back(Troop("Knight", 10, 10, 5, 6, 0));
        players[i]->village->troops.emplace_back(Troop("Wizard", 10, 15, 10, 4, 0));

        MarchingTroops marchingTroop("",0,0,0,0,0,0,0,0,3);
        players[i]->village->marching.push_back(marchingTroop);

        //players[i]->village->marching.emplace_back(MarchingTroops("",0,0,0,0,0,0,0,0,3));
        //Initialize Buildings
        for (const std::string &buildingType: buildingTypes) {
            players[i]->village->buildings.emplace_back(Building(buildingType, 0));
        }


        //Initialize Resources
        for (const std::string &resourceType: resourceTypes) {
            players[i]->village->resources.emplace_back(Resource(resourceType, 40));
        }


    }


    //std::vector<std::pair<int, int>> villageLocations;
    std::cout << "Start Village mapping\n";
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

        AIs[i]->village->troops.emplace_back(Troop("Archer", 10, 5, 2, 2, 0));
        AIs[i]->village->troops.emplace_back(Troop("Knight", 10, 10, 5, 6, 0));
        AIs[i]->village->troops.emplace_back(Troop("Wizard", 10, 15, 10, 4, 0));

        //Initialize Buildings
        for (const std::string &buildingType: buildingTypes) {
            AIs[i]->village->buildings.emplace_back(Building(buildingType, 0));
        }


        //Initialize Resources
        for (const std::string &resourceType: resourceTypes) {
            AIs[i]->village->resources.emplace_back(Resource(resourceType, 40));
        }



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

    bool deletePlayer = false;

    Village *targetVillage = nullptr;
    std::vector<Troop> troops;
    std::vector<int> arr;

    int round = 1;
    int goldTaken = 0;
    int foodTaken = 0;
    int woodTaken = 0;
    int archerReturned = 0;
    int knightReturned = 0;
    int wizardReturned = 0;

    // Game loop
    bool gameIsRunning = true;
    while (gameIsRunning) {
        std::cout << "Round "<<round<<" has started.\n";



        for (auto &player : players) {
            std::cout << "\nVillage of "<< player->name<<"\n";

            goldTaken = 0;
            foodTaken = 0;
            woodTaken = 0;
            archerReturned = 0;
            knightReturned = 0;
            wizardReturned = 0;

            for (int i = 0; i < player->village->marching.size(); i++) {
                int movement = player->village->marching[i].movement;

                // Do something with the movement variable
                //Friendly troops arrival
                if(movement==1){
                    if (!player->village->marching.empty()) {
                        for (auto &marchingTroops: player->village->marching) {

                            for (auto resource: marchingTroops.resources) {
                                if (resource.type == "Gold") {
                                    goldTaken = resource.amount;
                                    resource.amount = 0;
                                } else if (resource.type == "Food") {
                                    foodTaken = resource.amount;
                                    resource.amount = 0;
                                } else if (resource.type == "Wood") {
                                    woodTaken = resource.amount;
                                    resource.amount = 0;
                                }
                            }
                            for (auto &troop: marchingTroops.troops) {
                                if (troop.type == "Archer") {
                                    archerReturned = troop.amount;
                                    troop.amount = 0;
                                } else if (troop.type == "Knight") {
                                    knightReturned = troop.amount;
                                    troop.amount = 0;
                                } else if (troop.type == "Wizard") {
                                    wizardReturned = troop.amount;
                                    troop.amount = 0;
                                }
                            }
                            marchingTroops.movement=3;
                        }
                    }

                    for (Troop &troop : player->village->troops) {
                        if (troop.type == "Archer") {
                            troop.amount += archerReturned;
                            break;
                        }
                    }
                    for (Troop &troop : player->village->troops) {
                        if (troop.type == "Knight") {
                            troop.amount += knightReturned;
                            break;
                        }
                    }
                    for (Troop &troop : player->village->troops) {
                        if (troop.type == "Wizard") {
                            troop.amount += wizardReturned;
                            break;
                        }
                    }
                    for ( Resource resource : player->village->resources) {
                        if (resource.type == "Food") {
                            resource.amount += foodTaken;
                            break;
                        }
                    }
                    for ( Resource resource : player->village->resources) {
                        if (resource.type == "Wood") {
                            resource.amount += woodTaken;
                            break;
                        }
                    }
                    for ( Resource resource : player->village->resources) {
                        if (resource.type == "Gold") {
                            resource.amount += goldTaken;
                            break;
                        }
                    }

                }

                //enemy troops arrival
                if(movement==2){

                    for (auto marchingTroops: player->village->marching) {
                        for (auto resource: marchingTroops.resources) {
                            if (resource.type == "Gold") {
                                goldTaken += resource.amount;
                                resource.amount = 0;
                            } else if (resource.type == "Food") {
                                foodTaken += resource.amount;
                                resource.amount = 0;
                            } else if (resource.type == "Wood") {
                                woodTaken += resource.amount;
                                resource.amount = 0;
                            }
                        }
                        for (auto troop: marchingTroops.troops) {
                            if (troop.type == "Archer") {
                                archerReturned += troop.amount;
                                troop.amount = 0;
                            } else if (troop.type == "Knight") {
                                knightReturned += troop.amount;
                                troop.amount = 0;
                            } else if (troop.type == "Wizard") {
                                wizardReturned += troop.amount;
                                troop.amount = 0;
                            }
                        }
                        marchingTroops.movement=3;
                    }


                    for (Troop &troop : player->village->troops) {
                        if (troop.type == "Archer") {
                            troop.amount = 0;
                            troop.amount += archerReturned;
                            break;
                        }
                    }
                    for (Troop &troop : player->village->troops) {
                        if (troop.type == "Knight") {
                            troop.amount = 0;
                            troop.amount += knightReturned;
                            break;
                        }
                    }
                    for (Troop &troop : player->village->troops) {
                        if (troop.type == "Wizard") {
                            troop.amount = 0;
                            troop.amount += wizardReturned;
                            break;
                        }
                    }
                    for ( Resource resource : player->village->resources) {
                        if (resource.type == "Food") {
                            resource.amount -= foodTaken;
                            break;
                        }
                    }
                    for ( Resource resource : player->village->resources) {
                        if (resource.type == "Wood") {
                            resource.amount -= woodTaken;
                            break;
                        }
                    }
                    for ( Resource resource : player->village->resources) {
                        if (resource.type == "Gold") {
                            resource.amount -= goldTaken;
                            break;
                        }
                    }

                }
            }

            //Earn resources
            if(player && player->village)
                player->village->earnResources();
                std::cout << "Resource earned from buildings\n";

            //want to output all resources and troops;

            foodAmount = 0;
            goldAmount = 0;
            woodAmount = 0;
            archerAmount = 0;
            knightAmount = 0;
            wizardAmount = 0;
            farmLevel = 0;
            goldMineLevel = 0;
            lumberMillLevel = 0;

            for (const Resource &resource : player->village->resources) {
                if (resource.type == "Food") {
                    foodAmount = resource.amount;
                    break;
                }
            }
            for (const Resource &resource : player->village->resources) {
                if (resource.type == "Gold") {
                    goldAmount = resource.amount;
                    break;
                }
            }
            for (const Resource &resource : player->village->resources) {
                if (resource.type == "Wood") {
                    woodAmount = resource.amount;
                    break;
                }
            }

            for (const Troop &troop : player->village->troops) {
                if (troop.type == "Archer") {
                    archerAmount = troop.amount;
                    break;
                }
            }
            for (const Troop &troop : player->village->troops) {
                if (troop.type == "Knight") {
                    knightAmount = troop.amount;
                    break;
                }
            }
            for (const Troop &troop : player->village->troops) {
                if (troop.type == "Wizard") {
                    wizardAmount = troop.amount;
                    break;
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

            std::cout << "\n==========================\n";
            std::cout << "Total Food: "<<foodAmount;
            std::cout << "\nTotal Gold: "<<goldAmount;
            std::cout << "\nTotal Wood: "<<woodAmount<<"\n";
            std::cout << "----------------------\n";
            std::cout << "\nTotal Archers: "<<archerAmount;
            std::cout << "\nTotal Knights: "<<knightAmount;
            std::cout << "\nTotal Wizards: "<<wizardAmount;
            std::cout << "\n----------------------\n";
            std::cout << "\nLevel of Farm: "<<farmLevel;
            std::cout << "\nLevel of Gold Mine: "<<goldMineLevel;
            std::cout << "\nLevel of Lumber MIll: "<<lumberMillLevel;
            std::cout << "\n######################\n\n";
            //if there are no resources and troops the village is destroyed
            if(foodAmount+goldAmount+woodAmount+archerAmount+knightAmount+wizardAmount == 0){
                map.removeVillage(player->village);
                std::cout << "Village of " << player << "has been distroyed\n";
                turn = true;
                deletePlayer = true;
                if (deletePlayer==true) {
                    auto playerIter = std::find(players.begin(), players.end(), player);
                    if (playerIter != players.end()) {
                        delete *playerIter;
                        players.erase(playerIter);
                    }
                }
                turn= true;
            }

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

                    case 3:

                        for (const auto &player : players) {
                            std::cout << "Villages for player " << player->name << ": " << std::endl;
                            std::cout << "Village at x: " << player->village->x << ", y: " << player->village->y << std::endl;

                        }

                        for (const auto &aiplayer : AIs) {
                            std::cout << "Villages for AI player " << aiplayer << ": " << std::endl;
                            std::cout << "Village at x: " << aiplayer->village->x << ", y: " << aiplayer->village->y << std::endl;

                        }


                        std::cout << "Select a village to attack by entering its x-coordinate: \n";
                        int x;
                        std::cin >> x;
                        std::cout << "Select a village to attack by entering its y-coordinate: \n";
                        int y;
                        std::cin >> y;

                        // Search for the village the player selected
                        for (const auto &player : players) {
                            if (player->village->x == x && player->village->y == y) {
                                targetVillage = player->village;
                                break;
                            }

                        }

                        for (const auto &aiplayer : AIs) {
                            if (aiplayer->village->x == x && aiplayer->village->y == y) {
                                targetVillage = aiplayer->village;
                                break;
                            }

                        }

                        // Make sure a valid village was selected
                        if (targetVillage == nullptr) {
                            std::cout << "Invalid village selected.\n" << std::endl;
                            turn= false;
                            break;
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


                        troops.push_back(Troop("Archer", 100, 5, 2, 2, numArchers));
                        troops.push_back(Troop("Wizard", 100, 15, 10, 4, numWizards));
                        troops.push_back(Troop("Knight", 100, 10, 5, 6, numKnights));


                        for (Troop &troop : player->village->troops) {
                            if (troop.type == "Archer") {
                                troop.amount -= numArchers;
                                break;
                            }
                        }
                        for (Troop &troop : player->village->troops) {
                            if (troop.type == "Knight") {
                                troop.amount -= numKnights;
                                break;
                            }
                        }
                        for (Troop &troop : player->village->troops) {
                            if (troop.type == "Wizard") {
                                troop.amount -= numWizards;
                                break;
                            }
                        }
                        // Now you can call the attack method
                        if(player->village != targetVillage){
                            arr = player->village->attack(*targetVillage, troops);
                        }else{
                            std::cout<<"You can't attack your own Village.\n";
                            turn=false;
                            break;
                        }

//                        for (int i = 0; i < 7; i++) {
//                            std::cout << arr[i] << " ";
//                        }
                        if(arr[6]==1){
                            player->village->updateResourcesAndTroops(arr[0],arr[1],arr[2],arr[3],arr[4],arr[5],arr[6]);

                            for (const auto &player : players) {
                                if (player->village->x == x && player->village->y == y) {
                                    player->village->updateResourcesAndTroops(arr[0],arr[1],arr[2],0,0,0,2);
                                    break;
                                }

                            }

                            for (const auto &aiplayer : AIs) {
                                if (aiplayer->village->x == x && aiplayer->village->y == y) {
                                    aiplayer->village->updateResourcesAndTroops(arr[0],arr[1],arr[2],0,0,0,2);
                                    break;
                                }

                            }
                        }
                        if(arr[6]==2){
                            player->village->updateResourcesAndTroops(0,0,0,0,0,0,1);

                            for (const auto &player : players) {
                                if (player->village->x == x && player->village->y == y) {
                                    player->village->updateResourcesAndTroops(arr[0],arr[1],arr[2],arr[3],arr[4],arr[5],2);
                                    break;
                                }

                            }

                            for (const auto &aiplayer : AIs) {
                                if (aiplayer->village->x == x && aiplayer->village->y == y) {
                                    aiplayer->village->updateResourcesAndTroops(arr[0],arr[1],arr[2],arr[3],arr[4],arr[5],2);
                                    break;
                                }

                            }
                        }


                        turn=true;
                        break;


                    case 4:
                            map.removeVillage(player->village);
                            std::cout << "Village of " << player << "has been distroyed\n";
                            turn = true;
                            deletePlayer = true;
                            if (deletePlayer==true) {
                                auto playerIter = std::find(players.begin(), players.end(), player);
                                if (playerIter != players.end()) {
                                    delete *playerIter;
                                    players.erase(playerIter);
                                }
                            }
                            break;
//

                    case 5: turn = true;
                            break;

                    default: std::cout<< "Invalid Choice\n";


                }

            }while(!turn);


            turn = false;
        }

        for (auto aiPlayer : AIs) {
            std::cout << "\nAI turn\n";
            foodAmount = 0;
            goldAmount = 0;
            woodAmount = 0;
            archerAmount = 0;
            knightAmount = 0;
            wizardAmount = 0;
            farmLevel = 0;
            goldMineLevel = 0;
            lumberMillLevel = 0;

            goldTaken = 0;
            foodTaken = 0;
            woodTaken = 0;
            archerReturned = 0;
            knightReturned = 0;
            wizardReturned = 0;

            for (int i = 0; i < aiPlayer->village->marching.size(); i++) {
                int movement = aiPlayer->village->marching[i].movement;

                // Do something with the movement variable
                //Friendly troops arrival
                if(movement==1){
                    if (!aiPlayer->village->marching.empty()) {
                        for (auto &marchingTroops: aiPlayer->village->marching) {

                            for (auto resource: marchingTroops.resources) {
                                if (resource.type == "Gold") {
                                    goldTaken = resource.amount;
                                    resource.amount = 0;
                                } else if (resource.type == "Food") {
                                    foodTaken = resource.amount;
                                    resource.amount = 0;
                                } else if (resource.type == "Wood") {
                                    woodTaken = resource.amount;
                                    resource.amount = 0;
                                }
                            }
                            for (auto &troop: marchingTroops.troops) {
                                if (troop.type == "Archer") {
                                    archerReturned = troop.amount;
                                    troop.amount = 0;
                                } else if (troop.type == "Knight") {
                                    knightReturned = troop.amount;
                                    troop.amount = 0;
                                } else if (troop.type == "Wizard") {
                                    wizardReturned = troop.amount;
                                    troop.amount = 0;
                                }
                            }
                            marchingTroops.movement=3;
                        }
                    }

                    for (Troop &troop : aiPlayer->village->troops) {
                        if (troop.type == "Archer") {
                            troop.amount += archerReturned;
                            break;
                        }
                    }
                    for (Troop &troop : aiPlayer->village->troops) {
                        if (troop.type == "Knight") {
                            troop.amount += knightReturned;
                            break;
                        }
                    }
                    for (Troop &troop : aiPlayer->village->troops) {
                        if (troop.type == "Wizard") {
                            troop.amount += wizardReturned;
                            break;
                        }
                    }
                    for ( Resource resource : aiPlayer->village->resources) {
                        if (resource.type == "Food") {
                            resource.amount += foodTaken;
                            break;
                        }
                    }
                    for ( Resource resource : aiPlayer->village->resources) {
                        if (resource.type == "Wood") {
                            resource.amount += woodTaken;
                            break;
                        }
                    }
                    for ( Resource resource : aiPlayer->village->resources) {
                        if (resource.type == "Gold") {
                            resource.amount += goldTaken;
                            break;
                        }
                    }

                }

                //enemy troops arrival
                if(movement==2){

                    for (auto marchingTroops: aiPlayer->village->marching) {
                        for (auto resource: marchingTroops.resources) {
                            if (resource.type == "Gold") {
                                goldTaken += resource.amount;
                                resource.amount = 0;
                            } else if (resource.type == "Food") {
                                foodTaken += resource.amount;
                                resource.amount = 0;
                            } else if (resource.type == "Wood") {
                                woodTaken += resource.amount;
                                resource.amount = 0;
                            }
                        }
                        for (auto troop: marchingTroops.troops) {
                            if (troop.type == "Archer") {
                                archerReturned += troop.amount;
                                troop.amount = 0;
                            } else if (troop.type == "Knight") {
                                knightReturned += troop.amount;
                                troop.amount = 0;
                            } else if (troop.type == "Wizard") {
                                wizardReturned += troop.amount;
                                troop.amount = 0;
                            }
                        }
                        marchingTroops.movement=3;
                    }


                    for (Troop &troop : aiPlayer->village->troops) {
                        if (troop.type == "Archer") {
                            troop.amount = 0;
                            troop.amount += archerReturned;
                            break;
                        }
                    }
                    for (Troop &troop : aiPlayer->village->troops) {
                        if (troop.type == "Knight") {
                            troop.amount = 0;
                            troop.amount += knightReturned;
                            break;
                        }
                    }
                    for (Troop &troop : aiPlayer->village->troops) {
                        if (troop.type == "Wizard") {
                            troop.amount = 0;
                            troop.amount += wizardReturned;
                            break;
                        }
                    }
                    for ( Resource resource : aiPlayer->village->resources) {
                        if (resource.type == "Food") {
                            resource.amount -= foodTaken;
                            break;
                        }
                    }
                    for ( Resource resource : aiPlayer->village->resources) {
                        if (resource.type == "Wood") {
                            resource.amount -= woodTaken;
                            break;
                        }
                    }
                    for ( Resource resource : aiPlayer->village->resources) {
                        if (resource.type == "Gold") {
                            resource.amount -= goldTaken;
                            break;
                        }
                    }

                }
            }

            for (const Resource &resource : aiPlayer->village->resources) {
                if (resource.type == "Food") {
                    foodAmount = resource.amount;
                    break;
                }
            }

            for (const Resource &resource : aiPlayer->village->resources) {
                if (resource.type == "Gold") {
                    goldAmount = resource.amount;
                    break;
                }
            }

            for (const Resource &resource : aiPlayer->village->resources) {
                if (resource.type == "Wood") {
                    woodAmount = resource.amount;
                    break;
                }
            }

            for (const Troop &troop : aiPlayer->village->troops) {
                if (troop.type == "Archer") {
                    archerAmount = troop.amount;
                    break;
                }
            }

            for (const Troop &troop : aiPlayer->village->troops) {
                if (troop.type == "Knight") {
                    knightAmount = troop.amount;
                    break;
                }
            }

            for (const Troop &troop : aiPlayer->village->troops) {
                if (troop.type == "Wizard") {
                    wizardAmount = troop.amount;
                    break;
                }
            }

            for (const Building& building : aiPlayer->village->buildings) {
                if (building.type == "Farm") {
                    farmLevel = building.level;
                    break;
                }
            }

            for (const Building& building : aiPlayer->village->buildings) {
                if (building.type == "Gold Mine") {
                    goldMineLevel = building.level;
                    break;
                }
            }

            for (const Building& building : aiPlayer->village->buildings) {
                if (building.type == "Lumber Mill") {
                    lumberMillLevel = building.level;
                    break;
                }
            }

            std::cout <<"\n============================";
            std::cout << "\nTotal Food: "<<foodAmount;
            std::cout << "\nTotal Gold: "<<goldAmount;
            std::cout << "\nTotal Wood: "<<woodAmount<<"\n";

            std::cout << "----------------------\n";
            std::cout << "\nTotal Archers: "<<archerAmount;
            std::cout << "\nTotal Knights: "<<knightAmount;
            std::cout << "\nTotal Wizards: "<<wizardAmount<<"\n";


            std::cout << "----------------------\n";
            std::cout << "\nLevel of Farm: "<<farmLevel;
            std::cout << "\nLevel of Gold Mine: "<<goldMineLevel;
            std::cout << "\nLevel of Lumber MIll: "<<lumberMillLevel;
            std::cout << "\n######################\n\n";

            //if there are no resources and troops the village is destroyed
            if(foodAmount+goldAmount+woodAmount+archerAmount+knightAmount+wizardAmount == 0){
                map.removeVillage(aiPlayer->village);
                std::cout << "Village of " << aiPlayer << "has been distroyed\n";
                turn = true;
                deletePlayer = true;
                if (deletePlayer==true) {
                    auto playerIter = std::find(AIs.begin(), AIs.end(), aiPlayer);
                    if (playerIter != AIs.end()) {
                        delete *playerIter;
                        AIs.erase(playerIter);
                    }
                }

            }
            aiPlayer->takeTurn();
        }

        // Check for win condition
        if (map.villageLocations.size() == 1) {
            auto lastVillage = map.villageLocations.begin()->first;
            std::cout << "The winner is the owner of village at ("
                      << lastVillage->x << ", " << lastVillage->y << ") "
                       << std::endl;
            break;
        }

        //marching


        //end round

        std::cout<<"Round "<<round<< " has ended\n";
        //start round
        round++;
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

