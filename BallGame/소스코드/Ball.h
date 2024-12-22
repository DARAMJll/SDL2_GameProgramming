#pragma once

#include "SDL.h"
#include "Room.h"


class Ball
{
public:
	Ball(double radius, Room* room);

	void Reset();
	//�߿�
	virtual void Update(double timestep_s);
	void Launch(double initial_force_x, double initial_force_y);
	//

	void velocity(double x, double y) { v_[0] = x; v_[1] = y; }
	void setRadius(double radius) { radius_ = radius; } //������ ����
	void setPosition(double x, double y) { p_[0] = x; p_[1] = y; } //��ġ ����

	double radius() const { return radius_; }
	double mass() const { return mass_; }
	double coeff_of_restitution() { return coeff_of_restitution_; }

	double pos_x() const { return p_[0]; }
	double pos_y() const { return p_[1]; }
	double velocity_x() const { return v_[0]; } // x �ӵ�
	double velocity_y() const { return v_[1]; } // y �ӵ�

	double coeff_of_restitution_; // from 0 o 1

protected:
	Room* room_;
	
	double radius_;	// meter
	double mass_;	// kg
	
	double friction; //�������

	// position
	double p_[2];	// x, y

	// velocity
	double v_[2];	// x, y
};

