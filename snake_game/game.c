#include <ncurses.h>
#include<string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "snake.h"
#include "food.h"
//#include<SDL2/SDL.h>
//#include<SDL2_Mixer/SDL_mixer.h>
#include "game_window.h"
#include "key.h"
#include "game.h"
#include <curses.h>
//#include "inc/fmod.h"
//FSOUND_SAMPLE* handle;
void generate_points(int *food_x, int *food_y, int width, int height, int x_offset, int y_offset){
    *food_x = rand() % width + x_offset;
    *food_y = rand() % height + y_offset;
}
int game( ){
    enum State state = WELCOME; // Set the initial state
  //  FSOUND_INIT(44100,32,0);
   //handle=FSOUNDS_Sample_Load(0, "my.mp3",0,0,0);
    //FSOUND_PlaySound(0,handle);
/*    Mix_Music *music;
    if(SDL_INIT(SDL_INIT_AUDIO)< 0){
	printf("fail");
	}
    if(Mix_Init(MIX_INIT_MP#) != MIX_INIT_MP3)
	printf("fail");
    if(Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2 ,1024) !0)
	printf("fail");
    Mix_AllocateChannels(1);
    music=Mix_LoadMUS("./mp3/my.mp3");
    if(music){
	Mix_PlayMusic(music,-1);
	}*/
    static int x_max, y_max; //Max screen size variables
    static int x_offset, y_offset; // distance between the top left corner of your screen and the start of the board
    gamewindow_t *window; // Name of the board
    Snake *snake, *enemy, *enemy2; // The snake
    Food *foods,*new_food; // List of foods (Not an array)
    Food *obstacles, *nobs;
    int foodconsumed;
    int multi=0;
    const int height = 30; 
    const int width = 70;
    char ch;
    int al=1;
    int justgrew=0;
    int justinit;
    int score=0;
    int nHeight=height;
    int nWidth=width;
    int numdecreases=0;
    int numincreases=0;
    struct timespec timeret;
    timeret.tv_sec = 0;
    timeret.tv_nsec = 999999999/4;
    int colcount=0;
    int collision=0;
    char lastMove=RIGHT;
    FILE *sPtr;
    int saveLoc=0;
    char lm1=RIGHT;
    char lm2=RIGHT;
    char cfPtr[30];
    int lives=3;
    int diff=1;
    int loadfile;
    int stateDec=0;
    char lastMoveE=DOWN;
    int cycleCount=0;
//File stuff here
/*
 *
 * To Do:
 * Game over screen -- problems with sleep function in general
 * Pause Menu with Moveable cursor??
 *
 */
    
    while(state != EXIT){
	
        switch(state){
	case WELCOME:
	 initscr();
	 /*start_color();
	 nodelay(stdscr,TRUE);
	 noecho();
	 getmaxyx(stdscr,y_max,x_max);
	 keypad(stdscr, TRUE);
	 cur_set(0);
	 timeout(100);*/
	 mvprintw(10,20, "WELCOME TO SLITHER");
	 mvprintw(12,20, "Control the Snake using the Arrow Keys");
	 mvprintw(14,20, "Eat Green Food to Increase/Decrease your size");
	 mvprintw(16,20, "Yellow !'s are Obstacles, avoid them when you can");
	 mvprintw(18,20, "Watch Out! The Purple and Red Snakes are enemies, don't let them touch you!");
	 mvprintw(25,20, "Press e for Easy");
	 mvprintw(28,20, "Press h for Hard");
	 mvprintw(31,20, "Press a for Advanced");
	 mvprintw(34,20, "Press l to Display Saved Games and Load one of them");
	 mvprintw(37,20, "Press q to Quit\n");
	 refresh();
	 char a;
	 a= get_char();
	 switch(a){
	 case 'e':
		diff=1;
		state=INIT;
		break;
	 case 'h':
		diff=2;
		timeret.tv_nsec/=1.3;
		state=INIT;
		break;
	 case 'a':
		diff=3;
		timeret.tv_nsec/=1.3;
		timeret.tv_nsec/=1.3;
		state=INIT;
		break;
	 case 'l':
		loadfile=1;
		state=INIT;
		break;		
	 case 'q':
		state=EXIT;
		break;
	default:
		clear();
		mvprintw(20,20, "Invalid Input");
		refresh();
		sleep(2);
		state=WELCOME;
		break;
	}
	if(!loadfile && a!='q' && a!='l'){
	 clear();
	 mvprintw(20,20,"Would you like to play single or multiplayer? (s/m)");
	 refresh();
	 char b;
	 b=get_char();
	 if(b=='m'){
		multi=1;
		clear();
		mvprintw(10,10,"MULTIPLAYER MODE ACTIVATED");
		mvprintw(12,10,"Player 1 will control the Blue Snake using the arrow keys");
		mvprintw(14,10,"Player 2 will control the Purple Snake using the w/a/s/d");
		mvprintw(18,10,"Purple Player : Your job is to disrupt! Keep Player 1 from scoring at all costs! Don't forget, you have an ally!");
		mvprintw(25,20,"Good Luck! Game will start in 8 seconds");
		refresh();
		sleep(8);
	}
		
	}
	 break;
        case INIT:
            initscr();
            start_color();
            nodelay(stdscr, TRUE); //Dont wait for char
            noecho(); // Don't echo input chars
            getmaxyx(stdscr, y_max, x_max);
            keypad(stdscr, TRUE); // making keys work
            curs_set(0); // hide cursor
            timeout(100);
	    //sleep(1);
	    justinit=1;
            // Setting height and width of the board
            x_offset = (x_max / 2) - (width / 2);
            y_offset = (y_max / 2) - (height / 2);
            
            //Init board
            window = init_GameWindow(x_offset, y_offset, width, height);
            draw_Gamewindow(window);
	if(!loadfile){
            // Init snake
            snake = init_snake(x_offset + (width / 2), y_offset + (height / 2));
           enemy=  init_snake(x_offset + (width/ 4), y_offset + (height/4)); 
           enemy2= init_snake(x_offset  + (width/8), y_offset +(height/8));
            // Init foods
            int food_x, food_y, i;
            enum Type type;
	  //  if(!loadfile){
            //Generate 10 foods
            generate_points(&food_x, &food_y, width-1, height-1, x_offset+1, y_offset+1);
            type = (rand() > RAND_MAX/2) ? Increase : Decrease; // Randomly deciding type of food
            foods = create_food(food_x, food_y, type);
            for(i = 0; i < 5*diff; i++){
                generate_points(&food_x, &food_y, width-1, height-1, x_offset+1, y_offset+1);
                while (food_exists(foods,food_x, food_y))
                    generate_points(&food_x, &food_y, width-1, height-1, x_offset+1, y_offset+1);
                type = (rand() > RAND_MAX/2) ? Increase : Decrease;
		
                new_food = create_food(food_x, food_y, type);
                add_new_food(foods, new_food);
            }
	// Generate Obstacles

	    generate_points(&food_x,&food_y,width-1,height-1,x_offset+1,y_offset+1);
	    type=Obstacle;
	    obstacles=create_food(food_x,food_y,type);
	    for(i=0;i<5*diff;i++){
              generate_points(&food_x,&food_y,width-1,height-1,x_offset+1,y_offset+1);
		while(food_exists(obstacles,food_x,food_y))
			generate_points(&food_x,&food_y,width-1,height-1,x_offset+1,y_offset+1);
		int q;
		q= rand() %2;
		if(q==0){
		nobs=create_food(food_x,food_y,type);
		add_new_food(obstacles,nobs);
		}else if(q==1){
		while(food_exists(obstacles,food_x,food_y) || food_x <= x_offset+1 || food_y<=y_offset+1 ||food_x>=width-1 ||food_y>=height-1)
		 generate_points(&food_x,&food_y,width-2,height-2,x_offset+2,y_offset+2);
		nobs=create_food(food_x+1,food_y+1,type);
		add_new_food(obstacles,nobs);
		nobs=create_food(food_x,food_y+1,type);
		add_new_food(obstacles,nobs);
		nobs=create_food(food_x,food_y-1,type);
		add_new_food(obstacles,nobs);
		nobs=create_food(food_x+1,food_y,type);			
		add_new_food(obstacles,nobs); 
		nobs=create_food(food_x-1,food_y,type);
		add_new_food(obstacles,nobs);
 		nobs=create_food(food_x-1,food_y-1,type);
		add_new_food(obstacles,nobs);
		nobs=create_food(food_x+1,food_y-1,type);
		add_new_food(obstacles,nobs);
		nobs=create_food(food_x-1,food_y+1,type);
		add_new_food(obstacles,nobs);

}

	}
	stateDec=0;	
}else{
	//Display list of 5 files to load from, choose which one then load it.
	clear();
	char list[5][35];	
	int i=0;
	int spec;
	FILE* flPtr;
	if((flPtr=fopen("./saves/listofsaves.txt","r"))==NULL)
		printf("Failed to open file");
	while(!feof(flPtr)){
		fscanf(flPtr,"%s",&list[i]);
		i++;
		}
//	initscr();
//	cbreak();
//	noecho();
//	keypad(stdscr,TRUE);
	fclose(flPtr);
	mvprintw(10,20,"File Options:");
	mvprintw(12,20,"%s",list[0]);
	mvprintw(14,20,"%s",list[1]);
	mvprintw(16,20,"%s",list[2]);
	mvprintw(18,20,"%s",list[3]);
	mvprintw(20,20,"%s",list[4]);
	mvprintw(22,20,"Type in the File you want to load (Your selection will appear after presssing Enter)\n");
	refresh();
	char *choice= malloc(30);
	scanf(" %s",choice);
//	fgets(choice,20,stdin);
	mvprintw(25,20,"%s",choice);
	refresh();
	sleep(3);
	fflush(stdin);
	int valid=0;
	for(i=0;i<5;i++){
	 if(strcmp(list[i],choice)==0){
		valid=1;
		spec=i;}
	}
	if(!valid){
	clear();
	mvprintw(10,20,"Try Again");
	mvprintw(12,20,"%s",list[0]);
	mvprintw(14,20,"%s",list[1]);
	mvprintw(16,20,"%s",list[2]);
	mvprintw(18,20,"%s",list[3]);
	mvprintw(20,20,"%s\n",list[4]);
	refresh();
	char za =get_char();
	scanf(" %s",choice);
	for(i=0;i<5;i++){
		if(strcmp(list[i],choice)==0){
			valid=1;
			spec=i;}
	}
	}if(!valid){
	clear();
	 loadfile=0;
	  stateDec=1;
	 state=INIT;
	 
	}
	
	
	if(valid){
	FILE *mPtr;
	char empty;
	if((mPtr=fopen(list[spec],"r"))==NULL){
		clear();
		mvprintw(10,20,"Failed to open file : %s",list[spec]);
		refresh();
		sleep(3);
}	else{
	clear();
	mvprintw(10,20,"File found, loading game");
	refresh();
	fscanf(mPtr,"%d",&diff);
	if(diff==2)
	 timeret.tv_nsec/=1.3;
	if(diff==3){
	  timeret.tv_nsec/=1.3;
	  timeret.tv_nsec/=1.3;
	}
	fscanf(mPtr,"%c",&empty);
	fscanf(mPtr,"%d",&score);
	fscanf(mPtr,"%c",&empty);
	fscanf(mPtr,"%d",&lives);
	fscanf(mPtr,"%c",&empty);
	//clear();
	//mvprintw(10,20,"Empty: %d",empty);
//	refresh();
//	sleep(3);
	char *buf = malloc(4);
	int fx;
	int fy;
	char *word=malloc(9);
	enum Type ft;
	char skip[5]="skip";
	char nu[1]="\n";
	int mc=0;
	int mod=0;
	int firstFood=1;
	
	strcpy(buf,skip);
	while(strlen(buf)>1  || atoi(buf)){
	 fscanf(mPtr," %s",buf);
	// clear();
//	 mvprintw(10,20,ft==Increase?"Increase":"Decrease");
//	 refresh();
//	 sleep(1);
	if(strlen(buf)>1|| atoi(buf)){
	if(isdigit(*buf)){
	  if(mod%2==0){
		fx=atoi(buf);
		mod++;
	  }else if(mod%2==1){
		fy=atoi(buf); 
		mod++;}
	 }else{
	   if(*buf=='I')
		ft=Increase;
	   else
		ft=Decrease;
	   if(firstFood){
	     foods= create_food(fx,fy,ft);
		firstFood=0;
	}else{
		new_food=create_food(fx,fy,ft);
		add_new_food(foods,new_food);
		}  
	}
	 } 
	}
	
	firstFood=1;
	mod=0;
	strcpy(buf,skip);
	while(strlen(buf)>1 ||atoi(buf)){
	 fscanf(mPtr," %s",buf);
	// clear();
	// mvprintw(10,20,"%s",buf);
	 //refresh();
	 //sleep(1);
	 if(strlen(buf)>1 ||atoi(buf)){
	 if(isdigit(*buf)){
	  if(mod%2==0){
	   fx=atoi(buf);
	   mod++;
	}else if(mod %2==1){
	   fy=atoi(buf);
	   mod++;
	}
	}else{
	  ft=Obstacle;
	  if(firstFood){
		obstacles=create_food(fx,fy,ft);
	        firstFood=0;
	}else{
		nobs=create_food(fx,fy,ft);
		add_new_food(obstacles,nobs);
	}

	}
	}

	}
	mod=0;
	int firstSnake=1;
	strcpy(buf,skip);
	Snake* tmp;
	while(strlen(buf)>1 || atoi(buf)){
	 fscanf(mPtr," %s",buf);
	 if(strlen(buf)>1 || atoi(buf)){
	  if(isdigit(*buf)){
	   if(mod%2==0){
		fx=atoi(buf);
		mod++;

	}else{
	   	fy=atoi(buf);
		mod++;
		if(firstSnake){
		 snake= create_tail(fx,fy);
	//	 next=snake;
		 firstSnake=0;

		}else{
		tmp=create_tail(fx,fy);
		grow_tail(snake,tmp);
		}
	}


	}
	}
	}
	mod=0;
	firstSnake=1;
	strcpy(buf,skip);
	while(strlen(buf)>1 ||atoi(buf)){
	 fscanf(mPtr," %s",buf);
	 if(strlen(buf)>1 || atoi(buf)){
	  if(isdigit(*buf)){
	   if(mod%2==0){
		fx=atoi(buf);
		mod++;
	}else{
		fy=atoi(buf);
		mod++;
		if(firstSnake){
		 enemy=create_tail(fx,fy);
		 firstSnake=0;
		}else{
		  tmp=create_tail(fx,fy);
		   grow_tail(enemy,tmp);
		}
	}
		}

	}

	}
	mod=0;
	firstSnake=1;
	strcpy(buf,skip);
	while(strlen(buf)>1 || atoi(buf)){
	  fscanf(mPtr," %s",buf);
          if(strlen(buf)>1 || atoi(buf)){
	   if(isdigit(*buf)){
	    if(mod%2==0){
		fx=atoi(buf);
		mod++;
	    }else{
		fy=atoi(buf);
	        mod++;
		if(firstSnake){
		  enemy2=create_tail(fx,fy);
		  firstSnake=0;
		}else{
		  tmp=create_tail(fx,fy);
		  grow_tail(enemy2,tmp);
		}

		}
	}
	
	}

	}
	loadfile=0;	
	}
	}
}	
	

	
	if(!stateDec)
            state = ALIVE;
          break;

        case ALIVE:
//            mvprintw(40,20,"FoodConsumed: %d",foodconsumed);
	   // cycleCount++;
	    //if(cycleCount>=10000)
	//	cycleCount=0;	      
            ch = get_char();
            
            /* Write your code here */
	if(multi){
	    if(ch=='a'){
		enemy2=move_snake(enemy2,LEFT);
		lastMoveE=LEFT;
			
	  } else if(ch=='s'){
		enemy2=move_snake(enemy2,DOWN);
		lastMoveE=DOWN;
	    }else if(ch=='d'){
		enemy2=move_snake(enemy2,RIGHT);
		lastMoveE=RIGHT;
	   } else if(ch=='w'){
		enemy2=move_snake(enemy2,UP);
		lastMoveE=UP;
	} else{
		enemy2=move_snake(enemy2,lastMoveE);
	}
}
	    if(ch==LEFT || ch==RIGHT || ch==DOWN || ch==UP){
	     
 	     if((lastMove==RIGHT && ch==LEFT)|| (lastMove==LEFT && ch==RIGHT) || (lastMove==UP && ch== DOWN) || (lastMove==DOWN && ch==UP)){
		snake=move_snake(snake,lastMove);
		break;
		}
		snake=move_snake(snake,ch);
		lastMove=ch;
	    }else{
		snake=move_snake(snake,lastMove);
		
		}
	    //Here deal with moving outside the barrier
	    if(snake->x == x_offset){
		snake->x= x_offset+nWidth-1;

	}else if(snake->y == y_offset){
		snake->y= y_offset+nHeight-1;
	}else if(snake->x== x_offset+nWidth){
		snake->x= x_offset +1;
	}else if(snake->y== y_offset +nHeight){
		snake->y= y_offset+1;
	}

	   // lastMove=ch;
            if(ch=='q' || ch== 'Q'){
		state=EXIT;
	   	}

//Using the p character (pause) to toggle betweeen dead and alive states
	   if(ch=='P' || ch=='p'){
		state=DEAD;
	}
	   // int nHeight=height;
	   //  int nWidth=width;
	   //  If the + is called, increases the height and width and decrease the offsets so the board expands towards the edge of the screen and vice versa

 	    if(ch=='+'){
		if(numincreases<5){
		x_offset-=5;
		y_offset-=5;
		nHeight+=5;
		nWidth+=5;
		window=changeGameWindow(x_offset,y_offset,nWidth,nHeight,window);
		numincreases++;}
	  }if(ch=='-'){
	    if(numdecreases<5){
		x_offset+=5;
		y_offset+=5;
		nWidth-=5;
		nHeight-=5;
		window=changeGameWindow(x_offset,y_offset,nWidth,nHeight,window);
		numdecreases++;
		Food* helper= foods;
		//Here check if any food is outside the current window, then get rid of it and redraw foods in the window
		
		while(helper->next){
		 if(helper->x <= x_offset || helper->y <=y_offset || helper->x >= x_offset +nWidth || snake->y >= y_offset +nHeight){
			foods = remove_eaten_food(foods,helper->x,helper->y);
		
	  	int ranx,rany;
         	 generate_points(&ranx,&rany,nWidth,nHeight,x_offset,y_offset);
	         while(food_exists(foods,ranx,rany))
		   generate_points(&ranx,&rany,nWidth,nHeight,x_offset,y_offset);	 		
	//	 helper=helper->next;
		
		enum Type iOrD= (rand()> RAND_MAX/2)?Increase:Decrease;
		Food* f= create_food(ranx,rany,iOrD);
		add_new_food(foods,f);
		}helper=helper->next;
	 	}
		}
	}
	int rm1, rm2;
	rm1=rand()%4;
	rm2= rand()%4;
	int o=0;
//Checking here if the snake needs to bounce back from the walls and doing that if it does	
	if(enemy->y <= y_offset +5){
		enemy=move_snake(enemy,DOWN);
		lm1=DOWN;
		o=1;
	}else if(enemy->y >= y_offset+height -5){
		enemy=move_snake(enemy,UP);
		lm1=UP;
		o=1;
	}else if(enemy->x <=x_offset+5){
		enemy=move_snake(enemy,RIGHT);
		lm1=RIGHT;
		o=1;
	}else if(enemy->x >=x_offset+width -5){
		enemy=move_snake(enemy,LEFT);
		lm1=LEFT;
		o=1;
	}
	if(!o){
	  if(rm1==0){
		if(lm1==UP)
			enemy=move_snake(enemy,UP);
		else{
			enemy=move_snake(enemy,DOWN);
			lm1=DOWN;
		}
//			lm1=DOWN;}
//		else{ enemy=move_snake(enemy,lm1);
	  }if(rm1==1){
		if(lm1==LEFT)
			enemy=move_snake(enemy,LEFT);
		else{
			enemy=move_snake(enemy,RIGHT);
			lm1=RIGHT;
		}

	 } if(rm1==2){
		if(lm1==DOWN)
			enemy=move_snake(enemy,DOWN);
		else{
			enemy=move_snake(enemy,UP);
			lm1=UP;
		}
	  }if(rm1==3){
		if(lm1==RIGHT)
			enemy=move_snake(enemy,RIGHT);
		else{
			enemy=move_snake(enemy,LEFT);
			lm1=LEFT;
		}
	}
	}
	//rm2=0;
	if(!multi){
	int ob=0;
	if(enemy2->y <= y_offset+5 ){
		enemy2=move_snake(enemy2,DOWN);
		lm2=DOWN;
		ob=1;
	}else if(enemy2->y>=y_offset+height-5 ){
		enemy2=move_snake(enemy2,UP);
		lm2=UP;
		ob=1;
	}else if(enemy2->x<=x_offset+5 ){
		enemy2=move_snake(enemy2,RIGHT);
		lm2=RIGHT;
		ob=1;
	}else if(enemy2->x>=x_offset+width-5 ){
		enemy2=move_snake(enemy2,LEFT);
		lm2=LEFT;
		ob=1;
	}
	if(!ob){
	   if(rm2==0)
		if(lm2==UP)
			enemy2=move_snake(enemy2,UP);
		else{
			enemy2=move_snake(enemy2,DOWN);
			lm2=DOWN;
		}
	   if(rm2==1)
		if(lm2==LEFT)
			enemy2=move_snake(enemy2,LEFT);
		else{
			enemy2=move_snake(enemy2,RIGHT);
			lm2=RIGHT;
		}
	   if(rm2==2)
		if(lm2==DOWN)
			enemy2=move_snake(enemy2,DOWN);
		else{
			enemy2=move_snake(enemy2,UP);
			lm2=UP;
		}
	   if(rm2==3)
		if(lm2==RIGHT)
			enemy2=move_snake(enemy2,RIGHT);
		else{
			enemy2=move_snake(enemy2,LEFT);
			lm2=LEFT;
		}
		

	}
}
	Snake* es= snake;
	//Determine where the end of the snake is so I can add onto the end when I consume an increase food
	if(es->next){
	 while(es->next->next){
		es=es->next;
	}
	}
	 if(food_exists(foods,snake->x,snake->y)){
	  foodconsumed++;
	  enum Type foodType= food_type(foods, snake->x, snake->y);
          //Creating a random point x,y for a new food to spawn at, with a random type
	  int ranx,rany;
          generate_points(&ranx,&rany,nWidth-1,nHeight-1,x_offset+1,y_offset+1);
	  while(food_exists(foods,ranx,rany)){
	    generate_points(&ranx,&rany,nWidth-1,nHeight-1,x_offset+1,y_offset+1);	
	}
	  enum Type iOrD= (rand()> RAND_MAX/2)?Increase:Decrease;
	  Food* newFood= create_food(ranx,rany,iOrD);

	//If there is a food at the location, either add to the snake or delete the tail of the snake
	 
         //   	foodconsumed+=1;
	        foods=remove_eaten_food(foods,snake->x,snake->y);
	//	if(foodType=='X' 
	  	if( foodType== 'O' ||foodType== '+'){
		  
            	  Snake* newt= create_tail(es->x,es->y);
	    	  grow_tail(snake,newt);
 	    	  score+=20;
		  justgrew=1;
		}else if(foodType=='X' || foodType=='-'){
		  if(snake->next){
	   	  snake=remove_tail(snake);
	   	  score-=10;
		  }else{
		  if(lives>1){
		  lives--;
		  state=INIT;
	 	}else{
		 lives--;
		 clear();
		 game_over(score,lives);
		 sleep(5);
		 state=EXIT;
		 endwin();
		}	
		}
		}
	  add_new_food(foods,newFood);
  	  } 
	//Here if an obstacle exists where snake is 
	bool eatself=eat_itself(snake,snake->x,snake->y);
	
	if(food_exists(obstacles,snake->x,snake->y)){
	 if(lastMove==RIGHT){
		 lastMove=LEFT;
		 snake=move_snake(snake,LEFT);
	}
        else if(lastMove==LEFT){
		lastMove=RIGHT;
		snake=move_snake(snake,RIGHT);
	}
	 else if(lastMove==UP){
		lastMove=DOWN;
		snake=move_snake(snake,DOWN);
	}
	 else if(lastMove==DOWN){
		lastMove=UP;
		snake=move_snake(snake,UP);
	}
	 eatself==false;
	}
	if(justgrew){
	 eatself=false;
	 justgrew=0;
	}
	if(justinit){
	 eatself=false;
	 justinit=0;
	}
	if(eatself){
	snake=remove_tail(snake);

	}
	if(hit(snake,enemy) || hit(snake,enemy2) || hit(enemy,snake) ||hit(enemy2,snake)){
	  if(lives>1){
	    lives--;
	    state=INIT;
	}else{
	 lives--;
	 clear();
	 game_over(score,lives);
	 sleep(5);
	 state=EXIT;
	 endwin();
	}

	}
	Snake* ees1=enemy;
	if(ees1->next){
	while(ees1->next->next){
	 ees1=ees1->next;
	}
	}
	if(food_exists(foods,enemy->x,enemy->y)){
	 enum Type et1= food_type(foods,enemy->x,enemy->y);
	 foods= remove_eaten_food(foods,enemy->x,enemy->y);
	  if(et1=='O' || et1=='+'){
		Snake* newe1= create_tail(ees1->x, ees1->y);
		grow_tail(enemy,newe1);
	  }else if(et1=='X'||et1=='-'){
		if(enemy->next){
		 enemy=remove_tail(enemy);
	
		}

	}
	 
	}
	Snake* ees2=enemy2;
	if(ees2->next){
	 while(ees2->next->next)
		ees2=ees2->next;
	}
	if(food_exists(foods,enemy2->x,enemy2->y)){
	  enum Type et2= food_type(foods,enemy2->x,enemy2->y);
	  foods=remove_eaten_food(foods,enemy2->x,enemy2->y);
	  if(et2=='O' || et2 == '+'){
		Snake* newe2=create_tail(ees2->x,ees2->y);
		grow_tail(enemy2,newe2);
	}else if(et2=='X' || et2 =='-'){
	 	if(enemy2->next){
		 enemy2=remove_tail(enemy2);
		}

	}


	}

	//Comment

	// Draw everything on the screen
            clear();
	    init_pair(1,COLOR_WHITE,COLOR_BLACK);
	    attron(COLOR_PAIR(1));
	//Print the score in an abitrary spot in the top left corner
            mvprintw(20,20, "Key entered: %c", ch);
	    mvprintw(10,20, "Score: %d", score);
	    mvprintw(15,20, "Lives Left: %d",lives);
            draw_Gamewindow(window);
	   // attron(COLOR_PAIR(2));
	    init_pair(2,COLOR_BLUE,COLOR_BLACK);
	    attron(COLOR_PAIR(2));
            draw_snake(snake);
	    init_pair(3,COLOR_RED,COLOR_BLACK);
	    attron(COLOR_PAIR(3));
	    draw_snake(enemy);
	    init_pair(4,COLOR_MAGENTA,COLOR_BLACK);
	    attron(COLOR_PAIR(4));
	    draw_snake(enemy2);
	    init_pair(5,COLOR_GREEN,COLOR_BLACK);
	    attron(COLOR_PAIR(5));
            draw_food(foods);
	    init_pair(6,COLOR_YELLOW,COLOR_BLACK);
	    attron(COLOR_PAIR(6));
	    draw_food(obstacles);
	    attroff(COLOR_PAIR(1));
	    attroff(COLOR_PAIR(2));
	    attroff(COLOR_PAIR(3));
	    attroff(COLOR_PAIR(4));
	    attroff(COLOR_PAIR(5));
            attroff(COLOR_PAIR(6));
		break;

        case DEAD:
	clear();
	if(al==0){
	// mvprintw(50,50,"Final Score: %d",score);
	
	 state=EXIT;
	 endwin();
	}else{
	//Give users options to either continue playing or quit game completely
	clear();	
	mvprintw(5,30,"Game paused");
	mvprintw(8,30,"Resume snake game");
	mvprintw(11,30,"Save current state of game to a file");
	mvprintw(14,30,"Quit game completely");
	mvprintw(20,30, "Use Arrow Keys to Navigate Menu, Press Enter to Select");
	refresh();
	char ls;
	char options[][35] = {"Current Option: Resume Snake Game","Current Option: Save Game", "Current Option: Quit Game"};
	int option = 0;
	while(1){
	clear();
mvprintw(5,30,"Game paused");
mvprintw(8,30,"Resume snake game");
mvprintw(11,30,"Save current state of game to a file");
mvprintw(14,30,"Quit game completely");
mvprintw(20,30, "Use Arrow Keys to Navigate Menu, Press Enter to Select"); 
mvprintw(25,30,"%s",options[option]);	
	 ls= get_char();
	 if(ls=='\n'){
	  break;	
	}
	 switch(ls){
	  case UP: 
		if(option==0){
		 option=2;
		}else{
		 option--;
		}
//		mvprintw(25,30,"%s",options[option]);

		break;
	case DOWN:
		if(option==2){
		 option=0;
		}else{
		 option++;
		}
//		mvprintw(25,30,"%s",options[option]);
		break;
	}
	if(option==0){
	 ch='p';
	}else if(option==1){
	 ch='f';
	}else{ ch='q';}
	refresh();
	}
	//ch= get_char();
	if(ch=='p'||ch=='P'){
	   state=ALIVE;
	}else if(ch=='q' || ch == 'Q'){
	/*  clear();
	  mvprintw(20,20, "Game Over");
	  mvprintw(25,20, "Final Score: %d",score);
	  mvprintw(30,20, "Lives Left: %d",lives);
	  refresh();
//	  ch= get_char();*/
	  game_over(score,lives);
	  sleep(5);
	//game_over(score);
	 //  sleep(10);
	   state=EXIT;
	   endwin();
	}else if(ch=='f' || ch=='F'){
	//Save to a file
	clear();

	mvprintw(17,30,"Enter the file name (extenstion not included) to save to. After typing in the name, press Enter");
	mvprintw(20,30, "After typing in the file name and pressing enter, you should see the file name you chose below");
	refresh();
	//char finalname[40];
	char *fname=malloc(15);
	scanf(" %s", fname);
	mvprintw(23,30,"File Name: ./saves/%s.game",fname);
	refresh();
	sleep(3);
	char start[30]= "./saves/";
	strcat(start,fname);
	char ext[6]= ".game";
	strcat(start, ext);
	if((sPtr= fopen(start,"w"))==NULL)
	 printf("Failure to open file");
	FILE *lqPtr;
	if((lqPtr=fopen("./saves/listofsaves.txt","r"))==NULL)
	  printf("fail");
	char ls[5][35];
	int r=0;
	int no=0;
	while(!feof(lqPtr)){
	  fscanf(lqPtr,"%s",ls[r]);
	  r++;
	}
	fclose(lqPtr);
	for(r=0;r<5;r++){
	  if(strcmp(start,ls[r])==0){
		no=1;		
		}

	}
	if(!no){
	  strcpy(ls[4],ls[3]);
	  strcpy(ls[3],ls[2]);
	  strcpy(ls[2],ls[1]);
	  strcpy(ls[1],ls[0]);
	  strcpy(ls[0],start); 

	}
	if((lqPtr=fopen("./saves/listofsaves.txt","w"))==NULL)
		printf("Fail");
	for(r=0;r<5;r++)
		fprintf(lqPtr,"%s\n",ls[r]);
	fclose(lqPtr);
	fprintf(sPtr,"%d\n",diff);
	fprintf(sPtr,"\n");
	fprintf(sPtr,"%d\n",score);
	fprintf(sPtr,"\n");
	fprintf(sPtr,"%d\n",lives);
	fprintf(sPtr,"\n");
	Food* temp=foods;
	
	while(temp){
	 fprintf(sPtr,"%d\n",temp->x);
	 fprintf(sPtr,"%d\n",temp->y);
	 fprintf(sPtr,"%s\n",(temp->type=='+' || temp->type == 'O')?"Increase":"Decrease");
	 temp=temp->next;
	}
	
	fprintf(sPtr,"c\n");
	temp=obstacles;
	while(temp){
	 fprintf(sPtr,"%d\n",temp->x);
	 fprintf(sPtr,"%d\n",temp->y);
	 fprintf(sPtr,"%s\n",(temp->type=='+' || temp->type == 'O')?"Increase":"Decrease");
	 temp=temp->next;
	}
	fprintf(sPtr,"c\n");
	Snake *ts=snake;
	while(ts){
	 fprintf(sPtr,"%d\n",ts->x);
	 fprintf(sPtr,"%d\n",ts->y);
	 ts=ts->next;
	}
	fprintf(sPtr,"c\n");
	ts=enemy;
	while(ts){
	 fprintf(sPtr,"%d\n",ts->x);
	 fprintf(sPtr,"%d\n",ts->y);
	 ts=ts->next;
	}
	fprintf(sPtr,"c\n");
	ts=enemy2;
	while(ts){
	 fprintf(sPtr,"%d\n",ts->x);
	 fprintf(sPtr,"%d\n",ts->y);
	 ts=ts->next;
	}
	fprintf(sPtr,"c\n");
	//Print message here that file has been saved
	//Ask if they want to resume playing or quit
	//
	clear();
	mvprintw(20,30,"File Saved! Game Resumes in 5 Seconds");
	refresh();
	sleep(5);
	state=ALIVE;

 }}
        //    endwin();
            break;
        }
        refresh();
       //if the food consumed is at an interval of 10, increase the speed here
        if(foodconsumed!=0 && foodconsumed%10==0){
	foodconsumed=0;
	timeret.tv_nsec/=1.45;}
//	mvprintw(50,50,"Speed increased");}
	nanosleep(&timeret, NULL);
    }
clear();
//game_over(score);
//sleep(10);
//FSOUND_Sample_Free(handle);
//FSOUND_Close();
/*Mix_HaltMusic();
Mix_FreeMusic(music);
Mix_CloseAudio();
Mix_Quit();*/
endwin();
//fclose(sPtr);
return score;
}
void game_over(int score, int lives){
   
   clear();
   mvprintw(10,50,"GAME OVER");
   mvprintw(20,50, "Final Score: %d", score);
   mvprintw(30,50, "Lives Left: %d", lives);
   refresh();

}
