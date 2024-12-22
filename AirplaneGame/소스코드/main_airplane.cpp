#define _CRTDBG_MAP_ALLOC

#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <atlstr.h>
#include "SDL.h" 
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "obj.h"


#include <stdlib.h>
#include <crtdbg.h>



void InitGame();
void HandleEvent();
void Render();
void Update();
void ClearGame();
void Missile();
void eraseObj();
void UpdateScore(int score);
//void KeyEvent();

bool g_flag_running;
Uint32 g_elapsed_time_ms;
Uint32 g_frame_per_sec = 30;

SDL_Window* g_window;
SDL_Renderer* g_renderer;
SDL_Texture* background;
SDL_Texture* airplane;
SDL_Texture* missile;

SDL_Rect g_airplane_src_rect;
SDL_Rect g_airplane_dest_rect;


Mix_Music* g_bg_mus;
Mix_Chunk* g_shot_sound;

SDL_Texture* g_score;//한글
SDL_Texture* g_sc;//숫자
SDL_Rect g_score_rect;//한글
SDL_Rect g_sc_rect;//숫자
int score = 0;

int idx = 0;



int main(int argc, char* argv[]) {
	// Initializing SDL library

	
	//window
	SDL_Init(SDL_INIT_EVERYTHING);
	g_window = SDL_CreateWindow("First Window", 100, 100, 400, 800, 0);
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);


	//audio
	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Mix_OpenAudio" << Mix_GetError() << std::endl;
		exit(1);
	}


	//text
	TTF_Init();




	InitGame();

	g_elapsed_time_ms = SDL_GetTicks();

	while (g_flag_running) {

		Uint32 cur_time_ms = SDL_GetTicks();
		
		if (cur_time_ms - g_elapsed_time_ms < g_frame_per_sec)
			continue;

		HandleEvent();
		Update();
		Render();
		eraseObj();

		g_elapsed_time_ms = cur_time_ms;
	}

	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);

	ClearGame();
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();


	return 0;
}



void InitGame() {
	g_flag_running = true;
	
	eventkeys.Init();
	// BG
	SDL_Surface* bg_surface = IMG_Load("../../Resources/space_bg.png");
	background = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);



	//Airplane
	SDL_Surface* sprite = IMG_Load("../../Resources/airplane.png");
	airplane = SDL_CreateTextureFromSurface(g_renderer, sprite);
	SDL_FreeSurface(sprite);

	AirplaneObj ap = AirplaneObj(airplane, 160, 600);
	ap.control = true;
	AirplaneObjs.emplace_back(ap);



	//audio
	g_bg_mus = Mix_LoadMUS("../../Resources/GameBGM.mp3");
	if (g_bg_mus == 0) {
		std::cout << "Mix_LoadMUS(\"GameBGM.mp3\"): " << Mix_GetError() << std::endl;
	}
	
	g_shot_sound = Mix_LoadWAV("../../Resources/shotSound.mp3");

	Mix_PlayMusic(g_bg_mus, -1);
	Mix_VolumeMusic(80);


	//text
	
	{
		TTF_Font* font = TTF_OpenFont("../../Resources/Galmuri14.ttf", 30);
		SDL_Color white = { 255,255,255,0 };
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, CW2A(L"점수 : ", CP_UTF8), white); //L : 2byte가 한글자
		
		g_score_rect.x = 0;
		g_score_rect.y = 0;
		g_score_rect.w = tmp_surface->w;
		g_score_rect.h = tmp_surface->h;



		g_score = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
		SDL_FreeSurface(tmp_surface);
		TTF_CloseFont(font);
		
	}

	
}




void KeyEvent() {
	for (AirplaneObj& airplaneobj : AirplaneObjs) {
		if (airplaneobj.control == true) {
			if (eventkeys.left == true) {
				airplaneobj.setX(airplaneobj.x - 20);
			}
			if (eventkeys.right == true) {
				airplaneobj.setX(airplaneobj.x + 20);
			}
			if (eventkeys.up == true) {
				airplaneobj.setY(airplaneobj.y - 20);
			}
			if (eventkeys.down == true) {
				airplaneobj.setY(airplaneobj.y + 20);
			}
			
			
		}
	}
	if (eventkeys.space == true) { Missile(); }

}


void HandleEvent() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (event.type == SDL_KEYDOWN) {
			if (event.type == SDL_QUIT) {
				g_flag_running = false;
				return;
			}
			if (event.key.keysym.sym == SDLK_LEFT) {
				eventkeys.left = true;
				KeyEvent();
			}

			if (event.key.keysym.sym == SDLK_RIGHT) {
				eventkeys.right = true;
				KeyEvent();
			}

			if (event.key.keysym.sym == SDLK_UP) {
				eventkeys.up = true;
				KeyEvent();
			}

			if (event.key.keysym.sym == SDLK_DOWN) {
				eventkeys.down = true;
				KeyEvent();
			}

			if (event.key.keysym.sym == SDLK_SPACE) {
				eventkeys.space = true;
				KeyEvent();
				score += 10;
			}
		}

		if (event.type == SDL_KEYUP) {
			if (event.key.keysym.sym == SDLK_LEFT) {
				eventkeys.left = false;
			}

			if (event.key.keysym.sym == SDLK_RIGHT) {
				eventkeys.right = false;
			}

			if (event.key.keysym.sym == SDLK_UP) {
				eventkeys.up = false;
			}

			if (event.key.keysym.sym == SDLK_DOWN) {
				eventkeys.down = false;
			}

			if (event.key.keysym.sym == SDLK_SPACE) {
				eventkeys.space = false;
			}
		}



	}


}
/*
void HandleEvent() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch (event.type) {

		case SDL_QUIT: 
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:

			if (event.key.keysym.sym == SDLK_LEFT) {
				for (AirplaneObj& airplaneobj : AirplaneObjs) {
					if (airplaneobj.control == true) {
						airplaneobj.setX(airplaneobj.x - 20);
					}
				}
			}

			else if (event.key.keysym.sym == SDLK_RIGHT) {
				for (AirplaneObj& airplaneobj : AirplaneObjs) {
					if (airplaneobj.control == true) {
						airplaneobj.setX(airplaneobj.x + 20);
					}
				}
			}

			else if (event.key.keysym.sym == SDLK_UP) {
				for (AirplaneObj& airplaneobj : AirplaneObjs) {
					if (airplaneobj.control == true) {
						airplaneobj.setY(airplaneobj.y - 20);
					}
				}
			}

			else if (event.key.keysym.sym == SDLK_DOWN) {
				for (AirplaneObj& airplaneobj : AirplaneObjs) {
					if (airplaneobj.control == true) {
						airplaneobj.setY(airplaneobj.y + 20);
					}
				}
			}

			
			else if (event.key.keysym.sym == SDLK_SPACE) {
				score += 10;
				Missile();
			}

			break;

		case SDL_KEYUP: break;
		default: break;


		}


	}
}
*/




void Render() {
	SDL_RenderCopy(g_renderer, background, NULL, NULL);
	SDL_RenderCopy(g_renderer, airplane, &g_airplane_src_rect, &g_airplane_dest_rect);

	{///한글
		SDL_Rect tmp_r;
		tmp_r.x = 15;
		tmp_r.y = 15;
		tmp_r.w = g_score_rect.w;
		tmp_r.h = g_score_rect.h;
		SDL_RenderCopy(g_renderer, g_score, &g_score_rect, &tmp_r);
	}

	{//숫자
		SDL_Rect tmp_r;
		tmp_r.x = 110;
		tmp_r.y = 15;
		tmp_r.w = g_sc_rect.w;
		tmp_r.h = g_sc_rect.h;
		SDL_RenderCopy(g_renderer, g_sc, &g_sc_rect, &tmp_r);
	}


	for (AirplaneObj& airplaneobj : AirplaneObjs) {
		SDL_Rect dst = { airplaneobj.x, airplaneobj.y, 72, 60 };//크기
		SDL_RenderCopy(g_renderer, airplaneobj.sprite, NULL, &dst);
	}
	SDL_RenderPresent(g_renderer);

	
}



void Update() {
	for (AirplaneObj& airplaneobj : AirplaneObjs) {
		airplaneobj.Update();
	}

	
	UpdateScore(score);
}



void ClearGame() {
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(g_score);
	SDL_DestroyTexture(airplane);
	SDL_DestroyTexture(missile);
	SDL_DestroyTexture(g_sc);
	SDL_DestroyTexture(g_score);

	 Mix_FreeMusic(g_bg_mus);
	Mix_FreeChunk(g_shot_sound);
	
	
}




