#ifndef __DEAL_H__
#define __DEAL_H__

#include <cstdio>
#include <vector>
#include <set>

#include "utils.h"
#include "myrandom.h"

class Trick
{
  private:
    std::vector < int > cards;
  public:
    Trick() { cards.resize(4); };
    int whoWon(int trump);
    void playCard(int card, int player);
    int firstCard();
    int getCard(int i);
    void printTrick();
};

class Deal
{
  private:
    Trick currentTrick;
    CardsSet cards;
    std::vector < std::vector < int > > playedCards;
    int wonTricks;
    int contractSuit;
    int contractLevel;
    int playerNow;
    int cardsInTrick;
    static MyRandom a;
    
  public:
    Deal() {};
    Deal(CardsSet _cards, Trick _trick, int _suit, int _level, int _won, int _who, int _cit) {
      cards = _cards;
      currentTrick = _trick;
      contractSuit = _suit;
      contractLevel = _level;
      wonTricks = _won;
      playerNow = _who % 4;
      cardsInTrick = _cit;
      playedCards.resize(4);
    };
    int getWonTricks();
    int getContractSuit();
    int getContractLevel();
    Trick getCurrentTrick();
    CardsSet getCards();
    PlayerCardsSet getDummyCards();
    PlayerCardsSet getDeclarerCards();
    PlayerCardsSet getEastCards();
    PlayerCardsSet getWestCards();
    int whoNow();
    int getCardsInTrick();
    int suitOfTrick();
    void incWonTricks();
    int getLastCard();
    void printDeal();
    int playRandomly();
    int playCard1(int who);
    int playCard2(int who, int c1);
    int playCard3(int who, int c1, int c2);
    int playCard4(int who, int c1, int c2, int c3);
    
};

#endif // __DEAL_H__
