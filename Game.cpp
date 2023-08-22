//
//  Game.cpp
//  Project_3
//
//  Created by Aryan Gupta on 5/28/23.
//

#include "Game.h"
//intialize the game
Game::Game(const Board& b, Player* south, Player* north): m_board(b), m_south(south), m_north(north)
{
    m_turn = south;
    finished  = false;
}

void Game:: display() const
{
    //display the name of the player of the North Side
    cout<<"    "<<m_north->name()<<"    "<<endl;
    cout<<"   ";
    
    for(int i = 1; i<= m_board.holes(); i++)
    {
        cout<<beans(NORTH, i)<< "  ";
    }
    cout<<endl;
    //display the North Pot
    cout<<" "<<beans(NORTH, 0);
    int space = m_board.holes() * 3;

    for(int i = 0; i<space;i++)
    {cout<<" ";}
    //display South Pot
    cout<<beans(SOUTH, 0)<<endl;
    
    cout << "   ";
        for (int i = 1; i <= m_board.holes(); i++)           //South player holes
        {
            cout << beans(SOUTH, i) << "  ";
        }
        cout << endl;
    
    
    cout<<"    "<<m_south->name()<<"    "<<endl;  // South Name

}

void Game:: status(bool &over, bool &hasWinner, Side &winner) const
{
    //the game is NOT over
    if (m_board.beansInPlay(NORTH) > 0 && m_board.beansInPlay(SOUTH) > 0)   //more moves are possible
        {
            over = false;
            return;
        }
    else
        {
            over = true;
             if (beans(SOUTH, 0) > beans(NORTH, 0))
            {
                hasWinner = true; //south is the winner
                winner = SOUTH;
            }

            else if (beans(NORTH, 0) > beans(SOUTH, 0)) //north is the winner
            {
                hasWinner = true;
                winner = NORTH;
            }
            else
                hasWinner = false; //the game has ended in a tie
        }
}


bool Game:: move(Side s)
{
    
    display();
    status(finished, winner, winner_side);
    if(finished == true)
    {
        //sow the remaining bean on the south side
        if(m_board.beansInPlay(s) == 0 && m_board.beansInPlay(opponent(s)) != 0 )
        {
            for(int i = 1; i<= m_board.holes(); i++)
            {
                m_board.moveToPot(opponent(s), i, opponent(s));
            }
            display();
            //end the game
            status(finished, winner, winner_side);

        }

        return false;
    }
    
    if(s == SOUTH)
    {
        m_turn = m_south;
    }
    else
    {
        m_turn = m_north;
    }
    
    
    //helps us find out with hole we want to chose
    int new_hole = m_turn->chooseMove(m_board, s);
    
    int end_hole;
    Side last_side;
    //do the motion of actually moving which invovles sowing
    if(m_board.sow(s, new_hole, last_side, end_hole) == true)
    {
    
        status(finished, winner, winner_side);
        if(finished == true)
            return move(last_side); //if the game is over sow the remaining beans by calling the move function again
        //landed on your own pot and you have another turn
        if(end_hole == 0)
        {
            display();
            cout<<m_turn->name()<<" has another turn"<<endl;
            return move(last_side);
        }
        else if((beans(last_side,end_hole) == 1) && (beans(Opposite(s),end_hole) >0) && last_side == s )
        {
            //move your one bean to your own pot
            m_board.moveToPot(s, end_hole, s);
            //capture the other person's hole and move it to your pot
            m_board.moveToPot(Opposite(s), end_hole, s);
        }
    }
    
    
    return true;
}
void Game:: play()
{
    m_play(SOUTH, false, 0); // calls the helper function
}

void Game:: m_play(Side s, bool f, int x)
{
    //if neither player is interactive
    if(m_south->isInteractive() == false && m_north->isInteractive()== false)
    {
        if(move(s) == true)
        {
            if(f == true)
            {
                cout<<"Press ENTER";
                if(x == 1)
                {
                    string enter;
                    getline(cin, enter);
                }
                cin.ignore(10000, '\n');
                cout << endl;
            }
            //recursion to call the move function again and to change sides
            m_play(Opposite(s), !f, x+1);
        }
        else
        {
            move(s);
            if(winner == false)
            {
                cout<<"The game has resulted in a tie"<<endl;
            }
            else
            {
                move(s); // call the move function again to make sure the correct status is displayed
                if(winner_side == SOUTH)
                {
                    cout<<"The winner of the game is "<<m_south->name()<<endl;
                }
                else
                {
                    cout<<"The winner of the game is "<<m_north->name()<<endl;
                }
            }
        }
    }
     else
        {
            if(move(s) == true)
            {
                m_play(Opposite(s), !f, x+1); //change sides
            }
            else
            {
                move(s);
                if(winner == false)
                {
                    cout<<"The game has resulted in a tie";
                }
                else
                {
                    move(s); //call the move function again to make sure the correct status is displayed
                    if(winner_side == SOUTH)
                        cout<<"The winner of the game is "<<m_south->name();
                    else
                        cout<<"The winner of the game is "<<m_north->name();
                }
            }
        }
        
        
        
        
    }



int Game::beans(Side s, int hole) const
{
    return m_board.beans(s, hole);
}

Side Game:: Opposite(Side turn)
{
if(turn == NORTH)
    return SOUTH;
else
    return NORTH;
}
