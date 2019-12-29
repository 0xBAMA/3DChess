#ifndef GAME_H
#define GAME_H

#include "board_space.h"
#include "move.h"
#include "chess_board.h"

typedef enum game_state_t{

    b_initial_state,
    b_selected_state,
    w_initial_state,
    w_selected_state
} game_state;

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

    bool is_legal( chess_move mv );

    void handle_click(int red, int green);

private:
    game_state my_state;
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

contents game::return_contents_at(int x, int y)
{

  return c.get_space_at(x,y).get_contents();
}


void game::handle_click(int green, int red)
{

    switch(my_state){

        case b_initial_state:
          break;

        case b_selected_state:
          break;

        case w_initial_state:
          break;

        case w_selected_state:
          break;

        default:
          break;

    }
}
#endif
