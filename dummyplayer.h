#ifndef __DUMMYPLAYER_H__
#define __DUMMYPLAYER_H__

#include <vector>
#include <set>

#include "uct.h"
#include "myrandom.h"

class Dummyplayer {
  private:
    std::vector < std::vector < std::set <int> > > players;
    MyRandom a;
    int card[4];
    int whoWon (int* card);
    int playCard1(int who);
    int playCard2(int who, int c1);
    int playCard3(int who, int c1, int c2);
    int playCard4(int who, int c1, int c2, int c3);

  public:
    Dummyplayer (UctNode node) : a(time(NULL)) {
      //TODO
    };
    float play_randomly();
};
#endif
