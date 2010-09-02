#ifndef __DUMMYPLAYER_H__
#define __DUMMYPLAYER_H__

#include <vector>
#include <set>
#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "myrandom.h"
#include "deal.h"

class Dummyplayer {
  private:
    std::vector < std::vector < std::set <int> > > cards;
    static MyRandom a;
    Trick currentTrick;
    int cardsInTrick;
    int contractSuit;

  public:
    Dummyplayer (Deal* deal) {
      cards = deal->getCards();
      currentTrick = deal->getCurrentTrick();
      cardsInTrick = deal->getCardsInTrick();
      contractSuit = deal->getContractSuit();
    };
    int play_randomly(int tricksWon, int lastWinner);
    int playCard1(int who);
    int playCard2(int who, int c1);
    int playCard3(int who, int c1, int c2);
    int playCard4(int who, int c1, int c2, int c3);

};

class Betterplayer {
  private:
    std::vector < std::vector < std::set <int> > > cards;
    static MyRandom a;
    Trick currentTrick;
    int cardsInTrick;
    int contractSuit;

  public:
    Betterplayer (Deal* deal) {
      cards = deal->getCards();
      currentTrick = deal->getCurrentTrick();
      cardsInTrick = deal->getCardsInTrick();
      contractSuit = deal->getContractSuit();
    };
    int play_randomly(int tricksWon, int lastWinner);
    int playCard1(int who);
    int playCard2(int who, int c1);
    int playCard3(int who, int c1, int c2);
    int playCard4(int who, int c1, int c2, int c3);

};

#endif
