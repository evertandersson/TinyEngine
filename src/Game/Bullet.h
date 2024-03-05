#pragma once
#include "Actor.h"

class Bullet : public Actor
{
public:
	Bullet(Vector position, Vector direction);
	void update() override;
	float spawn_time = 10.f;

private:
	static constexpr float SPEED = 1000.f;
	static constexpr float BULLET_LIFETIME = 2.f;
	static constexpr int DAMAGE = 40;

	Vector direction;
};
