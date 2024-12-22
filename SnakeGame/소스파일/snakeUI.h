#pragma once

#include "include.h"
#include "Node.h"

class snakeUI :public Node {
public:
	snakeUI();
	virtual ~snakeUI();

	void HandleEvents();
	void Update();
	void Render();
	void reset();
	void restart();
	void crush();
	void eat();
	
	int randX, randY;
	int newX, newY;
	int life;
	int speed;
	int eatItems;
	int eatItems2;
	

	SDL_Texture* bg;
	SDL_Texture* intro_bg;
	SDL_Texture* gameover_bg;
	SDL_Texture* head_up;
	SDL_Texture* head_down;
	SDL_Texture* head_right;
	SDL_Texture* head_left;
	SDL_Texture* body;
	SDL_Texture* apple;
	SDL_Texture* heart_full;
	SDL_Texture* heart_2;
	SDL_Texture* heart_1;
	SDL_Texture* heart_empty;
	SDL_Texture* item_txt;
	SDL_Texture* score_txt;
	SDL_Texture* end_score;
	SDL_Texture* x_text;
	SDL_Texture* score_s_txt;

	SDL_Rect snake_rect;
	SDL_Rect apple_rect;
	SDL_Rect heart_rect;
	SDL_Rect item_rect;
	SDL_Rect score_rect;
	SDL_Rect x_rect;
	SDL_Rect score_s_rect;
	SDL_Rect end_score_rect;

	Mix_Chunk* eat_sound;
	Mix_Chunk* death_sound;

};

extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern bool g_flag_running;
extern Uint32 g_last_time_ms;

extern int length;
extern int dir;
extern std::string lastkey;
extern std::string nowkey;
extern int sec;
extern int count;
extern int score;

extern bool isright;
extern bool isleft;
extern bool isup;
extern bool isdown;

extern bool isPlay;
extern bool isIntro;
extern bool iscrushed;
extern bool eatItem;
extern bool keyboard;

extern Mix_Music* intro_music;
extern Mix_Music* play_music;
