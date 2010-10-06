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

  if (DEBUG_DEAL) fprintf(stderr, "playRandomSmallCard\n");

  for (i = 0; i < 4; i++) 
    if ((!cards[who][i].empty()) && (i != trumps))
      index++;
  /* index - how many different suits (non-trumps) a player has */
  if (index == 0) 
    return playLowestCardFromThisSuit(who, trumps);
  index2 = a.getRandomUint(index); /* index - which suit will be played */
  while ((suit < 4) && (cards[who][suit].empty()))
    suit++;
  for (i = suit; i < 4; i++) { 
    if ((index2 > 0) && ((cards[who][i].empty()) || (i == trumps))) { suit++; }
    else if (index2 > 0) { suit++; index2--; }
    else { while ((suit < 4) && ((cards[who][suit].empty())|| (suit == trumps))) suit++; break; }
  }
    
  iter = cards[who][suit].begin();
  ret = *(iter);
  playedCards[ret] = who;
  cards[who][suit].erase(iter);
  return ret;
}

/* pre: player 'who' has a card in this suit */
int Deal::playLowestCardFromThisSuit(int who, int suit) {
  int ret;
  std::set<int>::iterator iter;
  
  if (DEBUG_DEAL) fprintf(stderr, "playLowestCardFromThisSuit\n");
    
  iter = cards[who][suit].begin();
  ret = *(iter);
  playedCards[ret] = who;
  cards[who][suit].erase(iter);

  return ret;
}

int Deal::playTryToOverruff(int who, int suit, int card) {  
  int ret;
  std::set<int>::iterator iter;
  
  if (DEBUG_DEAL) fprintf(stderr, "playTryToOverruff\n");

  iter = cards[who][suit].begin();
  while (iter != cards[who][suit].end())
    if ((*iter) < card) 
      iter++;
    else 
      break;
  if (iter != cards[who][suit].end()) { /* play the lowest higher trump */
    ret = *(iter);
    playedCards[ret] = who;
    cards[who][contractSuit].erase(iter);
    return ret;
  }
  return playRandomSmallCard(who, suit);
}

int Deal::playRandomCardFromThisSuit(int who, int suit) {
  int ret, index;
  std::set < int >::iterator iter;
  
  if (DEBUG_DEAL) fprintf(stderr, "playRandomCardFromThisSuit\n");

  if (!cards[who][suit].empty()) { /* player has a card in this suit */
    index = a.getRandomUint(cards[who][suit].size());
    iter = cards[who][suit].begin();
    for (; index > 0; index--) iter++;
    ret = *(iter);
    playedCards[ret] = who;
    cards[who][suit].erase(iter);
    return ret;
  }
  return playRandomCard(who);
}

int Deal::playRandomCard(int who) {
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
  playedCards[ret]= who;
  cards[who][set_num].erase(iter);
  return ret;
}

int Deal::playDoNotBlock (int who, int suit) {
  int part = (who + 2) % 4, ret;
  std::set < int >::iterator iter;
  
  if (DEBUG_DEAL) fprintf(stderr, "playDoNotBlock\n");
  
  if (cards[part][suit].size() == 1) { iter = cards[who][suit].end(); iter--; }
  else if (cards[part][suit].size() <= cards[who][suit].size()) iter = cards[who][suit].begin();
  else {
    iter = cards[who][suit].end();
    iter--;
  }
  ret = *(iter);
  playedCards[ret] = who;
  cards[who][suit].erase(iter);
  return ret;
}

/* if doesn't have higher card, plays the lowest from this suit */
int Deal::playTheLowestHigherCard(int who, int suit, int card) {
  int ret;
  std::set < int >::iterator iter = cards[who][suit].begin();
 
  if (DEBUG_DEAL) fprintf(stderr, "playTheLowestHigher\n"); 
    
  for (; ((iter != cards[who][suit].end()) && (*iter < card)); iter++) {}
  if (iter == cards[who][suit].end())
    iter = cards[who][suit].begin();
  ret = *(iter);
  playedCards[ret] = who;
  cards[who][suit].erase(iter);
  return ret;
}
    
/* pre: player has a card in the lead's suit */
int Deal::playTryToTakeThisTrick3(int who, int c1, int c2) {
  int suit1 = c1 / 13, suit2 = c2 / 13, ret, nextpl = (who + 1) % 4, c3, c4, c4_h, c4_l;
  std::set < int >::iterator iter;
  
  if (DEBUG_DEAL) fprintf(stderr, "playTryToTake\n"); 
  
  if ((suit1 != contractSuit) && (suit2 == contractSuit)) /* couldn't take trick */
        return playLowestCardFromThisSuit(who, suit1);
  
  if (cards[nextpl][suit1].empty()) { /* player 4 doesn't have a card in that suit */
    if (suit2 != suit1) {
      if (contractSuit == NT) return playDoNotBlock(who, suit1);
      if (cards[nextpl][contractSuit].empty()) /* probably there are no trumps among opps at all */
        return playDoNotBlock(who, suit1);
      return playLowestCardFromThisSuit(who, suit1);
    }
    
    /* suit1 = suit2 = suit3 != suit4 */
    
    /* partner's card is higher -> play the lowest card from the lead's suit */
    if (c1 > c2) return playLowestCardFromThisSuit(who, suit1);
    /* play the lowest card higer than c2 */
    return playTheLowestHigherCard(who, suit1, c2);
  } 
  
  /* player 4 has a card in that suit */
  iter = cards[nextpl][suit1].end();
  iter--;
  c4 = *iter; /* the highest card in the suit */
  
  if (suit2 == suit1) {
    //if (c4 > c2) c2 = c4; /* c2 - higher from the opps' cards */
    if ((c1 > c2) && (c1 > c4))
      return playLowestCardFromThisSuit(who, suit1);
    if (c2 > c4)
      return playTheLowestHigherCard(who, suit1, c2);
  }  
  
  /* c2 < c4 i c1 < c4   or  suit1 = suit3 = suit 4 != suit2 */
  iter = cards[who][suit1].end();
  iter--;
  c3 = *iter; /* the highest card in the suit */
  if (c3 > c4) /* play the lowest higher card */
    return playTheLowestHigherCard(who, suit1, c4);
  if (c3 < c2)
    return playLowestCardFromThisSuit(who, suit1);
    
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
  
  ret = *(iter);
  playedCards[ret] = who;
  cards[who][suit1].erase(iter);
  return ret;
}

int Deal::playCard1(int who) {
  int index = a.getRandomUint(10), no_cards, suit = -1;
  int c = 0, d = 0, h = 0, s = 0, part = (who + 2) % 4, opp1 = (who + 1) % 4, opp2 = (who + 3) % 4;
  std::set<int>::iterator it;
  
  if (DEBUG_DEAL) fprintf(stderr, "playCard1\n");

  if ((contractSuit == NT) || (cards[(who + 1) % 4][contractSuit].empty()) || (cards[(who + 3) % 4][contractSuit].empty())) {
    
    if ((cards[opp1][C].empty()) && (cards[opp2][C].empty()) && (!cards[who][C].empty()))
      suit = C;
    else if ((cards[opp1][D].empty()) && (cards[opp2][D].empty()) && (!cards[who][D].empty()))
      suit = D;
    else if ((cards[opp1][H].empty()) && (cards[opp2][H].empty()) && (!cards[who][H].empty()))
      suit = H;
    else if ((cards[opp1][S].empty()) && (cards[opp2][S].empty()) && (!cards[who][S].empty()))
      suit = S;
    if ((suit != contractSuit) && (suit != -1) && (index != 9)) return playDoNotBlock(who, suit);
  }
  //if we play no trumps or opponents don't have trumps we play in our suits
  //if we play suit and partner have void we try to play card to ruff
  
  /* we took */
  if (index != 9) {
    if (currentTrick.whoWon(contractSuit) % 2 == 0) {
      suit = suitOfTrick();
      if (!cards[who][suit].empty())
        return playDoNotBlock(who, suit);
    }
  }

  if (index > 8)
    return playRandomCard(who);
    
  c = cards[who][C].size() + cards[part][C].size() - std::max(cards[opp1][C].size(), cards[opp2][C].size());
  d = cards[who][D].size() + cards[part][D].size() - std::max(cards[opp1][D].size(), cards[opp2][D].size());
  h = cards[who][H].size() + cards[part][H].size() - std::max(cards[opp1][H].size(), cards[opp2][H].size());
  s = cards[who][S].size() + cards[part][S].size() - std::max(cards[opp1][S].size(), cards[opp2][S].size());
  
  no_cards = 0;
  if (contractSuit != C) { suit = C; no_cards = c; }
  if ((d > no_cards) && (contractSuit != D)) { suit = D; no_cards = d; }
  if ((h > no_cards) && (contractSuit != H)) { suit = H; no_cards = h; }
  if ((s > no_cards) && (contractSuit != S)) { suit = S; no_cards = s; }
  return playRandomCardFromThisSuit(who, suit);
  
  for (it = cards[who][C].begin(); it != cards[who][C].end(); it++)
    if ((*it) > 8) c += 2 * ((*it) % 13 + 1);
    else c += (*it) % 13 + 1;
  for (it = cards[part][C].begin(); it != cards[part][C].end(); it++)
    if ((*it) > 8) c += 2 * ((*it) % 13 + 1);
    else c += (*it) % 13 + 1;
  for (it = cards[opp1][C].begin(); it != cards[opp1][C].end(); it++)
    if ((*it) > 8) c -= 2 * ((*it) % 13 + 1);
    else c -= (*it) % 13 + 1;
  for (it = cards[opp2][C].begin(); it != cards[opp2][C].end(); it++)
    if ((*it) > 8) c -= 2 * ((*it) % 13 + 1);
    else c -= (*it) % 13 + 1;
  
  for (it = cards[who][D].begin(); it != cards[who][D].end(); it++)
    if ((*it) > 8) d += 2 * ((*it) % 13 + 1);
    else d += (*it) % 13 + 1;
  for (it = cards[part][D].begin(); it != cards[part][D].end(); it++)
    if ((*it) > 8) d += 2 * ((*it) % 13 + 1);
    else d += (*it) % 13 + 1;
  for (it = cards[opp1][D].begin(); it != cards[opp1][D].end(); it++)
    if ((*it) > 8) d -= 2 * ((*it) % 13 + 1);
    else d -= (*it) % 13 + 1;
  for (it = cards[opp2][D].begin(); it != cards[opp2][D].end(); it++)
    if ((*it) > 8) d -= 2 * ((*it) % 13 + 1);
    else d -= (*it) % 13 + 1;
  
  for (it = cards[who][H].begin(); it != cards[who][H].end(); it++)
    if ((*it) > 8) h += 2 * ((*it) % 13 + 1);
    else h += (*it) % 13 + 1;
  for (it = cards[part][H].begin(); it != cards[part][H].end(); it++)
    if ((*it) > 8) h += 2 * ((*it) % 13 + 1);
    else h += (*it) % 13 + 1;
  for (it = cards[opp1][H].begin(); it != cards[opp1][H].end(); it++)
    if ((*it) > 8) h -= 2 * ((*it) % 13 + 1);
    else h -= (*it) % 13 + 1;
  for (it = cards[opp2][H].begin(); it != cards[opp2][H].end(); it++)
    if ((*it) > 8) h -= 2 * ((*it) % 13 + 1);
    else h -= (*it) % 13 + 1;
  
  for (it = cards[who][S].begin(); it != cards[who][S].end(); it++)
    if ((*it) > 8) s += 2 * ((*it) % 13 + 1);
    else s += (*it) % 13 + 1;
  for (it = cards[part][S].begin(); it != cards[part][S].end(); it++)
    if ((*it) > 8) s += 2 * ((*it) % 13 + 1);
    else s += (*it) % 13 + 1;
  for (it = cards[opp1][S].begin(); it != cards[opp1][S].end(); it++)
    if ((*it) > 8) s -= 2 * ((*it) % 13 + 1);
    else s -= (*it) % 13 + 1;
  for (it = cards[opp2][S].begin(); it != cards[opp2][S].end(); it++)
    if ((*it) > 8) s -= 2 * ((*it) % 13 + 1);
    else s -= (*it) % 13 + 1;
  
  no_cards = 0;
  if (contractSuit != C) { suit = C; no_cards = c; }
  if ((d > no_cards) && (contractSuit != D)) { suit = D; no_cards = d; }
  if ((h > no_cards) && (contractSuit != H)) { suit = H; no_cards = h; }
  if ((s > no_cards) && (contractSuit != S)) { suit = S; no_cards = s; }
  return playRandomCardFromThisSuit(who, suit);
}


int Deal::playCard2(int who, int c1) {
  int suit = c1 / 13, index, ret;
  std::set<int>::iterator iter;

  if (DEBUG_DEAL) fprintf(stderr, "playCard2\n");

  if (!cards[who][suit].empty()) { /* player has a card in this suit */
    index = a.getRandomUint(cards[who][suit].size()*2);
    iter = cards[who][suit].begin();
    if (index < (int) cards[who][suit].size()) /* with prob 50% we play lowest card */
       for (; index > 0; index--) iter++;
    ret = *(iter);
    playedCards[ret] = who;
    cards[who][suit].erase(iter);
    return ret;
  }
  
  index = a.getRandomUint(2); /* with prob 50% we ruff */
  if ((index == 0) && (contractSuit < 4) && (!cards[who][contractSuit].empty())) {
    ret = *(cards[who][contractSuit].begin());
    playedCards[ret] = who;
    cards[who][contractSuit].erase(cards[who][contractSuit].begin());
    return ret;
  }

  /* player hasn't got a card in this suit */
  return playRandomCard(who);
}


int Deal::playCard3(int who, int c1, int c2) {
  int index, ret, nextpl = (who + 1) % 4, c3, c4, c4_l;
  int suit1 = c1 / 13, suit2 = c2 / 13;
  std::set<int>::iterator iter;
  
  index = a.getRandomUint(10);
  
  if (DEBUG_DEAL) fprintf(stderr, "playCard3\n"); 
  
  if (index == 9) /* in 10% we play random card */
    return playRandomCardFromThisSuit(who, suit1);

  if (!cards[who][suit1].empty()) /* player 3 has a card in the lead's suit */
    return playTryToTakeThisTrick3(who, c1, c2);
    
  if ((contractSuit == NT) || ((contractSuit != NT) && (cards[who][contractSuit].empty())))
    return playRandomSmallCard(who, contractSuit);
  
  /* there are trumps */
  if (suit1 == contractSuit)
    return playRandomSmallCard(who, suit1);
    
  if (cards[who][contractSuit].empty()) /* player can't ruff */
    return playRandomSmallCard(who, contractSuit);
  
  /* and player 3 has trumps */
  if (suit2 == contractSuit) /* player 2 has ruffed */ {
    if (!cards[nextpl][suit1].empty() || (cards[nextpl][contractSuit].empty()))
      return playTryToOverruff(who, contractSuit, c2);
  }  
  else {
    if (!cards[nextpl][suit1].empty() || (cards[nextpl][contractSuit].empty()))
      return playLowestCardFromThisSuit(who, contractSuit);
  }
  
  /* player 4 has trumps */
  iter = cards[nextpl][contractSuit].end();
  iter--;
  c4 = *iter; /* the highest card in the suit */
  iter = cards[who][contractSuit].end();
  iter--;
  c3 = *iter; /* the highest card in the suit */

  if ((c3 > c4) && !(suit2 == contractSuit)) /* play the lowest higher card */
    return playTheLowestHigherCard(who, contractSuit, c4);
  if ((c3 > c4) && (c3 > c2))
    return playTheLowestHigherCard(who, contractSuit, std::max(c2,c4));
  if ((suit2 == contractSuit) && (c2 > c3)) /* can't overruff */
    return playRandomSmallCard(who, contractSuit);
  
  /* c4 > c2 (or c2 not a trump) and c4 > c3 */
  iter = cards[nextpl][contractSuit].end(); iter--;
  for (; (iter != cards[nextpl][contractSuit].begin()) && (*iter > c3); iter--) {}
  c4_l = 0;
  if (iter != cards[nextpl][contractSuit].begin())
   c4_l = *iter;  
  iter = cards[who][contractSuit].begin();
  for (; (iter != cards[who][contractSuit].end()) && ((*iter < c4_l) || (*iter < c2)); iter++) {}
  if (iter == cards[who][contractSuit].end())
    iter = cards[who][contractSuit].begin();
  ret = *(iter);
  playedCards[ret] = who;
  cards[who][contractSuit].erase(iter);
  return ret;
}

int Deal::playCard4(int who, int c1, int c2, int c3) {
  int suit = c1 / 13, index, ret;
  int suit2 = c2 / 13, suit3 = c3 / 13;
  std::set<int>::iterator iter;

  if (DEBUG_DEAL) fprintf(stderr, "playCard4\n");

  index = a.getRandomUint(10);
  
  if (index == 9) /* in 10% we play random card */
    return playRandomCardFromThisSuit(who, suit);
  
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
    playedCards[ret] = who;
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
  
  if (DEBUG_DEAL) fprintf(stderr, "playRandomly\n");
  if (DEBUG_DEAL) printDeal();
  
  playedCards.resize(52,5);
  
  if (getCardsInTrick() > 0) {
    if (getCardsInTrick() == 1) {
      currentTrick.playCard(playCard2((lastWinner + 1) % 4, currentTrick.firstCard()));
    }
    if (DEBUG_DEAL)
      currentTrick.printTrickSymbols();
    if (getCardsInTrick() <= 2) {
      currentTrick.playCard(playCard3((lastWinner + 2) % 4, currentTrick.firstCard(), currentTrick.getCard(1)));
    }
    if (DEBUG_DEAL)
      currentTrick.printTrickSymbols();
    if (getCardsInTrick() <= 3) {
      currentTrick.playCard(playCard4((lastWinner + 3) % 4, currentTrick.firstCard(), currentTrick.getCard(1), currentTrick.getCard(2)));
    }
    if (DEBUG_DEAL)
      currentTrick.printTrickSymbols();
  
    lastWinner = (lastWinner + currentTrick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
    }
    
  while (!(cards[N][C].empty() && cards[N][D].empty() && cards[N][H].empty() && cards[N][Sp].empty())) {
    currentTrick.playCard(playCard1(lastWinner));
    if (DEBUG_DEAL)
      currentTrick.printTrickSymbols();
    currentTrick.playCard(playCard2((lastWinner + 1) % 4, currentTrick.firstCard()));
    if (DEBUG_DEAL) 
      currentTrick.printTrickSymbols();
    currentTrick.playCard(playCard3((lastWinner + 2) % 4, currentTrick.firstCard(), currentTrick.getCard(1)));
    if (DEBUG_DEAL)
      currentTrick.printTrickSymbols();
    currentTrick.playCard(playCard4((lastWinner + 3) % 4, currentTrick.firstCard(), currentTrick.getCard(1), currentTrick.getCard(2)));
    if (DEBUG_DEAL)
      currentTrick.printTrickSymbols();
    lastWinner = (lastWinner + currentTrick.whoWon(contractSuit)) % 4;
    tricksWon += (lastWinner % 2);
  }
  
  for (i = 0; i < 52; i++)
    if (playedCards[i] != 5)
      cards[playedCards[i]][i/13].insert(i);
  
  return tricksWon;
}

void Deal::undoAllCards() {
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

bool Deal::isVoid(int player, int suit) {
  return cards[player][suit].empty();
}
