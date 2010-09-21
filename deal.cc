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
  cardsInTrick = (cardsInTrick + 1) % 4;
}

void Trick::playCard(int card) {
  cards[cardsInTrick] = card;
  cardsInTrick = (cardsInTrick + 1) % 4;
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

int Trick::getCardsInTrick() {
  return cardsInTrick;
}

int Deal::getLastCard() {
  return currentTrick.getCard((getCardsInTrick() + 3) % 4);
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

int Deal::getCardsInTrick() {
  return currentTrick.getCardsInTrick();
}

int Deal::getWhoNow() {
  return whoNow;
}

int Deal::suitOfTrick() {
  return currentTrick.firstCard() / 13;
}

void Deal::incWonTricks() {
  wonTricks++;
}

void Deal::printDeal() {
  char card[2];
  std::set<int>::iterator it;
  
  for (int i = 0; i < 4; i++) {
    fprintf(stderr, "cards - player no %d\n", i);
    fprintf(stderr, "S   ");
    for(it = cards[i][Sp].begin(); it != cards[i][Sp].end(); it++) 
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
    fprintf(stderr, "H   ");
    for(it = cards[i][H].begin(); it != cards[i][H].end(); it++)
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
    fprintf(stderr, "D   ");
    for(it = cards[i][D].begin(); it != cards[i][D].end(); it++)
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
    fprintf(stderr, "C   ");
    for(it = cards[i][C].begin(); it != cards[i][C].end(); it++)
      fprintf(stderr, "%s ", changeNumberToCard(*it, card));
    fprintf(stderr, "\n");
  }
}

int Deal::playRandomSmallCard(int who, int trumps) {
  int i = 0, ret, index = 0, index2, suit = 0;
  std::set<int>::iterator iter;

  if (DEBUG) fprintf(stderr, "playRandomSmallCard\n");

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

/* pre: player 'who' has a card in this suit */
int Deal::playLowestCardFromThisSuit(int who, int suit) {
  int ret;
  std::set<int>::iterator iter;
  
  if (DEBUG) fprintf(stderr, "playLowestCardFromThisSuit\n");

  iter = cards[who][suit].begin();
  ret = *(iter);
  playedCards[who].push_back(ret);
  cards[who][suit].erase(iter);
  return ret;
}

int Deal::playTryToOverruff(int who, int suit, int card) {  
  int ret;
  std::set<int>::iterator iter;
  
  if (DEBUG) fprintf(stderr, "playTryToOverruff\n");

  
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

int Deal::playRandomCard(int who, int suit) {
  int ret, index;
  std::set < int >::iterator iter;
  
  if (DEBUG) fprintf(stderr, "playRandomCard\n");

  
  if (!cards[who][suit].empty()) { /* player has a card in this suit */
    index = a.getRandomUint(cards[who][suit].size());
    iter = cards[who][suit].begin();
    for (; index > 0; index--) iter++;
    ret = *(iter);
    playedCards[who].push_back(ret);
    cards[who][suit].erase(iter);
    return ret;
  }
  return playCard1(who);
}

int Deal::playDoNotBlock (int who, int suit, int card) {
  int part = (who + 2) % 4, ret;
  std::set < int >::iterator iter;
  
  if (DEBUG) fprintf(stderr, "playDoNotBlock\n");
  
  if (cards[part][suit].size() == 1) { iter = cards[who][suit].end(); iter--; }
  else if (cards[part][suit].size() <= cards[who][suit].size()) iter = cards[who][suit].begin();
  else {
    iter = cards[who][suit].begin();
    for (; (iter != cards[who][suit].end() && (*iter < card)); iter++) {}
    if (iter != cards[who][suit].begin()) iter--;
  }
  ret = *(iter);
  playedCards[who].push_back(ret);
  cards[who][suit].erase(iter);
  return ret;
}

/* if doesn't have higher card, plays the lowest from this suit */
int Deal::playTheLowestHigherCard(int who, int suit, int card) {
  int ret;
  std::set < int >::iterator iter = cards[who][suit].begin();
 
  if (DEBUG) fprintf(stderr, "playTheLowestHigher\n"); 
    
  for (; (iter != cards[who][suit].end()) && (*iter < card); iter++) {}
  if (iter == cards[who][suit].end())
    iter = cards[who][suit].begin();
  ret = *(iter);
  playedCards[who].push_back(*iter);
  cards[who][suit].erase(iter);
  return ret;
}
    
/* pre: player has a card in the lead's suit */
int Deal::playTryToTakeThisTrick3(int who, int c1, int c2) {
  int suit1 = c1 / 13, suit2 = c2 / 13, ret, nextpl = (who + 1) % 4, c3, c4, c4_h, c4_l;
  std::set < int >::iterator iter;
  
  if (DEBUG) fprintf(stderr, "playTryToTake30\n"); 
  
  if ((suit1 != contractSuit) && (suit2 == contractSuit)) /* couldn't take trick */
        return playLowestCardFromThisSuit(who, suit1);
  
  if (DEBUG) fprintf(stderr, "playTryToTake31\n"); 
  
  if (cards[nextpl][suit1].empty()) { /* player 4 doesn't have a card in that suit */
    
    if (DEBUG) fprintf(stderr, "playTryToTake32\n"); 
  
    if (suit2 != suit1) {
      if (contractSuit == NT) return playDoNotBlock(who, suit1, c1);
      if (cards[nextpl][contractSuit].empty()) /* probably there are no trumps among opps at all */
        return playDoNotBlock(who, suit1, c1);
      return playLowestCardFromThisSuit(who, suit1);
    }
    if (DEBUG) fprintf(stderr, "playTryToTake33\n"); 
    
    /* suit1 = suit2 = suit3 != suit4 */
    
    /* partner's card is higher -> play the lowest card from the lead's suit */
    if (c1 > c2) return playLowestCardFromThisSuit(who, suit1);
    /* play the lowest card higer than c2 */
    return playTheLowestHigherCard(who, suit1, c2);
  
  }
  if (DEBUG) fprintf(stderr, "playTryToTake34\n"); 
  
  
  /* player 4 has a card in that suit */
  iter = cards[nextpl][suit1].end();
  iter--;
  c4 = *iter; /* the highest card in the suit */
  
  if (suit2 == suit1) {
      if (DEBUG) fprintf(stderr, "playTryToTake35\n"); 
  
    //if (c4 > c2) c2 = c4; /* c2 - higher from the opps' cards */
    if ((c1 > c2) && (c1 > c4))
      return playLowestCardFromThisSuit(who, suit1);
    if (c2 > c4)
      return playTheLowestHigherCard(who, suit1, c2);
  }
    if (DEBUG) fprintf(stderr, "playTryToTake36\n"); 
  
  
  /* c2 < c4 i c1 < c4   or  suit1 = suit3 = suit 4 != suit2 */
  iter = cards[who][suit1].end();
  iter--;
  c3 = *iter; /* the highest card in the suit */
  if (c3 > c4) /* play the lowest higher card */
    return playTheLowestHigherCard(who, suit1, c4);
  if (c3 < c2)
    return playLowestCardFromThisSuit(who, suit1);
    
     if (DEBUG) fprintf(stderr, "playTryToTake37\n"); 
   
    
  iter = cards[nextpl][suit1].end(); iter--;
  for (; (iter != cards[nextpl][suit1].begin()) && (*iter > c3); iter--) {}
  if (*iter < c3) c4_h = c4;
  else c4_h = *iter;
  c4_l = 0;
  if (iter != cards[nextpl][suit1].begin()) { iter--; c4_l = *iter; }  
  iter = cards[who][suit1].begin();
  for (; (iter != cards[who][suit1].end()) && ((*iter < c4_l) || (*iter > c2)); iter++) {}
  
  if (iter == cards[who][suit1].end())
    iter = cards[who][suit1].begin();
  
    if (DEBUG) fprintf(stderr, "playTryToTake38\n"); 
  
  ret = *(iter);
  playedCards[who].push_back(*iter);
  cards[who][suit1].erase(iter);
  return ret;
}

int Deal::playCard1(int who) {
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





int Deal::playCard3(int who, int c1, int c2) {
  int index, ret, nextpl = (who + 1) % 4, c3, c4, c4_l, c4_h;
  int suit1 = c1 / 13, suit2 = c2 / 13;
  std::set<int>::iterator iter;
  
  index = a.getRandomUint(10);
  
  if (DEBUG) fprintf(stderr, "playCard30\n"); 
  
  if (index == 9) /* in 10% we play random card */
    return playRandomCard(who, suit1);

  if (DEBUG) fprintf(stderr, "playCard31\n");

  if (!cards[who][suit1].empty()) /* player 3 has a card in the lead's suit */
    return playTryToTakeThisTrick3(who, c1, c2);
    
  if (DEBUG) fprintf(stderr, "playCard32\n");
    
  if ((contractSuit == NT) || ((contractSuit != NT) && (cards[who][contractSuit].empty())))
    return playRandomSmallCard(who, contractSuit);
  
  if (DEBUG) fprintf(stderr, "playCard33\n");
    
  /* there are trumps */
  if (suit1 == contractSuit)
    return playRandomSmallCard(who, suit1);
    
  if (DEBUG) fprintf(stderr, "playCard34\n");
    
  if (cards[who][suit2].empty()) /* player can't ruff */
    return playRandomSmallCard(who, contractSuit);
  
  if (DEBUG) fprintf(stderr, "playCard35\n");
  
  /* and player 3 has trumps */
  if (suit2 == contractSuit) /* player 2 has ruffed */ {
    if (!cards[nextpl][suit1].empty() || (cards[nextpl][contractSuit].empty()))
      return playTryToOverruff(who, contractSuit, c2);
  }  
  else {
    if (!cards[nextpl][suit1].empty() || (cards[nextpl][contractSuit].empty()))
      return playLowestCardFromThisSuit(who, contractSuit);
  }
  
  if (DEBUG) fprintf(stderr, "playCard36\n");
  
  
  /* player 4 has trumps */
  iter = cards[nextpl][contractSuit].end();
  iter--;
  c4 = *iter; /* the highest card in the suit */
  iter = cards[who][contractSuit].end();
  iter--;
  c3 = *iter; /* the highest card in the suit */

  if (DEBUG) fprintf(stderr, "playCard37\n");
  
  
  if ((c3 > c4) && (!(suit2 == contractSuit) || (c3 > c2))) /* play the lowest higher card */
    return playTheLowestHigherCard(who, suit1, std::max(c2, c4));
  if (c3 > c4) /* c3 < c2 */
    return playRandomSmallCard(who, contractSuit);
  
  if (DEBUG) fprintf(stderr, "playCard38\n");
    
  
  /* c4 > c2 (or c2 not a trump) and c4 > c3 */
  iter = cards[nextpl][contractSuit].end(); iter--;
  for (; (iter != cards[nextpl][contractSuit].begin()) && (*iter > c3); iter--) {}
  if (*iter < c3) c4_h = c4;
  else c4_h = *iter;
  c4_l = 0;
  if (iter != cards[nextpl][contractSuit].begin()) { iter--; c4_l = *iter; }  
  iter = cards[who][contractSuit].begin();
  for (; (iter != cards[who][contractSuit].end()) && ((*iter < c4_l) || (*iter > c2)); iter++) {}
  ret = *(iter);
  playedCards[who].push_back(*iter);
  cards[who][contractSuit].erase(iter);
  
  if (DEBUG) fprintf(stderr, "playCard39\n");
  
  return ret;
}

int Deal::playCard4(int who, int c1, int c2, int c3) {
  int suit = c1 / 13, index, ret;
  int suit2 = c2 / 13, suit3 = c3 / 13;
  std::set<int>::iterator iter;

  index = a.getRandomUint(10);
  
  if (index == 9) /* in 10% we play random card */
    return playRandomCard(who, suit);
  
  /* we try to take the trick */
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
  
  /* player doesn't have a card in this suit */
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


int Deal::playRandomly() {
  int i;
  int tricksWon = wonTricks;
  int lastWinner = (4 + whoNow - getCardsInTrick()) % 4;
  std::vector<int>::iterator iter;
  
  for (i = 0; i < 4; i++)
    playedCards[i].clear();
  
  if (getCardsInTrick() > 0) {
    if (getCardsInTrick() == 1) {
      currentTrick.playCard(playCard2((lastWinner + 1) % 4, currentTrick.firstCard()));
    }
    if (DEBUG)
      currentTrick.printTrickSymbols();
    if (getCardsInTrick() <= 2) {
      currentTrick.playCard(playCard3((lastWinner + 2) % 4, currentTrick.firstCard(), currentTrick.getCard(1)));
    }
    if (DEBUG)
      currentTrick.printTrickSymbols();
    if (getCardsInTrick() <= 3) {
      currentTrick.playCard(playCard4((lastWinner + 3) % 4, currentTrick.firstCard(), currentTrick.getCard(1), currentTrick.getCard(2)));
    }
    lastWinner = (lastWinner + currentTrick.whoWon(contractSuit)) % 4;
    wonTricks += (lastWinner % 2);
    //currentTrick.printTrickSymbols();
  }
    
  while (!(cards[N][C].empty() && cards[N][D].empty() && cards[N][H].empty() && cards[N][Sp].empty())) {
    currentTrick.playCard(playCard1(lastWinner));
    if (DEBUG)
      currentTrick.printTrickSymbols();
    
    currentTrick.playCard(playCard2((lastWinner + 1) % 4, currentTrick.firstCard()));
    if (DEBUG) 
      currentTrick.printTrickSymbols();
    
    currentTrick.playCard(playCard3((lastWinner + 2) % 4, currentTrick.firstCard(), currentTrick.getCard(1)));
    if (DEBUG)
      currentTrick.printTrickSymbols();
    
    currentTrick.playCard(playCard4((lastWinner + 3) % 4, currentTrick.firstCard(), currentTrick.getCard(1), currentTrick.getCard(2)));
    if (DEBUG)
      currentTrick.printTrickSymbols();
    
    lastWinner = (lastWinner + currentTrick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
    //currentTrick.printTrickSymbols();
  }
  
  for(i = 0; i < 4; i++)
    for (iter = playedCards[i].begin(); iter != playedCards[i].end(); iter++)
      cards[i][(*iter / 13)].insert(*iter);
  
  return tricksWon;
}

void Deal::undoAllCards() {
  //int i;
  //std::vector< std::pair <int, int> >::iterator iter;
  
  //for (iter = playedUctCards.begin(); iter != playedUctCards.end(); iter++)
  //    cards[iter->first][(iter->second / 13)].insert(iter->second);
      
  //playedUctCards.clear();
      
  currentTrick = startTrick;
  whoNow = whoStarts;
  wonTricks = startWonTricks;          
}

void Deal::undoCard(int player, int card) {
   cards[player][card / 13].insert(card);
}

void Deal::playCard(int card) {
  currentTrick.playCard(card);
  cards[whoNow][(card) / 13].erase(card);
  if (getCardsInTrick() != 0)  /* same trick */
    whoNow = (whoNow + 1) % 4;
  else { /* new trick */    
    whoNow = (whoNow + currentTrick.whoWon(contractSuit) + 1) % 4;
    if ((whoNow % 2) == 1)
      wonTricks++;
  }
}

void Deal::playUserCard(int card) {
  currentTrick.playCard(card);
  if ((whoNow % 2) == 1) 
    cards[whoNow][(card) / 13].erase(card);
  if (getCardsInTrick() != 0)  /* same trick */
    whoNow = (whoNow + 1) % 4;
  else { /* new trick */    
    whoNow = (whoNow + currentTrick.whoWon(contractSuit) + 1) % 4;
    if ((whoNow % 2) == 1)
      wonTricks++;
  }
}

bool Deal::endOfDeal(int player) {
  return (cards[player][Sp].empty() && cards[player][H].empty() && cards[player][D].empty() && cards[player][C].empty());
}

std::vector <int> Deal::getWhoseCards() {
  return whoseCards; 
}
