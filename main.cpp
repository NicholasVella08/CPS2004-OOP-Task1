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
    int playerChoice;
    int buildingChoice;

    // Game loop
    bool gameIsRunning = true;
    while (gameIsRunning) {
        for (auto &player : players) {
            player->village->earnResources();
            //want to output all resources and troops;
            std::cout << "Village of "<< player;
            std::cout << "Total Food: ";
            std::cout << "Total Gold: ";
            std::cout << "Total Wood: ";
            std::cout << "----------------------";
            std::cout << "Total Archers: ";
            std::cout << "Total Knights: ";
            std::cout << "Total Wizards: ";
            std::cout << "----------------------";
            std::cout << "Level of Farm: ";
            std::cout << "Level of Gold Mine: ";
            std::cout << "Level of Lumber MIll: ";
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
                    case 2:
                            break;
                    case 3:
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

