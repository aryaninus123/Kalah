//
//  Player.hpp
//  CS 32 Project 3
//
//  Created by Madeleine Murphy on 5/13/19.
//  Copyright © 2019 Madeleine Murphy. All rights reserved.
//
#include <iostream>

#ifndef Player_h
#define Player_h

#include "Board.h"

#include <stdio.h>
using namespace std;


class Player
{
public:
    Player(string name);
    string name() const;
    virtual bool isInteractive() const;
    virtual int chooseMove(const Board& b, Side s) const = 0;
    virtual ~Player();
private:
    string m_name;
};

class HumanPlayer : public Player
{
public:
    HumanPlayer(string name);
    bool isInteractive() const;
    int chooseMove(const Board& b, Side s) const;
};

class BadPlayer : public Player
{
public:
    BadPlayer(string name);
    bool isInteractive() const;
    int chooseMove(const Board& b, Side s) const;
};

class SmartPlayer : public Player
{
public:
    SmartPlayer(string name);
    bool isInteractive() const;
   int chooseMove(const Board& b, Side s) const;
private:
    Side Opposite(Side turn) const;
    int evaluate(Side s, Board& b, int hole, int& depth, int& value, Side original_board) const;
    int minimax(Side s, const Board& b, int& bestHole, int& value) const;
};


#endif /* Player_h */
