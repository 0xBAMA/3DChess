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

    contents return_contents_at(int x, int y);

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

contents game::return_contents_at(int x, int y){

  return c.get_space_at(x,y).get_contents();
}

#endif
