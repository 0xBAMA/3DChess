#include <iostream>
#include "board_space.h"

using namespace std;

class chess_board{

public:
    chess_board();

    space get_space_at(int x, int y);


private:
    space board[64];

};


space chess_board::get_space_at(int x, int y)
{
    
}

chess_board::chess_board()
{
    for(int i = 0; i < 64; i++){

        int x = i % 8;
        int y = i / 8;

        if(y % 2 == 0){

            if(x % 2 == 0){
                    chess_board.space[i].set_color(bool false);
            }else{
                    chess_board.speace[i].set_color(bool true);
            }   

        }else{

            if(x % 2 == 0){
                    chess_board.space[i].set_color(bool true);
                
            }else{
                    chess_board.space[i].set_color(bool false);
            }
        }
    }
}
