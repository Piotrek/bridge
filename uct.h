#ifndef __UCT_H__
#define __UCT_H__

#include <cstdio>
#include <list>
#include <vector>
#include <set>

#include "deal.h"
#include "dummyplayer.h"
#include "stats.h"
#include "utils.h"

#define MOVES_BEFORE_EXPAND 100
#define DEBUG 0
 
class UctNode {
  typedef std::list<UctNode> ChildrenList;

  private:
    int player;
    ChildrenList children;
    bool expand;
    bool last;
    Deal deal;
    Stats playoutsStats;

  public:
    UctNode (int _player, Deal _deal) {
      player = _player;
      children = ChildrenList();
      expand = false;
      deal = _deal;
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
    Deal getDeal();
    int getPlayer() { return player; };
    float statsMean();
    void printNode(int depth);
};


//-----------------------------------

class UctTree {
  
  private:
    std::vector < UctNode* > movesHistory;
    UctNode root;
    
  public:
    UctTree (int _player, Deal _deal) : root(_player, _deal) { root.addChildren(); };
    UctTree (int _player, std::set<int> set1, std::set<int> set2, Deal _deal) : root(_player, _deal) {};
    UctNode* selectBestMove();
    UctNode* genMove();
    void exploreTree();
    void printTree();
};
#endif
