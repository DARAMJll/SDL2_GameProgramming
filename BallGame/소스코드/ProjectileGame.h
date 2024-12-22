#pragma once

#include "SDL.h"
#include "Room.h"
#include "Ball.h"
#include <vector>

class ProjectileGame
{
public:
	ProjectileGame();
	~ProjectileGame();
	void Render();	
	void Update();
	void HandleEvents();

	

protected:
	void AddNewBall();
	bool CheckNetCollisionForPrediction(const Ball& ball, double& predicted_pos_x, double& predicted_pos_y, double& predicted_vel_x);
	bool CheckNetCollisionForReal(const Ball& ball); //update문에서 사용
	void HandleNetCollision(Ball& ball);

protected:

	// Room
	Room room_;


	// Balls
	std::vector<Ball> balls_;
	
	// Ball Texture
	SDL_Texture* ball_texture_; 
	SDL_Rect ball_src_rectangle_;

	// Mouse
	int mouse_win_x_;
	int mouse_win_y_;

	//ball size
	bool isSmall;
	
	//네트 위치 정의
	//Vector2D net_start = { room.vertiacal_fence_pos_x(), 0.0}; 
	//Vector2D net_point = {0.0, room.vertiacal_fence_height()};
};

class Vector2D {
public: float x_, y_;
};