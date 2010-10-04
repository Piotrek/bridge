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
  ChildrenList::iterator it = children.begin();
  float ucb, maxUcb = -1;
  float allPlayouts = playoutsStats.playNum();
  UctNode* bestNode;
  std::vector < int > whoseCards = deal->getWhoseCards();
  int suit = deal->suitOfTrick();
  int cards = deal->getCardsInTrick();
  int whoPlay = it->getPlayer();
  
  for (; it != children.end(); it++) {
    if (whoPlay % 2 == 0) {
      if (whoseCards[it->getCard()] != whoPlay)
        continue;
      if (!deal->isVoid(whoPlay, suit))  
        if ((it->getCard() / 13 != suit) && (cards != 0))
          continue;
      }
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
  float mean, maxMean = -1;
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
  int who, who2, suit, lastCard = 53;
  std::vector < std::set < int > > allCards, allCards2;
  std::set < int > cards;
  std::set < int >::iterator it;
  UctNode* node;
  
  expand = true;
  who = deal->getWhoNow();
  allCards = deal->getCards()[who];
  if (who % 2 == 0) {
    allCards2 = deal->getCards()[(who + 2) % 4];
    for (suit = C; suit <= Sp; suit++)
      allCards[suit].insert(allCards2[suit].begin(), allCards2[suit].end());
    cards.clear();
    for (int i = 0; i < 4; i++)
      cards.insert(allCards[i].begin(), allCards[i].end());  
  }
  else {
    if ((deal->getCardsInTrick() == 0) or (allCards[deal->suitOfTrick()].empty())) {
      cards.clear();
      for (int i = 0; i < 4; i++)
        cards.insert(allCards[i].begin(), allCards[i].end());
    }
    else {
      cards = allCards[deal->suitOfTrick()];
    }
  }
  for (it = cards.begin(); it != cards.end(); it++) {
    if ((who % 2 == 1) && (*it == lastCard + 1) && (*it / 13 == lastCard / 13)) {
      lastCard = *it;
      continue;
    }
    lastCard = *it;    
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
  
  if (depth >= 2) return;
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

int UctNode::getChildrenSize() {
  return children.size();
}

UctNode* UctTree::genMove() {
  if (root.getChildrenSize() == 1) {
    printTree();
    return root.selectUCBChild();
  }
  for (int i = 0; i < EXPLORE_NUM; i++) {
    deal = deals[ i % DEALS_NUM ];
    //fprintf(stderr, "%d\n", i);
    exploreTree();
  }
  printTree();
  return selectBestMove();
}

void UctTree::exploreTree() {
  UctNode* node = &root;
  float playoutScore;
  if (DEBUG_UCT) printf("nowe chodzenie po drzewie UCT\n");
  movesHistory.clear();
  movesHistory.push_back(node);
  if (DEBUG_UCT) printf("poki ma dzieci idziemy dalej\n");
  while (node->expanded()) {
    node = node->selectUCBChild();
    deal->playCard(node->getCard());
    movesHistory.push_back(node);
  }

  if (DEBUG_UCT) printf("jesli juz wystarczajaco ruchow to rozbudowywujemy drzewo UCT\n");
  if (node->playNum() > MOVES_BEFORE_EXPAND) {
    node->addChildren();
    node = node->selectUCBChild();
    deal->playCard(node->getCard());
    movesHistory.push_back(node);
  }
  if (DEBUG_UCT) printf("zapuszczamy playout\n");
  if (node->isLast()) {
    if (deal->getWonTricks() >= deal->getContractLevel() + 6)
      playoutScore = 1.0;
    else
      playoutScore = 0.0;
    movesHistory.pop_back();
  } 
  else {
    if (DEBUG_UCT) printf("random_play\n");
    int tricksWon = deal->playRandomly();
    if (DEBUG_UCT) printf("%d\n", tricksWon);
    if (tricksWon >= deal->getContractLevel() + 6)
      playoutScore = 1.0;
    else
      playoutScore = 0.0;
  }
  
  if (DEBUG_UCT) printf("uaktualniamy wezly dla ruchow ze sciezki\n");
  for (int i = movesHistory.size()-1; i >= 1; i--) {
    movesHistory[i]->updateStats(playoutScore);
    deal->undoCard(movesHistory[i]->getPlayer(), movesHistory[i]->getCard());
  }
  movesHistory[0]->updateStats(playoutScore);
    
  if (DEBUG_UCT) printf("cofamy wszystkie ruchy\n");
  deal->undoAllCards();
  if (DEBUG_UCT) printf("ruchy wycofane\n");
}

UctNode* UctTree::selectBestMove () {
  return root.selectBestChild();
}

void UctTree::printTree () {
  root.printNode(0);
}
