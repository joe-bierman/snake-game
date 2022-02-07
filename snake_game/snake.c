/* snake.c -------
 *
 * Filename: snake.c
 * Description:
 * Author: Rishabh Agrawal
 * Maintainer:
 * Created: Sun Sep 13 9:12:30 2020
 * Last-Updated: September 13 22:40 2020
 *           By: Rishabh Agrawal
 *
 */

/* Commentary:
 *
 *
 *
 */

/* Change log:
 *
 *
 */

/* Copyright (c) 2016 The Trustees of Indiana University and
 * Indiana University Research and Technology Corporation.
 *
 * All rights reserved.
 *
 * Additional copyrights may follow
 */


#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "snake.h"
#include "key.h"
#include <ncurses.h>


// Initialize snake
Snake* init_snake(int x, int y){
  Snake* head = create_tail(x, y);
  Snake* tail1 = create_tail(x-1, y);
  Snake* tail2 = create_tail(x-2, y);
  tail1->next = tail2;
  head->next = tail1;
  return head;
}

// Creates one tail
Snake* create_tail(int x, int y){
  Snake* snake = malloc(sizeof(snake));
  snake->color[0] = 0;
  snake->color[1] = 0;
  snake->color[2] = 255;
  /* snake->color = {0, 0, 255}; */
  snake->symbol = '#';
  snake->next = NULL;
  snake->x = x;
  snake->y = y;
  return snake;
}

// Moves the snake in the input direction
Snake* move_snake(Snake* snake, int direction){
    // TODO
  Snake* new_head = malloc(sizeof(new_head));
  new_head->x= snake->x;
  new_head->y= snake->y;
  /*if(lastDirection==UP && direction==DOWN){return snake;}
  if(lastDirection==DOWN && direction ==UP){return snake;}
  if(lastDirection==LEFT && direction ==RIGHT){return snake;}
  if(lastDirection==RIGHT && direction ==LEFT){return snake;}
  // Set the new head to have the x and y coordinates as the existing head of the snake
  */
  switch(direction) { 
    case UP:
        //Write code to make the new head go up by 1 cell
        new_head->y-=1;
      break;
    case DOWN:
        //Write code to make the new head go down by 1 cell
        new_head->y+=1;
      break;
    case RIGHT:
        //Write code to make the new head go right by 1 cell
        new_head->x+=1;
      break;
    case LEFT:
        //Write code to make the new head go left by 1 cell
	new_head->x-=1;
      break;
  }
  
  //Set new head as the new head of the entire snake
  //Add all the features (color and symbol) to the new cell
  // Delete the tail from the snake: HINT - there is a remove tail function below
 new_head->next=snake;
 new_head->color[0]=  snake->color[0];
 new_head->color[1]= snake->color[1];
 new_head->color[2]= snake->color[2];
 new_head->symbol = '#';
 new_head= remove_tail(new_head);
  
  return new_head;
}
void grow_tail(Snake* head, Snake* tail){
 Snake* x =head;
 while(x->next){
  x=x->next;

}
 x->next=tail;
// return x;
}
Snake* remove_tail(Snake* snake){
    Snake* end = snake;
    
    while(end->next->next)
        end = end->next;
    free(end->next);
    end->next = NULL;
	
    return snake;
}

// draws the snake on the board
void draw_snake(Snake* snake){
  while(snake){
    mvprintw(snake->y, snake->x, "%c", snake->symbol);
    snake = snake->next;
  }
}

// checks if it eats itself, if it does, then return true
bool eat_itself(Snake* snake,int x, int y){
    // TODO for Milestone 2 only
    Snake *t= snake->next;
    while(t){
	if(t->y==y && t->x==x)
	   return true;
	t=t->next;
	}
	return false;
}
bool hit(Snake* snake, Snake* e){
	Snake* t= e;
	while(t){
	 if(t->x==snake->x && t->y ==snake->y)
		return true;
	 t=t->next;

	}
	return false;
}
