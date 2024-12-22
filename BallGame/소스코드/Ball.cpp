
#include "Ball.h"
#include "SDL_image.h"
#include <iostream>




Ball::Ball(double radius, Room *room)
{
	room_ = room;
	radius_ = radius;

	
	v_[0] = 0;
	v_[1] = 0;

	mass_ = 2; // 2kg
	coeff_of_restitution_ = 0.7; //반발계수
	friction = 0.25; //마찰계수 초기화

	Reset();
}

void
Ball::Reset()
{
	p_[0] = radius_ + room_->left_wall_x();
	p_[1] = radius_;

	v_[0] = 0;
	v_[1] = 0;
}

void
Ball::Launch(double initial_force_x, double initial_force_y)
{//순간ㅏ속도
	v_[0] = v_[0] + (initial_force_x/mass_);
	v_[1] = v_[1] + (initial_force_y/mass_);
}

void 
Ball::Update(double timestep_s)
{
	double dt = timestep_s;	// seconds
	
	// 가속도
	double a[2];
	a[0] = 0;
	a[1] = room_->gravitational_acc_y();// Gravity

	// Move
	p_[0] = p_[0] + dt * v_[0]; //현재위치 + 거리
	p_[1] = p_[1] + dt * v_[1];


	// Collision with Ground 검사
	if ( p_[1]-radius_ < room_->ground_height() && v_[1] < 0 ) 
	{
		p_[1] = radius_ + room_->ground_height();

		v_[1] = -1*v_[1];

		// Coefficient of restitution
		v_[1] = coeff_of_restitution_ * v_[1];
	}

	// Collision with Ceilling
	else if ( p_[1]+radius_ > room_->height() && v_[1] > 0 ) 
	{
		p_[1] = room_->height() - radius_;

		v_[1] = -1*v_[1];

		// Coefficient of restitution
		v_[1] = coeff_of_restitution_ * v_[1];
	}

	// Collision with Left Wall
	if ( p_[0] - radius_ < room_->left_wall_x() && v_[0] < 0 )
	{
		p_[0] = room_->left_wall_x() + radius_;

		v_[0] = -1 * v_[0];

		// Coefficient of restitution
		v_[0] = coeff_of_restitution_ * v_[0];
	}

	// Collision with Right Wall
	else if (p_[0] + radius_ > room_->right_wall_x() && v_[0] > 0)
	{
		p_[0] = room_->right_wall_x() - radius_;

		v_[0] = -1 * v_[0];

		// Coefficient of restitution
		v_[0] = coeff_of_restitution_ * v_[0];
	}//다른 if 문 해서 네트 부딪힘 구현

	//속도 = 이전속도 + 시간(dt) * 가속도;
	v_[0] = v_[0] + dt * a[0];
	v_[1] = v_[1] + dt * a[1];

	//마찰력 구현
	if (p_[1] - radius_ <= room_->ground_height()) //바닥일때
	{
		// 마찰력 계산
		double friction_force = friction * mass_ * -room_->gravitational_acc_y();
		double friction_acceleration = friction_force / mass_;

		if (v_[0] > 0) //공이 오른쪽으로 움직임
		{
			v_[0] -= friction_acceleration * dt; //속도 감소
			if (v_[0] < 0) v_[0] = 0; 
		}
		else if (v_[0] < 0) //왼쪽
		{
			v_[0] += friction_acceleration * dt; //속도 감소
			if (v_[0] > 0) v_[0] = 0;
		}
	}
	
}
