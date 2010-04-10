#include <cstdio>

#include "uct.h"


void UctNode::addChild (UctNode& node) {
  children.push_front(node); 
}

void UctNode::updateStats (float playout) {
  playoutsStats.update(playout);  
}

float UctNode::updateUcb (float coeff) {
  return playoutsStats.updateUcb(coeff); 
}

UctNode UctNode::selectBestChild () {
	ChildrenList::iterator it;
	float ucb, maxUcb = 0;
	float coeff = playoutsStats.playNum();
	UctNode* bestNode;
	
	for (it = children.begin(); it != children.end(); it++) {
		ucb = (*it).updateUcb(coeff);
		if (ucb > maxUcb)
			bestNode = &(*it);
	}
	
  return *bestNode;  
}

bool UctNode::expanded() {
	return expand;
}

UctNode UctTree::selectBestMove () {
	UctNode node = root;
	float playout;
	
	moves.clear();
	moves.push_back(node);
	while (node.expanded()) {
		node = node.selectBestChild();
		moves.push_back(node);
	}
	
	playout = 1.0; //play best node

  for (int i=moves.size(); i >= 0; i--)
    moves[i].updateStats(playout);
	
  return node;
}
