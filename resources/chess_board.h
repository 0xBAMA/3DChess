#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H


#include <iostream>
#include <string>
#include "board_space.h"
#include "colors.h"

using namespace std;

class chess_board
{
public:
  chess_board();

  space get_space_at(int x, int y);
  bool is_legal(std::string input_move);

  void dump();


private:
  void init_colors();
  void populate_with_pieces();

  void terminal_output(contents c);

  // castling is not allowed if the king has made a move
  bool white_king_moved = false;
  bool black_king_moved = false;
  
  space board[64];

};

bool chess_board::is_legal(std::string input_move)
{
  // function takes an input move in the following notation:
    // four characters - [source x][source y][destination x][destination y]
  
  // consider the following:
  
    // input validation:
      // is input_move valid?
      // is it four characters long
      // are both source and destination valid board spaces
      // does source contain a piece, you have functions to check this
      // ... other considerations?
    
    // piece moves:
      // the knight is the only piece which does not need to consider the pieces in its path. That is to say, it can jump over pieces, and others can not.
      // consider which color piece is being moved, and which direction that color moves.
      // a piece cannot capture a piece of the same color.
      // castling - if the move is attempting to castle, has the king moved? check black_king_moved/white_king_moved
      // king can not move into check
      // if the king is in check, it must be moved - that is to say, if the king is in check, no move involving any other piece will be legal, unless it resolves the fact that the king is in check
      // pawns can only move two spaces if they are located in their starting positions - ignore the en passant rule, as it would require a retained board state
      // this is by no means an exhaustive list of considerations, for more information, check this https://en.wikipedia.org/wiki/Rules_of_chess
  
}


space chess_board::get_space_at(int x, int y)
{
  space temp;

  if(x >= 0 && x <= 7 && y >= 0 && y <= 7)
  {
    return board[(8*y) + x];
  }
  else
  {
    return temp;
  }
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
  board[59].set_contents(whiteking);

  //queen
  board[60].set_contents(whitequeen);
}


void chess_board::dump()
{ //prints the board
  int counter = 0;

  cout << "       ";

  for(int i = 0; i < 8; i++)
  {
    cout << "   " << i;
  }

  cout << endl << "      ";

  for(int i = 0; i < 64; i++)
  {

    space temp = board[i];

    if(counter == 0)
    {
      cout  << i/8;
    }


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
      cout << "    "  << RESET;
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
    case whitepawn:    cout << T_RED << " pa ";    break;
    case blackpawn:    cout << T_MAGENTA << " pa ";   break;

    case whiteknight:  cout << T_RED << " kn ";    break;
    case blackknight:  cout << T_MAGENTA << " kn ";   break;

    case whitebishop:  cout << T_RED << " bi ";    break;
    case blackbishop:  cout << T_MAGENTA << " bi ";   break;

    case whiterook:    cout << T_RED << " ro ";    break;
    case blackrook:    cout << T_MAGENTA << " ro ";   break;

    case whitequeen:   cout << T_RED << " Qu ";    break;
    case blackqueen:   cout << T_MAGENTA << " Qu ";   break;

    case whiteking:    cout << T_RED << " Ki ";    break;
    case blackking:    cout << T_MAGENTA << " Ki ";   break;

    default:           //cout << " " << c << " ";
      break;
  }
}

#endif
