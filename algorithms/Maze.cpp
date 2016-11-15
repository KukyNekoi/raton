//bitmasks for unsigned char
#include <stdlib.h>
// #define BIT_0        0b00000001
// #define BIT_1        0b00000010
// #define BIT_2        0b00000100
// #define BIT_3        0b00001000
// #define BIT_4        0b00010000
// #define BIT_5        0b00100000
// #define BIT_6        0b01000000
// #define BIT_7        0b10000000
// #define BIT_CLEAR    0b00000000

// #define UP      'U'
// #define DOWN    'D'
// #define LEFT    'L'
// #define RIGHT   'R'
#define NORTH   'N'
#define SOUTH   'S'
#define WEST    'W'
#define EAST    'E'

// //global variables
// unsigned char bitmask[8] = {BIT_0, BIT_1, BIT_2, BIT_3, BIT_4, BIT_5, BIT_6, BIT_7};

/*
-1: undiscovered
-2: discarded
0: forward
1: right
2: down
3: left
*/
class Maze
{
public:
    Maze(){
        node_count = 1;
        current_node = 0;
        // north[0] = 
        parent = 0;
        for(int i=0; i<100; i++){
            for(int j=0; j<4; j++){
                nodes[i][j] = -1;
            }
        }
    }

    void add_node(char direction){
        if(nodes[current_node][direction] >= 0){
            bool has_potential = false;
            for(int i=0; i<4; i++){
                if(i != direction && nodes[current_node][i] == -1){
                    has_potential = true;
                }
            }

            if(has_potential){
                .,,
            }
            
        }
        else if( nodes[current_node][direction] == -1 ){
            //route unexprored
            nodes[current_node][direction] = node_count;
            nodes[node_count][(direction + 2) % 4] = current_node;
            current_node = node_count;
            node_count++;
        }
    }

    ~Maze(){
        free(this->maze);
    };

private:
    char nodes[100][4];
    int node_count, current_node;
};