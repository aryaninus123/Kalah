//
//  Game.cpp
//  Project_3
//
//  Created by Aryan Gupta on 5/24/23.
//
#include <vector>
#include "Board.h"
#include "Side.h"
using namespace std;
#include <numeric>
# include <iostream>

//construct the board
Board::Board(int nHoles, int nInitialBeansPerHole)
{
    if (nHoles < 0)
        nHoles = 1;
    if (nInitialBeansPerHole < 0)
        nInitialBeansPerHole = 0;
    

    for(int i = 0; i<nHoles+1;i++)
    {
        m_north.push_back(nInitialBeansPerHole);
        m_south.push_back(nInitialBeansPerHole);
    }
    //intialize the two sides as vectors
    m_north.at(0) = 0;
    m_south.at(0) = 0;
    m_intial_beans = nInitialBeansPerHole;
    m_holesOnSide = nHoles;
    m_totalHoles = 2*nHoles;
    m_totalBeans = m_intial_beans*m_totalHoles;
    
}


int Board:: holes() const
{
    return m_holesOnSide;
}

int Board:: beans(Side s, int hole) const
{
 if(s == NORTH)
 {
  if(hole == 0)
      return m_north.at(0);
  else if(hole <= holes())
        return m_north[hole];
 }
    
   if(s == SOUTH)
   {
       if(hole == 0)
           return m_south.at(0);
       else if(hole <= holes())
           return m_south[hole];
   }
    return -1;
}

int Board:: beansInPlay(Side s) const
{
    int m_total = 0;
    if (s == NORTH)
    {
        for (int i = 1; i < m_north.size(); i++)
        {
            m_total += m_north.at(i);
        }
    }
    
    if (s == SOUTH)
    {
        for (int k = 1; k < m_south.size(); k++)
        {
            m_total += m_south.at(k);
        }
    }
    return m_total;
}

int Board:: totalBeans() const
{
    return m_totalBeans;
}
//moves the beans of the selected hole to the pot
bool Board:: moveToPot(Side s, int hole, Side potOwner)
{
    if( hole <=0 || hole > holes())
        return false;

    if(s == NORTH)
    {
      if(potOwner == NORTH)
      {
          m_north[0] += m_north[hole];
          m_north[hole] = 0;
      }
      if(potOwner  == SOUTH)
      {
          m_south[0] += m_north[hole];
          m_north[hole] = 0;
      }
    }
    
    if(s == SOUTH)
    {
        if(potOwner == SOUTH)
        {
            m_south[0] += m_south[hole];
            m_south[hole] = 0;
        }
        if(potOwner == NORTH)
        {
            m_north[0] += m_south[hole];
            m_south[hole] = 0;
        }
    }
    return true;
}

bool Board :: setBeans(Side s, int hole, int beans)
{
    if( hole <=0 || hole > holes())
        return false;
    
    if(s == NORTH)
    {
        m_north[hole] = beans;
    }
    if(s == SOUTH)
    {
        m_south[hole] = beans;
    }
    //add up the total amount of beans
    m_totalBeans = 0;
    for(auto it = m_north.begin(); it!= m_north.end(); it++)
    {
        m_totalBeans+= *it;
    }
    cout<<endl;

    for(auto it = m_south.begin(); it!= m_south.end(); it++)
    {
        m_totalBeans+= *it;
    }
    return true;
}
bool Board:: sow(Side s, int hole, Side &endSide, int &endHole)
{
    // if the hole is invalid or the hole is the pot
    if( hole <=0 || hole > holes())
        return false;
    if (s == NORTH)
    {
        // to sow the hole can not be empty
        if(m_north[hole] == 0)
            return false;
        int amount = m_north[hole];
        //we are starting our counterclockwise movement
        int current_hole = hole-1;
        Side current  = NORTH;
        //the current hole must become empty
        m_north[hole] = 0;
        
        //the loop will continue untill all the beens have been sown
        while(amount != 0)
        {
            //You are at North's pot
            if(current == NORTH && current_hole == 0)
            {
                m_north[0] +=1 ;
                amount-= 1;
                current = SOUTH;
                current_hole = 1;
            }
            else if(current == NORTH && current_hole < m_north.size())
            {
                m_north[current_hole]  = m_north[current_hole] + 1 ;
                amount = amount -1;
                current = NORTH;
                current_hole = current_hole - 1;
            }
            // you are at South's pot
            else if (current == SOUTH && current_hole == m_north.size())
            {
                current = NORTH;
                current_hole = m_north.size()-1.0;
            }
            else if(current == SOUTH)
            {
                m_south[current_hole]+=1;
                amount-=1;
                current = SOUTH;
                current_hole+=1;
            }
        }
           
        endSide = current;
        endHole = current_hole;
        //At South's Pot
        if(current == SOUTH && current_hole == m_south.size())
        {
            endHole = current_hole-1;

        }
        else if(current == SOUTH && current_hole == 1)
        {
            endSide = NORTH;
            endHole = 0;

        }
        else if (endSide == SOUTH)
        {
            endHole-= 1;

        }
        else if(endSide ==NORTH)
        {
            endHole = endHole + 1;
        }
    }
    //repeat the same process if the side is SOUTH
    if(s == SOUTH)
    {
        if(m_south[hole] == 0)
            return false;
        int amount = m_south[hole];
        //we are starting our counterclockwise movement
        int current_hole = hole+1;
        Side current  = SOUTH;
        //the current hole must become empty
        m_south[hole] = 0;
        while(amount!=0)
        {
            //at South's pot
            if(current == SOUTH && current_hole > holes() )
            {
                current = NORTH;
                m_south[0]+=1;;
                amount--;
                current_hole = holes();
            }
            else if(current == SOUTH && current_hole < m_south.size() )
            {
                current = SOUTH;
                m_south[current_hole]+=1;
                amount--;
                current_hole++;
            }
            //At North's pot
            else if(current == NORTH && current_hole == 0)
            {
                current_hole = 1;
                current = SOUTH;
            }
            else if (current == NORTH  )
            {
                current = NORTH;
                m_north[current_hole]+=1;
                amount--;
                current_hole--;
            }
        }
        endSide = current;
        endHole = current_hole;
        //the last hole in North, meaning one before is South Pot
        if(endSide == NORTH && endHole == holes())
        {
            endSide = SOUTH;
            endHole = 0;
        }
        else if (endSide == SOUTH)
        {
            endHole = current_hole-1;
        }
        else if (endSide == NORTH)
        {
            endHole = current_hole + 1;
        }

    }
    return true;
}
//void Board :: Helper()
//{
////    cout<<m_north.size();
////    cout<<holes();
//    for(auto it = m_north.begin(); it!= m_north.end(); it++)
//    {
//         cout<<*it;
//    }
//    cout<<endl;
//
//    for(auto it = m_south.begin(); it!= m_south.end(); it++)
//    {
//         cout<< *it;
//    }
//    //    cout << "\nSum = " << accumulate(m_north.begin(), m_south.end(), 0);
//}
