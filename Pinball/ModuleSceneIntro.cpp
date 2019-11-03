#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"
#include "ModuleFonts.h"


#include <stdio.h>//for the sprintf_s function

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	scene = NULL;
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	bonus_fx = App->audio->LoadFx("pinball/bonus.wav");
	scene = App->textures->Load("pinball/level_elements.png");
	plattform_tex = App->textures->Load("pinball/plattform.png");
	
	font_name = App->fonts->Load("pinball/Font.png", "0123456789ABCDEFGHIJKLMNOPQRSTUWYZ+-", 1);
	font2 = App->fonts->Load("pinball/Font2.png", "0123456789", 1);
	fontblue = App->fonts->Load("pinball/Font-blue.png", " 123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1);
	fontlightblue = App->fonts->Load("pinball/Font-light-blue.png", " 123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ", 1);

	bumper_hit= App->audio->LoadFx("pinball/audio/bumper_hit.wav");
	yellowSquare_hit = App->audio->LoadFx("pinball/audio/yellowSquare_hit.wav");
	tags_hit = App->audio->LoadFx("pinball/audio/tags_hit.wav");
	numeric_hit = App->audio->LoadFx("pinball/audio/numeric_hit.wav");
	card_hit = App->audio->LoadFx("pinball/audio/card_hit.wav");
	egg_hit  = App->audio->LoadFx("pinball/audio/egg_hit.wav");

	stage = ST_TITLE_SCREEN;

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	App->textures->Unload(scene);

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{
	if (generate_player == true) {
		CreatePlayer(220, 150);
		generate_player = false;
	}

	// Player position update
	App->player->buffer_position = App->player->position;
	if (player_circle != NULL) {
		App->player->position.x = METERS_TO_PIXELS(player_circle->body->GetPosition().x) / SCREEN_SIZE - 5;
		App->player->position.y = METERS_TO_PIXELS(player_circle->body->GetPosition().y) / SCREEN_SIZE - 5;
	}
	CheckOneWayColliders();

	
	// Stage Print
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 15, true);
	}

	if (stage == ST_TITLE_SCREEN) {
		TitleBlit();
	}
	else if (stage == ST_LOW_STAGE) {
		if(buffer_stage == ST_TITLE_SCREEN){
			App->physics->MouseJointDestroy();
			App->physics->Disable();
			App->physics->Enable();
			CreatePlayer(220, 150);
			ChargeLowStage();
		}
		else if (buffer_stage == ST_HIGH_STAGE) {
			iPoint aux_position = App->player->position;
			App->physics->MouseJointDestroy();
			App->physics->Disable();
			App->physics->Enable();
			if (aux_position.x > 84 && aux_position.x < 95) {	// Weird bug fix (when changing map, gets stuck inside a collider)
				CreatePlayer(96, 12);
			}
			else{ CreatePlayer(aux_position.x + 1, 12); }
			ChargeLowStage();
		}
		LowStageBlit();
	}
	else if (stage == ST_HIGH_STAGE) {
		if (buffer_stage == ST_LOW_STAGE) {
			iPoint aux_position = App->player->position;
			App->physics->MouseJointDestroy();
			App->physics->Disable();
			App->physics->Enable();
			CreatePlayer(aux_position.x, 230);
			b2Vec2 kicker;
			kicker.y = -300;
			App->scene_intro->player_circle->body->ApplyForce(kicker, App->scene_intro->player_circle->body->GetWorldCenter(), true);
			ChargeHighStage();
		}
		HighStageBlit();
		pinkPlatformUpdate();
	}	

	if (stage != ST_TITLE_SCREEN)
	{
		//print of the top score
		sprintf_s(topscore_text, 10, "%7d", topscore);
		App->fonts->BlitText(5, 40, font_name, topscore_text);
		App->fonts->BlitText(20, 25, font_name, "TOP");

		//print of the current score
		sprintf_s(score_text, 10, "%7d", score);
		App->fonts->BlitText(5, 80, font_name, score_text);
		App->fonts->BlitText(20, 65, font_name, "1UP");

		//print of the number of balls
		sprintf_s(ball_text, 10, "%7d", ball);
		App->fonts->BlitText(35, 170, font_name, ball_text);
		App->fonts->BlitText(20, 155, font_name, "BALL");
	}
	

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

}

void ModuleSceneIntro::TitleBlit() {
	elements_rect.x = 8;
	elements_rect.y = 199;
	elements_rect.w = 192;
	elements_rect.h = 104;
	App->renderer->Blit(scene, 24, 24, &elements_rect);
	
	App->fonts->BlitText(60, 137, fontlightblue, "1 PLAYER GAME A");
	App->fonts->BlitText(60, 157, fontlightblue, "1 PLAYER GAME B");
	App->fonts->BlitText(60, 177, fontblue, "2 PLAYER GAME A");
	App->fonts->BlitText(60, 197, fontblue, "2 PLAYER GAME B");

}

void ModuleSceneIntro::LowStageBlit() {
	scene_rect.x = 233;	// Scene blit
	scene_rect.y = SCREEN_HEIGHT;
	scene_rect.w = SCREEN_WIDTH;
	scene_rect.h = SCREEN_HEIGHT;
	App->renderer->Blit(scene, 0, 0, &scene_rect);
	scene_rect.x = 117;	// Kicker blit
	scene_rect.y = 316 - (App->player->kicker_timer / 5);
	if (scene_rect.y <= 303) { scene_rect.y = 303; }
	scene_rect.w = 10;
	scene_rect.h = 44;
	App->renderer->Blit(scene, 218, 164, &scene_rect);
	scene_rect.x = 61;	// Orange circle blit
	scene_rect.y = 45;
	scene_rect.w = 22;
	scene_rect.h = 24;
	App->renderer->Blit(scene, 132, 101, &scene_rect);
	scene_rect.x = 12;	// Left pink circle blit
	scene_rect.y = 44;
	scene_rect.w = 22;
	scene_rect.h = 24;
	App->renderer->Blit(scene, 111, 67, &scene_rect);
	scene_rect.x = 12;	// Right pink circle blit
	scene_rect.y = 44;
	scene_rect.w = 22;
	scene_rect.h = 24;
	App->renderer->Blit(scene, 152, 67, &scene_rect);
	scene_rect.x = 9;	// First green card blit
	scene_rect.y = 79;
	scene_rect.w = 14;
	scene_rect.h = 24;
	App->renderer->Blit(scene, 104,	20, &scene_rect);
	scene_rect.x = 9;	// Second green card blit
	scene_rect.y = 79;
	scene_rect.w = 14;
	scene_rect.h = 24;
	App->renderer->Blit(scene, 120, 20, &scene_rect);
	scene_rect.x = 9;	// Third green card blit
	scene_rect.y = 79;
	scene_rect.w = 14;
	scene_rect.h = 24;
	App->renderer->Blit(scene, 136, 20, &scene_rect);
	scene_rect.x = 9;	// Fourth green card blit
	scene_rect.y = 79;
	scene_rect.w = 14;
	scene_rect.h = 24;
	App->renderer->Blit(scene, 152, 20, &scene_rect);
	scene_rect.x = 9;	// Fifth green card blit
	scene_rect.y = 79;
	scene_rect.w = 14;
	scene_rect.h = 24;
	App->renderer->Blit(scene, 168, 20, &scene_rect);
	scene_rect.x = 8;	// Tag 1 blit
	scene_rect.y = 109;
	scene_rect.w = 15;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 80, 55, &scene_rect);
	scene_rect.x = 8;	// Tag 2 blit
	scene_rect.y = 117;
	scene_rect.w = 15;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 80, 63, &scene_rect);
	scene_rect.x = 8;	// Tag 3 blit
	scene_rect.y = 125;
	scene_rect.w = 15;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 80, 71, &scene_rect);
	scene_rect.x = 8;	// Tag 4 blit
	scene_rect.y = 133;
	scene_rect.w = 15;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 80, 79, &scene_rect);
	scene_rect.x = 8;	// Tag 5 blit
	scene_rect.y = 141;
	scene_rect.w = 15;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 80, 87, &scene_rect);
	scene_rect.x = 8;	// Tag 6 blit
	scene_rect.y = 149;
	scene_rect.w = 15;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 80, 95, &scene_rect);
	scene_rect.x = 8;	// Tag 7 blit
	scene_rect.y = 157;
	scene_rect.w = 15;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 80, 103, &scene_rect);
	scene_rect.x = 60;	// Left egg blit
	scene_rect.y = 10;
	scene_rect.w = 9;
	scene_rect.h = 13;
	App->renderer->Blit(scene, 119, 149, &scene_rect);
	scene_rect.x = 60;	// Middle egg blit
	scene_rect.y = 10;
	scene_rect.w = 9;
	scene_rect.h = 13;
	App->renderer->Blit(scene, 137, 149, &scene_rect);
	scene_rect.x = 60;	// Right egg blit
	scene_rect.y = 10;
	scene_rect.w = 9;
	scene_rect.h = 13;
	App->renderer->Blit(scene, 155, 149, &scene_rect);
	scene_rect.x = 1;	
	scene_rect.y = 304;
	scene_rect.w = 51;
	scene_rect.h = 11;
	App->renderer->Blit(scene, 4, 39, &scene_rect); // topScore box blit
	App->renderer->Blit(scene, 4, 79, &scene_rect); // Score box blit
	scene_rect.x = 1;
	scene_rect.y = 316;
	scene_rect.w = 19;
	scene_rect.h = 11;
	App->renderer->Blit(scene, 34, 169, &scene_rect); //ball num blit
	if (flicker1->body->GetAngle() >= 0) {
		scene_rect.x = 31;
		scene_rect.y = 107;
		scene_rect.w = 29;
		scene_rect.h = 18;
		App->renderer->Blit(scene, 102, 193, &scene_rect); // Left flicker blit
	}
	else if (flicker1->body->GetAngle() > -45 * DEGTORAD) {
		scene_rect.x = 30;
		scene_rect.y = 131;
		scene_rect.w = 32;
		scene_rect.h = 10;
		App->renderer->Blit(scene, 102, 193, &scene_rect); // Left flicker blit
	}
	else if (flicker1->body->GetAngle() <= -45 * DEGTORAD) {
		scene_rect.x = 31;
		scene_rect.y = 150;
		scene_rect.w = 29;
		scene_rect.h = 18;
		App->renderer->Blit(scene, 102, 180, &scene_rect); // Left flicker blit
	}
	if (flicker2->body->GetAngle() <= 0) {
		scene_rect.x = 64;
		scene_rect.y = 308;
		scene_rect.w = 29;
		scene_rect.h = 18;
		App->renderer->Blit(scene, 156, 193, &scene_rect); // Right flicker blit
	}
	else if (flicker2->body->GetAngle() < 45 * DEGTORAD) {
		scene_rect.x = 62;
		scene_rect.y = 332;
		scene_rect.w = 32;
		scene_rect.h = 10;
		App->renderer->Blit(scene, 156, 193, &scene_rect); // Right flicker blit
	}
	else if (flicker2->body->GetAngle() >= 45 * DEGTORAD) {
		scene_rect.x = 64;
		scene_rect.y = 351;
		scene_rect.w = 29;
		scene_rect.h = 18;
		App->renderer->Blit(scene, 156, 182, &scene_rect); // Right flicker blit
	}
}

void ModuleSceneIntro::HighStageBlit() {
	scene_rect.x = 233;
	scene_rect.y = 0;
	scene_rect.w = SCREEN_WIDTH;
	scene_rect.h = SCREEN_HEIGHT;
	App->renderer->Blit(scene, 0, 0, &scene_rect);
	scene_rect.x = 8;	// First vertical orange rectangle blit
	scene_rect.y = 168;
	scene_rect.w = 2;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 88, 128, &scene_rect);
	scene_rect.x = 8;	// Second vertical orange rectangle blit
	scene_rect.y = 168;
	scene_rect.w = 2;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 88, 136, &scene_rect);
	scene_rect.x = 8;	// Third vertical orange rectangle blit
	scene_rect.y = 168;
	scene_rect.w = 2;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 88, 144, &scene_rect);
	scene_rect.x = 8;	// Fourth vertical orange rectangle blit
	scene_rect.y = 168;
	scene_rect.w = 2;
	scene_rect.h = 7;
	App->renderer->Blit(scene, 88, 152, &scene_rect);
	scene_rect.x = 101;	// Diagonal orange rectangle blit
	scene_rect.y = 167;
	scene_rect.w = 8;
	scene_rect.h = 8;
	App->renderer->Blit(scene, 107, 80, &scene_rect);
	scene_rect.x = 12;	// Pink circle blit
	scene_rect.y = 44;
	scene_rect.w = 22;
	scene_rect.h = 24;
	App->renderer->Blit(scene, 135, 89, &scene_rect);
	scene_rect.x = 61;	// First (from above) circle blit
	scene_rect.y = 169;
	scene_rect.w = 4;
	scene_rect.h = 5;
	App->renderer->Blit(scene, 88, 39, &scene_rect);
	scene_rect.x = 61;	// Second (from above) circle blit
	scene_rect.y = 169;
	scene_rect.w = 4;
	scene_rect.h = 5;
	App->renderer->Blit(scene, 83, 48, &scene_rect);
	scene_rect.x = 61;	// Three (from above) circle blit
	scene_rect.y = 169;
	scene_rect.w = 4;
	scene_rect.h = 5;
	App->renderer->Blit(scene, 78, 57, &scene_rect);
	scene_rect.x = 61;	// Fourth (from above) circle blit
	scene_rect.y = 169;
	scene_rect.w = 4;
	scene_rect.h = 5;
	App->renderer->Blit(scene, 76, 67, &scene_rect);
	scene_rect.x = 61;	// Fifth (from above) circle blit
	scene_rect.y = 169;
	scene_rect.w = 4;
	scene_rect.h = 5;
	App->renderer->Blit(scene, 76, 77, &scene_rect);
	scene_rect.x = 61;	// Sixth (from above) circle blit
	scene_rect.y = 169;
	scene_rect.w = 4;
	scene_rect.h = 5;
	App->renderer->Blit(scene, 78, 88, &scene_rect);
	scene_rect.x = 61;	// Seventh (from above) circle blit
	scene_rect.y = 169;
	scene_rect.w = 4;
	scene_rect.h = 5;
	App->renderer->Blit(scene, 83, 97, &scene_rect);
	scene_rect.x = 61;	// Eight (from above) circle blit
	scene_rect.y = 169;
	scene_rect.w = 4;
	scene_rect.h = 5;
	App->renderer->Blit(scene, 88, 105, &scene_rect);
	scene_rect.x = 29;	// Left wallrus blit
	scene_rect.y = 79;
	scene_rect.w = 16;
	scene_rect.h = 17;
	App->renderer->Blit(scene, 111, 111, &scene_rect);
	scene_rect.x = 90;	// Right wallrus blit
	scene_rect.y = 177;
	scene_rect.w = 16;
	scene_rect.h = 17;
	App->renderer->Blit(scene, 163, 113, &scene_rect);
	scene_rect.x = 10;	// Left penguin blit
	scene_rect.y = 8;
	scene_rect.w = 12;
	scene_rect.h = 14;
	App->renderer->Blit(scene, 123, 146, &scene_rect);
	scene_rect.x = 27;	// Center penguin blit
	scene_rect.y = 8;
	scene_rect.w = 12;
	scene_rect.h = 14;
	App->renderer->Blit(scene, 139, 146, &scene_rect);
	scene_rect.x = 43;	// Right penguin blit
	scene_rect.y = 8;
	scene_rect.w = 12;
	scene_rect.h = 14;
	App->renderer->Blit(scene, 155, 146, &scene_rect);
	if (flicker1->body->GetAngle() >= 0) {
		scene_rect.x = 31;
		scene_rect.y = 107;
		scene_rect.w = 29;
		scene_rect.h = 18;
		App->renderer->Blit(scene, 102, 193, &scene_rect); // Left flicker blit
	}
	else if (flicker1->body->GetAngle() > -45 * DEGTORAD) {
		scene_rect.x = 30;
		scene_rect.y = 131;
		scene_rect.w = 32;
		scene_rect.h = 10;
		App->renderer->Blit(scene, 102, 193, &scene_rect); // Left flicker blit
	}
	else if (flicker1->body->GetAngle() <= -45 * DEGTORAD) {
		scene_rect.x = 31;
		scene_rect.y = 150;
		scene_rect.w = 29;
		scene_rect.h = 18;
		App->renderer->Blit(scene, 102, 180, &scene_rect); // Left flicker blit
	}
	if (flicker2->body->GetAngle() <= 0) {
		scene_rect.x = 64;
		scene_rect.y = 308;
		scene_rect.w = 29;
		scene_rect.h = 18;
		App->renderer->Blit(scene, 156, 193, &scene_rect); // Right flicker blit
	}
	else if (flicker2->body->GetAngle() < 45 * DEGTORAD) {
		scene_rect.x = 62;
		scene_rect.y = 332;
		scene_rect.w = 32;
		scene_rect.h = 10;
		App->renderer->Blit(scene, 156, 193, &scene_rect); // Right flicker blit
	}
	else if (flicker2->body->GetAngle() >= 45 * DEGTORAD) {
		scene_rect.x = 64;
		scene_rect.y = 351;
		scene_rect.w = 29;
		scene_rect.h = 18;
		App->renderer->Blit(scene, 156, 182, &scene_rect); // Right flicker blit
	}

	scene_rect.x = 1;
	scene_rect.y = 304;
	scene_rect.w = 51;
	scene_rect.h = 11;
	App->renderer->Blit(scene, 4, 39, &scene_rect); // topScore box blit
	App->renderer->Blit(scene, 4, 79, &scene_rect); // Score box blit
	scene_rect.x = 1;
	scene_rect.y = 316;
	scene_rect.w = 19;
	scene_rect.h = 11;
	App->renderer->Blit(scene, 34, 169, &scene_rect); //ball num blit

	//blit of the pulsator award
	sprintf_s(pulsatorUP_text, 10, "%7d", pulsatorUP);
	App->fonts->BlitText(95, 64, font2, pulsatorUP_text);
}

void ModuleSceneIntro::ChargeLowStage() {

	CreateFlicker();

	App->physics->CreateCircle(143, 113, 30, false);// Orange circle hitbox
	bumperSensors.add(App->physics->CreateCircleSensor(143 * SCREEN_SIZE, 113 * SCREEN_SIZE, 30)); //sensor for the hitbox	
	
	App->physics->CreateCircle(122, 79, 30, false);		// Left pink circle hitbox
	bumperSensors.add(App->physics->CreateCircleSensor(122 * SCREEN_SIZE, 79 * SCREEN_SIZE, 30)); //sensor for the hitbox	
	
	App->physics->CreateCircle(163, 79, 30, false);		// Right pink circle hitbox
	bumperSensors.add(App->physics->CreateCircleSensor(163 * SCREEN_SIZE, 79 * SCREEN_SIZE, 30)); //sensor for the hitbox	
	
	App->physics->CreateRectangle(233, 80, 4, 162, false); // Last right border hitbox
		
	App->physics->CreateRectangle(104, 50, 3, 12, false);	// First (from left) palet under the green cards hitbox
	cardSensors.add(App->physics->CreateRectangleSensor(111, 45, 4, 2)); //sensor for the first green card

	App->physics->CreateRectangle(120, 50, 3, 12, false);	// Second (from left) palet under the green cards hitbox
	cardSensors.add(App->physics->CreateRectangleSensor(127, 45, 4, 2)); //sensor for the first green card

	App->physics->CreateRectangle(136, 50, 3, 12, false);	// Third (from left) palet under the green cards hitbox
	cardSensors.add(App->physics->CreateRectangleSensor(143, 45, 4, 2)); //sensor for the first green card
	
	App->physics->CreateRectangle(152, 50, 3, 12, false);	// Fourth (from left) palet under the green cards hitbox
	cardSensors.add(App->physics->CreateRectangleSensor(159, 45, 4, 2)); //sensor for the first green card

	App->physics->CreateRectangle(168, 50, 3, 12, false);	// Fifth (from left) palet under the green cards hitbox
	cardSensors.add(App->physics->CreateRectangleSensor(175, 45, 4, 2)); //sensor for the first green card

	App->physics->CreateRectangle(184, 50, 3, 12, false);	// Sixth (from left) palet under the green cards hitbox
	// Low scene charge
	int scene1part1[20] = { 71, 0, 71, 22, 79, 44, 79, 114, 87, 121, 72, 136, 72, 195, 118, 242, 67, 242, 67, 0 };
	App->physics->CreateChain(0, -2, scene1part1, 20, false);	// Left border
	int scene1part2[30] = { 218, 0, 218, 162, 231, 162, 231, 241, 169, 241, 214, 195, 214, 136, 199, 121, 207, 113,
	207, 48, 215, 33, 213, 21, 198, 21, 191, 10, 191, 0 };
	App->physics->CreateChain(0, -2, scene1part2, 30, false);	// Right border
	int scene1part3[10] = { 84, 0, 84, 18, 87, 25, 94, 25, 94, 0 };
	App->physics->CreateChain(0, -5, scene1part3, 10, false);	// Left upper object
	int scene1part4[6] = { 111, 173, 100, 166, 100, 144 };
	App->physics->CreateChain(0, 0, scene1part4, 6, false);	// Pink left triangle
	int scene1part5[6] = { -11, 173, 0, 166, 0, 144 };
	App->physics->CreateChain(185, 0, scene1part5, 6, false);	// Pink right triangle
	int scene6[12] = { 84, 146, 84, 176, 104, 192, 104, 186, 89, 175, 89, 146 };
	App->physics->CreateChain(-1, -2, scene6, 12, false);		// Left white edge
	int scene7[12] = { 20, 146, 20, 176, 0, 192, 0, 186, 15, 175, 15, 146 };
	App->physics->CreateChain(182, -2, scene7, 12, false);		// Right white edge

	//Tags 1 to 7 sensors
	tagSensors.add(App->physics->CreateRectangleSensor(81,106,2,7));
	tagSensors.add(App->physics->CreateRectangleSensor(81, 98, 2, 7));
	tagSensors.add(App->physics->CreateRectangleSensor(81, 90, 2, 7));
	tagSensors.add(App->physics->CreateRectangleSensor(81, 82, 2, 7));
	tagSensors.add(App->physics->CreateRectangleSensor(81, 74, 2, 7));
	tagSensors.add(App->physics->CreateRectangleSensor(81, 66, 2, 7));
	tagSensors.add(App->physics->CreateRectangleSensor(81, 58, 2, 7));

	loseSensor = App->physics->CreateRectangleSensor(143, SCREEN_HEIGHT, 48, 5);
}

void ModuleSceneIntro::ChargeHighStage() {

	CreateFlicker();

	// High scene scenario
	App->physics->CreateCircle(146, 101, 30, false);		// Pink circle hitbox
	bumperSensors.add(App->physics->CreateCircleSensor(146 * SCREEN_SIZE, 101 * SCREEN_SIZE, 30)); //((we send the position of the sensor, and the score

	App->physics->CreateRectangle(136, 62, 3, 12, false);	// First (from left) palet under the scores
	App->physics->CreateRectangle(152, 62, 3, 12, false);	// Second (from left) palet under the scores


	int scene1[36] = { 71, 242, 71, 176, 86, 161, 86, 125, 72, 83, 72, 65, 78, 47, 89, 33, 105, 25, 195, 25, 210, 32,
	223, 46, 231, 65, 231, 240, 236, 240, 236, 0, 66, 0, 66, 242 };
	App->physics->CreateChain(0, 0, scene1, 36, false);		// Border
	int scene2[14] = { 84, 241, 84, 186, 91, 179, 102, 191, 102, 201, 94, 208, 94, 241 };
	App->physics->CreateChain(0, 0, scene2, 14, false);		// Left down object
	int scene3[18] = { 104, 42, 119, 56, 119, 71, 102, 88, 102, 103, 95, 103, 88, 82, 88, 61, 92, 51 };
	App->physics->CreateChain(0, 0, scene3, 18, false);		// Left upper object
	int scene4[58] = { 191, 241, 191, 207, 183, 200, 183, 191, 194, 181, 203, 188, 209, 188, 214, 183, 214, 175,
	199, 160, 199, 146, 206, 142, 210, 137, 212, 131, 212, 84, 206, 70, 195, 64, 185, 65, 179, 67, 175, 71, 166, 71,
	166, 56, 182, 41, 193, 41, 203, 45, 208, 50, 213, 59, 217, 72, 217, 241 };
	App->physics->CreateChain(0, 0, scene4, 58, false);		// Right object
	
	int scene5[12] = { 183, 88, 190, 96, 190, 128, 198, 127, 198, 87, 190, 83 };
	App->physics->CreateChain(0, 0, scene5, 12, false);		// Green object
	hundred_scoreSensors.add(App->physics->CreateChainSensor(0, 0, scene5, 12));

	//sensors for the lights (yellow dots in upper left screen)
	hundred_scoreSensors.add(App->physics->CreateRectangleSensor(90, 107, 4, 4));
	hundred_scoreSensors.add(App->physics->CreateRectangleSensor(85, 99, 4, 4));
	hundred_scoreSensors.add(App->physics->CreateRectangleSensor(80, 90, 4, 4));
	hundred_scoreSensors.add(App->physics->CreateRectangleSensor(78, 79, 4, 4));
	hundred_scoreSensors.add(App->physics->CreateRectangleSensor(78, 69, 4, 4));
	hundred_scoreSensors.add(App->physics->CreateRectangleSensor(80, 59, 4, 4));
	hundred_scoreSensors.add(App->physics->CreateRectangleSensor(85, 50, 4, 4));
	hundred_scoreSensors.add(App->physics->CreateRectangleSensor(90, 41, 4, 4));

	//sensors for table numbers: 500,1000,...
	fivehundred_scoreSensors.add(App->physics->CreateRectangleSensor(206, 98, 3, 4));
	fivehundred_scoreSensors.add(App->physics->CreateRectangleSensor(126, 62, 8, 2));
	thousand_scoreSensors.add(App->physics->CreateRectangleSensor(143, 62, 8, 2));
	fivehundred_scoreSensors.add(App->physics->CreateRectangleSensor(160, 62, 8, 2));

	//sensors for the yellow squares in the left
	yellowSquareSensors.add(App->physics->CreateRectangleSensor(89,131, 2, 7));
	yellowSquareSensors.add(App->physics->CreateRectangleSensor(89, 139, 2, 7));
	yellowSquareSensors.add(App->physics->CreateRectangleSensor(89, 148, 2, 7));
	yellowSquareSensors.add(App->physics->CreateRectangleSensor(89, 156, 2, 7));

	//Yellow pulsator sensor
	p.x = 6.65;
	p.y = 5;
	pulsator= App->physics->CreateRectangleSensor(107, 80, 10, 2);
	pulsator->body->SetTransform(p, 15); //rotate the rectangle

	//pink plattform sensor
	pinkPlattformSensor = (App->physics->CreateRectangleSensor(113, 140, 14, 5));
	pinkPlattformSensor->body->SetType(b2_kinematicBody);
	plat = App->physics->CreateRectangle(113, 140, 14, 5, true);
	plat->body->SetType(b2_kinematicBody);	

}

void ModuleSceneIntro::CreateFlicker() {
	int left_flicker[8] = { 3, 0, 0, 5, 24, 18, 29, 13 };
	int right_flicker[8] = { 26, 0, 29, 5, 5, 18, 0, 13 };
	flicker1 = App->physics->CreateChain(102, 193, left_flicker, 8, true);	// Left flicker
	flicker2 = App->physics->CreateChain(156, 193, right_flicker, 8, true);	// Right flicker
	revolute_joint_left = App->physics->CreateFlicker(*flicker1, false);		// Left flicker joint creation
	revolute_joint_right = App->physics->CreateFlicker(*flicker2, true);		// Right flicker joint creation
}

void ModuleSceneIntro::ScoreUpdater(uint s) {
	score += s;
	if (score > topscore)
	{
		topscore = score;
	}
}

void ModuleSceneIntro::CreatePlayer(float x, float y) {
	player_circle = App->physics->CreateCircle(x, y, 16, true);	// Creates player
}

void ModuleSceneIntro::CheckOneWayColliders() {
	b2Vec2 speed_aux;
	if (stage == ST_HIGH_STAGE) {
		if (App->player->position.x >= 191 && App->player->position.x <= 197 && App->player->position.y >= 25 && App->player->position.y <= 40) {
			if (App->player->position.x > App->player->buffer_position.x) {
				player_circle->body->GetLinearVelocityFromWorldPoint(speed_aux);
				speed_aux.x = -30;
				player_circle->body->SetLinearVelocity(speed_aux);
			}
		}
		else if (App->player->position.x >= 95 && App->player->position.x <= 104 && App->player->position.y >= 26 && App->player->position.y <= 43) {
			if (App->player->position.x < App->player->buffer_position.x) {
				player_circle->body->GetLinearVelocityFromWorldPoint(speed_aux);
				speed_aux.x = 30;
				player_circle->body->SetLinearVelocity(speed_aux);
			}
		}
		else if (App->player->position.x >= 200 && App->player->position.x <= 208 && App->player->position.y >= 128 && App->player->position.y <= 139) {
			if (App->player->position.y < App->player->buffer_position.y) {
				player_circle->body->GetLinearVelocityFromWorldPoint(speed_aux);
				speed_aux.x = -30;
				speed_aux.y = 0;
				player_circle->body->SetLinearVelocity(speed_aux);
			}
		}
	}
}

void ModuleSceneIntro::pinkPlatformUpdate()
{
	int x, y;
	//We check if the position of the plattform is off one limit, then we set the state to moving to the opposite side
   // The limits we set may look weird because positions are set in float32 vectors
	if (plat->body->GetPosition().x >= 9.5)
	{
		pstate = ST_LEFT;
	}

	if (plat->body->GetPosition().x <= 7.5)
	{
		pstate = ST_RIGHT;
	}
	//We set linear velocity to the body depending on the move state
	switch (pstate)
	{
	case ST_RIGHT:
		pVel.x = 1;

		break;
	case ST_LEFT:
		pVel.x = -1;

	}
	plat->body->SetLinearVelocity(pVel);
	plat->GetPosition(x, y);
	pinkPlattformSensor->body->SetLinearVelocity(pVel);
	App->renderer->Blit(plattform_tex, x / SCREEN_SIZE, y / SCREEN_SIZE, NULL, 1.0f, plat->GetRotation());

}