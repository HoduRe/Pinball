#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "SDL/include/SDL.h"
#include "Box2D/Box2D/Box2D.h"

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
// Blit functions
	void LowStageBlit();
	void HighStageBlit();
	void TitleBlit();
// Charge functions
	void ChargeLowStage();
	void ChargeHighStage();
	void CreateFlicker();

public:
	SDL_Texture* scene;
	PhysBody* player_circle;
	PhysBody* circle1;
	PhysBody* circle2;
	PhysBody* flicker1;
	PhysBody* flicker2;
	b2RevoluteJoint* revolute_joint_left;
	b2RevoluteJoint* revolute_joint_right;
	SDL_Rect scene_rect;
	SDL_Rect elements_rect;
	stage_state stage;
	stage_state buffer_stage;
	uint bonus_fx;
	bool cien = false;
	uint score = 0u;

	//Lists to storage sensors of the elements for the score
	p2List<PhysBody*> scoreSensors;	
};
