#include "main.h"


using namespace std;

MyRandom Deal::a = MyRandom(time(NULL));

Deal *deal;
Deal mydeal;
DealSet deals;
vector < set < int > > defendersCards;
vector < int > noCardsEast, noCardsWest, possEast, possWest, whoseCards;


/* function generating random East-West hands */
void generate_deals(int eastCardsNum, int westCardsNum) {
  MyRandom a(time(NULL));
  int i, j, k, eastNum, westNum;
  CardsSet cards = mydeal.getCards();
  Trick tr = mydeal.getCurrentTrick();
  int suit = mydeal.getContractSuit();
  int level = mydeal.getContractLevel();
  int whoNow = mydeal.getWhoNow();
  int wonTricks = mydeal.getWonTricks();
  vector < set < int > > eastCards, westCards;
  set < int > allDefendersCards, allDefendersCards2;
  set < int >::iterator it;
  
  eastCards.resize(4);
  westCards.resize(4);
  
  /* maybe distribution of some suits is known */
  for (i = C; i <= Sp; i++) {
    if (noCardsEast[i]) {
      westCards[i] = defendersCards[i];
      westCardsNum -= westCards[i].size();
      continue;
    }
    if (noCardsWest[i]) {
      eastCards[i] = defendersCards[i];
      eastCardsNum -= eastCards[i].size();
      continue;
    }
    allDefendersCards2.insert(defendersCards[i].begin(), defendersCards[i].end());
  }
  
  /* creating sample deals */
  deals.resize(DEALS_NUM);
  for (i = 0; i < DEALS_NUM; i++) {
    cards[E] = eastCards;
    cards[W] = westCards;
    eastNum = eastCardsNum;
    westNum = westCardsNum;
    allDefendersCards = allDefendersCards2;
    while ((eastNum > 0) && (westNum > 0)) {
      
      /* random card */
      it = allDefendersCards.begin();
      j = a.getRandomUint(allDefendersCards.size());
      for (k = 0; k < j; k++)
        it++;
      //char buf[4]; printf("%d %d %s", eastNum, westNum, changeNumberToCard(*it, buf));  
      /* random hand */
      j = a.getRandomUint(possEast[*it]+possWest[*it]);
      if (j < possEast[*it]) { /* card in East hand */
        cards[E][*it / 13].insert(*it);
        eastNum--;
      }
      else { /* cards in West hand */
        cards[W][*it / 13].insert(*it);
        westNum--;
      }
      allDefendersCards.erase(*it);
    }
    /* rest of cards */
    it = allDefendersCards.begin();
    if (eastNum == 0)
      while (it != allDefendersCards.end()) {
        cards[W][*it / 13].insert(*it);
        it++;
      }
    else
      while (it != allDefendersCards.end()) {
        cards[E][*it / 13].insert(*it);
        it++;
      }
    /* set whoseCards - used to select good cards in selectUCBChild */   
    for (int s = C; s <= Sp; s++)  
      for (it = cards[E][s].begin(); it != cards[E][s].end(); it++)
        whoseCards[*it] = E;
    for (int s = C; s <= Sp; s++)  
      for (it = cards[W][s].begin(); it != cards[W][s].end(); it++)
        whoseCards[*it] = W;
        
    deals[i] = new Deal(cards, tr, suit, level, wonTricks, whoNow, whoseCards);
  }
  deal = deals[0]; /* needed to initialized UCTTree - root.addChildren */
  
}



int main()
{
  CardsSet cards;
  int eastCards = 13, westCards = 13;
  
  int count = 13, suit = NT, level = 3, wonTricks = 0, whoNow = 0;
  int num;
  Trick tr(0);
  
  cards.resize(4);
  defendersCards.resize(4);
  noCardsEast.resize(4);
  noCardsWest.resize(4);
  possEast.resize(52,1);
  possWest.resize(52,1);
  whoseCards.resize(52);
  for (int i = 0; i < 4; i++)
    cards[i].resize(4);
    
  while (true) {
    char cmd[16];
    scanf("%s", cmd);
    if (!strcmp(cmd, "set_dummy_cards")) {
      for (int i = 0; i < count; i++) {
        char buf[4];
        scanf("%s", buf);
        num = changeCardToNumber(buf);
        cards[N][num / 13].insert(num);
        whoseCards[num] = N;
      }
      printf("=\n\n");
    } 
    else if (!strcmp(cmd, "set_declarer_cards")) {
      for (int i = 0; i < count; i++) {
        char buf[4];
        scanf("%s", buf);
        num = changeCardToNumber(buf);
        cards[S][num / 13].insert(num);
        whoseCards[num] = S;
      }
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "set_defenders_cards")) {
      defendersCards.clear();
      for (int i = 0; i < 2*count; i++) {
        int c;
        char buf[4];
        scanf("%s", buf);
        c = changeCardToNumber(buf);
        defendersCards[c / 13].insert(c);
      }
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "auto_set_defenders_cards")) {
      vector < int > possibleCards;
      set < int >::iterator it;
      int i;

      possibleCards.resize(52);
      for (i = C; i <= Sp; i++)
        for (it = cards[N][i].begin(); it != cards[N][i].end(); it++)
          possibleCards[*it] = 1;     
      for (i = C; i <= Sp; i++)
        for (it = cards[S][i].begin(); it != cards[S][i].end(); it++)
          possibleCards[*it] = 1;     
      
      for (i = C; i <= Sp; i++)
        defendersCards[i].clear();  
      for (i = 0; i < 52; i++) {
        if (possibleCards[i] != 1)
          defendersCards[ i / 13].insert(i);
      }
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "gen_move")) {
      char c[2];
      int card;
      generate_deals(eastCards, westCards);
      UctTree tree = UctTree(1, deals);
      card = tree.genMove()->getCard();
      printf("=%s ", changeNumberToCard(card,c));
      printf("\n\n");
    } 
    else if (!strcmp(cmd, "start")) {
      mydeal = Deal(cards, tr, suit, level, wonTricks, whoNow, whoseCards);
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "play")) {
      char card[3];
      set < int >::iterator it;
      scanf("%s", card);
      int c = changeCardToNumber(card);
      int player = mydeal.getWhoNow();
      mydeal.playUserCard(c); 
      if ((c / 13) != mydeal.suitOfTrick()) {
        if (player == E)
          noCardsEast[mydeal.suitOfTrick()] = 1;
        else if (player == W)
          noCardsWest[mydeal.suitOfTrick()] = 1;
      }
      if (player % 2 == 0) {
        /* change probability of adjacent cards */
        if (defendersCards[c/13].find(c+1) != defendersCards[c/13].end()) {
          if (player == E)
            possWest[c+1] *= 2;
          if (player == W)
            possEast[c+1] *= 2;
        }
        if (defendersCards[c/13].find(c-1) != defendersCards[c/13].end()) {
          if (player == E)
            possWest[c-1] *= 2;
          if (player == W)
            possEast[c-1] *= 2;
        }
        defendersCards[c / 13].erase(c);
      }
      if (player == E)
        eastCards--;
      if (player == W)
        westCards--;
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "set_board_size")) {
      scanf("%d", &count);
      eastCards = count;
      westCards = count;
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "set_won_tricks")) {
      scanf("%d", &wonTricks);
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "set_who_now")) {
      char cont[4];
      scanf("%s", cont);
      switch (cont[0]) {
        case 'E' : whoNow = E; break;
        case 'N' : whoNow = N; break;
        case 'W' : whoNow = W; break;
        case 'S' : whoNow = S; break;
        default : break;
      }
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "set_contract")) {
      char cont[4];
      scanf("%s", cont);
      level = cont[0] - '0';
      switch (cont[1]) {
        case 'C' : suit = C; break;
        case 'D' : suit = D; break;
        case 'H' : suit = H; break;
        case 'S' : suit = Sp; break;
        case 'N' : suit = NT; break;
        default : break;
      }
      printf("=\n\n");
    } 
    else if (!strcmp(cmd, "print_dummy_cards")) {
      char c[3];
      int suit;
      set < int >::iterator it;
      for (suit = C; suit <= Sp; suit++)
        for (it = cards[N][suit].begin(); it != cards[N][suit].end(); it++)
          printf("%s ", changeNumberToCard(*it, c));
    }
    else if (!strcmp(cmd, "print_declarer_cards")) {
      char c[3];
      int suit;
      set < int >::iterator it;
      for (suit = C; suit <= Sp; suit++)
        for (it = cards[S][suit].begin(); it != cards[S][suit].end(); it++)
          printf("%s ", changeNumberToCard(*it, c));
    }
    else if (!strcmp(cmd, "quit")) {
      break;
    }
    else { /* wrong command */
      scanf("%*[^\n]");
      printf("? wrong command `%s'\n\n", cmd);
    }
    fflush(stdout);  
  }
  return 0;
}
