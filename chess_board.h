#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


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

  void terminal_output(contents c);

  space board[64];

};


space chess_board::get_space_at(int x, int y)
{
  space temp;

  //todo

  return temp;
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
  //other ways to populate a board like this:
  //->    run a cellular automata with as many states as you have types of pieces to place - after some number of iterations, map states to pieces and play from that board
 //   ->   std::random functions, just straight up random generation of the board based on integers 0-14, excluding 1 (invalid)
//      ->  keeping things fair(er), by giving sides random pieces that add up to some point total that is either equal or close to equal

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
  int counter = 0;
  cout << endl << endl  << "3DChess copyright WEAPON OF DURGASOFT, LLC 2004" << endl << endl << "      ";
  for(int i = 0; i < 64; i++)
  {
    // cout << board[i].get_contents()<<"  "<< board[i].get_color()<<"  "<< board[i].is_empty()<<"        ";

    space temp = board[i];


    if(temp.get_color())
    {
      cout << B_WHITE;
    }
    else
    {
      cout << B_BLACK;
    }



    if(temp.is_empty())
    {
      cout << "    " << RESET;
    }
    else
    {
      terminal_output(temp.get_contents());
      cout << RESET;
    }

    counter++;

    if(counter == 8)
    {
      cout << RESET << endl << "      ";
      counter = 0;
    }
  }

  cout << endl << endl;
}


void chess_board::terminal_output(contents c)
{
  switch(c)
  {
    case whitepawn:    cout << T_CYAN << " pa ";    break;
    case blackpawn:    cout << T_BLUE << " pa ";   break;

    case whiteknight:  cout << T_CYAN << " kn ";    break;
    case blackknight:  cout << T_BLUE << " kn ";   break;

    case whitebishop:  cout << T_CYAN << " bi ";    break;
    case blackbishop:  cout << T_BLUE << " bi ";   break;

    case whiterook:    cout << T_CYAN << " ro ";    break;
    case blackrook:    cout << T_BLUE << " ro ";   break;

    case whitequeen:   cout << T_CYAN << " Qu ";    break;
    case blackqueen:   cout << T_BLUE << " Qu ";   break;

    case whiteking:    cout << T_CYAN << " Ki ";    break;
    case blackking:    cout << T_BLUE << " Ki ";   break;

    default:           //cout << " " << c << " ";
      break;
  }
}

#endif
