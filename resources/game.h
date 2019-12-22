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

class game
{

public:

    game();

    result play();

private:

    void make_move(chess_move m);
    chess_board c;

};


game::game()
{
    cout << endl << "3DChess copyright WEAPON OF DURGASOFT, LLC 2004" << endl;

    cout << "GAME BEGINS:" << endl << endl;

    c.dump();

}

void game::make_move(chess_move m)
{


}



#endif
