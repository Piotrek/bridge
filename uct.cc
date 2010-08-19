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

float UctNode::playNum () {
  return playoutsStats.playNum();
}	

UctNode UctNode::selectBestChild () {
  ChildrenList::iterator it;
  float ucb, maxUcb = 0;
  float allPlayouts = playoutsStats.playNum();
  UctNode* bestNode;

  for (it = children.begin(); it != children.end(); it++) {
    ucb = (*it).updateUcb(allPlayouts);
    if (ucb > maxUcb)
      bestNode = &(*it);
  }
  return *bestNode;  
}

//TODO
void UctNode::addChildren() {
}

int UctNode::getCard() {
  return card;
}

bool UctNode::expanded() {
  return expand;
}

UctNode UctTree::genMove() {
  exploreTree();
  return selectBestMove();
}

void UctTree::exploreTree() {
  UctNode node = root;
  float playoutScore;

  // nowe chodzenie po drzewie UCT
  movesHistory.clear();
  movesHistory.push_back(node);
  // poki ma dzieci idziemy dalej
  while (node.expanded()) {
    node = node.selectBestChild();
    movesHistory.push_back(node);
  }

  // jesli juz wystarczajaco ruchow to rozbudowywujemy drzewo UCT
  if (node.playNum() > MOVES_BEFORE_EXPAND) {
    node.addChildren();
    node = node.selectBestChild();
  }

  // zapuszczamy playout
  playoutScore = 1.0;

  // uaktualniamy wezly dla ruchow ze sciezki
  for (int i = movesHistory.size()-1; i >= 0; i--)
    movesHistory[i].updateStats(playoutScore);
}

UctNode UctTree::selectBestMove () {
  return root.selectBestChild();
}
