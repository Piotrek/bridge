#ifndef __UCT_H__
#define __UCT_H__

#include <cstdio>
#include <list>
#include <vector>
#include <set>

#include "deal.h"
#include "stats.h"
#include "utils.h"
#include "main.h"

#define MOVES_BEFORE_EXPAND 50
#define DEBUG_UCT 0
#define EXPLORE_NUM 1000000

extern Deal *deal;
 
 
class UctNode {
  typedef std::list<UctNode> ChildrenList;
  
  private:
    int player;
    int card;
    ChildrenList children;
    bool expand;
    bool last;
    Stats playoutsStats;

  public:
    UctNode (int _player, int _card) {
      player = _player;
      card = _card;
      children = ChildrenList();
      expand = false;
      last = false;
    };
    
    void addChild (UctNode& node);
    void addChildren ();
    void updateStats (float playoutScore);
    int getCard();
    float playNum ();
    float updateUcb (float allPlayouts);
    UctNode* selectUCBChild ();
    UctNode* selectBestChild ();
    bool expanded();
    bool isLast();
    void setLast();
    int getPlayer();
    float statsMean();
    void printNode(int depth);
    int getChildrenSize();
};


//-----------------------------------

class UctTree {
  
  private:
    std::vector < UctNode* > movesHistory;
    DealSet deals;
    UctNode root;
    
  public:
    UctTree (int _player, DealSet _deals) : root(_player, 0) {
      deals = _deals;
      root.addChildren();
    };
    UctNode* selectBestMove();
    UctNode* genMove();
    void exploreTree();
    void printTree();
};
#endif
