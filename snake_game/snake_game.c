#include "game.h"
#include <stdlib.h>
#include <stdio.h>
#include <ncurses.h>

int main(void){

    printf("\n");
    printf("*****************************\n");
    printf("Slither Game\n");
    printf("\n");
    printf("Controls: Use arrow keys to move the snake in the direction that you choose!\n");
    printf("\n");
    printf("Eating food increases or decreases the snake size\n");
    printf("\n");
    printf("\'s'\ to start || \'q'\ to quit\n");
    printf("******************************\n");
    char c;
    int score;
    int i,j,temp,k,l;
    int scores[5];
    scanf(" %c",&c);
    if(c=='s')
	score=game();


	FILE *nPtr;
    if((nPtr=fopen("./saves/save_best_5.game","r+"))==NULL)
	printf("Failed to open file");
    while(!feof(nPtr)){
	fscanf(nPtr,"%d", &scores[i]);
	i++;

	}
//   int z;
  //  for(z=0;z<5;z++){
//	printf("%d\n",scores[z]);
//	}
    fclose(nPtr);
    for(k=0;k<5;k++){
	for(l=k+1;l<5;l++){
		if(scores[k]<scores[l]){
		 temp=scores[k];
		 scores[k]=scores[l];
		 scores[l]=temp;
		}
	}
	}
    printf("\n");
  //   for(z=0;z<5;z++){
//	printf("%d\n",scores[z]);
//	}
    if(score> scores[4])
	scores[4]=score;
    for(k=0;k<5;k++){
	for(l=k+1;l<5;l++){
	 if(scores[k]<scores[l]){
	 	temp=scores[k];
		scores[k]=scores[l];
		scores[l]=temp;
		}
	
	}

	}
  //  for(z=0;z<5;z++){
//	printf("%d\n",scores[z]);
//	}
    if((nPtr=fopen("./saves/save_best_5.game","w"))==NULL)
	printf("Failed to open file");

     for(i=0;i<5;i++){
	fprintf(nPtr,"%d\n",scores[i]);
	}
     printf("GAME OVER\nYour score was %d \nRecompile to play again!\n",score);


    return(0);
}
