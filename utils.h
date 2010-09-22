#ifndef __UTILS_H__
#define __UTILS_H__

#include <vector>
#include <set>

#define W 0
#define N 1
#define E 2
#define S 3

#define NT 4
#define Sp 3
#define H 2
#define D 1
#define C 0

#define DEALS_NUM 10

typedef std::vector < std::vector < std::set < int > > > CardsSet; 
typedef std::vector < std::set < int > >  PlayerCardsSet; 


int changeCardToNumber (char *c);
char* changeNumberToCard (int num, char* c);

#endif
