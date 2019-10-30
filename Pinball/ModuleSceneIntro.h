#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "SDL/include/SDL.h"

enum stage_state {
	ST_UNKNOWN,
	ST_TITLE_SCREEN,
	ST_LOW_STAGE,
	ST_HIGH_STAGE,
	ST_BONUS_STAGE
};

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);

public:
	p2List<PhysBody*> circles;
	p2List<PhysBody*> boxes;
	p2List<PhysBody*> ricks;

	PhysBody* sensor;
	bool sensed;

	SDL_Rect a{ 10,10,10,10 };

	SDL_Texture* circle;
	SDL_Texture* box;
	SDL_Texture* rick;
	SDL_Texture* level;
	SDL_Texture* title_stage;
	SDL_Rect scene_rect;
	stage_state stage;
	uint bonus_fx;
	p2Point<int> ray;
	bool ray_on;
};
