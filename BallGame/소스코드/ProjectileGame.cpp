#include <iostream>

#include "ProjectileGame.h" 
#include "SDL_image.h"
#include "G2W.h"
#include "math.h"

extern int g_current_game_phase;
extern bool g_flag_running;
extern SDL_Renderer* g_renderer;
extern SDL_Window* g_window;
extern double g_timestep_s;



ProjectileGame::ProjectileGame()
{
	g_flag_running = true;

	// Texture
	{
		SDL_Surface* ball_surface = IMG_Load("../../Resources/ball.png");
		ball_src_rectangle_.x = 0;
		ball_src_rectangle_.y = 0;
		ball_src_rectangle_.w = ball_surface->w;
		ball_src_rectangle_.h = ball_surface->h;

		ball_texture_ = SDL_CreateTextureFromSurface(g_renderer, ball_surface);
		SDL_FreeSurface(ball_surface);
	}

	mouse_win_x_ = 0;
	mouse_win_y_ = 0;

	isSmall = false;

	AddNewBall();
}



ProjectileGame::~ProjectileGame()
{
	SDL_DestroyTexture(ball_texture_);
}


void
ProjectileGame::AddNewBall()
{ //마지막 인덱스: 발사대기상태
	if (isSmall) {
		balls_.push_back(Ball(0.055f, &room_));
	}
	else {
		balls_.push_back(Ball(0.11f, &room_));
	}
}


void
ProjectileGame::Update()
{

	// Update balls
	for (Ball& b : balls_) //벡터나 for 문  하나씩 가져와서.. 
	{
		b.Update(g_timestep_s);

		// 네트와의 충돌 체크
		if (CheckNetCollisionForReal(b))
		{
			HandleNetCollision(b);
		}
	}


}

void 
ProjectileGame::Render()
{
	SDL_SetRenderDrawColor(g_renderer, 255,255,255,255);
	SDL_RenderClear(g_renderer); // clear the renderer to the draw color
	
	
	// Draw room_
	{
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 0, 255);

		// Left Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
										G2W_Y(0), 
										G2W_X(room_.left_wall_x()),
										G2W_Y(room_.height()) );

		
		// Right Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.right_wall_x()),
										G2W_Y(0), 
										G2W_X(room_.right_wall_x()),
										G2W_Y(room_.height()) );

		// Top Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
										G2W_Y(room_.height()), 
										G2W_X(room_.right_wall_x()),
										G2W_Y(room_.height()) );

		// Bottom Wall
		SDL_RenderDrawLine(g_renderer, G2W_X(room_.left_wall_x()),
										G2W_Y(0), 
										G2W_X(room_.right_wall_x()),
										G2W_Y(0) );

		// Fence
		SDL_RenderDrawLine(g_renderer, G2W_X( room_.vertiacal_fence_pos_x() ), 
										G2W_Y(0), 
										G2W_X( room_.vertiacal_fence_pos_x()  ),
										G2W_Y( room_.ground_height()+room_.vertiacal_fence_height() ) );
	}


	// Draw Balls
	for ( Ball &b : balls_ )
	{

		int ball_win_x = G2W_X(b.pos_x());
		int ball_win_y = G2W_Y(b.pos_y());

		double win_radius = G2W_Scale * b.radius();

		SDL_Rect dest_rect;
		dest_rect.w = (int)(2*win_radius);
		dest_rect.h = (int)(2*win_radius);
		dest_rect.x = (int)(ball_win_x - win_radius);
		dest_rect.y = (int)(ball_win_y - win_radius);

		SDL_RenderCopy(g_renderer, ball_texture_, &ball_src_rectangle_, &dest_rect);
	}

	
	
	// Draw the Guide Line //사진참고(코드가 질량 고려 x )
	if (balls_.size() > 0)
	{
		SDL_SetRenderDrawColor(g_renderer, 255, 0, 0, 100);
		SDL_RenderDrawLine(g_renderer, G2W_X(balls_.back().pos_x()), 
										G2W_Y(balls_.back().pos_y()),
										mouse_win_x_, 
										mouse_win_y_ );
	}

	// 예측경로
	if (balls_.size() > 0)
	{
		SDL_SetRenderDrawColor(g_renderer, 0, 0, 255, 100);

		double mouse_game_x = W2G_X(mouse_win_x_);
		double mouse_game_y = W2G_Y(mouse_win_y_);
		double guide_line_x = mouse_game_x - balls_.back().pos_x();
		double guide_line_y = mouse_game_y - balls_.back().pos_y();
		double launching_force_x = 8.0 * guide_line_x;
		double launching_force_y = 8.0 * guide_line_y;

		double predicted_pos_x = balls_.back().pos_x();
		double predicted_pos_y = balls_.back().pos_y();
		double predicted_vel_x = launching_force_x / balls_.back().mass();
		double predicted_vel_y = launching_force_y / balls_.back().mass();

		double time_1s = 1.0;  // 1초 동안의 예측 경로
		double time_elapsed = 0.0;

		while (time_elapsed < time_1s)
		{
			predicted_pos_x += predicted_vel_x * g_timestep_s;
			predicted_pos_y += predicted_vel_y * g_timestep_s;
			predicted_vel_y += room_.gravitational_acc_y() * g_timestep_s;


			// 충돌:땅
            if (predicted_pos_y - balls_.back().radius() < room_.ground_height())
            {
                predicted_pos_y = room_.ground_height() + balls_.back().radius();
                predicted_vel_y = -predicted_vel_y * balls_.back().coeff_of_restitution();
            }

            // 충돌:천장
            if (predicted_pos_y + balls_.back().radius() > room_.height())
            {
                predicted_pos_y = room_.height() - balls_.back().radius();
                predicted_vel_y = -predicted_vel_y * balls_.back().coeff_of_restitution();
            }

            // 충돌:왼쪽 벽
            if (predicted_pos_x - balls_.back().radius() < room_.left_wall_x())
            {
                predicted_pos_x = room_.left_wall_x() + balls_.back().radius();
                predicted_vel_x = -predicted_vel_x * balls_.back().coeff_of_restitution();
            }

            // 충돌:오른쪽 벽
            if (predicted_pos_x + balls_.back().radius() > room_.right_wall_x())
            {
                predicted_pos_x = room_.right_wall_x() - balls_.back().radius();
                predicted_vel_x = -predicted_vel_x * balls_.back().coeff_of_restitution();
            }

			// 충돌: 네트
			if (CheckNetCollisionForPrediction(balls_.back(), predicted_pos_x, predicted_pos_y, predicted_vel_x))
			{
				predicted_vel_x = -predicted_vel_x * balls_.back().coeff_of_restitution();
			

			}

			SDL_RenderDrawPoint(g_renderer, G2W_X(predicted_pos_x), G2W_Y(predicted_pos_y));

			// draw a circle at the predicted position
			SDL_SetRenderDrawColor(g_renderer, 0, 0, 255, 100);
			SDL_Rect dest_rect;
			dest_rect.w = 10;
			dest_rect.h = 10;
			dest_rect.x = G2W_X(predicted_pos_x) - 1;
			dest_rect.y = G2W_Y(predicted_pos_y) - 1;
			SDL_RenderFillRect(g_renderer, &dest_rect);

			time_elapsed += g_timestep_s;
		}
	}

	SDL_RenderPresent(g_renderer); // draw to the screen
}



void ProjectileGame::HandleEvents()
{
	SDL_Event event;
	if(SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			g_flag_running = false;
			break;

		case SDL_MOUSEBUTTONDOWN:
		
			// If the mouse left button is pressed. 
			if ( event.button.button == SDL_BUTTON_LEFT )
			{
				// Get the cursor's x position.
				mouse_win_x_ = event.button.x;
				mouse_win_y_ = event.button.y;

				
				double mouse_game_x = W2G_X(mouse_win_x_); // 게임 좌표로 변환
				double mouse_game_y = W2G_Y(mouse_win_y_);
				

				
				// Luanch
				if ( balls_.size() > 0)
				{
					// Guide Line Vector
					double guide_line_x = mouse_game_x - balls_.back().pos_x(); //공에서 마우스 위치까지 벡터
					double guide_line_y = mouse_game_y - balls_.back().pos_y();

					// Lauching Force
					double launcing_force_x = 8.0 * guide_line_x;
					double launcing_force_y = 8.0 * guide_line_y;//발사할 때 힘

					// Launch
					balls_.back().Launch(launcing_force_x, launcing_force_y);


					// Add a new ball for the next //왼쪽 모서리에 나타남
					AddNewBall();
				}
			}

		

		case SDL_MOUSEMOTION:
			{
				// Get the cursor's x position.
				mouse_win_x_ = event.button.x;
				mouse_win_y_ = event.button.y;

				
				
			}
			break;

		case SDL_KEYDOWN: 
			if (event.key.keysym.sym == SDLK_2) { //공 크기 축소
				isSmall = true;
				if (!balls_.empty())
				{
					balls_.back().setRadius(0.055f);
					balls_.back().setPosition(room_.left_wall_x() + 0.055f, 0.055f); //모서리에 맞춤
				}
			}
			if (event.key.keysym.sym == SDLK_1) { //공 크기 원래대로
				isSmall = false;
				if (!balls_.empty())
				{
					balls_.back().setRadius(0.11f);
					balls_.back().setPosition(room_.left_wall_x() + 0.11f, 0.11f);
				}
			}


		default:
			break;
		}
	}


	
}

bool ProjectileGame::CheckNetCollisionForPrediction(const Ball& ball, double& predicted_pos_x, double& predicted_pos_y, double& predicted_vel_x)
{
	double radius = ball.radius();
	//double ball_x = ball.pos_x();
	//double ball_y = ball.pos_y();

	//네트 위치
	double net_x = room_.vertiacal_fence_pos_x();
	double net_height = room_.vertiacal_fence_height();

	// 공이 네트와 수평으로 겹치는지
	if (predicted_pos_x + radius > net_x && predicted_pos_x - radius < net_x)
	{
		// 공이 네트 꼭짓점 근처에 있는지
		if (predicted_pos_y - radius  < net_height && predicted_pos_y + radius > 0)
		{

			//double ball_vel_x = ball.velocity_x();
			//predicted_vel_x = -ball_vel_x * ball.coeff_of_restitution();

			// 공의 위치를 네트 바깥으로 조정
			if (ball.pos_x() > net_x)
			{
				predicted_pos_x = net_x + radius;
			}
			else
			{
				predicted_pos_x = net_x - radius;
			}
			return true;
		}
	}
	return false;
}

bool ProjectileGame::CheckNetCollisionForReal(const Ball& ball)
{
	double radius = ball.radius();
	double ball_x = ball.pos_x();
	double ball_y = ball.pos_y();

	//네트 위치
	double net_x = room_.vertiacal_fence_pos_x();
	double net_height = room_.vertiacal_fence_height();

	// 공이 네트와 수평으로 겹치는지
	if (ball_x + radius > net_x && ball_x - radius < net_x)
	{
		// 공이 네트 꼭짓점 근처에 있는지
		if (ball_y - radius < net_height && ball_y + radius > 0)
		{
			return true;
		}
	}
	return false;
}

void ProjectileGame::HandleNetCollision(Ball& ball)
{
	double net_x = room_.vertiacal_fence_pos_x();

	// 공의 속도를 반사
	double ball_x2 = ball.velocity_x();
	ball.velocity(-ball_x2 * ball.coeff_of_restitution(), ball.velocity_y());

	// 공의 위치를 네트 바깥으로 조정
	if (ball.pos_x() > net_x)
	{
		ball.setPosition(net_x + ball.radius(), ball.pos_y());
	}
	else
	{
		ball.setPosition(net_x - ball.radius(), ball.pos_y());
	}
}

