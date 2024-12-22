#pragma once

#include <iostream>
#include <string>
#include <windows.h>
#include <vector>
#include <atlstr.h>
#include "SDL.h" 
#include "SDL_image.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"



extern int idx ;

extern SDL_Window* g_window;
extern SDL_Renderer* g_renderer;
extern SDL_Texture* background;
extern SDL_Texture* airplane;
extern SDL_Texture* missile;


extern Mix_Music* g_bg_mus;
extern Mix_Chunk* g_shot_sound;

extern SDL_Rect g_airplane_src_rect;
extern SDL_Rect g_airplane_dest_rect;

extern SDL_Texture* g_score;//한글
extern SDL_Texture* g_sc;//숫자
extern SDL_Rect g_score_rect;//한글
extern SDL_Rect g_sc_rect;//숫자
extern int score;

class AirplaneObj {
public:

	int x = 0;
	int y = 0;
	SDL_Texture* sprite;
	bool control = false;
	std::string type;

	AirplaneObj() {}

	AirplaneObj(SDL_Texture* sp, int a, int b) {
		this->sprite = sp;
		this->x = a;
		this->y = b;
	}

	void setX(int a) { this->x = a; }
	void setY(int b) { this->y = b; }

	void Update() {
		if (type == "missile") {
			this->y = this->y - 15;
		}
		else if (control == true) {
			if (this->x < 0) { this->x = 0; }
			else if (this->x > 330) { this->x = 329; }
			else if (this->y < 0) { this->y = 0; }
			else if (this->y > 660) { this->y = 659; }
		}


	}
};


std::vector<AirplaneObj> AirplaneObjs;

class KeyBoard {
public:
	bool up;
	bool down;
	bool left;
	bool right;
	bool space;

	void Init() {
		up = false;
		down = false;
		left = false;
		right = false;
		space = false;
	}

};

KeyBoard eventkeys;

void UpdateScore(int score) {

	std::string st;
	std::string st_;

	if (score == 0) {
		st = "0000";
		st_ = std::to_string(score);

		st += st_;
	}
	else if (score > 0 && score < 100) {
		st = "000";
		st_ = std::to_string(score);
		st += st_;
	}
	else if (score >= 100 && score < 1000) {
		st = "00";
		st_ = std::to_string(score);
		st += st_;
	}
	else if (score >= 1000 && score < 10000) {
		st = "0";
		st_ = std::to_string(score);
		st += st_;
	}
	else {
		st = std::to_string(score);
	}



	TTF_Font* font = TTF_OpenFont("../../Resources/Galmuri14.ttf", 30);
	SDL_Color white = { 255,255,255,0 };
	SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, st.c_str(), white);

	g_sc_rect.x = 0;
	g_sc_rect.y = 0;
	g_sc_rect.w = tmp_surface->w;
	g_sc_rect.h = tmp_surface->h;



	g_sc = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
	SDL_FreeSurface(tmp_surface);
	TTF_CloseFont(font);


}



static void Missile() {
	if (idx >= 3) {
		return;
	}

	SDL_Surface* sprite = IMG_Load("../../Resources/missile.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(g_renderer, sprite);
	AirplaneObj missile = AirplaneObj(texture, (int)AirplaneObjs[0].x, (int)AirplaneObjs[0].y - 15);
	missile.type = "missile";
	idx++;
	AirplaneObjs.push_back(missile);


	SDL_FreeSurface(sprite);
	Mix_PlayChannel(-1, g_shot_sound, 0);


}


void eraseObj() {
	auto item = AirplaneObjs.begin();
	for (AirplaneObj& airplaneobj : AirplaneObjs) {
		if (airplaneobj.x < 0) {
			if (airplaneobj.type == "missile") { idx--; }
			AirplaneObjs.erase(item);
		}
		else if (airplaneobj.x > 400) {
			if (airplaneobj.type == "missile") { idx--; }
			AirplaneObjs.erase(item);
		}
		else if (airplaneobj.y < 0) {
			if (airplaneobj.type == "missile") { idx--; }
			AirplaneObjs.erase(item);
		}
		else if (airplaneobj.y > 800) {
			if (airplaneobj.type == "missile") { idx--; }
			AirplaneObjs.erase(item);
		}
		else { ++item; }
	}
}