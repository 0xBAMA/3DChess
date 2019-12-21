#ifndef MOVE_H
#define MOVE_H
#include "board_space.h"

class chess_move
{
private:
    /* data */
public:
    chess_move(/* args */);
    ~chess_move();

    space source, destination;
    int score;
};

chess_move::chess_move(/* args */)
{

}

chess_move::~chess_move()
{
    
}

#endif