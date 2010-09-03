#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <set>

#include "uct.h"
#include "utils.h"
#include "dummyplayer.h"

using namespace std;

MyRandom Deal::a = MyRandom(time(NULL));
MyRandom Dummyplayer::a = MyRandom(time(NULL));
MyRandom Betterplayer::a = MyRandom(time(NULL));

int main()
{
  set <int> dummyCards, declarerCards, defendersCards;
  CardsSet cards;
  int count, suit = H, level = -1, cit = 0;
  int num;
  Trick tr;
  
  cards.resize(4);
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
      }
      printf("=\n\n");
    } 
    else if (!strcmp(cmd, "set_declarer_cards")) {
      for (int i = 0; i < count; i++) {
        char buf[4];
        scanf("%s", buf);
        num = changeCardToNumber(buf);
        cards[S][num / 13].insert(num);
      }
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "set_east_cards")) {
      for (int i = 0; i < count; i++) {
        char buf[4];
        scanf("%s", buf);
        num = changeCardToNumber(buf);
        cards[E][num / 13].insert(num);
      }
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "set_west_cards")) {
      for (int i = 0; i < count; i++) {
        char buf[4];
        scanf("%s", buf);
        num = changeCardToNumber(buf);
        cards[W][num / 13].insert(num);
      }
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "set_defenders_cards")) {
      defendersCards.clear();
      for (int i = 0; i < 2*count; i++) {
        char buf[4];
        scanf("%s", buf);
        defendersCards.insert(changeCardToNumber(buf));
      }
      //cards[1] = defendersCards;
      printf("=\n\n");
    }
    else if (!strcmp(cmd, "auto_set_defenders_cards")) {
      vector < int > possibleCards;
      set < int >::iterator it;
      
      possibleCards.resize(52);
      for (it = dummyCards.begin(); it != dummyCards.end(); it++)
        possibleCards[*it] = 1;  
      for (it = declarerCards.begin(); it != declarerCards.end(); it++)
        possibleCards[*it] = 1;
        
      defendersCards.clear();
      for (int i = 0; i < 52; i++) {
        if (possibleCards[i] != 1)
          defendersCards.insert(i);
      }
      //cards[1] = defendersCards;
      printf("=\n\n");
    } // TODO
    else if (!strcmp(cmd, "gen_move")) {
      char c[2];
      int card;
      printf("w gen_move\n");
      Deal newDeal(cards, tr, suit, level, 0, 3, cit);
      printf("stworzono deal\n");
      UctTree tree = UctTree(1, newDeal); //TODO
      card = tree.genMove()->getCard();
      printf("%s ", changeNumberToCard(card,c));
      printf("\n");
    } // TODO
    else if (!strcmp(cmd, "play")) {
      char player[2], card[3];
      scanf("%s %s", player, card);
      int c = changeCardToNumber(card);
      if (player[0] == 'N')
        dummyCards.erase(c);
      else {
        if (player[0] == 'S')
          declarerCards.erase(c);
        else 
          defendersCards.erase(c);
      }
    }
    else if (!strcmp(cmd, "set_board_size")) {
      scanf("%d", &count);
      printf("\n");
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
      printf("\n");
    } 
    else if (!strcmp(cmd, "print_dummy_cards")) {
      char c[3];
      set < int >::iterator it;
      for (it = dummyCards.begin(); it != dummyCards.end(); it++)
        printf("%s ", changeNumberToCard(*it, c));
    }
    else if (!strcmp(cmd, "print_declarer_cards")) {
      char c[3];
      set < int >::iterator it;
      for (it = declarerCards.begin(); it != declarerCards.end(); it++)
        printf("%s ", changeNumberToCard(*it, c));
    }
    else if (!strcmp(cmd, "print_defenders_cards")) {
      char c[3];
      set < int >::iterator it;
      for (it = defendersCards.begin(); it != defendersCards.end(); it++)
        printf("%s ", changeNumberToCard(*it, c));
    }
    else if (!strcmp(cmd, "quit")) {
      break;
    }
    else { // wrong command
      scanf("%*[^\n]");
      printf("? wrong command `%s'\n\n", cmd);
    }
    fflush(stdout);  
  }
  return 0;
}
