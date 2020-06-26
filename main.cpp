/*This source code copyrighted by Lazy Foo' Productions (2004-2020)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>
#include <vector>
#include <iostream>
#include "LTexture.h"


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;



//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Rendered texture
// LTexture gTextTexture;

// LTexture gTexts[10];


bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else {
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else{
			//Create vsynced renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( gRenderer == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else {
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}

				 //Initialize SDL_ttf
				if( TTF_Init() == -1 ) {
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

SDL_Color textColor = { 0, 0, 0 };

bool loadMedia() {
	//Loading success flag
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont( "16_true_type_fonts/DejaVuSans.ttf", 20 );
	if( gFont == NULL ) {
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	return success;
}

void close() {

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

int colorTable[3][3] = { { 0,0,255},  { 0,255,0}, { 255, 0,0}};


// drawRect:
//    renderer: pointer to an SDL_Renderer object
//    x, y, width, height: position and dimensions of the rectangle
//    color: an SDL_color structure that contains r,g,b,and a values
//    txt: the text that will be printed (right justified) inside the rectangle

void drawRect(SDL_Renderer* renderer, int x, int y, int width, int height, const SDL_Color& color, std::string txt){
	SDL_Rect fillRect = { x, y, width, height };
	SDL_SetRenderDrawColor( renderer, color.r, color.g, color.b, color.a );		
	SDL_RenderFillRect( gRenderer, &fillRect );
	LTexture gText(renderer);
	gText.loadFromRenderedText( txt, textColor, gFont );
	gText.render( width - gText.getWidth() , y );
}




int main( int argc, char* args[] ) {
	std::vector < std::vector < std::pair<int,std::string> > > data;
	int idx = 0;

	std::vector < std::pair<int,std::string> > tmp;
	tmp.push_back(std::pair<int,std::string> (90, "PR"));
	tmp.push_back(std::pair<int,std::string> (70, "Cuba"));

	data.push_back(tmp);

	tmp.clear();
	tmp.push_back(std::pair<int,std::string> (80, "PR"));
	tmp.push_back(std::pair<int,std::string> (75, "Cuba"));
	data.push_back(tmp);

	tmp.clear();
	tmp.push_back(std::pair<int,std::string> (95, "Cuba"));
	tmp.push_back(std::pair<int,std::string> (60, "PR"));
	data.push_back(tmp);


	//Start up SDL and create window
	if( !init() ) {
		printf( "Failed to initialize!\n" );
	}
	else {
		//Load media
		if( !loadMedia() ) {
			printf( "Failed to load media!\n" );
		}
		else {	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			//While application is running
			while( !quit ) {
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 ) {
					//User requests quit
					if( e.type == SDL_QUIT ) {
						quit = true;
					}

					else if (e.type == SDL_KEYDOWN ) {
						if ( e.key.keysym.sym== SDLK_q)  quit = true;
						if ( e.key.keysym.sym== SDLK_d) {
							idx = (idx + 1) % data.size();

						}
					}
				}

				//Clear screen
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );


				int y = 0;

				for (auto e: data[idx]) {
					drawRect(gRenderer, 0, y, static_cast<int>(SCREEN_WIDTH* (e.first/100.)), static_cast<int>(SCREEN_HEIGHT * .1), SDL_Color {.r= 0, .g = 0xff, .b = 0, .a = 0xff }  ,e.second);
					y = y + 60;
				}

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}