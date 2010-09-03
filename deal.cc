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

void Trick::printTrickSymbols() {
  char c1[2], c2[2], c3[2], c4[2];
  
  fprintf(stderr, "%s ", changeNumberToCard(cards[0], c1));
  fprintf(stderr, "%s ", changeNumberToCard(cards[1], c2));
  fprintf(stderr, "%s ", changeNumberToCard(cards[2], c3));
  fprintf(stderr, "%s\n", changeNumberToCard(cards[3], c4));
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
  char card[2];
  
  for (int i = 0; i < 4; i++) {
    fprintf(stderr, "cards - player no %d\n", i);
    fprintf(stderr, "S   ");
    for(std::set<int>::iterator it = cards[i][Sp].begin(); it != cards[i][Sp].end(); it++) 
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
    fprintf(stderr, "H   ");
    for(std::set<int>::iterator it = cards[i][H].begin(); it != cards[i][H].end(); it++)
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
    fprintf(stderr, "D   ");
    for(std::set<int>::iterator it = cards[i][D].begin(); it != cards[i][D].end(); it++)
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
    fprintf(stderr, "C   ");
    for(std::set<int>::iterator it = cards[i][C].begin(); it != cards[i][C].end(); it++)
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
  }
}

int Deal::playRandomSmallCard(int who, int trumps) {
  int i = 0, ret, index = 0, index2, suit = 0;
  std::set<int>::iterator iter;

  for (i = 0; i < 4; i++) if ((!cards[who][i].empty()) && (i != trumps)) index++;
  /* index - how many different suits (non-trumps) a player has */
  if (index == 0) return playLowestCardFromThisSuit(who, trumps);
  index2 = a.getRandomUint(index); /* index - which suit will be played */
  while ((suit < 4) && (cards[who][suit].empty())) suit++;
  for (i = suit; i < 4; i++) { 
    if ((index2 > 0) && ((cards[who][i].empty()) || (i == trumps))) { suit++; }
    else if (index2 > 0) { suit++; index2--; }
    else { while ((suit < 4) && ((cards[who][suit].empty())|| (suit == trumps))) suit++; break; }
  }
    
  iter = cards[who][suit].begin();
  ret = *(iter);
  playedCards[who].push_back(ret);
  cards[who][suit].erase(iter);
  return ret;
}

int Deal::playLowestCardFromThisSuit(int who, int suit) {
  int ret;
  std::set<int>::iterator iter;

  iter = cards[who][suit].begin();
  ret = *(iter);
  playedCards[who].push_back(ret);
  cards[who][suit].erase(iter);
  return ret; 
}

int Deal::playTryToOverruff(int who, int suit, int card) {  
  int ret;
  std::set<int>::iterator iter;
  
  iter = cards[who][suit].begin();
  while (iter != cards[who][suit].end())
    if ((*iter) < card) iter++;
    else break;
  if (iter != cards[who][suit].end()) { /* play the lowest higer trump */
    ret = *(iter);
    playedCards[who].push_back(ret);
    cards[who][contractSuit].erase(iter);
    return ret;
  }
  return playRandomSmallCard(who, suit);
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
  playedCards[who].push_back(ret);
  cards[who][set_num].erase(iter);
  return ret;
}

int Deal::playCard2(int who, int c1) {
  int suit = c1 / 13, index, ret;
  std::set<int>::iterator iter;

  if (!cards[who][suit].empty()) { /* player has a card in this suit */
    index = a.getRandomUint(cards[who][suit].size()*2);
    iter = cards[who][suit].begin();
    if (index < (int) cards[who][suit].size()) /* with prob 50% we play lowest card */
       for (; index > 0; index--) iter++;
    ret = *(iter);
    playedCards[who].push_back(ret);
    cards[who][suit].erase(iter);
    return ret;
  }
  
  index = a.getRandomUint(2); /* with prob 50% we ruff */
  if ((index == 0) && (contractSuit < 4) && (!cards[who][contractSuit].empty())) {
    ret = *(cards[who][contractSuit].begin());
    playedCards[who].push_back(ret);
    cards[who][contractSuit].erase(cards[who][contractSuit].begin());
    return ret;
  }

  /* player hasn't got a card in this suit */
  return playCard1(who);
}

//TODO
int Deal::playCard3(int who, int c1, int c2) { return 0; }

int Deal::playCard4(int who, int c1, int c2, int c3) {
  int suit = c1 / 13, index, ret;
  int suit2 = c2 / 13, suit3 = c3 / 13;
  std::set<int>::iterator iter;

  index = a.getRandomUint(10);
  
  if (index == 9) { /* in 10% we play random card */
    if (!cards[who][suit].empty()) { /* player has a card in this suit */
      index = a.getRandomUint(cards[who][suit].size());
      iter = cards[who][suit].begin();
      for (; index > 0; index--) iter++;
      ret = *(iter);
      playedCards[who].push_back(ret);
      cards[who][suit].erase(iter);
      return ret;
    }
    /* player hasn't got a card in this suit */
    return playCard1(who);
  }
  
  else { /* we try to take the trick */
    if (!cards[who][suit].empty()) { /* player has a card in this suit */
      if ((suit != contractSuit) && ((suit2 == contractSuit) || (suit3 == contractSuit))) /* couldn't take trick */
        return playLowestCardFromThisSuit(who, suit);
      if ((suit == suit2) && (c2 > c1))
        c1 = c2;
      if ((suit == suit3) && (c3 > c1))
        c1 = c3;
      iter = cards[who][suit].begin();
      if (c1 != c2) { /* opps take the trick */ 
        for (; (iter != cards[who][suit].end()) && (*iter < c1); iter++) {}
        if (iter == cards[who][suit].end())
          iter = cards[who][suit].begin();
      }
      ret = *(iter);
      playedCards[who].push_back(*iter);
      cards[who][suit].erase(iter);
      return ret;
    }
    else { /* player doesn't have a card in this suit */
      /* if there is not a trump suit, we have to throw a small card */
      if (contractSuit == NT)
        return playRandomSmallCard(who, NT);
      /* if there is a trump suit and no trump has been played and a player has a trump, they ruff */
      if ((suit != contractSuit) && (suit2 != contractSuit) && (suit3 != contractSuit) && (!cards[who][contractSuit].empty()))
        return playLowestCardFromThisSuit(who, contractSuit);
      /* if there is a trump suit, opponent has ruffed and partner hasn't */
      if ((suit2 != contractSuit) && ((suit == contractSuit) || (suit3 == contractSuit))) {
        if (((suit == suit3) && (c1 < c3)) || ((suit != contractSuit) && (suit3 == contractSuit)))
          c1 = c3; /* c1 - higher trump */
        return playTryToOverruff(who, contractSuit, c1);  
      }
      /* if there is a trump suit, partner has ruffed and opponents hasn't */
      if ((suit2 == contractSuit) && (suit != contractSuit) && (suit3 != contractSuit))
        return playRandomSmallCard(who, contractSuit);
      /* if there is a trump suit, partner has ruffed and so did opponents */
      if ((suit2 == contractSuit) && ((suit == contractSuit) || (suit3 == contractSuit))) {
        if (((suit == suit3) && (c1 < c3)) || ((suit != contractSuit) && (suit3 == contractSuit)))
          c1 = c3; /* c1 - higher trump */
        if (c2 > c1) /* partner takes this trick */
          return playRandomSmallCard(who, contractSuit);
        /* can you overruff? */
        return playTryToOverruff(who, contractSuit, c1);        
      }
      return playRandomSmallCard(who, contractSuit);
    }
  }
    
  return 0;
}


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
      trick.playCard(playCard4((lastWinner + 3) % 4, trick.firstCard(), trick.getCard(1), trick.getCard(2)), 3);
    }
    lastWinner = (lastWinner + trick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
  }
    
  while (!(cards[N][C].empty() && cards[N][D].empty() && cards[N][H].empty() && cards[N][Sp].empty())) {
    trick.playCard(playCard1(lastWinner), 0);
    trick.playCard(playCard2((lastWinner + 1) % 4, trick.firstCard()), 1);
    trick.playCard(playCard2((lastWinner + 2) % 4, trick.firstCard()), 2);
    trick.playCard(playCard4((lastWinner + 3) % 4, trick.firstCard(), trick.getCard(1), trick.getCard(2)), 3);
    lastWinner = (lastWinner + trick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
  }
  
  for(i = 0; i < 4; i++)
    for (iter = playedCards[i].begin(); iter != playedCards[i].end(); iter++)
      cards[i][(*iter / 13)].insert(*iter);
  
  return tricksWon;
}
