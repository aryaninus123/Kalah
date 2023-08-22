//
//  Player.cpp
//  Project_3
//
//  Created by Aryan Gupta on 5/28/23.
//

#include "Player.h"
#include <string>

//========================================================================
// Timer t;                 // create and start a timer
// t.start();               // restart the timer
// double d = t.elapsed();  // milliseconds since timer was last started
//========================================================================

#include <chrono>

class Timer
{
  public:
    Timer()
    {
        start();
    }
    void start()
    {
        m_time = std::chrono::high_resolution_clock::now();
    }
    double elapsed() const
    {
        std::chrono::duration<double, std::milli> diff =
                          std::chrono::high_resolution_clock::now() - m_time;
        return diff.count();
    }
  private:
    std::chrono::high_resolution_clock::time_point m_time;
};


using namespace std;

//Base class

Player:: Player(string name) : m_name(name)
{}

string Player :: name() const
{
    return m_name;
}

bool Player :: isInteractive() const
{
    return false;
}

int Player :: chooseMove(const Board &b, Side s) const
{
    return  -1;
}

Player :: ~Player()
{}

//The human Player class
HumanPlayer::HumanPlayer(string name) : Player(name)
{}

bool HumanPlayer::isInteractive() const
{
    return true;
}

int HumanPlayer:: chooseMove(const Board &b, Side s) const
{
    //if there are no beans in player then there are no moves to make
    if(b.beansInPlay(s) == 0 )
        return -1;
    int move;
    cout<<this->name()<<": "<<"Please enter a valid hole: ";
    cin>>move;
    cout<<endl;
    if(move<=0 || move > b.holes() || b.beans(s, move) == 0 )
    {
        cout<<"Please enter a valid hole " <<endl;
        return chooseMove(b, s); // recursion to make sure the player enters a valid hole
    }
    else
    {
        return move;
    }
}
//BAD PLAYER//

BadPlayer :: BadPlayer(string name) : Player(name)
{}

bool BadPlayer:: isInteractive() const
{   return false; }

int BadPlayer:: chooseMove(const Board &b, Side s) const
{
   //Returns the first hole on the North Side that has beans
    if (s == NORTH)
    {
        for(int i = 1; i<=b.holes(); i++)
        {
         if(b.beans(s,i) != 0)
             return i;
        }
    }
    //Returns the first hole on the South Side that has beans
    if (s == SOUTH)
    {
        for(int i = 1; i<=b.holes(); i++)
        {
         if(b.beans(s,i) != 0)
             return i;
        }
    }
    return -1;
}

//SMART PLAYER//


SmartPlayer::SmartPlayer(string name) : Player(name)
{}

bool SmartPlayer::isInteractive() const
{
    return false;
}

int SmartPlayer:: chooseMove(const Board &b, Side s) const

{
    //by default the besthole is the first hole
    int bestHole = 1;
    int value = 0;
    return minimax(s, b, bestHole, value);
}

Timer timer;
int SmartPlayer:: minimax(Side s, const Board& b, int& bestHole, int& value) const
{
    Board copy = b;
    Board original_board  = b;
    
    int depth = 5;
    //if there are no beans then we cant continue
    if(copy.beansInPlay(s) == 0)
    {
        bestHole = -1;
        value = evaluate(s,  copy, bestHole, depth, value, s);
        return bestHole;
    }
    
    int max = -1000;
    for(int i = 1; i<= copy.holes(); i++)
    {
        depth = 5;
        value = 0;
        copy = original_board;
        
        if(copy.beans(s, i) > 0)
        {
            int maximum = evaluate(s, copy, i, depth, value, s); //if the value found is greater then that value becomes the maximum
            if(maximum > max)
            {
                max = maximum;
                bestHole = i;
            }
            
        }
        
        
        
        //we already found the best hole we dont have to do anything else
        if(max == 1000)
        {
            return bestHole;
        }
        //find the value of the next hole
        copy = original_board;
        if(i+1 > copy.holes() || copy.beans(s, i+1) == 0 )
            continue;
        depth = 5;
        value = 0;
        //if the value of the next hole is greater that becomes the bestHole
        if (max <= evaluate(s,copy, i+1, depth, value, s))
        {
            bestHole = i+1;
            max = evaluate( s,copy, i+1, depth, value, s);
            
        }
    }
    depth = 5;
    evaluate(s, copy,bestHole, depth, value, s);
    
    //return the best hole through our algorithim
    return bestHole;
}

int SmartPlayer::evaluate(Side s, Board& b, int hole, int& depth, int& value, Side original_board) const

{
//    if(timer.elapsed() > 4990)
//        return value;
    //if there is no depth then just return the value
    if(depth == 0)
    {
        return value;
    }
    
    int endHole = 0;
    Side endSide;
    //sow at the hole if its fale return the value
    if(b.sow(s, hole, endSide, endHole) == false)
        return value;
    //track all the next moves
    if(endHole == 0)
    {
        if(b.beansInPlay(SOUTH) != 0 && b.beansInPlay(NORTH)!= 0)
        {
            //arbitary value can be any value
            int max = -1000;
            Board current = b;
            
            for(int k = 1; k <= b.holes();k++)
            {
                b = current;
                value = 1;
                int check_board = evaluate(s, b, k, depth, value, original_board);
                if(check_board >= max)
                {
                    max = check_board;
                }
                
            }
            return max;
        }
    }
    
    //make a capture
    if((b.beans(Opposite(s), endHole)> 0) && (endSide == s && b.beans(s, endHole) == 1))
    {
        //move the bean to their respective pots
        b.moveToPot(s, endHole, s);
        b.moveToPot(Opposite(s), endHole, s);
    }
    
    if(b.beansInPlay(SOUTH) == 0 && b.beansInPlay(NORTH) == 0)
    {
        for(int i = 0 ; i<= b.holes(); i++)
        {
            b.moveToPot(NORTH, i, NORTH); // sweep the remaining beans to their respective pots
            b.moveToPot(SOUTH, i, SOUTH);
        }
        if(original_board == SOUTH)
        {
            //we lost
            if(b.beans(original_board, 0) < b.beans(NORTH, 0))
                value = -1000;
            //we won
            else if( b.beans(original_board, 0)> b.beans(NORTH, 0) )
                value = 1000;
            //we tied
            else
                value = 0;
        }
        //repeat the process if the side is NORTH
        if(original_board == NORTH)
        {
            if(b.beans(original_board, 0) < b.beans(SOUTH, 0))
                value = -1000;
            else if( b.beans(original_board, 0)> b.beans(SOUTH, 0) )
                value = 1000;
            else
                value = 0;
        }

       //return the value if the game is over
        return value;
    }
    
    //if the game is still going on
    value = b.beans(original_board, 0) - b.beans(Opposite(original_board), 0);
    
    
    //checking the value of our opponent
    if(original_board == s)
    {
        int minimum = 1000;
        Board copy = b;
        
        depth = depth -1;
        for(int i = 1; i < b.holes(); i++)
        {
            b = copy; // make a copy of the board
            if(original_board == SOUTH)
            {
                if(b.beans(NORTH, i) == 0) //if there are no beans at the hole continue the for loop and dont check the other functions
                    continue;
                int checkMove = evaluate(NORTH, b, i, depth, value, original_board);
                if(checkMove < minimum) //finds the worst move to make
                {
                    minimum = checkMove;
                }
            }
            else
            {
                if(b.beans(SOUTH, i) == 0)
                    continue;
                int checkMove = evaluate(SOUTH, b, i, depth, value, original_board);
                if(checkMove < minimum)
                {
                    minimum = checkMove;
                }

            }
            
        }
        int lastMove = 0;
        if(original_board == SOUTH)
            lastMove = evaluate(SOUTH, b, b.holes(), depth, value, original_board);
        else
            lastMove = evaluate(NORTH, b, b.holes(), depth, value, original_board);
        if(lastMove < minimum)
            minimum = lastMove;
        return minimum;
    }
    else
    {
        //its out turn again
        depth = depth -1 ;
        int maximum = -1000;
        Board copy = b;
        
        depth = depth -1;
        for(int i = 1; i< b.holes(); i++)
        {
            b = copy; // make a copy of the board
            if(original_board == SOUTH)
            {
                if(b.beans(NORTH, i) == 0)
                    continue;
                int checkMove = evaluate(NORTH, b, i, depth, value, original_board); //checks the best move to make
                if(checkMove > maximum)
                {
                    maximum = checkMove;
                }
            }
            else
            {
                if(b.beans(SOUTH, i) == 0)
                    continue;
                int checkMove = evaluate(SOUTH, b, i, depth, value, original_board);
                if(checkMove > maximum)
                {
                    maximum = checkMove;
                }

            }
            
        }
        int lastMove = 0;
        if(original_board == SOUTH)
            lastMove = evaluate(SOUTH, b, b.holes(), depth, value, original_board);
        else
            lastMove = evaluate(NORTH, b, b.holes(), depth, value, original_board);
        if(lastMove > maximum)
            maximum = lastMove;
        return maximum;

    }
    
    
    
    
    return true;
}

Side SmartPlayer::Opposite (Side turn) const
{
    if (turn == SOUTH)
        return NORTH;
    else
        return SOUTH;
}
