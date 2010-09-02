#ifndef __DEAL_H__
#define __DEAL_H__

#include <cstdio>
#include <vector>
#include <set>
#include "utils.h"

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
    std::vector < std::vector < std::set < int > > > cards;
    int wonTricks;
    int contractSuit;
    int contractLevel;
    int playerNow;
    int cardsInTrick;
    
  public:
    Deal() {};
    Deal(std::vector < std::vector < std::set < int > > > _cards, Trick _trick, int _suit, int _level, int _won, int _who, int _cit) {
      cards = _cards;
      currentTrick = _trick;
      contractSuit = _suit;
      contractLevel = _level;
      wonTricks = _won;
      playerNow = _who % 4;
      cardsInTrick = _cit;
    };
    int getWonTricks();
    int getContractSuit();
    int getContractLevel();
    Trick getCurrentTrick();
    std::vector < std::vector < std::set < int > > > getCards();
    std::vector < std::set < int > > getDummyCards();
    std::vector < std::set < int > > getDeclarerCards();
    std::vector < std::set < int > > getEastCards();
    std::vector < std::set < int > > getWestCards();
    int whoNow();
    int getCardsInTrick();
    int suitOfTrick();
    void incWonTricks();
    int getLastCard();
    void printDeal();
};

#endif // __DEAL_H__
