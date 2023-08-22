//
//  main.cpp
//  Project_3
//
//  Created by Aryan Gupta on 5/24/23.
//

#include <iostream>
#include "Board.h"
#include "Side.h"
#include "Game.h"
#include <iostream>
#include <cassert>
using namespace std;


int main()
{
    SmartPlayer bp1("Bart");
    BadPlayer bp2("Homer");
    Board b(3, 2);
    Game g(b, &bp1, &bp2);
    g.play();
}
