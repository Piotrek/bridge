#include "deal.h"


int Trick::whoWon(int trump) {
  int who = 0;
  
  for (int i = 1; i < 4; i++)
    if (((cards[i] > cards[who]) && (cards[i] / 13 == cards[who] / 13)) ||
        ((cards[i] / 13 == trump) && (cards[who] / 13 != trump)))
      who = i;
  
  return who;
}

void Trick::playCard(int card, int player) {
  cards[player] = card;
}

int Trick::firstCard() {
  return cards[0];
}

int Trick::getCard(int i) {
  return cards[i];
}

void Trick::printTrick() {
  fprintf(stderr, "%d %d %d %d\n", cards[0], cards[1], cards[2], cards[3]);
}

int Deal::getLastCard() {
  return currentTrick.getCard((cardsInTrick + 3) % 4);
}


int Deal::getContractLevel() {
  return contractLevel;
}

int Deal::getContractSuit() {
  return contractSuit;
}

Trick Deal::getCurrentTrick() {
  return currentTrick;
}

CardsSet Deal::getCards() {
  return cards;
}

PlayerCardsSet Deal::getDeclarerCards() {
  return cards[S];
}

PlayerCardsSet Deal::getDummyCards() {
  return cards[N];
}

PlayerCardsSet Deal::getEastCards() {
  return cards[E];
}

PlayerCardsSet Deal::getWestCards() {
  return cards[W];
}

int Deal::getWonTricks() {
  return wonTricks;
}

int Deal::whoNow() {
  return playerNow;
}

int Deal::getCardsInTrick() {
  return cardsInTrick;
}

int Deal::suitOfTrick() {
  return currentTrick.firstCard() / 13;
}


void Deal::incWonTricks() {
  wonTricks++;
}

void Deal::printDeal() {
  for (int i = 0; i < 4; i++) {
    fprintf(stderr, "cards - player no %d\n", i);
    fprintf(stderr, "SPADES");
    for(std::set<int>::iterator it = cards[i][Sp].begin(); it != cards[i][Sp].end(); it++) fprintf(stderr, "%d ", *it);
    fprintf(stderr, "\n");
    fprintf(stderr, "HEARTS");
    for(std::set<int>::iterator it = cards[i][H].begin(); it != cards[i][H].end(); it++) fprintf(stderr, "%d ", *it);
    fprintf(stderr, "\n");
    fprintf(stderr, "DIAMONDS");
    for(std::set<int>::iterator it = cards[i][D].begin(); it != cards[i][D].end(); it++) fprintf(stderr, "%d ", *it);
    fprintf(stderr, "\n");
    fprintf(stderr, "CLUBS");
    for(std::set<int>::iterator it = cards[i][C].begin(); it != cards[i][C].end(); it++) fprintf(stderr, "%d ", *it);
    fprintf(stderr, "\n");
  }
}


int Deal::playCard1 (int who) {
  int s[4], set_num, ret;
  s[0] = cards[who][0].size();
  s[1] = cards[who][1].size();
  s[2] = cards[who][2].size();
  s[3] = cards[who][3].size();
  int card = a.getRandomUint(s[0] + s[1] + s[2] + s[3]);

  std::set < int >::iterator iter;
  set_num = 0;
  while (card >= s[set_num]) {
    card -= s[set_num];
    set_num++;
  }
  iter = cards[who][set_num].begin();
  for (; card > 0; card--) iter++;
  ret = *(iter);
  playedCards[who].push_back(*iter);
  cards[who][set_num].erase(iter);
  return ret;
}

int Deal::playCard2(int who, int c1) {
  int suit = c1 / 13, index, ret;
  std::set<int>::iterator iter;

  if (!cards[who][suit].empty()) { /* player has a card in this suit */
    index = a.getRandomUint(cards[who][suit].size());
    iter = cards[who][suit].begin();
    for (; index > 0; index--) iter++;
    ret = *(iter);
    playedCards[who].push_back(*iter);
    cards[who][suit].erase(iter);
    return ret;
  }

  /* player hasn't got a card in this suit */
  return playCard1(who);
}

//TODO
int Deal::playCard3(int who, int c1, int c2) { return 0; }

//TODO
int Deal::playCard4(int who, int c1, int c2, int c3) { return 0; }

//TODO
int Deal::playRandomly() {
  int i;
  int tricksWon = wonTricks;
  int lastWinner = (4 + playerNow - cardsInTrick) % 4;
  Trick trick = currentTrick;
  std::vector<int>::iterator iter;
  
  for (i = 0; i < 4; i++)
    playedCards[i].clear();
  
  if (cardsInTrick > 0) {
    if (cardsInTrick == 1) {
      trick.playCard(playCard2((lastWinner + 1) % 4, trick.firstCard()), 1);
    }
    if (cardsInTrick <= 2) {
      trick.playCard(playCard2((lastWinner + 2) % 4, trick.firstCard()), 2);
    }
    if (cardsInTrick <= 3) {
      trick.playCard(playCard2((lastWinner + 3) % 4, trick.firstCard()), 3);
    }
    lastWinner = (lastWinner + trick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
  }
    
  while (!(cards[N][C].empty() && cards[N][D].empty() && cards[N][H].empty() && cards[N][Sp].empty())) {
    trick.playCard(playCard1(lastWinner), 0);
    trick.playCard(playCard2((lastWinner + 1) % 4, trick.firstCard()), 1);
    trick.playCard(playCard2((lastWinner + 2) % 4, trick.firstCard()), 2);
    trick.playCard(playCard2((lastWinner + 3) % 4, trick.firstCard()), 3);
    lastWinner = (lastWinner + trick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
  }
  
  for(i = 0; i < 4; i++)
    for (iter = playedCards[i].begin(); iter != playedCards[i].end(); iter++)
      cards[i][(*iter / 13)].insert(*iter);
  
  return tricksWon;
}
