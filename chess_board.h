#include <iostream>
#include "board_space.h"

using namespace std;

class chess_board
{
public:
  chess_board();

  space get_space_at(int x, int y);


private:
  void init_colors();
  void populate_with_pieces();

  space board[64];

};


space chess_board::get_space_at(int x, int y)
{

}

chess_board::chess_board()
{
  init_colors();
  populate_with_pieces();
}

void chess_board::init_colors()
{
  for(int i = 0; i < 64; i++)
  {
    int x = i % 8;
    int y = i / 8;

    if(y % 2 == 0)
    {
      if(x % 2 == 0)
      {
          chess_board.space[i].set_color(false); //set to black
      }
      else
      {
          chess_board.speace[i].set_color(true); //set to white
      }
    }
    else
    {
      if(x % 2 == 0)
      {
          chess_board.space[i].set_color(true);
      }
      else
      {
          chess_board.space[i].set_color(false);
      }
    }
  }
}

void chess_board::populate_with_pieces()
{
//get the black pawns
  for(int i = 8; i < 16; i++)
  {
    board[i].set_contents(blackpawn);
  }

//then the other black pieces

  //rooks
  board[0].set_contents(blackrook);
  board[7].set_contents(blackrook);

  //knights
  board[1].set_contents(blackknight);
  board[6].set_contents(blackknight);

  //bishops
  board[2].set_contents(blackbishop);
  board[5].set_contents(blackbishop);

  //king
  board[3].set_contents(blackking);

  //queen
  board[4].set_contents(blackqueen);

//get the white pawns
  for(int i = 48; i < 56; i++)
  {
    board[i].set_contents(whitepawn);
  }

//then the other white pieces

  //rooks
  board[56].set_contents(whiterook);
  board[63].set_contents(whiterook);

  //knights
  board[57].set_contents(whiteknight);
  board[62].set_contents(whiteknight);

  //bishops
  board[58].set_contents(whitebishop);
  board[61].set_contents(whitebishop);

  //king
  board[60].set_contents(whiteking);

  //queen
  board[59].set_contents(whitequeen);
}
