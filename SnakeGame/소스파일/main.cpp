#include "include.h"
#include "snakeUI.h"
#include "Node.h"

void InitGame();
void ClearGame();


SDL_Window* g_window;
SDL_Renderer* g_renderer;
Mix_Music* intro_music;
Mix_Music* play_music;

bool g_flag_running;
Uint32 g_last_time_ms;



int dir; //이동방향, 0 : 위 / 1 : 아래 / 2 : 왼 / 3 : 오
int itemX, itemY; //아이템 좌표

std::string lastkey;
std::string nowkey;
int sec;
int count;

bool isright;
bool isleft;
bool isup;
bool isdown;

bool isPlay;
bool isIntro;
bool iscrushed;
bool eatItem;
bool keyboard;
int score;

int main(int argc, char* argv[]) {
	// Initializing SDL library
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();

	g_flag_running = true;


	sec = 0;
	isPlay = false;
	isIntro = true;
	keyboard = true;
	lastkey = "Right";
	nowkey = "Right";
	score = 0;

	if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		std::cout << "Mix_OpenAudio " << Mix_GetError() << std::endl;
		exit(1);
	}
	

	g_window = SDL_CreateWindow("Snake Game", 100, 100, 1200, 800, 0); 
	g_renderer = SDL_CreateRenderer(g_window, -1, 0);
	InitGame();
	
	snakeUI SnakeUI;
	g_last_time_ms = SDL_GetTicks();

	

	while (g_flag_running) { 


		Uint32 cur_time_ms = SDL_GetTicks();

		if (cur_time_ms - g_last_time_ms < 33) 
			continue;

		SnakeUI.HandleEvents();
		SnakeUI.Update();
		SnakeUI.Render();
		g_last_time_ms = cur_time_ms;

		SDL_Delay(1);
	}

	ClearGame();
	

	return 0;
}

void InitGame() {
	g_flag_running = true;
	g_last_time_ms = 0;
}

void ClearGame() {
	Mix_FreeMusic(play_music);
	Mix_FreeMusic(intro_music);
	SDL_DestroyRenderer(g_renderer);
	SDL_DestroyWindow(g_window);
	Mix_CloseAudio();
	TTF_Quit();
	SDL_Quit();
}


