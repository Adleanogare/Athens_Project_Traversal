/* The terminal output */

#include "map_gen.cpp"

#include <iostream>

char* Map::to_string() {
    /* In the form of :
    |---|'\n' : 1 '|' + '---|'*size + 1 '\n' for a line 
    |# 1|'\n' : * 1 + line*2
    |---|'\0' + 1 '\0'

    Assuming a bounded height of 9, else height will overflow with other ascii characters.
    */

    int size = this->board_size;
    int size_M = (1+2*size);
    int size_N = 2 * size_M; 

    char* map_str = new char[size_N*size_M]();

    for (int i = 0; i < size; i++) {
        map_str[size_N*(2*i+1)] = '|';
        map_str[size_N*(2*(i+1))] = '|';
        map_str[size_N*(2*i+1)-1] = '\n';
        map_str[size_N*(2*(i+1))-1] = '\n';
        map_str[1+4*i + size_N*(size_M - 1)] = '-';
        map_str[2+4*i + size_N*(size_M - 1)] = '-';
        map_str[3+4*i + size_N*(size_M - 1)] = '-';
        map_str[4+4*i + size_N*(size_M - 1)] = '|';
        for (int j = 0; j < size; j++) {
            map_str[1+4*j + size_N*2*i] = '-';
            map_str[2+4*j + size_N*2*i] = '-';
            map_str[3+4*j + size_N*2*i] = '-';
            map_str[4+4*j + size_N*2*i] = '|';
            map_str[1+4*j + size_N*(2*i+1)] = this->pieces[i][j];
            map_str[2+4*j + size_N*(2*i+1)] = ' ';
            map_str[3+4*j + size_N*(2*i+1)] = this->board_height[i][j] + 48; // In ascii '0' = 0 + 48
            map_str[4+4*j + size_N*(2*i+1)] = '|';
        }
    }

    map_str[0] = '|';
    map_str[size_N*size_M-1] = '\0';

    return map_str;
}

void Map::show() {
    std::cout << this->to_string() << std::endl;
}

int main() { /* Temporary main for tests */

    Map test_map = Map();

    test_map.show();

    return 0;
}

/* EOF */