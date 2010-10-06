#ifndef __DEAL_H__
#define __DEAL_H__

#include <cstdio>
#include <vector>
#include <set>
#include <utility>
#include <iostream>

#include "utils.h"
#include "myrandom.h"



#define DEBUG_DEAL 0

class Trick
{
  private:
    std::vector < int > cards;
    int cardsInTrick;
  public:
    Trick() : cardsInTrick(0) { cards.resize(4); };
    Trick(int _cardsInTrick) : cardsInTrick(_cardsInTrick) { cards.resize(4); };
    int whoWon(int trump);
    void playCard(int card, int player);
    void playCard(int card);
    int firstCard();
    int getCard(int i);
    int getCardsInTrick();
    void printTrick();
    void printTrickSymbols();
};

class Deal
{
  private:
    Trick currentTrick;
    Trick startTrick;
    CardsSet cards;
    std::vector < int > playedCards;
    std::vector < int > whoseCards;
    int wonTricks;
    int startWonTricks;
    int contractSuit;
    int contractLevel;
    int whoNow;
    int whoStarts;
    static MyRandom a;
    
  public:
    Deal() {};
    Deal(CardsSet _cards, Trick _trick, int _suit, int _level, int _won, int _who, std::vector < int > _whoseCards) {
      cards = _cards;
      currentTrick = _trick;
      startTrick = _trick;
      contractSuit = _suit;
      contractLevel = _level;
      wonTricks = _won;
      startWonTricks = _won;
      whoNow = _who % 4;
      whoStarts = _who % 4;
      whoseCards = _whoseCards;
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
    int getWhoNow();
    int suitOfTrick();
    int getCardsInTrick();
    void incWonTricks();
    int getLastCard();
    void printDeal();
    int playRandomly();
    int playCard1(int who);
    int playCard2(int who, int c1);
    int playCard3(int who, int c1, int c2);
    int playCard4(int who, int c1, int c2, int c3);
    int playRandomSmallCard(int who, int trumps);
    int playLowestCardFromThisSuit(int who, int suit);
    int playTryToOverruff(int who, int suit, int card);
    int playRandomCardFromThisSuit(int who, int suit);
    int playRandomCard(int who);
    int playTryToTakeThisTrick3(int who, int c1, int c2); /* plays player 3 */
    int playDoNotBlock(int who, int suit);
    int playTheLowestHigherCard(int who, int suit, int card);
    void playCard(int card);
    void playUserCard(int card);
    void undoAllCards(); 
    void undoCard(int player, int card);
    bool endOfDeal(int player);
    bool isVoid(int player, int suit);
    std::vector <int> getWhoseCards(); 
};

typedef std::vector <Deal*>  DealSet;

#endif // __DEAL_H__
