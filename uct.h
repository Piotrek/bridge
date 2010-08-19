#ifndef __UCT_H__
#define __UCT_H__

#include <list>
#include <vector>
#include <set>

#include "stats.h"

#define MOVES_BEFORE_EXPAND 100
 
class UctNode {
  typedef std::list<UctNode> ChildrenList;

  private:
    int player;
    int card;
    ChildrenList children;
    Stats playoutsStats;
    bool expand;  

  public:
    UctNode (int _player, int _card) {
      player = _player;
      card = _card;
      children = ChildrenList();
      expand = false;
    };
    
    void addChild (UctNode& node);
    void addChildren ();
    void updateStats (float playoutScore);
    int getCard();
    float playNum ();
    float updateUcb (float allPlayouts);
    UctNode selectBestChild ();
    bool expanded();
};


//-----------------------------------

class UctTree {
  
  private:
    std::vector<UctNode> movesHistory;
    UctNode root;
    
  public:
    UctTree (int _player) : root(_player,0) {};
    UctTree (int _player, std::set<int> set1, std::set<int> set2) : root(_player,0) {};
    UctNode selectBestMove();
    UctNode genMove();
    void exploreTree();
};
#endif
