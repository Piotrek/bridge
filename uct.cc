#include <set>

#include "uct.h"


void UctNode::addChild (UctNode& node) {
  children.push_front(node); 
}

void UctNode::updateStats (float playoutScore) {
  playoutsStats.update(player * playoutScore + (1 - player) * (1.0 - playoutScore));  
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
    //fprintf(stderr, "%d %f %f\n", it->getCard(), ucb, maxUcb);
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
    //fprintf(stderr, "%d %f %f %f\n", it->getCard(), it->playNum(), mean, maxMean);
  }
  return bestNode;  
}

void UctNode::addChildren() {
  int who, who2;
  std::vector < std::set < int > > allCards;
  std::set < int > cards;
  std::set < int >::iterator it;
  UctNode* node;
  Deal* newDeal;
  Trick newTrick;
  std::vector < std::vector < std::set < int > > > newCards;
  
  expand = true;
  who = deal.whoNow();
  allCards = deal.getCards()[who];
  if ((deal.getCardsInTrick() == 0) or (allCards[deal.suitOfTrick()].empty())) {
    cards.clear();
    for (int i = 0; i < 4; i++)
      cards.insert(allCards[i].begin(), allCards[i].end());
  }
  else {
    cards = allCards[deal.suitOfTrick()];
  }
  if (deal.getCardsInTrick() != 3) /* same trick */
    for (it = cards.begin(); it != cards.end(); it++) {
      //fprintf(stderr, "tworze childa dla %d\n", *it);
      newCards = deal.getCards();
      newCards[who][(*it) / 13].erase(*it);
      newTrick = deal.getCurrentTrick();
      newTrick.playCard(*it, deal.getCardsInTrick());
      newDeal = new Deal(newCards, newTrick, deal.getContractSuit(), deal.getContractLevel(), deal.getWonTricks(), who + 1, deal.getCardsInTrick() + 1);
      node = new UctNode(who % 2, *newDeal);
      addChild(*node);
    }
  else { /* new trick */
    for (it = cards.begin(); it != cards.end(); it++) {
      //fprintf(stderr, "tworze childa dla %d\n", *it);
      newCards = deal.getCards();
      newCards[who][(*it) / 13].erase(*it);
      newTrick = deal.getCurrentTrick();
      newTrick.playCard(*it, deal.getCardsInTrick());
      who2 = (who + newTrick.whoWon(deal.getContractSuit()) + 1) % 4;
      newDeal = new Deal(newCards, newTrick, deal.getContractSuit(), deal.getContractLevel(), deal.getWonTricks(), who2, 0);
      if ((who2 % 2) == 1)
        newDeal->incWonTricks();
      node = new UctNode(who % 2, *newDeal);
      if (newCards[N][C].empty() && newCards[N][D].empty() && newCards[N][H].empty() && newCards[N][Sp].empty())
        node->setLast();
      addChild(*node);
    }
  }
}

int UctNode::getCard() {
  return deal.getLastCard();
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

Deal UctNode::getDeal() {
  return deal;
}

float UctNode::statsMean() {
  return playoutsStats.mean();
}

void UctNode::printNode(int depth) {
  char c[3];
  
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
  for (int i = 0; i < 1000000; i++) {
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
    movesHistory.push_back(node);
  }

  if (DEBUG) printf("jesli juz wystarczajaco ruchow to rozbudowywujemy drzewo UCT\n");
  if (node->playNum() > MOVES_BEFORE_EXPAND) {
    node->addChildren();
    node = node->selectUCBChild();
    movesHistory.push_back(node);
  }
  if (DEBUG) printf("zapuszczamy playout\n");
  if (node->isLast()) {
    if (node->getDeal().getWonTricks() >= node->getDeal().getContractLevel() + 6)
      playoutScore = 1.0;
    else
      playoutScore = 0.0;
    movesHistory.pop_back();
  } 
  else {
    Dummyplayer dummypl(&(node->getDeal()));
    int tricksWon = 0;//dummypl.play_randomly(node->getDeal().getWonTricks(), (4 + node->getDeal().whoNow() - node->getDeal().getCardsInTrick()) % 4);
    if (tricksWon >= node->getDeal().getContractLevel() + 6)
      playoutScore = 1.0;
    else
      playoutScore = 0.0;
  }
  //printf("playout score %f %d\n", playoutScore, node->getCard());

  if (DEBUG) printf("uaktualniamy wezly dla ruchow ze sciezki\n");
  for (int i = movesHistory.size()-1; i >= 0; i--)
    movesHistory[i]->updateStats(playoutScore);
}

UctNode* UctTree::selectBestMove () {
  return root.selectBestChild();
}

void UctTree::printTree () {
  root.printNode(0);
}
