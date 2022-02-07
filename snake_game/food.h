/*food.h -------
*
* Filename: food.h
* Description:
* Author: Manisha Suresh Kumar
* Maintainer:
* Created: Sat Sep 12 13:21:55 2020
* Last-Updated: September 12 16:01 2020
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

#include <ncurses.h>

//Two types of food
//Either Increase or Decrease length of snake
enum Type {Increase, Decrease,Obstacle}; 

//Structure to hold properties of food
struct Food {
  int x;
  int y;
  char type;
  struct Food* next;
};

typedef struct Food Food; 

//Function prototypes 
void add_new_food(Food* foods, Food* new_food);
bool food_exists(Food* foods, int x, int y);
Food* create_food(int x, int y, enum Type type);
enum Type food_type(Food* foods, int x, int y);
Food* remove_eaten_food(Food* foods, int x, int y);
void draw_food(Food *food);
