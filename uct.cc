#include "uct.h"

void UctNode::addChild (UctNode& node) {
  children.push_front(node); 
}

void UctNode::updateStats (float playoutScore) {
  playoutsStats.update((player % 2) * playoutScore + (1 - (player % 2)) * (1.0 - playoutScore));  
}

float UctNode::updateUcb (float allPlayouts) {
  return playoutsStats.updateUcb(allPlayouts);
}

float UctNode::playNum () {
  return playoutsStats.playNum();
}	

UctNode* UctNode::selectUCBChild () {
  ChildrenList::iterator it;
  float ucb, maxUcb = 0;
  float allPlayouts = playoutsStats.playNum();
  UctNode* bestNode;

  for (it = children.begin(); it != children.end(); it++) {
    ucb = (*it).updateUcb(allPlayouts);
    if (ucb > maxUcb) {
      bestNode = &(*it);
      maxUcb = ucb;
    }
  }
  return bestNode;  
}

UctNode* UctNode::selectBestChild () {
  ChildrenList::iterator it;
  float mean, maxMean = 0;
  UctNode* bestNode;

  for (it = children.begin(); it != children.end(); it++) {
    mean = (*it).statsMean();
    if (mean > maxMean) {
      bestNode = &(*it);
      maxMean = mean;
    }
  }
  return bestNode;  
}

void UctNode::addChildren() {
  int who, who2;
  std::vector < std::set < int > > allCards;
  std::set < int > cards;
  std::set < int >::iterator it;
  UctNode* node;
  
  expand = true;
  who = deal->getWhoNow();
  allCards = deal->getCards()[who];
  if ((deal->getCardsInTrick() == 0) or (allCards[deal->suitOfTrick()].empty())) {
    cards.clear();
    for (int i = 0; i < 4; i++)
      cards.insert(allCards[i].begin(), allCards[i].end());
  }
  else {
    cards = allCards[deal->suitOfTrick()];
  }
  
  for (it = cards.begin(); it != cards.end(); it++) {
    node = new UctNode(who, *it);
    if (deal->getCardsInTrick() == 3) {
      who2 = (who + 1) % 4;
      if (deal->endOfDeal(who2))
        node->setLast();
    }
    addChild(*node);
  }  
}

int UctNode::getCard() {
  return card;
}

int UctNode::getPlayer() {
  return player;
}

bool UctNode::expanded() {
  return expand;
}

bool UctNode::isLast() {
  return last;
}

void UctNode::setLast() {
  last = true;
}

float UctNode::statsMean() {
  return playoutsStats.mean();
}

void UctNode::printNode(int depth) {
  char c[3];
  
  if (depth >= 4) return;
  for (int i = 0; i < depth; i++)
    fprintf(stderr, "   ");
  changeNumberToCard(getCard(), c); 
  if (last)
    fprintf(stderr, "NODE - pl: %d card: %s mean: %f\n", player, c, -1.0);
  else
    fprintf(stderr, "NODE - pl: %d card: %s mean: %f\n", player, c, playoutsStats.mean());
  for (ChildrenList::iterator it = children.begin(); it != children.end(); it++)
    it->printNode(depth + 1);
}

//TODO
UctNode* UctTree::genMove() {
  for (int i = 0; i < 2; i++) {
    //printf("%d\n", i);
    exploreTree();
  }
  printTree();
  return selectBestMove();
}

void UctTree::exploreTree() {
  UctNode* node = &root;
  float playoutScore;
  if (DEBUG) printf("nowe chodzenie po drzewie UCT\n");
  movesHistory.clear();
  movesHistory.push_back(node);
  if (DEBUG) printf("poki ma dzieci idziemy dalej\n");
  while (node->expanded()) {
    node = node->selectUCBChild();
    deal->playCard(node->getCard());
    movesHistory.push_back(node);
  }

  if (DEBUG) printf("jesli juz wystarczajaco ruchow to rozbudowywujemy drzewo UCT\n");
  if (node->playNum() > MOVES_BEFORE_EXPAND) {
    node->addChildren();
    node = node->selectUCBChild();
    deal->playCard(node->getCard());
    movesHistory.push_back(node);
  }
  if (DEBUG) printf("zapuszczamy playout\n");
  if (node->isLast()) {
    if (deal->getWonTricks() >= deal->getContractLevel() + 6)
      playoutScore = 1.0;
    else
      playoutScore = 0.0;
    movesHistory.pop_back();
  } 
  else {
    if (DEBUG) printf("random_play\n");
    int tricksWon = deal->playRandomly();
    if (DEBUG) printf("%d\n", tricksWon);
    if (tricksWon >= deal->getContractLevel() + 6)
      playoutScore = 1.0;
    else
      playoutScore = 0.0;
  }
  
  if (DEBUG) printf("uaktualniamy wezly dla ruchow ze sciezki\n");
  for (int i = movesHistory.size()-1; i >= 1; i--) {
    movesHistory[i]->updateStats(playoutScore);
    deal->undoCard(movesHistory[i]->getPlayer(), movesHistory[i]->getCard());
  }
  movesHistory[0]->updateStats(playoutScore);
    
  if (DEBUG) printf("cofamy wszystkie ruchy\n");
  deal->undoAllCards();
  if (DEBUG) printf("ruchy wycofane\n");
  
    
}

UctNode* UctTree::selectBestMove () {
  return root.selectBestChild();
}

void UctTree::printTree () {
  root.printNode(0);
}
