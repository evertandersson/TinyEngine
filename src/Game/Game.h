#pragma once
#include "Camera.h"

#define MAX_ACTORS 200

// Forward-declare
class Actor;
enum class Collision_Channel;

class Game
{
public:
	Game();

	void update();
	void render();

	Actor* get_player() { return player; }
	Camera& get_camera() { return camera; }

	void spawn_actor(Actor* actor);

	Actor* get_colliding_actor(Actor* other, Collision_Channel channel);

private:
	static constexpr float SPAWN_INTERVAL = 0.5f;
	static constexpr float PICKUP_INTERVAL = 5.f;
	static constexpr float GRID_SIZE = 24.f;

	Actor* actors[MAX_ACTORS] = { nullptr };
	Actor* player = nullptr;

	Camera camera;

	float last_spawn_time = 0.f;
	float last_pickup_spawn_time = 0.f;
};

extern Game* game;
