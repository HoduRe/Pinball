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
enum plat_state
{
	ST_LEFT,
	ST_RIGHT,
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
	void CreatePlayer(float x, float y);
//Score update
	void ScoreUpdater(uint s);
// Checks special one way colliders
	void CheckOneWayColliders();

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
	bool generate_player = false;
	
	
	PhysBody* loseSensor;


	//elements for the pulsator
	PhysBody* pulsator;
	b2Vec2 p;

	//Moving platform
	SDL_Texture* plattform_tex;
	PhysBody* plat;
	float pinkPlattformPos = 115; //we wont change Y pos
	void pinkPlatformUpdate();
	bool Rightlimit = false;
	bool Leftlimit = true;

	b2Vec2 pVel;


	plat_state pstate;
	
	//FX
	uint bonus_fx;
	uint bumper_hit;
	uint yellowSquare_hit;
	uint tags_hit;
	uint numeric_hit;
	uint card_hit;
	uint egg_hit;
	
	
	//fonts
	int font_name;
	int font2;
	int fontblue;
	int fontlightblue;

	//score related variables
	char score_text[10];
	uint score = 0u;

	char topscore_text[10];
	uint topscore = 0u;

	char ball_text[10];
	uint ball = 02;

	uint pulsatorUP = 100u;
	char pulsatorUP_text[10];

	//------SENSORS------//
			
	//score sensors
	p2List<PhysBody*> hundred_scoreSensors;
	
	p2List<PhysBody*> bumperSensors;
	p2List<PhysBody*> yellowSquareSensors;
	p2List<PhysBody*> tagSensors;
	p2List<PhysBody*> cardSensors;
	p2List<PhysBody*> variableSensors;
	PhysBody* pinkPlattformSensor;
	
	p2List<PhysBody*> fivehundred_scoreSensors;	
	p2List<PhysBody*> thousand_scoreSensors;
	
};
