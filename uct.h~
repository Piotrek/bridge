#ifndef __UCT_H__
#define __UCT_H__

#include <list>

#include "stats.h"

 
class UctNode {
  typedef std::list<UctNode> ChildrenList;
  
  public:
    UctNode (int _player) {
      player = _player;
      expand = false;
    };
    
    void addChild (UctNode& node);
    void updateStats (float playout);
    void updateUcb (float coeff);
    UctNode selectBestChild ();
    
  private:
    int player;
    ChildrenList children;
    Stats playouts_stats;
    bool expand;
};

#endif