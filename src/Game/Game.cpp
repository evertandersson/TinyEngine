#include "Game.h"
#include "Actor.h"
#include "Player.h"
#include "Enemy.h"
#include "Math/AABB.h"
#include "Math/Math.h"
#include "PickUp.h"

Game* game = nullptr;

Game::Game()
{
	actors[0] = new Player(Vector(100.f, 100.f));
	player = actors[0];

	actors[1] = new Enemy(Vector(600.f, 250.f));

	last_spawn_time = engCurrentTime();
}

void Game::update()
{
	if (engTimePassedSince(last_spawn_time) > SPAWN_INTERVAL)
	{
		if (Enemy::NUM_ENEMIES < 20)
		{
			float angle = engRandomF() * Math::TAU;
			Vector offset = Vector(cosf(angle), sinf(angle)) * 1000.f;

			spawn_actor(new Enemy(player->position + offset));
		}
		last_spawn_time = engCurrentTime();
	}

	if (engTimePassedSince(last_pickup_spawn_time) > PICKUP_INTERVAL && player != nullptr) {
		float angle = engRandomF() * Math::TAU;
		Vector offset = Vector(cosf(angle), sinf(angle)) * 300.f;

		spawn_actor(new PickUp(player->position + offset));
		last_pickup_spawn_time = engCurrentTime();
	}

	for (int i = 0; i < MAX_ACTORS; ++i)
	{
		if (actors[i] != nullptr)
		{
			actors[i]->update();
		}
	}

	for (int i = 0; i < MAX_ACTORS; ++i)
	{
		if (actors[i] == nullptr)
			continue;

		if (actors[i]->get_is_destroyed())
		{
			if (actors[i] == player)
			{
				player = nullptr;
			}

			delete actors[i];
			actors[i] = nullptr;
		}
	}

	camera.update();
}

void Game::render()
{
	for (int x = -50; x <= 50; ++x)
	{
		for (int y = -50; y <= 50; ++y)
		{
			if ((x + y) % 2 == 0)
			{
				engSetDrawColor(0x2c1d36);
			}
			else
				engSetDrawColor(0x88679e);

			Vector position = Vector(x * GRID_SIZE, y * GRID_SIZE);
			position = camera.world_to_screen(position);

			engFillRect(position.x, position.y, GRID_SIZE, GRID_SIZE);
		}
	}
	for (int i = 0; i < MAX_ACTORS; ++i)
	{
		if (actors[i] != nullptr)
		{
			actors[i]->draw();
		}
	}
}

void Game::spawn_actor(Actor* actor)
{
	for (int i = 0; i < MAX_ACTORS; ++i)
	{
		if (actors[i] == nullptr)
		{
			actors[i] = actor;
			break;
		}
	}
}

Actor* Game::get_colliding_actor(Actor* other, Collision_Channel channel)
{
	for (int i = 0; i < MAX_ACTORS; ++i)
	{
		if (actors[i] == other)
			continue;

		if (actors[i] == nullptr)
			continue;

		if (actors[i]->collision_channel != channel)
			continue;

		AABB a = AABB::from_position_size(other->position, other->size);
		AABB b = AABB::from_position_size(actors[i]->position, actors[i]->size);

		if (aabb_overlap(a, b))
		{
			return actors[i];
		}
	}

	return nullptr;
}
