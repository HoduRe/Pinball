#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "ModulePhysics.h"

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

	// Player position update
	if (player_circle != NULL) {
		App->player->position.x = METERS_TO_PIXELS(player_circle->body->GetPosition().x) / SCREEN_SIZE - 5;
		App->player->position.y = METERS_TO_PIXELS(player_circle->body->GetPosition().y) / SCREEN_SIZE - 5;
	}

	// Stage Print
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25, true);
	}

	if (stage == ST_TITLE_SCREEN) {
		TitleBlit();
	}
	else if (stage == ST_LOW_STAGE) {
		if (buffer_stage == ST_HIGH_STAGE) {
			App->physics->Disable();
			App->physics->Enable();
			ChargeLowStage();
		}
		else if(buffer_stage == ST_TITLE_SCREEN){
			App->physics->Disable();
			App->physics->Enable();
			player_circle = App->physics->CreateCircle(App->player->position.x + 4, App->player->position.y, 16, true);	// Creates player
			ChargeLowStage();
		}
		LowStageBlit();
	}
	else if (stage == ST_HIGH_STAGE) {
		if (buffer_stage == ST_LOW_STAGE) {
				App->physics->Disable();
			App->physics->Enable();
			ChargeHighStage();
		}
		HighStageBlit();
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

}

void ModuleSceneIntro::TitleBlit() {
	App->renderer->DrawQuad(scene_rect, 25, 25, 25);
	elements_rect.x = 8;
	elements_rect.y = 199;
	elements_rect.w = 192;
	elements_rect.h = 104;
	App->renderer->Blit(scene, 24, 24, &elements_rect);
}

void ModuleSceneIntro::LowStageBlit() {
	scene_rect.x = 233;	// Scene blit
	scene_rect.y = SCREEN_HEIGHT;
	scene_rect.w = SCREEN_WIDTH;
	scene_rect.h = SCREEN_HEIGHT;
	App->renderer->Blit(scene, 0, 0, &scene_rect);
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
	scene_rect.x = 113;	// Pink moving platform blit
	scene_rect.y = 33;
	scene_rect.w = 14;
	scene_rect.h = 5;
	App->renderer->Blit(scene, SCREEN_WIDTH / 2, 138, &scene_rect);
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

}

void ModuleSceneIntro::ChargeLowStage() {
	CreateFlicker();	// Creates both flickers' hitboxes
	App->physics->CreateCircle(143, 113, 30, false);	// Orange circle hitbox
	App->physics->CreateCircle(122, 79, 30, false);		// Left pink circle hitbox
	App->physics->CreateCircle(163, 79, 30, false);		// Right pink circle hitbox
	App->physics->CreateRectangle(233, 80, 4, 162, false); // Last right border hitbox
	App->physics->CreateRectangle(104, 50, 3, 12, false);	// First (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(120, 50, 3, 12, false);	// Second (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(136, 50, 3, 12, false);	// Third (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(152, 50, 3, 12, false);	// Fourth (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(168, 50, 3, 12, false);	// Fifth (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(184, 50, 3, 12, false);	// Sixth (from left) palet under the green cards hitbox
	// Low scene charge
	int scene1part1[20] = { 71, 0, 71, 22, 79, 44, 79, 114, 87, 121, 72, 136, 72, 195, 118, 242, 67, 242, 67, 0 };
	App->physics->CreateChain(0, -2, scene1part1, 20, false);	// Left border
	int scene1part2[30] = { 218, 0, 218, 162, 231, 162, 231, 241, 169, 241, 214, 195, 214, 136, 199, 121, 207, 113,
	207, 48, 215, 33, 213, 21, 198, 21, 192, 10, 192, 0 };
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

	/*App->physics->CreateRectangle(225, 204, 21, 87, false);	// Kicker hitbox hitbox
	App->physics->CreateRectangle(69, 121, 4, 242, false); // Left border hitbox
	App->physics->CreateRectangle(216, 148, 4, 24, false); // First right border hitbox
	*/
}

void ModuleSceneIntro::ChargeHighStage() {
/*	App->physics->CreateRectangle(151, 13, 160, 25, false); // Upper border hitbox
	App->physics->CreateRectangle(216, 107, 6, 47, false); // Right obstacle border hitbox
	App->physics->CreateRectangle(69, 121, 4, 242, false); // Left border hitbox
	App->physics->CreateRectangle(233, 121, 4, 242, false); // Last right border hitbox*/
	CreateFlicker();
	// High scene scenario
	App->physics->CreateCircle(146, 101, 30, false);		// Pink circle hitbox
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

}

void ModuleSceneIntro::CreateFlicker() {
	int left_flicker[6] = { 4, 0, 0, 5, 29, 17 };
	int right_flicker[6] = { 25, 0, 29, 5, 0, 17 };
	flicker1 = App->physics->CreateChain(102, 193, left_flicker, 6, true);	// Left flicker
	flicker2 = App->physics->CreateChain(156, 193, right_flicker, 6, true);	// Right flicker
	revolute_joint_left = App->physics->CreateFlicker(*flicker1, false);		// Left flicker joint creation
	revolute_joint_right = App->physics->CreateFlicker(*flicker2, true);		// Right flicker joint creation
}