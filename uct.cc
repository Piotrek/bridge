#include <cstdio>

#include "uct.h"


void UctNode::addChild (UctNode& node) {
  children.push_front(node); 
}

void UctNode::updateStats (float playout) {
  playouts_stats.update(playout);  
}

void UctNode::updateUcb (float coeff) {
  playouts_stats.updateUcb(coeff); 
}

UctNode UctNode::selectBestChild () {
  return UctNode(0);  
}

