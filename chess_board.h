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

}