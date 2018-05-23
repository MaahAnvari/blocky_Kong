#include "Genio.h"
#include <math.h>

struct object{
    G_Rect pos;
    G_Texture* texture;
};

enum GameState{START,SHOWLEVEL,PAUSE,PLAY1,PLAY2,WIN1,WIN2,LOSE,EXITE};
enum ArchitectDirection{LEFT,RIGHT,NONE};
enum Lable{HELTH,STEP};

bool colision(object obj1,object obj2);
void inital(int level);
void load();
void start();
void play();
void lose();
void win();
void pause();
void showLevel();
void DrawLine(object b1);
void constantMove();
void showLable();
void destroy();

GameState game_state = START;
ArchitectDirection lastDirection = NONE;
ArchitectDirection newDirection = NONE;
Lable lable = HELTH;

//all of our object
object block1, block2, block3, architect, backGround,dot;
object btn_start, btn_exit, btn_pause, btn_level, btn_backToMenu;
object btn_level1, btn_level2;
object l_helth,l_step;
object pause_menu;
object architectRight,architectLeft;
object textImg;
object mouse;

//textures
G_Texture* bg_gameOver;
G_Texture* bg_win;
G_Texture* bg_game_L2;
G_Texture* bg_game_L1;

//music and sounds
G_Music* mix_game;
G_Sound* mix_lose,*mix_win,*mix_start;

//texts font
G_Font* myfont;

G_Rect winPosition;

int elevator_inc1,elevator_inc2,elevator_inc3;//show the speed of elevators
int step,helth;
int level[6]={0,650,650-145,650-2*145,650-3*145,650-4*145};
bool is_colicion;

int main( int argc, char* args[] )
{

	//initialize window position
	winPosition.x = winPosition.y = SDL_WINDOWPOS_UNDEFINED;
	winPosition.w = 600;
	winPosition.h = 800;

		G_InitSDL();
	//Start up SDL and create window & renderer
	G_CreatWindow("blocky kong",winPosition,0,0,0);

	//load all texture and sounds
	load();

    //initializa position of all things
	inital(1);


    //While application is running
    while( game_state != EXITE )
    {

        //Clear screen
        G_Update();

        switch(game_state)
        {
            case START:    start();        break;

            case SHOWLEVEL:showLevel();    break;

            case PLAY1:
            case PLAY2:{    play();
            }
            break;

            case LOSE:     lose();         break;

            case WIN1:
            case WIN2:     win();          break;

            case PAUSE:    pause();        break;
        }
	}
    destroy();
	return 0;
}


bool colision(object obj1,object obj2)
{
    if(((obj1.pos.x >= obj2.pos.x) && (obj1.pos.x-obj2.pos.x)<obj2.pos.w)||
    ((obj2.pos.x >= obj1.pos.x) && ((obj2.pos.x-obj1.pos.x)<obj1.pos.w)))
    {
        if(((obj1.pos.y >= obj2.pos.y) && (obj1.pos.y-obj2.pos.y)<obj2.pos.h)||
        ((obj2.pos.y >= obj1.pos.y) && ((obj2.pos.y-obj1.pos.y)<obj1.pos.h)))
            return true;
    }
    return false;
}



void inital(int level)
{


    //initialize blocks position (should use random but this IDE is very poor!!!)
	block1.pos.h = block2.pos.h = block3.pos.h = 100;
    block1.pos.w = block2.pos.w = block3.pos.w = 50;
    block1.pos.x = 100;
    block2.pos.x = 300-25;
    block3.pos.x = 500-50;
    block1.pos.y = rand()%400+100;
    block2.pos.y = rand()%400+100;
    block3.pos.y = rand()%400+100;

    //initialize Architecture attribute

    architect.texture = architectRight.texture;
    architect.pos.h = 60;
    architect.pos.w = 60;
    architect.pos.x = 0;
    architect.pos.y = 650;

    architectRight.pos.h = 65;
    architectRight.pos.w = 55;
    architectRight.pos.x = 0;
    architectRight.pos.y = 0;

    architectLeft.pos.h = 65;
    architectLeft.pos.w = 55;
    architectLeft.pos.x = 0;
    architectLeft.pos.y = 0;

    //initialize Dot attribute
       dot.pos.h = 2;
       dot.pos.w = 2;
       dot.pos.x = 0;
       dot.pos.y = 0;

    //initialize background attribute
    backGround.pos.x = backGround.pos.y = 0;
    backGround.pos.h = 800;
    backGround.pos.w = 600;

    btn_start.pos.x = 200;
    btn_start.pos.y = 100;
    btn_start.pos.w = 200;
    btn_start.pos.h = 100;

    btn_exit.pos.x = 200;
    btn_exit.pos.y = 500;
    btn_exit.pos.w = 200;
    btn_exit.pos.h = 100;

    btn_pause.pos.x = 0;
    btn_pause.pos.y = 0;
    btn_pause.pos.w = 150;
    btn_pause.pos.h = 80;

    btn_level.pos.x = 200;
    btn_level.pos.y = 300;
    btn_level.pos.w = 200;
    btn_level.pos.h = 100;

    btn_backToMenu.pos.x = 200;
    btn_backToMenu.pos.y = 100;
    btn_backToMenu.pos.w = 200;
    btn_backToMenu.pos.h = 100;

    btn_level1.pos.x = 200;
    btn_level1.pos.y = 100;
    btn_level1.pos.w = 200;
    btn_level1.pos.h = 200;
    btn_level1.texture = bg_game_L1;

    btn_level2.pos.x=200;
    btn_level2.pos.y=500;
    btn_level2.pos.w=200;
    btn_level2.pos.h=200;
    btn_level2.texture=bg_game_L2;

    pause_menu.pos.x=150;
    pause_menu.pos.y=200;
    pause_menu.pos.w=300;
    pause_menu.pos.h=300;

    elevator_inc1=rand()%5;
    elevator_inc3=rand()%5;
    elevator_inc2=rand()%5;

    if(level==1)
    {
    	lastDirection = newDirection = NONE;

        if(elevator_inc1%2 ==0){elevator_inc1=3;} else{elevator_inc1=-3;}
        if(elevator_inc2%2 ==0){elevator_inc2=3;} else{elevator_inc2=-3;}
        if(elevator_inc3%2 ==0){elevator_inc3=3;} else{elevator_inc3=-3;}

    }
    else if(level==2)
    {
    	lastDirection = newDirection = NONE;

    	if(elevator_inc1%2 ==0){elevator_inc1=5;} else{elevator_inc1=-5;}
    	if(elevator_inc2%2 ==0){elevator_inc2=5;} else{elevator_inc2=-5;}
    	if(elevator_inc3%2 ==0){elevator_inc3=5;} else{elevator_inc3=-5;}
    }

    step = 1;
    helth = 2;

    l_helth.pos.x = 570;
    l_helth.pos.y = 20;
    l_helth.pos.w = 20;
    l_helth.pos.h = 30;

    l_step.pos.x = 570;
    l_step.pos.y = 55;
    l_step.pos.w = 20;
    l_step.pos.h = 30;

    is_colicion=true;
}



void load()
{

    //load textures
    bg_gameOver = G_LoadImage("gameover.bmp");
    bg_game_L1 = G_LoadImage(" background1.png");
    bg_game_L2 = G_LoadImage(" background2.png");
    bg_win = G_LoadImage(" win.bmp");


    //load objects texture
    architectRight.texture = G_LoadImage(" right run.png",193,235,255);
    architectLeft.texture = G_LoadImage(" left run.png",193,235,255);
    btn_start.texture = G_LoadImage(" STARTMENU.png",0,0,0);
    btn_exit.texture = G_LoadImage(" quitMENU.png",0,0,0);
    btn_level.texture = G_LoadImage(" LEVELMENU.png",0,0,0);
    btn_backToMenu.texture = G_LoadImage(" BACKTOMENU.png",0,0,0);
    btn_pause.texture = G_LoadImage(" pause_menu.png",255,174,201);
    pause_menu.texture = G_LoadImage(" resume.png");

    block1.texture = G_LoadImage( " block.png" ,0,0,0);
    block2.texture = G_LoadImage( " block.png" ,0,0,0);
    block3.texture = G_LoadImage( " block.png" ,0,0,0);
    dot.texture = G_LoadImage( " pixel.bmp");

    mix_game = G_LoadMusic(" game.wav");
    mix_start = G_LoadSound(" start menu.wav");
    mix_lose = G_LoadSound(" lose.wav");
    mix_win = G_LoadSound(" win.wav");


    myfont=TTF_OpenFont("arial.ttf",20);
}


void play()
{

        //Handle events on queue
        object mouse;
        mouse.pos.x=GM_LEFT.X;
        mouse.pos.y=GM_LEFT.Y;
        mouse.pos.w=5;
        mouse.pos.h=5;

        if(GM_LEFT.down)
        {
            if(colision(mouse,btn_pause))
            {
                game_state=PAUSE;
                return;
            }
        }

        if(GK_LEFT.down){
        	newDirection = LEFT;
        }
        if(GK_RIGHT.down){
            newDirection = RIGHT;
        }

    //Render texture to screen
    G_Draw(backGround.texture,&backGround.pos);
    if(architect.pos.x == 0 )
    {
    	architectRight.pos.x = 0;
    	G_Draw(architectRight.texture,&architectRight.pos,&architect.pos,false);
    }
    else if(architect.pos.x == 540)
        {
        	architectLeft.pos.x = 165;
        	G_Draw(architectLeft.texture,&architectLeft.pos,&architect.pos,false);
        }
    else if(architect.texture == architectRight.texture){
    	G_Draw(architectRight.texture,&architectRight.pos,&architect.pos,false);
 	   architectRight.pos.x = 55*(G_GetTicks()%4);
    }
    else
    {
    	G_Draw(architectLeft.texture,&architectLeft.pos,&architect.pos,false);
 	   architectLeft.pos.x = 55*(G_GetTicks()%4);
    }
    G_Draw(block1.texture,&block1.pos,false);
    G_Draw(block2.texture,&block2.pos,false);
    G_Draw(block3.texture,&block3.pos,false);

    lable = HELTH;
    showLable();
    lable = STEP;
    showLable();

    elevator_inc1=(block1.pos.y>=730 || block1.pos.y<=40)?-elevator_inc1: elevator_inc1;
    block1.pos.y+=elevator_inc1;
    elevator_inc2=(block2.pos.y>=730 || block2.pos.y<=40)?-elevator_inc2: elevator_inc2;
    block2.pos.y+=elevator_inc2;
    elevator_inc3=(block3.pos.y>=730 || block3.pos.y<=40)?-elevator_inc3: elevator_inc3;
    block3.pos.y+=elevator_inc3;

    if(colision(block1,architect) || colision(block2,architect) || colision(block3,architect))
    {
        if(helth==0 && is_colicion)
        {
            G_StopMusic();
           G_PlaySound(mix_lose,0);
            game_state=LOSE;
            backGround.texture=bg_gameOver;

        }
        else if(is_colicion)
        {
            helth--;
            is_colicion=false;
            if(architect.pos.y==level[2] || architect.pos.y==level[4]){
            	architect.pos.x=540;
            	architect.texture=architectLeft.texture;
            	G_PauseMusic();
            	G_PlaySound(mix_lose,0);
            	G_Delay(3000);
            	G_PlayMusic( mix_game, -1 );
            	lastDirection = newDirection = NONE;
          }
          else{
           	architect.pos.x=0;
           	architect.texture=architectRight.texture;
          	G_PauseMusic();
          	G_PlaySound(mix_lose,0);
           	G_Delay(3000);
           	G_PlayMusic( mix_game, -1 );
           	lastDirection = newDirection = NONE;
           }
        }
    }
    else
        is_colicion=true;
    constantMove();
}


void constantMove(){

	   switch(lastDirection){
	   case LEFT:{
	       architect.texture = architectLeft.texture;
	       if(architect.pos.x>=5){
	    	   architect.pos.x-=5;
	       }
	   }
	   break;
	   case RIGHT:{
	       architect.texture=architectRight.texture;
	       if(architect.pos.x<=540)
	       architect.pos.x+=5;
	   }
	   break;
	   }

	   if(lastDirection !=newDirection && architect.pos.x>=0 && architect.pos.x<=700){
		   lastDirection = newDirection;
	   }
	   if(architect.pos.x<=0)
	   {
		   if(architect.pos.y  ==level[2] || architect.pos.y == level[4])
		   {
			   architect.pos.x=0;
			   architect.pos.y-=145;
			   step++;
			   lastDirection = NONE;
		   }
	   }
	   if(architect.pos.x>=540) {
		   if(architect.pos.y == level[1] || architect.pos.y == level[3])
		   {
			   architect.pos.y-=145;
			   step++;
			   lastDirection = NONE;
		   }
		   else if(architect.pos.y== level[2] || architect.pos.y==level[4])
		   {
			   architect.pos.x=540;
		   }
		   else
		   {
			   game_state=(game_state==PLAY1)?WIN1:WIN2;
			   G_StopMusic();
			   lastDirection = newDirection = NONE;
			   G_PlaySound(mix_win,0);
			   backGround.texture=bg_win;
		   }
	   }
	   DrawLine(block1);
	   DrawLine(block2);
	   DrawLine(block3);
	   G_Draw(btn_pause.texture,&btn_pause.pos,false);
}

void start()
{
	G_Draw(btn_start.texture,&btn_start.pos,false);
	G_Draw(btn_level.texture,&btn_level.pos,false);
	G_Draw(btn_exit.texture,&btn_exit.pos,false);

	G_PlaySound(mix_start, 0 );

    //Handle events on queue
        mouse.pos.x= GM_LEFT.X;
        mouse.pos.y= GM_LEFT.Y;
        mouse.pos.w=5;
        mouse.pos.h=5;
        if(GM_LEFT.down)
        {

            if(colision(btn_start,mouse))
            {
                game_state=PLAY1;
                inital(1);
                backGround.texture=bg_game_L1;
                G_StopMusic();
                G_PlayMusic( mix_game, -1 );
            }
            else if(colision(btn_exit,mouse))
            {
                game_state=EXITE;
                G_StopMusic();
            }
            else if(colision(btn_level,mouse))
            {
                game_state=SHOWLEVEL;
                G_StopMusic();
            }
        }

}



void lose()
{
	btn_exit.pos.y=600;

    G_Draw(backGround.texture,&backGround.pos,false);
    G_Draw(btn_backToMenu.texture,&btn_backToMenu.pos,false);
    G_Draw(btn_exit.texture,&btn_exit.pos,false);

    //Handle events on queue
        mouse.pos.x = GM_LEFT.X;
        mouse.pos.y = GM_LEFT.Y;
        mouse.pos.w = 5;
        mouse.pos.h = 5;
        if(GM_LEFT.down)
        {
            if(colision(btn_backToMenu,mouse))
            {
                game_state=START;
                btn_exit.pos.y=500;
                GM_LEFT.down = false;
            }
            else if(colision(btn_exit,mouse))
                game_state=EXITE;
        }
}



void showLevel()
{
    G_Draw(btn_level1.texture,&btn_level1.pos,false);
    G_Draw(btn_level2.texture,&btn_level2.pos,false);

    //Handle events on queue
    	mouse.pos.x=GM_LEFT.X;
        mouse.pos.y=GM_LEFT.Y;
        mouse.pos.w=5;
        mouse.pos.h=5;
        if(GM_LEFT.down)
        {
            if(colision(btn_level1,mouse))
            {
                game_state=PLAY1;
                inital(1);
                backGround.texture=bg_game_L1;
                G_StopMusic();
                G_PlayMusic( mix_game, -1 );
                G_PlaySound( mix_start, 0 );
            }
            else if(colision(btn_level2,mouse))
            {
                game_state=PLAY2;
                inital(2);
                backGround.texture=bg_game_L2;
                G_StopMusic();
                G_PlayMusic( mix_game, -1 );
               G_PlaySound( mix_start, 0 );
            }
        }

}



void win()
{
    backGround.texture=bg_win;
    G_Draw(backGround.texture,&backGround.pos,false);
    helth++;
    if(helth>100 && game_state==WIN1)
    {
        inital(2);
        backGround.texture=bg_game_L2;
        game_state=PLAY2;
    }
    else if(helth>100)
    {
        inital(1);
        game_state=START;
    }
}



void pause()
{
    G_Draw(backGround.texture,&backGround.pos,false);
    G_Draw(l_helth.texture,&l_helth.pos,false);
    G_Draw(l_step.texture,&l_step.pos,false);
    G_Draw(btn_pause.texture,&btn_pause.pos,false);
    G_Draw(pause_menu.texture,&pause_menu.pos,false);
    if(architect.texture == architectRight.texture)
    	G_Draw(architectRight.texture,&architectRight.pos,&architect.pos,false);
    else
    	G_Draw(architectLeft.texture,&architectLeft.pos,&architect.pos,false);

        mouse.pos.x=GM_LEFT.X;
        mouse.pos.y=GM_LEFT.Y;
        mouse.pos.w=5;
        mouse.pos.h=5;
        if(GM_LEFT.down)
        {
            if(colision(pause_menu,mouse))
            {
                if(backGround.texture==bg_game_L1)
                    game_state=PLAY1;
                else
                    game_state=PLAY2;

            }
        }

}


void DrawLine(object b1){
	dot.pos.x=b1.pos.x+25;
		    	for(dot.pos.y=0;dot.pos.y < b1.pos.y;dot.pos.y ++)
		    	{
		    		G_Draw(dot.texture,&dot.pos,false);
		    	}
}



void showLable(){
	switch(lable)
	{
	case HELTH:{
		textImg.pos.x = 500;
		textImg.pos.y = 20;
		textImg.pos.w = 60;
		textImg.pos.h = 30;

		textImg.texture = G_LoadFont(myfont,"helth : ",255,128,128);
		if(helth == 0)
			l_helth.texture = G_LoadFont(myfont,"0 ",255,0,0);
		if(helth == 1)
			l_helth.texture = G_LoadFont(myfont,"1 ",255,0,0);
		if(helth == 2)
			l_helth.texture = G_LoadFont(myfont,"2 ",255,0,0);

		G_Draw(textImg.texture,&textImg.pos,false);
		G_Draw(l_helth.texture,&l_helth.pos,false);
	}break;
	case STEP:{
		textImg.pos.x = 500;
		textImg.pos.y = 55;
		textImg.pos.w = 50;
		textImg.pos.h = 30;

		textImg.texture = G_LoadFont(myfont,"step : ",255,128,128);
		if(step == 1)
			l_step.texture = G_LoadFont(myfont,"1 ",255,0,0);
		if(step == 2)
			l_step.texture = G_LoadFont(myfont,"2 ",255,0,0);
		if(step == 3)
			l_step.texture = G_LoadFont(myfont,"3 ",255,0,0);
		if(step == 4)
			l_step.texture = G_LoadFont(myfont,"4 ",255,0,0);
		if(step == 5)
			l_step.texture = G_LoadFont(myfont,"5 ",255,0,0);
		if(step == 6)
			l_step.texture = G_LoadFont(myfont,"6 ",255,0,0);
		G_Draw(textImg.texture,&textImg.pos,false);
		G_Draw(l_step.texture,&l_step.pos,false);
		}break;
	}
}

void destroy()
{
	G_DestroyTexture(architectRight.texture);
	G_DestroyTexture(architectLeft.texture);
	G_DestroyTexture(btn_backToMenu.texture);
	G_DestroyTexture(btn_exit.texture);
	G_DestroyTexture(btn_level.texture);
	G_DestroyTexture(btn_pause.texture);
	G_DestroyTexture(btn_pause.texture);
	G_DestroyTexture(block1.texture);
	G_DestroyTexture(block2.texture);
	G_DestroyTexture(block3.texture);
	G_Quit();
}
