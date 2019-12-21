#ifndef GAME_H
#define GAME_H

#include "board_space.h"
#include "move.h"
#include "chess_board.h"

typedef enum result
{
    BLACK_SUCCESS,
    WHITE_SUCCESS,
    STALEMATE
} result;

class game{ 

public:
    game();

    result play();
    void make_move(chess_move m);

private:
    chess_board c;


};


game::game()
{

    c.dump();

}

void game::make_move(chess_move m){


}



#endif