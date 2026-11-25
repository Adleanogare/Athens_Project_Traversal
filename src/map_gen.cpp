/* The map class file */

#define N 10 /* The basic board size */

#include "tools.cpp"

#include <cstdlib> // TODO : Might need to change later with an home made random function
#include <ctime> /* Time to work */
#include <stdexcept>

class Map {
    int board_size;
    int** board_height; /* We will take 0 as the minimum height */
    char** pieces; /* Might be changed later to a piece type (first change done) */

public:
    Map(int = N);
    ~Map();
    bool is_valid();
    void fully_random_generation(int = 10);
    void fully_random_valid_generation(int = 10, int = 2048);
    void spread_generation(int = 10);
    //void spread_valid_generation(int = 10, int = 2048);
    Map rotate(bool = true); /* Clockwise if true  else the other way around */
    Map vertical_sym(bool = true); /* Left over right if false */
    Map horizontal_sym(bool = true); /* Top over bottom if true */
    Map desc_sym(bool = true); /* Top Right over Bottom Left */
    Map asc_sym(bool = true); /* Top Left over Bottom Right */
    Map cross_sym(); /* Left over everything */
    Map plus_sym(); /* Top Left over everything */
    char* to_string();
    void show();
    void invalid_change();
};

Map::Map(int size) {
    board_size = size;
    board_height = new int*[size]();
    pieces = new char*[size]();
    for (int i = 0; i < size; i++) {
        board_height[i] = new int[size]();
        pieces[i] = new char[size]();
        for (int j = 0; j < size; j++) {
            board_height[i][j] = -1; //not valid
            pieces[i][j] = ' ';
        }
    }
};

Map::~Map() {
    for (int i = 0; i < board_size; i++) {
        delete[] board_height[i];
        delete[] pieces[i]; 
    }
    delete[] board_height;
    delete[] pieces;
}

bool Map::is_valid() {

    int size = this->board_size;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (this->board_height[i][j] < 0) {
                return false;
            } else if (i > 0 && absolute(this->board_height[i][j] - this->board_height[i-1][j]) > 2) { /* Using lazy check for and */
                return false;
            } else if (i < size-1 && absolute(this->board_height[i][j] - this->board_height[i+1][j]) > 2) { /* Using lazy check for and */
                return false;
            } else if (j > 0 && absolute(this->board_height[i][j] - this->board_height[i][j-1]) > 2) { /* Using lazy check for and */
                return false;
            } else if (j < size-1 && absolute(this->board_height[i][j] - this->board_height[i][j+1]) > 2) { /* Using lazy check for and */
                return false;
            } 
        }
    }

    return true;
}

void Map::invalid_change() {
    int size = this->board_size;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (this->board_height[i][j] < 0) {
                pieces[i][j] = 'X';
            } else if (i > 0 && absolute(this->board_height[i][j] - this->board_height[i-1][j]) > 2) { /* Using lazy check for and */
                pieces[i][j] = 'W';
            } else if (i < size-1 && absolute(this->board_height[i][j] - this->board_height[i+1][j]) > 2) { /* Using lazy check for and */
                pieces[i][j] = 'S';
            } else if (j > 0 && absolute(this->board_height[i][j] - this->board_height[i][j-1]) > 2) { /* Using lazy check for and */
                pieces[i][j] = 'Q';
            } else if (j < size-1 && absolute(this->board_height[i][j] - this->board_height[i][j+1]) > 2) { /* Using lazy check for and */
                pieces[i][j] = 'D';
            } else {
                pieces[i][j] = ' ';
            }
        }
    }
}

void Map::fully_random_generation(int max_height) {
    std::srand(std::time(nullptr)); // Graine avec le temps actuel
    for (int i = 0; i < this->board_size; i++) {
        for (int j = 0; j < this->board_size; j++) {
            this->board_height[i][j] = std::rand() % max_height;
        }
    }
}

void Map::fully_random_valid_generation(int max_height, int max_attempt) {
    
    int try_nb = 1;
    this->fully_random_generation(max_height);
    
    while (try_nb < max_attempt && !this->is_valid()) {
        this->fully_random_generation(max_height);
        try_nb += 1;
    }
    if (try_nb == max_attempt) {throw std::runtime_error("Failed to generate a valid map in time.");}
}

void Map::spread_generation(int max_height) {
    std::srand(std::time(nullptr)); // Graine avec le temps actuel

    this->board_height[0][0] = std::rand() % max_height;
    for (int i = 1; i < this->board_size; i++) {
        int a = std::min(max_height, this->board_height[0][i-1] + 2);
        int b = std::max(0, this->board_height[0][i-1] - 2);
        this->board_height[0][i] = std::rand() % (a-b+1) + b;

        int c = std::min(max_height, this->board_height[i-1][0] + 2);
        int d = std::max(0, this->board_height[i-1][0] - 2);
        this->board_height[i][0] = std::rand() % (c-d+1) + d;
    }

    for (int i = 1; i < this->board_size; i++) { // the fill order could be different
        for (int j = 1; j < this->board_size; j++) { // We seek to update [i][j] here
            int a = std::min(max_height, this->board_height[i-1][j] + 2);
            int b = std::max(0, this->board_height[i-1][j] - 2);
            int c = std::min(max_height, this->board_height[i][j-1] + 2);
            int d = std::max(0, this->board_height[i][j-1] - 2);
            int mini = std::max(b,d);
            int maxi = std::min(a,c);
            if (mini > maxi) {
                throw std::range_error("Failed to generate a valid map");
            } else {
                this->board_height[i][j] = rand()%(maxi-mini+1) + mini;
            }
        }
    }
}

/* Useless when doing the math
void Map::spread_valid_generation(int max_height, int max_attempt) {
    
    int try_nb = 0;
    try {
        this->spread_generation(max_height);
    } catch (const std::range_error& e) {}
    
    
    while (try_nb < max_attempt && !this->is_valid()) {
        try {
            try_nb += 1;
            this->spread_generation(max_height);
        } catch (const std::range_error& e) {}
    }
    if (try_nb == max_attempt) {throw std::range_error("Failed to generate a valid map in time.");}
}
*/

Map Map::rotate(bool clockwise) {

    int size = this->board_size;

    Map rotated = Map(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (clockwise) {
                // i=0,j=0 goes to i=0,j=n | i=0,j=n goes to i=n,j=n | i=n,j=n goes to i=n,j=0
                // i=0,j=k goes to i=k,j=n | i=k,j=0 goes to i=0,j=k | i,j = j,n-i 
                rotated.board_height[j][size-1-i] = this->board_height[i][j];
            } else {
                rotated.board_height[size-1-j][i] = this->board_height[i][j];
            }
        }
    }

    return rotated;
}

Map Map::horizontal_sym(bool top_over_bottom) {

    int size = this->board_size;

    Map hori_sym = Map(size);

    for (int i = 0; i < (size+1)/2; i++) {
        for (int j = 0; j < size; j++) {
            if (top_over_bottom) { 
                hori_sym.board_height[i][j] = this->board_height[i][j];
                hori_sym.board_height[size-1-i][j] =  this->board_height[i][j];
            } else {
                hori_sym.board_height[i][j] = this->board_height[size-1-i][j];
                hori_sym.board_height[size-1-i][j] =  this->board_height[size-1-i][j];
            }
        }
    }

    return hori_sym;
}

Map Map::vertical_sym(bool left_over_right) {
    return this->rotate(left_over_right).horizontal_sym().rotate(!left_over_right);
}

Map Map::plus_sym() {
    return this->horizontal_sym().vertical_sym();
}

Map Map::desc_sym(bool top_left_over) {
    
    int size = this->board_size;

    Map sym = Map(size);

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size-i; j++) {
            if (top_left_over) { 
                sym.board_height[i][j] = this->board_height[i][j];
                sym.board_height[size-1-j][size-1-i] =  this->board_height[i][j];
            } else {
                sym.board_height[i][j] = this->board_height[size-1-j][size-1-i];
                sym.board_height[size-1-j][size-1-i] =  this->board_height[size-1-j][size-1-i];
            }
        }
    }

    return sym;
}

/* EOF */