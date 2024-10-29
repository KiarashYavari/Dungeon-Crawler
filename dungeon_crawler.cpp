// Project Idea: C++ Text-Based Game Dungeon Crawler

// Create a simple text-based dungeon crawler game where
//  the player navigates through a grid-based dungeon. The project will involve:

//     Implementing various data structures linked lists, stacks, queues, trees, graphs, etc...
//     Practicing algorithms searching, sorting, pathfinding, etc..
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Constants for map symbols
const char EMPTY = '.';
const char PLAYER = 'P';
const char TREASURE = 'T';
const char ENEMY = 'E';
const char TRAP = 'X';

// Structure to represent the player's position
struct Position {
    int row;
    int col;
};

// Function to get user choice for difficulty level
int getDifficulty() {
    int difficulty;
    cout << "Choose Difficulty Level:\n";
    cout << "1. Easy (5x5)\n";
    cout << "2. Medium (10x10)\n";
    cout << "3. Hard (15x15)\n";
    cout << "Enter your choice: ";
    cin >> difficulty;
    return difficulty;
}

// Function to initialize the map size based on difficulty
int getMapSize(int difficulty) {
    switch (difficulty) {
        case 1: return 5; // Easy
        case 2: return 10; // Medium
        case 3: return 15; // Hard
        default:
            cout << "Invalid difficulty! select default to easy." << endl;
            return 5; // Default to Easy
    }
}

// Function to generate a map with the player, treasure, enemies, and traps
vector<vector<char>> generateMap(int size, Position &playerPos, Position &treasurePos, vector<Position> &enemies, vector<Position> &traps) {
    vector<vector<char>> map(size, vector<char>(size, EMPTY));

    // Place the player at the top-left corner
    map[playerPos.row][playerPos.col] = PLAYER;

    // Place the treasure on the map
    map[treasurePos.row][treasurePos.col] = TREASURE;

    // Place enemies on the map
    for (const auto &enemy : enemies) {
        map[enemy.row][enemy.col] = ENEMY;
    }

    // Place traps on the map
    for (const auto &trap : traps) {
        map[trap.row][trap.col] = TRAP;
    }

    return map;
}

Position getRandomPosition(int size, const Position &exclude) {
    Position pos;
    do {
        pos.row = rand() % size;
        pos.col = rand() % size;
    } while (pos.row == exclude.row && pos.col == exclude.col); // Ensure we don't place at the excluded position
    return pos;
}

// Function to move the player based on input
void movePlayer(Position &playerPos, char direction, int size) {
    switch (direction) {
        case 'w': // Move up
            if (playerPos.row > 0) playerPos.row--;
            break;
        case 's': // Move down
            if (playerPos.row < size - 1) playerPos.row++;
            break;
        case 'a': // Move left
            if (playerPos.col > 0) playerPos.col--;
            break;
        case 'd': // Move right
            if (playerPos.col < size - 1) playerPos.col++;
            break;
        default:
            cout << "Invalid move! Use 'w', 'a', 's', or 'd'." << endl;
            break;
    }
}

// Function to check for encounters and update lives
bool checkEncounters(Position &playerPos, const Position &treasurePos,
 const vector<Position> &enemies, const vector<Position> &traps, int &lives) {
    // Check for treasure encounter
    if (playerPos.row == treasurePos.row && playerPos.col == treasurePos.col) {
        cout << "You found the treasure! You win!" << endl;
        return true; // Game over (win)
    }

    // Check for enemy encounter
    for (const auto &enemy : enemies) {
        if (playerPos.row == enemy.row && playerPos.col == enemy.col) {
            lives--; // Lose a life
            cout << "You encountered an enemy! Lives remaining: " << lives << endl;
            if (lives <= 0) {
                cout << "You have no lives left! Game over!" << endl;
                return true; // Game over (loss)
            }
            return false; // Continue game if lives remain
        }
    }

    for (const auto &trap : traps) {
        if (playerPos.row == trap.row && playerPos.col == trap.col) {
            lives--; // Lose a life
            cout << "You enter a trap! Lives remaining: " << lives << endl;
            if (lives <= 0) {
                cout << "You have no lives left! Game over!" << endl;
                return true; // Game over (loss)
            }
            return false; // Continue game if lives remain
        }
    }

}

// Function to display the map --> only showing the player's position
// Function to display the map, hiding traps and enemies from the player
void displayMap(const vector<vector<char>> &map, const Position &playerPos) {
    for (int i = 0; i < map.size(); ++i) {
        for (int j = 0; j < map[i].size(); ++j) {
            if (i == playerPos.row && j == playerPos.col) {
                cout << PLAYER << ' '; // Show player position
            } else if (map[i][j] == TREASURE) {
                cout << TREASURE << ' '; // Show treasure
            } else {
                cout << EMPTY << ' '; // Show empty space for hidden traps and enemies
            }
        }
        cout << endl;
    }
}


int main() {
    srand(static_cast<unsigned>(time(0))); // Seed for random number generation

    // Get user choice for difficulty
    int difficulty = getDifficulty();
    int mapSize = getMapSize(difficulty);
    int numEnemies, numTraps = 0;
    int lives = 4;

    // get the num of enemies and traps from map size (or difficulty indirectly)
    switch(mapSize){
        case 10:
            numEnemies = 2;
            numTraps = 2;
            break;
        case 15:
            numEnemies = 3;
            numTraps = 3;
            break;
        default:
            numEnemies = 1;
            numTraps = 1;
            break;
    }

    // Initialize map and player position
    Position playerPos = {0, 0}; // Starting position of the player
    Position treasurePos = getRandomPosition(mapSize, playerPos); // Random treasure position

    // Generate random positions for enemies and traps
    vector<Position> enemies, traps;
    for (int i = 0; i < numEnemies; i++) {
        enemies.push_back(getRandomPosition(mapSize, playerPos));
    }
    for (int i = 0; i < numTraps; i++) {
        traps.push_back(getRandomPosition(mapSize, playerPos));
    }
    vector<vector<char>> dungeonMap = generateMap(mapSize, playerPos, treasurePos, enemies, traps);

    // Display the initial map
    cout << "Initial Map (Player's position shown as 'P'):\n";
    displayMap(dungeonMap, playerPos);

    // Placeholder for future steps (e.g., player movement, game loop)
    bool gameOver = false;
    while (!gameOver) {
        cout << "Move (w/a/s/d) --> means (up, left, down and right) ";
        char move;
        cin >> move;

        // Update player position based on input
        movePlayer(playerPos, move, mapSize);

        // Check for encounters after moving
        gameOver = checkEncounters(playerPos, treasurePos, enemies, traps, lives);

        // Regenerate the map with updated player position and display it
        dungeonMap = generateMap(mapSize, playerPos, treasurePos, enemies, traps);
        displayMap(dungeonMap, playerPos);
    }

    return 0;
}
