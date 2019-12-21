#include <iostream>
#include "board_space.h"
#include "colors.h"

using namespace std;

class chess_board
{
public:
  chess_board();

  space get_space_at(int x, int y);

  void dump();


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
    int y = (int) i / 8;

    if(y % 2 == 0)
    {
      if(x % 2 == 0)
      {
          board[i].set_color(false); //set to black
      }
      else
      {
          board[i].set_color(true); //set to white
      }
    }
    else
    {
      if(x % 2 == 0)
      {
          board[i].set_color(true);
      }
      else
      {
          board[i].set_color(false);
      }
    }
    board[i].set_contents(empty);
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


void chess_board::dump()
{ //prints the board
  int counter = 1;
  for(int i = 0; i < 64; i++)
  {
    // cout << board[i].get_contents()<<"  "<< board[i].get_color()<<"  "<< board[i].is_empty()<<"        ";

    if(board[i].get_color())
    {
      cout << B_WHITE;
    }
    else
    {
      cout << B_BLUE;
    }



    if(board[i].is_empty())
    {
      cout << "   " << RESET;
    }
    else
    {
      cout << " p " << RESET;
    }



    bool set = false;

    switch(board[i].get_contents())
    { //group by piece
      whitepawn:    cout << T_BLUE;   set = true;
      blackpawn:
          if(!set)  cout << T_BLACK;
          cout << " p ";
          break;
      whiteknight:    cout << T_BLUE;   set = true;
      blackknight:
          if(!set)  cout << T_BLACK;
          cout << " k ";
          break;
      whitebishop:    cout << T_BLUE;   set = true;
      blackbishop:
          if(!set)  cout << T_BLACK;
          cout << " b ";
          break;
      whiterook:    cout << T_BLUE;   set = true;
      blackrook:
          if(!set)  cout << T_BLACK;
          cout << " r ";
          break;
      whitequeen:   cout << T_BLUE;   set = true;
      blackqueen:
          if(!set)  cout << T_BLACK;
          cout << " q ";
          break;
      whiteking:    cout << T_BLUE;   set = true;
      blackking:
          if(!set)  cout << T_BLACK;
          cout << " K ";
          break;
    }


    counter++;

    if(counter == 8)
    {
      cout << RESET << endl;
      counter = 0;
    }
  }

  cout << endl << endl;
}
