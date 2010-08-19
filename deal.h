#ifndef __DEAL_H__
#define __DEAL_H__

#include <vector>
#include <set>

class Deal
{
  private:
    std::vector < int > currentTrick;
    std::set < int > dummyCards;
    std::set < int > declarerCards;
    std::set < int > eastCards;
    std::set < int > westCards;
    int numEastCards;
    int numWestCards;
    int wonTricks;
    int contractSuit;
    int contractLevel;      
    
  public:
    Deal(std::set<int> _dummy, std::set<int> _declarer, std::set<int> _defenders, std::vector<int> _trick, int _suit, int _level) {
      dummyCards = _dummy;
      declarerCards = _declarer;
      eastCards = _defenders;
      westCards = _defenders;
      currentTrick = _trick;
      contractSuit = _suit;
      contractLevel = _level;
      wonTricks = 0;
    };
    int getWonTricks();
    int getContractSuit();
    int getContractLevel();
    int getNumEastCards();
    int getNumWestCards();
    std::vector < int > getCurrentTrick();
    std::set < int > getDummyCards();
    std::set < int > getDeclarerCards();
    std::set < int > getEastCards();
    std::set < int > getWestCards();
    void decEastCards();
    void decWestCards();
};

#endif // __DEAL_H__
