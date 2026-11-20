/* The map generator file */

#define N 10 /* The basic board size */

class Map {
    int board_size;
    int** board_height; /* We will take 0 as the minimum height */
    char** pieces; /* Might be change later to a piece type */

public:
    Map(int = N);
    ~Map();
    bool is_valid();
    Map random_generation(); /* Will be pseudo random for now */
    Map rotate(); /* Clockwise */
    char* to_string();
    void show();
};

Map::Map(int size) {
    board_size = size;
    board_height = new int*[size]();
    pieces = new char*[size]();
    for (int i = 0; i < size; i++) {
        board_height[i] = new int[size]();
        pieces[i] = new char[size]();
        for (int j = 0; j < size; j++) {
            board_height[i][j] = 0;
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

/* EOF */