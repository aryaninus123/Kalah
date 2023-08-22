//
//  Game.hpp
//  Project_3
//
//  Created by Aryan Gupta on 5/24/23.
//
#include "Side.h"
#include <vector>
using namespace std;
#ifndef Game_hpp
#define Game_hpp

#include <stdio.h>
class Board
{
public:
    Board(int nHoles, int nInitialBeansPerHole);
    int holes() const;
    int beans(Side s, int hole) const;
    int beansInPlay(Side s) const;
    int totalBeans() const;
    bool sow(Side s, int hole, Side& endSide, int& endHole);
    bool moveToPot(Side s, int hole, Side potOwner);
    bool setBeans(Side s, int hole, int beans);
private:
    int m_totalHoles;
    int m_holesOnSide;
    int m_totalBeans;
    vector<int>m_north;
    vector<int>m_south;
    int m_intial_beans;
//    void Helper();


};
#endif /* Game_hpp */
