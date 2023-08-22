//
//  Game.hpp
//  Project_3
//
//  Created by Aryan Gupta on 5/28/23.
//

#ifndef Game_h
#define Game_h

#include <stdio.h>
#include "Side.h"
#include "Player.h"



class Game
{
public:
    Game(const Board& b, Player* south, Player* north);
    void display() const;
    void status(bool& over, bool& hasWinner, Side& winner) const;
    bool move(Side S);
    void play();
    int beans(Side s, int hole) const;

private:
   
    bool finished;
    bool winner;
    Side winner_side;
    Board m_board;
    Player* m_north;
    Player* m_south;
    Player* m_turn;
    
    Side Opposite(Side turn);
    void m_play(Side s, bool f, int x);
    
};

#endif /* Game_h */
