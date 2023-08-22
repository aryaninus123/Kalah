//
//  Header.h
//  Project_3
//
//  Created by Aryan Gupta on 5/24/23.
//

#ifndef Header_h
#define Header_h

enum Side { NORTH, SOUTH };

const int NSIDES = 2;
const int POT = 0;

inline
Side opponent(Side s)
{
    return Side(NSIDES - 1 - s);
}

#endif /* Header_h */
