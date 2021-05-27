//  A simple game of blackjack.
//  main.c
//
//  Created by Matthew Chan on 29/4/2018.
//  Copyright © 2018 Matthew Chan. All rights reserved.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define DECK_SIZE 52

typedef enum  //Assign names to integral constants
{
    ACE,
    TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
    JACK, QUEEN, KING
} CARD_NAME;

typedef struct blackjack
{
    int value;  //Hold the card values
    char p_name[25];  //Hold player's name
    CARD_NAME name;  //Hold card names
} CARD;

int clean_input(void) {     //Clear the invalid input
    while (getchar()!='\n');
    return 1;
}

void rules(struct blackjack *b, char *rules)
{
    rules = (char*)calloc(1000, sizeof(char));  //Dynamic allocation of memory for displaying the characters in file
    FILE *fin;
    fin = fopen("rules.txt","r");
    if (fin == NULL) {
        printf("Oops, this file doesn't exist!\n");
        exit(0);    //Exit if file could not be found
    }
    else
    {
        fscanf(fin, "%[^\t]", rules);  //Scan every character in file
        printf("%s",rules);  //Display rules
    }
    free(rules);  //Freeing allocated memory
}

void Input(struct blackjack *b)  //Get the name of the player
{
    printf("\nWhat is your name? ");
    scanf("%s", b->p_name);
    clean_input();
    printf("OK, %s, LETS PLAY!\n", b->p_name);
}

void initDeck(CARD deck[])  //Initializes the given card array to a typical 52 card deck.
{
    int counter;
    int curr_value = 0;
    for(counter = 0; counter < DECK_SIZE; counter++)
    {
        deck[counter].name = counter % 13; //Gets card name
        if(deck[counter].name == JACK ||
           deck[counter].name == QUEEN ||
           deck[counter].name == KING)
            deck[counter].value = 10; //Gets card value for face cards
        else if(deck[counter].name == ACE)
            deck[counter].value = 1; //Gets card value for Ace
        else
        {
            deck[counter].value = curr_value % 9 + 2; //Gets card value for # cards
            curr_value++;
        }
    }
}

void Shuffle(CARD deck[])  //Shuffle the cards in the deck
{
    CARD temp; //Temporary value holder
    int curr_index; //Used to switch places with new_loc
    int new_loc; //New location of card in deck
    for(curr_index = 0; curr_index < DECK_SIZE; curr_index++)
    {
        time_t t; /* Gets time for a random value */
        srand((unsigned)time(&t)); /* Seeds the random-number generator. */
        new_loc = rand() % DECK_SIZE; //Picks subscript from DECK_SIZE - 1 for array
        temp = deck[curr_index]; //Type CARD temp gets card from 0 - 51
        deck[curr_index] = deck[new_loc]; //Current card gets new location
        deck[new_loc] = temp; //New location gets temp
    }
}

void dispCard(CARD card)  //Prints Card Name and Card Value assigned from initDeck
{
    switch(card.name)
    {
        case TWO:
            printf(" Two (%d)", card.value);
            break;
        case THREE:
            printf(" Three (%d)", card.value);
            break;
        case FOUR:
            printf(" Four (%d)", card.value);
            break;
        case FIVE:
            printf(" Five (%d)", card.value);
            break;
        case SIX:
            printf(" Six (%d)", card.value);
            break;
        case SEVEN:
            printf(" Seven (%d)", card.value);
            break;
        case EIGHT:
            printf(" Eight (%d)", card.value);
            break;
        case NINE:
            printf(" Nine (%d)", card.value);
            break;
        case TEN:
            printf(" Ten (%d)", card.value);
            break;
        case JACK:
            printf(" Jack (%d)", card.value);
            break;
        case QUEEN:
            printf(" Queen (%d)", card.value);
            break;
        case KING:
            printf(" King (%d)", card.value);
            break;
        case ACE:
            printf(" Ace (%d)", card.value);
            break;
    }
}

int dealCard(CARD deck[], int *curr)
{
    dispCard(deck[*curr]); //Calls printCard with current card
    ++*curr;  //curr is incremented to point to the next card to be dealt.
    return deck[*curr - 1].value;  //Returns the value of the card which was just dealt.
}

int playerGetsCard(struct blackjack b,CARD deck[], int *player_value, int *current_card)  //Deals cards to the player. Checks for a bust.
{
    char opt,hold_c;
    printf("%s's turn:\n",b.p_name);
    do
    {
        do{
            printf("Hit or Stand (H/S)?\n");  //Prompts the user for stand and hit
        } while (((scanf("%[H,S]%c", &opt, &hold_c)!=2 || hold_c!='\n') && clean_input()));    //Scanf for A-Z only and did not read 2 objects
        if(opt == 'H')
        {
            printf("New card:");
            *player_value += dealCard(deck, current_card);
            if(*player_value < 22)
                printf("\n%s has: %d\n", b.p_name, *player_value);
        }
        else if(opt == 'S')
            return 1;  //A return value of 1 indicates the player did not bust.
    } while(*player_value < 22);      //Loops for player action when player has not busted
    printf("\n%s busted(%d)!  You lose.\n",b.p_name,*player_value);
    return 0;  //A return value of 0 indicates the player did bust.
}

int dealerGetsCard(CARD deck[], int *dealer_value, int *current_card)  //Deals cards to the dealer
{
    printf("Dealer's turn:\n");
    do
    {
        //Check if dealer_value > 17
        if(*dealer_value > 16)
        {
            printf("Dealer stands.\n");
            return 1;  //A return value of 1 indicates the dealer did not bust.
        }
        //Dealer must hit until value > 17 or bust occurs
        else
        {
            printf("New card:");
            *dealer_value += dealCard(deck, current_card);
            printf("\nDealer has: %d\n", *dealer_value);
        }
    } while(*dealer_value < 22);  //Loops for dealer action
    printf("Dealer busted(%d)!  You win.\n",*dealer_value);
    return 0;  //A return value of 0 indicates the dealer did bust.
}

void findWinner(int player_value, int dealer_value, int *balance, int bet)  //Compare player value with dealer value
{
    if(player_value == dealer_value)
        printf("Tie!\n"); //Player ties, balance remains the same
    else if(player_value > dealer_value)
    {
        printf("You win!\n");
        *balance += bet; //Player wins, balance incremented by bet
    }
    else
    {
        printf("You lose!\n");
        *balance -= bet; //Player loses, decremented by bet
    }
}

void Start(CARD deck[], int *player_value, int *dealer_value, int *current_card)  //Deals two cards to each player.
{
    int counter;
//    int num_ace = 0;
    printf("\nCurrent hand:");
    //Deal two cards to player, then add card values together into player_value
    for(counter = 0; counter < 2; counter++)
    {
        *player_value += dealCard(deck, current_card);
    }
    printf(".  Player has: %d\n", *player_value);
    printf("Dealer hand:");
    //Deal two cards to dealer, then add card values together into dealer_value
    for(counter = 0; counter < 2; counter++)
    {
        *dealer_value += dealCard(deck, current_card);
    }
    printf(".  Dealer has: %d\n\n", *dealer_value);
}

int getBet(int balance)
{
    int bet,hold_f;
    char hold_c;
    printf("Current balance: %d\nEnter bet:", balance);
    do{
        hold_f = scanf("%d%c", &bet, &hold_c);   //Scans input from user
        while (hold_f !=2 || hold_c!='\n') {   //While scanf did not read 2 objects or the user inputed a number and then a character (eg. 12te)
            clean_input();
            printf("\nYou have typed non numeric characters.\n"
                   "Please enter valid bet:");    //Ask user to type a valid value
            hold_f = scanf("%d%c", &bet, &hold_c);
        }
        if(bet<=0 || bet>balance) {    //Set the range for balance
            printf("\nBet must be greater than zero and at most %d\nPlease enter valid bet:", balance);
        } else {     //If user typed in correctly, exit do-while.
            break;
        }
    }while(1);
    return bet;
}

int playAgain(int balance)
{
    char choice,hold_c;
    if(balance == 0)  //Balance is not sufficient to continue
    {
        printf("You are broke, get out of the casino!\n");
        return 0;
    }
    else  //Ask if user would like to play again
    {
        do{
            printf("Play again? (Y/N):");    //Gets input from user
        } while (((scanf("%[Y,N]%c", &choice, &hold_c)!=2 || hold_c!='\n') && clean_input()));    //Scanf for A-Z only and did not read 2 objects
        if(choice == 'Y')
            return 1;  //A return value of 1 indicates the game should continue
        else if(choice == 'N')
            return 0;  // A return value of 0 indicates the game should stop
    }
    return 0;
}

int main()
{
    int balance = 1000, hold_f, options;
    int current_card = 0, bet = 0; // Next card to be dealt.  Valid values are 0 to (DECK_SIZE-1)
    char hold_c,r;
    struct blackjack b1;
    CARD deck[DECK_SIZE];
    
    printf("Welcome to a game of blackjack!\n");
    printf("\nPlease select one of the following option:\n(1)Start Game\n(2)Rules\n(3)EXIT\n");
    do{
        hold_f = scanf("%d%c", &options, &hold_c);   //Scans input from user
        while (hold_f !=2 || hold_c!='\n') {   //While scanf did not read 2 objects or the user inputed a number and then a character (eg. 12te)
            clean_input();
            printf("\nYou have typed non numeric characters.\n"
                   "Please select one of the following option:\n(1)Start Game\n(2)Rules\n(3)EXIT\n");
            hold_f = scanf("%d%c", &options, &hold_c);
        }
        if(options<=0 || options>3) {    //Set the range for options
            printf("\nInvalid options\n"
                   "Please select one of the following option:\n(1)Start Game\n(2)Rules\n(3)EXIT\n");
        }
        else
        {
            switch(options)
            {
                case 1:  //Start game
                    Input(&b1);
                    initDeck(deck);
                    do
                    {
                        int player_value = 0; // Value of the player's hand
                        int dealer_value = 0; //Value of the dealer's hand
                        printf("\nShuffling deck...\n\n");
                        Shuffle(deck);
                        bet = getBet(balance);
                        Start(deck, &player_value, &dealer_value, &current_card);
                        //If the player didn't bust
                        if(playerGetsCard(b1,deck, &player_value, &current_card))
                        {
                            //If the dealer didn't bust
                            if(dealerGetsCard(deck, &dealer_value, &current_card))
                                findWinner(player_value, dealer_value, &balance, bet);
                            else
                                balance += bet;  //Update current balance
                        }
                        else
                            balance -= bet;  //Update current balance
                    } while(playAgain(balance));
                    printf("Goodbye!\n");  //If player type N then exit game
                    exit(0);
                    break;
                case 2:  //Scan file for the rules 
                    rules(&b1, &r);
                    main();
                case 3:  //Exit
                    printf("Terminating...\n");
                    exit(0);
            }
        }
    }while(1);
}
