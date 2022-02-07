/*food.c -------
*
* Filename: food.c
* Description:
* Author: Manisha Suresh Kumar
* Maintainer:
* Created: Sat Sep 12 13:16:12 2020
* Last-Updated: September 12 16:51 2020
*	  By: Manisha Suresh Kumar
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

/*Copyright (c) 2016 The Trustees of Indiana University and
* Indiana University Research and Technology Corporation.
*
* All rights reserved.
*
* Additional copyrights may follow
*/

#include <stdlib.h>
#include <ncurses.h>
#include <stdbool.h>
#include "food.h"


//Create new food
Food* create_food(int x, int y, enum Type type){
    Food* new_food = malloc(sizeof(Food));
     
    new_food->x = x;
    new_food->y = y;
    if (type == Increase){
	int i= rand();
	if(i> RAND_MAX/2){new_food->type= 'O';}
	else{new_food->type='+';}
//        new_food->type = 'O';
    }
    else if(type == Decrease){
       // new_food->type = 'X';
	int i= rand();
	if(i> RAND_MAX/2){new_food->type='X';}
	else{new_food->type='-';}
    }else{
	new_food->type= '!';
	}
	//new_food->next=malloc(sizeof(Food));
	new_food->next=NULL;
    return new_food;
}

//Check if food exists at coordinates
bool food_exists(Food* foods, int x, int y){
    Food* temp = foods;
    while(temp){
        if(temp->x == x && temp->y == y)
            return true;
        temp = temp->next;
    }
    return false;
}

//Add new food to end of food list
void add_new_food(Food* foods, Food* new_food){
    Food* temp = foods;
    while(temp->next) {
        temp = temp->next;
    }
    temp->next = new_food;
}


enum Type food_type(Food* foods, int x, int y){
    //Implement the code to return the type of the food 
    //present at position (x, y)	
    Food* f= foods;
    while(f){
     if(f->y ==y && f->x==x){
	return f->type;	
	}
	f=f->next;
	}
	
}
Food* remove_eaten_food(Food* foods, int x, int y){
	//Implement the code to remove food at position (x,y).
	//Create a new linked list of type Food containing only the
	//needed food and return this new list
	Food* new= foods;
	if(new->y==y && new->x==x){return new->next;}
	while(new->next){
//Here just addding everything that isn't the food at the location x,y
	 if(new->next->y ==y && new->next->x == x){
	  new->next=new->next->next;
	  break;
	}else{
	 new=new->next;
	}
}
	return foods;
}
// Display all the food
void draw_food (Food *foods)
{   Food* temp = foods;
   int count=0;
    while(temp) {
	
//	mvprintw(50,80,"Success : Count= %d",count);
        mvprintw(temp->y, temp->x, "%c", temp->type);
        temp = temp->next;
	count++;
    }
//	mvprintw(40,20,"Count: %d",count);
}
