/*
 * score.c
 * score system for the card game wizard
 *  released: Dec 13, 2016
 *           V0.0
 *      Author: Zachary Rosborough
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_CHARS	11		//the max number of letters in a name +1 for null
#define NUM_OF_CARDS 60		//the number of cards in the deck
#define MAX_NUM_PLAYERS	10	//the max number of players
#define DEBUG
//------------------------------------------------------------------------------
void setup(int*,int*,int*,int*,char[]);
/*
 * will get the number of players, number of hands, up or up and down,
 * and who will deal first and players name
 */
void deal(int,int,const char[]);
// will print the dealer and the number of cards to be dealt
void bid(int,int,int,int*,const char[],int[]);
//will get the bid from each person and display number of cards left
void play(int,const int[],const char[],int);
//will display each players bid and wait for enter
void count(int,int,const int[],int[],const char[]);
//will get the number of hands woon and update there score
void score(int,const int[],const char[],int[]);
//will print the current scores
void get_num_players(int*);
//will get the number of players from user
void get_num_of_hands(int*,int*);
//will get the number of hands from user
void get_up_n_down(int*,int*);
//will ask if the user wants to play up and down
void get_names(char[],int*);
//will get the players names
void get_dealer(char[],int*,int*);
//will get the first dealer
int ranker(const int a, const int b,const int[]);
//Comparator function for sort
void sort(int[],const int[],int);
//will sort to get the ranking
//------------------------------------------------------------------------------
int main(void)
{
//print welcome
	printf("**********WIZARD SCORING SYSTEM**********\n "
			"         by:Zachary Rosborough\n");
//declare variables
	char names[MAX_CHARS*MAX_NUM_PLAYERS];	//array of names
	int bids[MAX_NUM_PLAYERS];				//array of bids
	int scores[MAX_NUM_PLAYERS];			//array of scores
	int rank[MAX_NUM_PLAYERS];				//array of the current ranking
	int dealer=0;							//Current dealer
	int hand=1;								//Current number of cards in hand
	int remaining=0;						//number cards not bid
	int end_hand=0;							//max number of cards in hand
	int down=0;								//are you playing up and down
	int players=0;							//the number of players
	int flag=1;								//flag to end game
	int up=1;								// if going up then 1
//Clear the arrays
	memset(names,'\0',MAX_CHARS*MAX_NUM_PLAYERS);
	memset(bids,'\0',MAX_NUM_PLAYERS);
	int xx;
	for(xx=0;xx<MAX_NUM_PLAYERS;++xx){
	  scores[xx]=0;
	}
//get setup
	setup(&players,&end_hand,&down,&dealer,names);
//loop until the end is reached if down then go back down
	while(flag){
	//deal the cards
		deal(dealer,hand,names);
	//get bids
		bid(players,hand,dealer,&remaining,names,bids);
	//play
		play(dealer,bids,names,players);
	//count hands won
		count(players,hand,bids,scores,names);
	//display the scores
		score(players,scores,names,rank);
	//set next hand
		if (up){
			++hand;
		}
		if (hand>end_hand){
			if (down){
				up=0;
				--hand;
			}
			else flag=0;
		}
		if (!up){
			--hand;
		}
		if (hand==0){
			flag=0;
		}
		++dealer;
		if (dealer>=players){
			dealer=0;
		}
	}
//print the final score
	printf("**********THE_END***********\n");
}
//------------------------------------------------------------------------------
void get_num_players(int* players)
{
	//get the number of players
	int test = 0;
	while (!test) {
		printf("Enter the number of players>");
		test = scanf("%d", players);
		if (test > 0 && *players<= MAX_NUM_PLAYERS  && (*players>0)){
			break;
		} else
			printf("ERROR INVALID INPUT THE MAX NUMBER OF PLAYERS IS %d\n",
					MAX_NUM_PLAYERS);
		char x;
			scanf("%c",&x);
		test = 0;
	}
}
//------------------------------------------------------------------------------
void get_num_of_hands(int *players,int *end_hand)
{
	//calculate the number of hands that can be played
	int cards = (NUM_OF_CARDS / *players);
	//print the max number of hands
	printf("With %d players you can play %d hands.\n", *players, cards);
	//Prompt for, get and check the number of hands
	int test = 1;
	while (1) {
		printf("Enter number of hands>");
		test = scanf("%d", end_hand);
		//check that input was valid number
		if (test < 1) {
			printf("***INPUT ERROR***\n");
			//clear the input
			char x;
			scanf("%c", &x);
			continue;
		} else
		//check the input is less then cards
		if (*end_hand > cards) {
			printf("***ERROR***\n number of hands must be less then %d\n",
					cards);
			continue;
		} else
		//check that the input is grater then 0
		if (*end_hand <= 0) {
			printf("***ERROR***\n number of cards must be grate then 0\n");
			continue;
		} else
			break;
	}
}
//------------------------------------------------------------------------------
void get_up_n_down(int *end_hand,int *down)
{
	//ask if you want to play up and down or just up
	printf("Do you want to go up to %d and back down to 1?\n", *end_hand);
	//prompt for answer
	while (1) {
		printf("Enter Y/N>");
		//clear input
		char x;
		scanf("%c", &x);
		//get input
		char input;
		scanf("%c", &input);
		//check input
		if ((input != 'Y') && (input != 'y') && (input != 'N')
				&& (input != 'n')) {
			printf("***INPUT ERROR***\n");
			continue;
		} else if (input == 'y' || input == 'Y') {
			*down = 1;
			break;
		} else if (input == 'n' || input == 'N') {
			*down = 0;
			break;
		}
	}
}
//------------------------------------------------------------------------------
void get_names(char names[],int *players)
{
	//ask for the players names in the order of play
	printf("Enter the players names in order of play\n");
	//prompt for, get, and check the input
	int x;
	//clear the input
	char z;
	scanf("%c", &z);
	for (x = 0; x < *players; ++x) {
		printf("Enter player %d's name>", x + 1);
		char input[255];
		memset(input, 0, 255);
		fgets(input, 255, stdin);
		//check the length of the input
		if ((strlen(input)) > MAX_CHARS) {
			printf("***WARNING***\n");
			printf("your input is too long and will be shortened\n");
		}
		//remove newline
		strtok(input, "\n");
		//Transfer the input to name
		strncpy(&names[x * MAX_CHARS], input, MAX_CHARS);
	}
}
//------------------------------------------------------------------------------
void get_dealer(char names[],int *players,int *dealer)
{
	//get the player who will deal first
	printf("Who will deal first?\n");
	while (1) {
		printf("Enter first dealer's name>");
		char input[MAX_CHARS];
		fgets(input, MAX_CHARS, stdin);
		//remove new line
		strtok(input, "\n");
		int x;
		int flag = 0;
		for (x = 0; x < *players; ++x) {
			if ((strcmp(input, &names[x * MAX_CHARS])) == 0) {
				*dealer = x;
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			printf("***ERROR NO MATCH***\n");
			continue;
		} else
			break;
	}
}
//------------------------------------------------------------------------------
void setup(int *players,int *end_hand,int *down,int *dealer,char names[])
{
//get the number of players
	get_num_players(players);
//get the number of hands
	get_num_of_hands(players,end_hand);
//ask if you want to play up and down or just up
	get_up_n_down(end_hand,down);
//ask for the players names in the order of play
	get_names(names,players);
//get the player who will deal first
	get_dealer(names,players,dealer);
}
//------------------------------------------------------------------------------
void deal(int dealer,int hand,const char names[])
{
//print the dealer and number of cards to deal
  printf("\n**********TIME TO DEAL**********\n");
  printf("dealer>%s\nnumber of cards>%d\n",&names[dealer*MAX_CHARS],hand);
  printf("HIT ENTER TO CONTINUE!\n");
  getchar();
  getchar();
}
//------------------------------------------------------------------------------
void bid(int players,int hand,int dealer,int *remaining,const char names[],int bids[])
{
	printf("\n**********TIME TO BID**********\n");
//declare variables
	int loop=0;		//loop control
	int total=0;	//number bid
//set the starting number of cards
	*remaining=hand;
//set up the first person to bid
	int bider=dealer+1;		//the first person to bid
	if (bider==players){
		bider=0;
	}
//loop for each player
	for (loop=0;loop<players;++loop){
	//print the appropriate message
		if (*remaining>1 || *remaining==0){
		printf("%s, there are %d hands not claimed how many will you take?\n",
				&names[bider*MAX_CHARS],*remaining);
		}
		else {
			printf("%s, there is %d hand not claimed how many will you take\n",
				&names[bider*MAX_CHARS],*remaining);
		}
		while (1){
			printf("ENTER %s's bid>",&names[bider*MAX_CHARS]);
		//get input
			int test=scanf("%d",&bids[bider]);
		//validate input
			if ((test==1) && (bids[bider]<=hand) && (bids[bider]>=0)){
				break;
			}
			else if (test!=1){
				printf("***INPUT ERROR***\n");
				continue;
			}
			else if (bids[bider]<0){
				printf("***ERROR***\n");
				printf("BID CAN NOT BE LEES THEN ZERO!!!\n");
				continue;
			}
			else if (bids[bider]>hand){
				printf("***ERROR***\n");
				printf("BID CAN NOT BE GRATER THE THE AMOUNT DELT\n");
				continue;
			}
			else printf("**************BUG***************\n");
		}
	//update the remaining
		*remaining-=bids[bider];
	//move to next bidder
		++bider;
				if (bider==players){
					bider=0;
				}
	}
}
//------------------------------------------------------------------------------
void play(int dealer,const int bids[],const char names[],int players)
{
//calculate who plays first
	int leader=0;
	leader=dealer+1;
	if (leader>=players){
		leader=0;
	}
	printf("\n**********MAY THE ODDS BE EVER  IN YOU'R FAVOR**********\n\n");
	printf("------------%10s leads!!!----------\n",&names[leader*MAX_CHARS]);
// print every ones bids
	int loop;
	printf("                --name-------bid-  \n");
	for (loop=0;loop<players;++loop){
		printf("                |%-10s   %d |\n",&names[loop*MAX_CHARS],bids[loop]);
		printf("                ----------------\n");
	}
	printf("STRIKE ENTER TO CONTINUE!!!\n");
	getchar();
	getchar();
}
//------------------------------------------------------------------------------
void count(int players,int hand,const int bids[],int scores[],const char names[])
{
	printf("\n**********SCORING TIME**********\n");
	printf("Enter the number of hands won by each player.\n");
//loop for the number of players
	int loop=0;
	int input=0;
	int total=0;
	for (loop=0;loop<players;++loop){
	//print name
		while(1){
			printf("%s>",&names[loop*MAX_CHARS]);
		//get input
			int test=scanf("%d",&input);
		//validate input
			if (test!=1){
				printf("***INPUT ERROR***\n");
			//clear input
				getchar();
			}
			else if(input>hand){
				printf("***ERROR***\n");
				printf("YOU CAN NOT WINN MORE HANDS THEN WHERE PLAYED\n");
			//clear input
				getchar();
			}
			else if (input<0){
				printf("***ERROR***\n");
				printf("YOU CAN NOT WIN NEGITIVE HANDS\n");
			//clear input
				getchar();
			}
			else if (input+total>hand){
				printf("***ERROR***\n");
				printf("TOTAL OF HAND EXSEDED\n");
			}
			else{
				//calculate score
				if (input==bids[loop]){
					scores[loop]+=20;
					scores[loop]+=(bids[loop]*10);
					total+=input;
					break;
				}
				else if (input!=bids[loop]){
					if (input>bids[loop]){
						scores[loop]-=((input-bids[loop])*10);
						total+=input;
						break;
					}
					else {
						scores[loop]-=((bids[loop]-input)*10);
						total+=input;
						break;
					}
				}
			}
		}
	}
}
//------------------------------------------------------------------------------
void score(int players,const int scores[],const char names[],int rank[])
{
//calculate the ranking
	sort(rank,scores,players);
	printf("\n**********SCORES**********\n");
	printf("  ---names------score---\n");
//loop for the number of players
	int loop=0;
	for (loop=0;loop<players;++loop){
	//print there score
		printf("     |%-10s%4d |\n",&names[rank[loop]*MAX_CHARS],scores[rank[loop]]);
		printf("     ------------------\n");
	}
}
//------------------------------------------------------------------------------
int ranker(const int a, const int b,const int scores[])
{
	return(scores[a]-scores[b]);
}
//------------------------------------------------------------------------------
void sort(int rank[],const int scores[],int players)
{
//reset the rank array
	int x;
	for (x=0;x<players;++x){
		rank[x]=x;
	}
	int swap=0;
//loop until no swaps
	do{
		swap=0;
		int loop=0;
	//loop for number of players
		for (loop=0;loop<players-1;++loop){
			int diff=ranker(rank[loop],rank[loop+1],scores);
			if (diff<0){
				int temp=rank[loop];
				rank[loop]=rank[loop+1];
				rank[loop+1]=temp;
				++swap;
			}
		}
	}while(swap);
}
//------------------------------------------------------------------------------



