#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <stdbool.h>
#include <math.h>

#define FALSE 0
#define TRUE 1
#define WIDTH_BY_BOXS 15
#define BOX_WIDTH 20
#define BOX_HEIGHT 20
#define WIDTH (BOX_WIDTH*WIDTH_BY_BOXS)
#define HEIGHT (BOX_HEIGHT*WIDTH_BY_BOXS)
#define FPS 50
#define FRAME_TARGET_TIME (1000/FPS)

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
int GameIsRuning = FALSE;
int LastFrameTime;
char diraction1 ='E';
char diraction2 ='E';
int lastMoveTime =0;
int frameCounter = 0;
int hroizontal_limit = WIDTH - BOX_WIDTH;
int virtical_limit = HEIGHT - BOX_HEIGHT;

float snack[10000][2] = {{0,0}};
int snackLength = 0;
int target_x;
int target_y;

float STIP = 1.5;
int HOW_MANY_FRAME;
int WAIT_TO_MOVE;

void setTarget(void){
	int the_x, the_y;
	int xx = 1;
	while(xx){
		the_x = (rand()% WIDTH_BY_BOXS) * BOX_WIDTH;
		the_y = (rand()% WIDTH_BY_BOXS) * BOX_WIDTH;
		for (int i=0; i<snackLength; i++){
			if(snack[i][0] == the_x && snack[i][1] == the_y ){
				xx = 1;
				break;
			}else{xx = 0 ;}
		}
	}
	target_x = the_x;
	target_y = the_y;
}
void setup(void){
	HOW_MANY_FRAME = (int) (BOX_WIDTH/STIP);
	WAIT_TO_MOVE = HOW_MANY_FRAME+5;

	for (int i=0; i<=HOW_MANY_FRAME; i++){
		snack[i][0]=0;
		snack[i][1]=0;
		snackLength++;
	}
	setTarget();
}
int intialize_window(void){
	if(SDL_Init(SDL_INIT_EVERYTHING) != 0){
		fprintf(stderr,"Error intialize SDL");
		return FALSE;
	}
	window = SDL_CreateWindow(NULL,100,100,WIDTH,HEIGHT,SDL_WINDOW_BORDERLESS);
	if(!window){
		fprintf(stderr,"Error: creating SDL window!");
		return FALSE;
	}
	renderer = SDL_CreateRenderer(window,-1,0);
	if(!renderer){
		fprintf(stderr,"Error: creating SDL rednerer!");
		return FALSE;
	}
	return TRUE;
}
void destroyWindow(void){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void process_input(void){
	SDL_Event event;
	SDL_PollEvent(&event);
	switch(event.type){
		case SDL_QUIT:
			GameIsRuning = FALSE;
			break;
		case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE){
			GameIsRuning = FALSE;
		}else if(event.key.keysym.sym == SDLK_w){
			diraction1 = 'N';

		}else if(event.key.keysym.sym == SDLK_d){
			diraction1 = 'E';

		}else if(event.key.keysym.sym == SDLK_a){
			diraction1 = 'W';

		}else if(event.key.keysym.sym == SDLK_s){
			diraction1 = 'S';

		}else if(event.key.keysym.sym == SDLK_UP){
			diraction1 = 'N';

		}else if(event.key.keysym.sym == SDLK_RIGHT){
			diraction1 = 'E';

		}else if(event.key.keysym.sym == SDLK_LEFT){
			diraction1 = 'W';

		}else if(event.key.keysym.sym == SDLK_DOWN){
			diraction1 = 'S';
		}break;

	}

}

void drawCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x; // Decision criterion divided by 2

    while (y <= x) {
        // Draw the 8 symmetrical points of the circle
        SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY + y);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY + x);
        SDL_RenderDrawPoint(renderer, centerX - x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX - y, centerY - x);
        SDL_RenderDrawPoint(renderer, centerX + x, centerY - y);
        SDL_RenderDrawPoint(renderer, centerX + y, centerY - x);

        y++;
        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1; // Change in decision criterion for y -> y+1
        } else {
            x--;
            decisionOver2 += 2 * (y - x) + 1; // Change for y -> y+1, x -> x-1
        }
    }
}

void drawFilledCircle(SDL_Renderer* renderer, int centerX, int centerY, int radius) {
    for (int y = -radius; y <= radius; y++) {
        for (int x = -radius; x <= radius; x++) {
            if (x * x + y * y <= radius * radius) {
                SDL_RenderDrawPoint(renderer, centerX + x, centerY + y);
            }
        }
    }
}

float easeOutQuad(float t) { return 1 - (1 - t) * (1 - t); }

float easeOutCubic(float t) { return 1 - pow(1 - t, 3); }

float move = 0;

void update(void) {
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - LastFrameTime);
    if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) SDL_Delay(time_to_wait);
    LastFrameTime = SDL_GetTicks();

    frameCounter++;
    static float prevEasedT = 0.0f;

    if (frameCounter <= HOW_MANY_FRAME) {
        float t = (float)frameCounter / (float)HOW_MANY_FRAME;
        float easedT = easeOutCubic(t);

        float stip = 0.0f;
		
        if (frameCounter == HOW_MANY_FRAME) {
            stip = (float)BOX_WIDTH - move;
			move = 0.0f;
        } else {
            stip = (float)BOX_WIDTH * (easedT - prevEasedT);
			prevEasedT = easedT;
			move += stip;
        }

        // Move the snake
        for (int i = snackLength - 1; i > 0; i--) {
            snack[i][0] = snack[i - 1][0];
            snack[i][1] = snack[i - 1][1];
        }
        if        (diraction2 == 'E') {
            if (snack[0][0] < hroizontal_limit) {
                snack[0][0] += stip;
            } else {
                snack[0][0] = -((float)BOX_WIDTH - stip);
            }
        } else if (diraction2 == 'W') {
            if (snack[0][0] > 0) {
                snack[0][0] -= stip;
            } else {
                snack[0][0] = (float)hroizontal_limit + ((float)BOX_WIDTH - stip);
            }
        } else if (diraction2 == 'N') {
            if (snack[0][1] > 0) {
                snack[0][1] -= stip;
            } else {
                snack[0][1] = (float)virtical_limit + ((float)BOX_WIDTH - stip);
            }
        } else if (diraction2 == 'S') {
            if (snack[0][1] < virtical_limit) {
                snack[0][1] += stip;
            } else {
                snack[0][1] = -((float)BOX_WIDTH - stip);
            }
        }
		
    } else if (frameCounter == WAIT_TO_MOVE) {
        frameCounter = 0;
        prevEasedT = 0.0f;
        diraction2 = diraction1;
    }

    if (frameCounter == HOW_MANY_FRAME) {
		// Round and fix the position
		snack[0][0] = roundf(snack[0][0] / BOX_WIDTH) * BOX_WIDTH;
		snack[0][1] = roundf(snack[0][1] / BOX_HEIGHT) * BOX_HEIGHT;
		
		// Check for collisions with the target
		if (snack[0][0] == target_x && snack[0][1] == target_y) {
			for (int i = 0; i < HOW_MANY_FRAME; i++) {
				snack[snackLength][0] = snack[1][0];
				snack[snackLength][1] = snack[1][1];
				snackLength++;
			}
			setTarget();
		}
		// Check for collisions with itself
		for (int i = 1; i < snackLength; i++) {
			if (snack[0][0] == snack[i][0] && snack[0][1] == snack[i][1]) {
				GameIsRuning = FALSE;
			}
		}
    }	
}

void render(){
	SDL_SetRenderDrawColor(renderer,200,50,0,255);
	SDL_RenderClear(renderer);
	// seting my elemmment here =>
	SDL_Rect target = { target_x, target_y, BOX_WIDTH, BOX_HEIGHT};
	SDL_SetRenderDrawColor(renderer, 250,140,100,100);
	SDL_RenderFillRect(renderer,&target);

	
	
	SDL_SetRenderDrawColor(renderer,0,160,0,100);
	for (int i=1; i<snackLength; i++){
		
		drawFilledCircle(renderer, snack[i][0] + BOX_WIDTH / 2, snack[i][1] + BOX_HEIGHT / 2, BOX_WIDTH / 2);
	}
	SDL_SetRenderDrawColor(renderer,0,120,0,100);
	drawFilledCircle(renderer, snack[0][0] + BOX_WIDTH / 2, snack[0][1] + BOX_HEIGHT / 2, BOX_WIDTH / 2);

	//////////////////////////////
	SDL_RenderPresent(renderer);

}

int main(int argc, char *argv[]){
	srand(time(0));
	GameIsRuning = intialize_window();
	setup();
	while(GameIsRuning){
		process_input();
		update();
		render();
	}
	destroyWindow();
	return 0;
}