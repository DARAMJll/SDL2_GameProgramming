#include "snakeUI.h"

std::list<Node> Nodes;
std::vector<Node> apples;



snakeUI::snakeUI() {
	
	Nodes.push_back({ 14,9 });

	keyboard = true;
	isup = false;
	isdown = false;
	isleft = false;
	isright = true;
	iscrushed = false;
	eatItem = false;
	count = 0;
	sec = 0;
	newX = 0;
	newY = 0;
	speed = 9;
	eatItems = 0;
	eatItems2 = 0;
	life = 3;

	srand(time(0));
	randX = rand() % 28;
	randY = rand() % 18;
	std::cout << randX << " " << randY << std::endl;
	apples.push_back({ 0,0 });
	apples.push_back({ randX + 1, randY + 1 });
 
	// BG
	SDL_Surface* bg_surface = IMG_Load("../../Resources/background2.png");
	bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
	SDL_FreeSurface(bg_surface);

	//intro bg
	{
		SDL_Surface* bg_surface = IMG_Load("../../Resources/ready_bg.png");
		intro_bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
		SDL_FreeSurface(bg_surface);
	}

	//gameover bg
	{
		SDL_Surface* bg_surface = IMG_Load("../../Resources/gameover_bg.png");
		gameover_bg = SDL_CreateTextureFromSurface(g_renderer, bg_surface);
		SDL_FreeSurface(bg_surface);
	}

	//head
	{//up
		SDL_Surface* h_surface = IMG_Load("../../Resources/Head_up.png");
		head_up = SDL_CreateTextureFromSurface(g_renderer, h_surface);

		snake_rect.x = 0;
		snake_rect.y = 0;
		snake_rect.w = h_surface->w;
		snake_rect.h = h_surface->h;

		SDL_FreeSurface(h_surface);
	}
	{//down
		SDL_Surface* h_surface2 = IMG_Load("../../Resources/Head_down.png");
		head_down = SDL_CreateTextureFromSurface(g_renderer, h_surface2);
		SDL_FreeSurface(h_surface2);
	}
	{//right
		SDL_Surface* h_surface3 = IMG_Load("../../Resources/Head_right.png");
		head_right = SDL_CreateTextureFromSurface(g_renderer, h_surface3);
		SDL_FreeSurface(h_surface3);
	}
	{//left
		SDL_Surface* h_surface3 = IMG_Load("../../Resources/Head_left.png");
		head_left = SDL_CreateTextureFromSurface(g_renderer, h_surface3);
		SDL_FreeSurface(h_surface3);
	}

	//body
	{
		SDL_Surface* b_surface = IMG_Load("../../Resources/body.png");
		body = SDL_CreateTextureFromSurface(g_renderer, b_surface);
		SDL_FreeSurface(b_surface);
	}


	{//item(apple)
		SDL_Surface* a_surface = IMG_Load("../../Resources/apple.png");
		apple = SDL_CreateTextureFromSurface(g_renderer, a_surface);

		apple_rect.x = 0;
		apple_rect.y = 0;
		apple_rect.w = a_surface->w;
		apple_rect.h = a_surface->h;

		SDL_FreeSurface(a_surface);
	}

	{ //full heart
		SDL_Surface* h_f = IMG_Load("../../Resources/heart_full.png");
		heart_full = SDL_CreateTextureFromSurface(g_renderer, h_f);

		heart_rect.x = 0;
		heart_rect.y = 0;
		heart_rect.w = h_f->w;
		heart_rect.h = h_f->h;

		SDL_FreeSurface(h_f);
	}

	{
		SDL_Surface* h_2 = IMG_Load("../../Resources/heart_2.png");
		heart_2 = SDL_CreateTextureFromSurface(g_renderer, h_2);
		SDL_FreeSurface(h_2);
	}

	{
		SDL_Surface* h_1 = IMG_Load("../../Resources/heart_1.png");
		heart_1 = SDL_CreateTextureFromSurface(g_renderer, h_1);
		SDL_FreeSurface(h_1);
	}

	{
		SDL_Surface* h_e = IMG_Load("../../Resources/heart_empty.png");
		heart_empty = SDL_CreateTextureFromSurface(g_renderer, h_e);
		SDL_FreeSurface(h_e);
	}
	
	SDL_Color white = { 255,255,255,0 };
	SDL_Color pink = { 255, 192,201 };
	TTF_Font* font = TTF_OpenFont("../../Resources/DNFBitBitv2.ttf", 22);
	//score txt
	{
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, "SCORE : ", white);
		//std::to_string(score).c_str()
		score_rect.x = 0;
		score_rect.y = 0;
		score_rect.w = tmp_surface->w;
		score_rect.h = tmp_surface->h;
		score_txt = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
		SDL_FreeSurface(tmp_surface);
	}
	{
		SDL_Surface* tmp2_surface = TTF_RenderUTF8_Blended(font, CW2A(L"× ", CP_UTF8), pink);
		x_rect.x = 0;
		x_rect.y = 0;
		x_rect.w = tmp2_surface->w;
		x_rect.h = tmp2_surface->h;
		x_text = SDL_CreateTextureFromSurface(g_renderer, tmp2_surface);
		SDL_FreeSurface(tmp2_surface);
	}


	//music
	play_music = Mix_LoadMUS("../../Resources/PlayMusic.mp3");
	if (play_music == 0) {
		std::cout << "error\"): " << Mix_GetError() << std::endl;
	}
	intro_music = Mix_LoadMUS("../../Resources/IntroMusic.mp3");
	if (intro_music == 0) {
		std::cout << "error\"): " << Mix_GetError() << std::endl;
	}


	

	eat_sound = Mix_LoadWAV("../../Resources/EatingSound.mp3");
	death_sound = Mix_LoadWAV("../../Resources/GameoverSound.mp3");

	Mix_VolumeMusic(80);
	Mix_PlayMusic(intro_music, -1);
	
	TTF_CloseFont(font);
}

snakeUI::~snakeUI() {
	SDL_DestroyTexture(bg);
	SDL_DestroyTexture(head_up);
	SDL_DestroyTexture(head_down);
	SDL_DestroyTexture(head_right);
	SDL_DestroyTexture(head_left);
	SDL_DestroyTexture(body);
	SDL_DestroyTexture(apple);
	SDL_DestroyTexture(heart_full);
	SDL_DestroyTexture(heart_2);
	SDL_DestroyTexture(heart_1);
	SDL_DestroyTexture(heart_empty);
	SDL_DestroyTexture(item_txt);
	SDL_DestroyTexture(x_text);
	SDL_DestroyTexture(score_txt);
	SDL_DestroyTexture(score_s_txt);
	SDL_DestroyTexture(intro_bg);
	SDL_DestroyTexture(gameover_bg);
	SDL_DestroyTexture(end_score);
	Mix_FreeChunk(eat_sound);
	Mix_FreeChunk(death_sound);
	SDL_Quit();
	TTF_Quit();
}

void snakeUI::Render() {
	
	SDL_RenderCopy(g_renderer, bg, NULL, NULL);

	if (isPlay == false) {
		if (isIntro == true) {
			SDL_RenderCopy(g_renderer, intro_bg, NULL, NULL);
		}
		else if (isIntro == false){
			SDL_RenderCopy(g_renderer, gameover_bg, NULL, NULL);
		}
	}

	


	auto iter = Nodes.begin();

	SDL_Color white = { 255,255,255,0 };
	SDL_Color pink = { 255, 192,201 };
	SDL_Color Green = { 143, 201, 206 };
	

	std::string score_s = std::to_string(score);

	if (isPlay || isIntro == false) {
		//body 
		

		if (nowkey == "Up") {
			{
				SDL_Rect tmp_r;
				tmp_r.x = iter->x;
				tmp_r.y = iter->y;
				tmp_r.w = snake_rect.w;
				tmp_r.h = snake_rect.h;
				SDL_RenderCopy(g_renderer, head_up, &snake_rect, &tmp_r);
			}
		}
		else if (nowkey == "Down") {
			{
				SDL_Rect tmp_r;
				tmp_r.x = iter->x;
				tmp_r.y = iter->y;
				tmp_r.w = snake_rect.w;
				tmp_r.h = snake_rect.h;
				SDL_RenderCopy(g_renderer, head_down, &snake_rect, &tmp_r);
			}
		}
		else if (nowkey == "Right") {
			{
				SDL_Rect tmp_r;
				tmp_r.x = iter->x;
				tmp_r.y = iter->y;
				tmp_r.w = snake_rect.w;
				tmp_r.h = snake_rect.h;
				SDL_RenderCopy(g_renderer, head_right, &snake_rect, &tmp_r);
			}
		}
		else if (nowkey == "Left") {
			{
				SDL_Rect tmp_r;
				tmp_r.x = iter->x;
				tmp_r.y = iter->y;
				tmp_r.w = snake_rect.w;
				tmp_r.h = snake_rect.h;
				SDL_RenderCopy(g_renderer, head_left, &snake_rect, &tmp_r);
			}
		}



		for (iter++; iter != Nodes.end(); iter++)
		{
			SDL_Rect tmp_r;
			tmp_r.x = iter->x;
			tmp_r.y = iter->y;
			tmp_r.w = snake_rect.w;
			tmp_r.h = snake_rect.h;
			SDL_RenderCopy(g_renderer, body, &snake_rect, &tmp_r);
		}
	}if(isPlay){

		TTF_Font* font = TTF_OpenFont("../../Resources/DNFBitBitv2.ttf", 22);

		if (iscrushed == false) {
			SDL_Rect tmp_r;
			tmp_r.x = apples[1].x;
			tmp_r.y = apples[1].y;
			tmp_r.w = apple_rect.w;
			tmp_r.h = apple_rect.h;
			SDL_RenderCopy(g_renderer, apple, &apple_rect, &tmp_r);
		}


		SDL_Rect tmp_r;
		tmp_r.x = 40;
		tmp_r.y = 2;
		tmp_r.w = heart_rect.w * 0.3;
		tmp_r.h = heart_rect.h * 0.3;

		if (life == 3) {


			SDL_RenderCopy(g_renderer, heart_full, &heart_rect, &tmp_r);
		}
		else if (life == 2) {
			SDL_RenderCopy(g_renderer, heart_2, &heart_rect, &tmp_r);
		}
		else if (life == 1) {

			SDL_RenderCopy(g_renderer, heart_1, &heart_rect, &tmp_r);
		}
		else if (life == 0) {
			SDL_RenderCopy(g_renderer, heart_empty, &heart_rect, &tmp_r);
		}

		{
			SDL_Rect tmp_r1;
			tmp_r1.x = 40 * 4;
			tmp_r1.y = 0;
			tmp_r1.w = apple_rect.w * 0.8;
			tmp_r1.h = apple_rect.h * 0.8;
			SDL_RenderCopy(g_renderer, apple, &apple_rect, &tmp_r1);
		}

		
		//×text
		{
			std::string item_s;
			item_s = std::to_string(eatItems);

			SDL_Surface* tmp2_surface = TTF_RenderUTF8_Blended(font, item_s.c_str(), pink);
			item_rect.x = 0;
			item_rect.y = 0;
			item_rect.w = tmp2_surface->w;
			item_rect.h = tmp2_surface->h;
			item_txt = SDL_CreateTextureFromSurface(g_renderer, tmp2_surface);
			SDL_FreeSurface(tmp2_surface);



		}

		{
			SDL_Rect tmp_r2;
			tmp_r2.x = 200;
			tmp_r2.y = 0;
			tmp_r2.w = x_rect.w;
			tmp_r2.h = x_rect.h;
			SDL_RenderCopy(g_renderer, x_text, &x_rect, &tmp_r2);
		}

		{
			SDL_Rect tmp_r2;
			tmp_r2.x = 225;
			tmp_r2.y = 0;
			tmp_r2.w = item_rect.w;
			tmp_r2.h = item_rect.h;
			SDL_RenderCopy(g_renderer, item_txt, &item_rect, &tmp_r2);
		}

		{
			SDL_Rect tmp_r2;
			tmp_r2.x = 280;
			tmp_r2.y = 0;
			tmp_r2.w = score_rect.w;
			tmp_r2.h = score_rect.h;
			SDL_RenderCopy(g_renderer, score_txt, &score_rect, &tmp_r2);
		}

		{ //score_s texture
			
			SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, score_s.c_str(), white);
			//std::to_string(score).c_str()
			score_s_rect.x = 0;
			score_s_rect.y = 0;
			score_s_rect.w = tmp_surface->w;
			score_s_rect.h = tmp_surface->h;
			score_s_txt = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
			SDL_FreeSurface(tmp_surface);

		}
		{
			SDL_Rect tmp_r2;
			tmp_r2.x = 385;
			tmp_r2.y = 0;
			tmp_r2.w = score_s_rect.w;
			tmp_r2.h = score_s_rect.h;
			SDL_RenderCopy(g_renderer, score_s_txt, &score_s_rect, &tmp_r2);
		}
		TTF_CloseFont(font);
		
	}
if (isPlay == false && isIntro == false) {
	{ //ending score
		TTF_Font* font = TTF_OpenFont("../../Resources/DNFBitBitv2.ttf", 72);
		SDL_Surface* tmp_surface = TTF_RenderUTF8_Blended(font, score_s.c_str(), Green);
		//std::to_string(score).c_str()
		end_score_rect.x = 0;
		end_score_rect.y = 0;
		end_score_rect.w = tmp_surface->w;
		end_score_rect.h = tmp_surface->h;
		end_score = SDL_CreateTextureFromSurface(g_renderer, tmp_surface);
		SDL_FreeSurface(tmp_surface);
		TTF_CloseFont(font);

	}
	{
		SDL_Rect tmp_r2;
		tmp_r2.x = 630;
		tmp_r2.y = 388;
		tmp_r2.w = end_score_rect.w;
		tmp_r2.h = end_score_rect.h;
		SDL_RenderCopy(g_renderer, end_score, &end_score_rect, &tmp_r2);
	}

	}
	
	
	SDL_RenderPresent(g_renderer);
}

void snakeUI::HandleEvents() {
	//std::cout << lastkey << " " << nowkey << std::endl;
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		if (isPlay == false && isIntro == false) {
		if (event.type == SDL_MOUSEBUTTONDOWN) {
			if (event.button.button == SDL_BUTTON_LEFT) {
				restart();
				isIntro = true;
				score = 0;
				eatItem = 0;
				eatItems = 0;
			}
		}

	}
		switch (event.type) {

		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_KEYDOWN:
			if (isPlay && keyboard) {
				if (nowkey != "Right" && event.key.keysym.sym == SDLK_LEFT) { //횡이동 금지

					lastkey = nowkey;
					nowkey = "Left";

					isup = false;
					isdown = false;
					isleft = true;
					isright = false;
				}
				else if ( nowkey != "Left" && event.key.keysym.sym == SDLK_RIGHT) {
					lastkey = nowkey;
					nowkey = "Right";
					isup = false;
					isdown = false;
					isleft = false;
					isright = true;
				}
				else if ( nowkey != "Down" && event.key.keysym.sym == SDLK_UP) {
					lastkey = nowkey;
					nowkey = "Up";
					isup = true;
					isdown = false;
					isleft = false;
					isright = false;
				}
				else if ( nowkey != "Up" && event.key.keysym.sym == SDLK_DOWN) {
					lastkey = nowkey;
					nowkey = "Down";
					isup = false;
					isdown = true;
					isleft = false;
					isright = false;
				}
				break;
			}
			else if (isPlay == false && isIntro == true) {
				if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_DOWN) {
					isPlay = true;
					isIntro = false;
					Mix_HaltMusic();
					Mix_VolumeMusic(40);
					Mix_PlayMusic(play_music, -1);
				}
			}
			

		default: break;
		}
	}
}

void snakeUI::Update() {
	
	if (life == 0) {
		
		
	}

	if (isPlay) {

		srand(time(0));
		newX = rand() % 28;
		newY = rand() % 18;

		sec++;
		auto iter = Nodes.begin();
		//std::cout << lastkey << std::endl;

		crush();
		eat();
		//std::cout << eatItems2 << "  " << speed << std::endl;

		if (eatItems2 >= 0 && eatItems2 < 3) { speed = 8; }
		else if (eatItems >= 3 && eatItems2 < 6) { speed = 6; }
		else if (eatItems2 >= 6 && eatItems2 < 10) { speed = 5; }
		else if (eatItems2 >= 10 && eatItems2 < 15) { speed = 4; }
		else { speed = 3; }
		//std::cout << sec << std:: endl;
		//sec%33 = 1초
		if (sec % speed == 0) {
			if (keyboard) {
				
				//std::cout << g_last_time_ms << std::endl;
				if (isleft) {
					if (eatItem) {
						Nodes.push_front({ Nodes.front().x / S_partSize - 1, Nodes.front().y / S_partSize });
						eatItem = false;
					}
					else if (eatItem == false) {

						{
							Nodes.push_front({ Nodes.front().x / S_partSize - 1, Nodes.front().y / S_partSize });
							Nodes.pop_back();
						}
					}
				}
				else if (isright) {
					if (eatItem) {
						Nodes.push_front({ Nodes.front().x / S_partSize + 1, Nodes.front().y / S_partSize });
						eatItem = false;
					}
					else if (eatItem == false) {

						{
							Nodes.push_front({ Nodes.front().x / S_partSize + 1, Nodes.front().y / S_partSize });
							Nodes.pop_back();
						}
					}
				}
				else if (isup) {
					if (eatItem) {
						Nodes.push_front({ Nodes.front().x / S_partSize, Nodes.front().y / S_partSize - 1 });
						eatItem = false;
					}
					else if (eatItem == false) {

						{
							Nodes.push_front({ Nodes.front().x / S_partSize, Nodes.front().y / S_partSize - 1 });
							Nodes.pop_back();
						}
					}
				}
				else if (isdown) {
					if (eatItem) {
						Nodes.push_front({ Nodes.front().x / S_partSize, Nodes.front().y / S_partSize + 1 });
						eatItem = false;
					}
					else if (eatItem == false) {

						{
							Nodes.push_front({ Nodes.front().x / S_partSize, Nodes.front().y / S_partSize + 1 });
							Nodes.pop_back();
						}
					}
				}
				//벽과 충돌검사
				if (Nodes.front().x < leftX || Nodes.front().x > rightX || Nodes.front().y < upY || Nodes.front().y > downY) {

					if (life > 1) {
						reset();
						isPlay = false;
						SDL_Delay(1000);
						isPlay = true;
						keyboard = true;

					}
					else {
						isup = false;
						isdown = false;
						isright = false;
						isleft = false;
						life--;
						keyboard = false;
						isPlay = false; 
						
						Mix_PlayChannel(-1, death_sound, 0);
						SDL_Delay(1000);
					}
				

				}
				else { //몸과 충돌검사

					for (iter++; iter != Nodes.end(); iter++) {
						if (Nodes.front().x == iter->x && Nodes.front().y == iter->y) {
							if (life > 1) {
								reset();
								isPlay = false;
								SDL_Delay(1000);
								keyboard = true;
								isPlay = true;
								break;
							}
							else {
								isup = false;
								isdown = false;
								isright = false;
								isleft = false;
								life--;
								keyboard = false;
								isPlay = false; 
								
								Mix_PlayChannel(-1, death_sound, 0);
								SDL_Delay(1000);
							}
						}

					}
				}
			}
		}
		
	}
	
	
}

void snakeUI::reset() {
	isup = false;
	isdown = false;
	isleft = false;
	isright = false;
	keyboard = false;
	Mix_PlayChannel(-1, death_sound, 0);
	
	
	nowkey = "None";
	lastkey = "None";

	speed = 8;

	eatItems2 = 0;

	auto iter = Nodes.begin();

	//body 
	if (life > 0) {
		for (iter; iter != Nodes.end(); iter++)
		{
			Nodes.erase(iter);
		}
	}

	newX = rand() % 28;
	newY = rand() % 18;
	apples.pop_back();
	apples.push_back({ newX + 1,newY + 1 });


	iscrushed = false;
	eatItem = false;


	life--;

	Nodes.push_back({ 14,9 });

	lastkey = "Right";
	nowkey = "Right";
	isright = true;
	
	
}

void snakeUI::restart() {
	Mix_HaltMusic();
	Mix_VolumeMusic(80);
	Mix_PlayMusic(intro_music, -1);

	isPlay = false;
	isIntro = false;
	isup = false;
	isdown = false;
	isleft = false;
	isright = true;
	keyboard = true;

	nowkey = "None";
	lastkey = "None";

	speed = 8;
	life = 3;
	score = 0;
	eatItems = 0;
	eatItems2 = 0;

	auto iter = Nodes.begin();

	//body 
	if (life > 0) {
		for (iter; iter != Nodes.end(); iter++)
		{
			Nodes.erase(iter);
		}
	}

	newX = rand() % 28;
	newY = rand() % 18;
	apples.pop_back();
	apples.push_back({ newX + 1,newY + 1 });

	Nodes.push_back({ 14,9 });
	iscrushed = false;
	eatItem = false;

	

	lastkey = "Right";
	nowkey = "Right";
}




void snakeUI::crush() {
	
	//std::cout << randX << " " << randY << std::endl;
	//std::cout << iscrushed << std::endl;

	auto iter = Nodes.begin();
	for (iter; iter != Nodes.end(); iter++)
	{
		if (randX + 1 == iter->x) { iscrushed = true; }
		else if(randX + 1 != iter->x) { iscrushed = false; }
		
		if (randY + 1 == iter->y) { iscrushed = true; }
		else if(randY + 1 != iter->y) { iscrushed = false; }
	}

	
}

void snakeUI::eat() {
	if (apples[1].x == Nodes.begin()++->x && apples[1].y == Nodes.begin()++->y) {
		Mix_PlayChannel(-1, eat_sound, 0);
		eatItem = true;
		srand(time(0));
		newX = rand() % 28;
		newY = rand() % 18;
		apples.pop_back();
		apples.push_back({ newX + 1,newY + 1 });
		eatItems++;
		eatItems2++;
		score += 10;
	}
	
}








