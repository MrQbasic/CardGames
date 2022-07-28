#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "time.h"
#include <vector>

enum CardType{
    spade = 0,
    heart = 1,
    diamond = 2,
    club = 3,
};

enum CardVal{
    ACE = 1,
    TWO = 2,
    THREE = 3,
    FOUR = 4,
    FIVE = 5,
    SIX = 6,
    SEVEN = 7,
    EIGHT = 8,
    NINE = 9,
    TEN = 10,
    JACK = 11,
    QUEEN = 12,
    KING = 13
};

struct Card{
    int CardType;
    int CardVal;
};

struct Game{
    int winner;
    bool eightused;
    bool repeat; 
    int cardCount; 
    int botCount;
    bool over;
    struct Card middleCard;
    std::vector<std::vector<Card>> Hands;
};

void printHelp(){
    printf("-----Mau-Mau-----");
    printf("  Usage: maumau [number of bots] [number of cards] [option]\n");
    printf("  Options:\n");
    printf("    --help          shows this menu\n");
    printf("    --repeat        starts new round after win of lose\n");
}

void printHand(Game &game){
    //print top line
    for(int i=0; i<game.Hands[0].size(); i++){
        printf("+---+ ");
    }
    printf("\n");
    //print type
    for(int i=0; i<game.Hands[0].size(); i++){
        switch(game.Hands[0][i].CardType){
            case spade:   printf("| S | "); break;
            case heart:   printf("| H | "); break;
            case diamond: printf("| D | "); break;
            case club:    printf("| C | "); break;
        }
    }
    printf("\n");
    //print val
    for(int i=0; i<game.Hands[0].size(); i++){
        switch(game.Hands[0][i].CardVal){
            case QUEEN: printf("| Q | "); break;
            case JACK:  printf("| J | "); break;
            case KING:  printf("| K | "); break;
            case ACE:   printf("| A | "); break;
            default:
                if(game.Hands[0][i].CardVal < 10){
                    printf("| %d | ", game.Hands[0][i].CardVal);
                }else{
                    printf("| %d| ", game.Hands[0][i].CardVal);
                }
        }
    }
    printf("\n");
    //print bottom line
    for(int i=0; i<game.Hands[0].size(); i++){
        printf("+---+ ");
    }
    printf("\n");
    //print index line
    for(int i=0; i<game.Hands[0].size(); i++){
        if(i < 10){
            printf("  %d   ", i);
        }else{
            printf("  %d  ", i);
        }
    }
    printf("\n");
}

void printCard(Card &card){
    //print top
    printf("+---+\n");
    //print type
    switch(card.CardType){
        case spade:   printf("| S |\n"); break;
        case heart:   printf("| H |\n"); break;
        case diamond: printf("| D |\n"); break;
        case club:    printf("| C |\n"); break;
    }
    //print val
    switch(card.CardVal){
        case QUEEN: printf("| Q |\n"); break;
        case JACK:  printf("| J |\n"); break;
        case KING:  printf("| K |\n"); break;
        case ACE:   printf("| A |\n"); break;
        default:
            if(card.CardVal < 10){
                printf("| %d |\n", card.CardVal);
            }else{
                printf("| %d|\n", card.CardVal);
            }
    }
    //print bottom
    printf("+---+\n");
}

void printStats(Game &game){
    for(int i=1; i<=game.botCount; i++){
        printf("Bot %d: %d  ", i, game.Hands[i].size());
    }
    printf("\n");
}

Card* getRandomCard(){
    struct Card* card = (struct Card*)malloc(sizeof(struct Card));
    card->CardType = rand() %  4;
    card->CardVal  = rand() % 14;
    return card;
}

void playBots(Game &game){
    for(int i=1; i<game.botCount+1; i++){
        if((game.middleCard.CardVal != EIGHT) || (!game.eightused && game.middleCard.CardVal == EIGHT)){
            if(!(game.eightused && game.middleCard.CardVal == EIGHT)) game.eightused = true;
            //check for special cards
            switch(game.middleCard.CardVal){
                case SEVEN:
                    game.Hands[i].push_back(*getRandomCard());
                    game.Hands[i].push_back(*getRandomCard());
                    break;
            }
            //check if bot needs to draw a card
            bool draw = true;
            while(draw){
            for(auto card : game.Hands[i]){
                if(card.CardType == game.middleCard.CardType ||
                   card.CardVal  == game.middleCard.CardVal)
                {
                    draw = false;
                }
                }
                if(draw){
                    game.Hands[i].push_back(*getRandomCard());
                }
            }
            //play random valid card
            std::vector<int> validCards;
            for(int i1=0; i1<game.Hands[i].size(); i1++){
                if(game.Hands[i][i1].CardType == game.middleCard.CardType ||
                   game.Hands[i][i1].CardVal  == game.middleCard.CardVal)
                {
                    validCards.push_back(i1);
                }
            }
            int cardIndex = validCards[rand() % validCards.size()];
            game.middleCard = game.Hands[i][cardIndex];
            if(game.Hands[i][cardIndex].CardVal == EIGHT) game.eightused = false;
            game.Hands[i].erase(game.Hands[i].begin()+cardIndex);
        }
    }
}

void playCard(Game &game){
    if((game.middleCard.CardVal != EIGHT) || (!game.eightused && game.middleCard.CardVal == EIGHT)){
        if(!(game.eightused && game.middleCard.CardVal == EIGHT)) game.eightused = true;
        //check for special cards
        switch(game.middleCard.CardVal){
            case SEVEN:
                game.Hands[0].push_back(*getRandomCard());
                game.Hands[0].push_back(*getRandomCard());
                break;
        }
        //check if bot needs to draw a card
        bool draw = true;
        while(draw){
            for(auto card : game.Hands[0]){
                if(card.CardType == game.middleCard.CardType ||
                    card.CardVal  == game.middleCard.CardVal)
                {
                    draw = false;
                }
            }
            if(draw){
                game.Hands[0].push_back(*getRandomCard());
            }
        }
        //print hand
        printHand(game);
        //get cardIndex from player input
        int cardIndex = -1;
        while(1){
            printf("Card Number: ");
            scanf("%d", &cardIndex);
            if(cardIndex >= 0 && cardIndex < game.Hands[0].size()){
                Card* c = &(game.Hands[0][cardIndex]);
                //check if card is ok to place
                if(c->CardType == game.middleCard.CardType || c->CardVal == game.middleCard.CardVal){
                    game.middleCard = *c;
                    if(game.Hands[0][cardIndex].CardVal == EIGHT) game.eightused = false;
                    game.Hands[0].erase(game.Hands[0].begin()+cardIndex);
                    return;
                }
            }
        }
    }
}

bool win(Game &game){
    int i = 0;
    for(auto hand : game.Hands){
        if(hand.size() == 0){
            game.winner = i;
            return true;
        }
        i++;
    }
    return false;
}

int main(int argc, char** argv){
    srand(time(NULL));
    struct Game game;
    //check args
    if(argc < 3){ printHelp(); return 0;}
    for(int i=0; i<argc; i++){
        if(argv[i] == "--help")   {printHelp(); return 0;}
        if(argv[i] == "--repeat") {game.repeat = true;}
    }
    game.botCount  = atoi(argv[1]);
    game.cardCount = atoi(argv[2]);
    //init hands
    for(int i1=0; i1<game.botCount+1; i1++){
        std::vector<Card> hand;
        for(int i2=0; i2<game.cardCount; i2++){
            hand.push_back(*getRandomCard());
        }
        game.Hands.push_back(hand);
    }
    //main game loop
    game.middleCard = *getRandomCard();
    game.over = false; 
    while(!game.over){
        printStats(game);
        printf("Middle card: \n");
        printCard(game.middleCard);
        printf("\n\n"); 
        
        playCard(game);
        
        playBots(game);

        game.over = win(game);
    }
    if(game.winner == 0){
        printf("YOU WIN!\n");
    }else{
        printf("BOT %d WINS!\n", game.winner);
    }

    return 0;
}