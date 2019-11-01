#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
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
	// Stage Print
	if (App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25, true);
	}
	if (stage == ST_TITLE_SCREEN) {
		TitleBlit();
	}
	else if (stage == ST_LOW_STAGE) {
		if (buffer_stage != ST_LOW_STAGE) {
			App->physics->Disable();
			App->physics->Enable();
			ChargeLowStage();
		}
		LowStageBlit();
	}
	else if (stage == ST_HIGH_STAGE) {
		if (buffer_stage != ST_HIGH_STAGE) {
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

void ModuleSceneIntro::TitleBlit() {
	App->renderer->DrawQuad(scene_rect, 25, 25, 25);
	elements_rect.x = 8;
	elements_rect.y = 199;
	elements_rect.w = 192;
	elements_rect.h = 104;
	App->renderer->Blit(scene, 24, 24, &elements_rect);
}

void ModuleSceneIntro::ChargeLowStage() {
	int left_flicker[6] = { 4, 0, 0, 5, 29, 17 };
	int right_flicker[6] = { 25, 0, 29, 5, 0, 17 };
	b2Vec2 circle1 (102, 193);
	b2Vec2 circle2 (188, 193);
	App->physics->CreateCircle(107, 193, 5, false);	// Left flicker anchor point
	App->physics->CreateCircle(180, 193, 5, false);	// Right flicker anchor point
	App->physics->CreateChain(102, 193, left_flicker, 6, false);	// Left flicker
	App->physics->CreateChain(156, 193, right_flicker, 6, false);	// Right flicker
	App->physics->CreateFlicker(circle1);		// Left flicker joint creation
	App->physics->CreateFlicker(circle2);		// Right flicker joint creation

	App->physics->CreateCircle(143, 113, 30, false);	// Orange circle hitbox
	App->physics->CreateCircle(122, 79, 30, false);		// Left pink circle hitbox
	App->physics->CreateCircle(163, 79, 30, false);		// Right pink circle hitbox
	App->physics->CreateRectangle(104, 50, 3, 12, false);	// First (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(120, 50, 3, 12, false);	// Second (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(136, 50, 3, 12, false);	// Third (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(152, 50, 3, 12, false);	// Fourth (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(168, 50, 3, 12, false);	// Fifth (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(184, 50, 3, 12, false);	// Sixth (from left) palet under the green cards hitbox
	App->physics->CreateRectangle(225, 204, 21, 87, false);	// Kicker hitbox hitbox
	App->physics->CreateRectangle(69, 121, 4, 242, false); // Left border hitbox
	App->physics->CreateRectangle(216, 148, 4, 24, false); // First right border hitbox
	App->physics->CreateRectangle(233, 80, 4, 162, false); // Last right border hitbox
}

void ModuleSceneIntro::ChargeHighStage() {
	App->physics->CreateRectangle(151, 13, 160, 25, false); // Upper border hitbox
	App->physics->CreateRectangle(216, 107, 6, 47, false); // Right obstacle border hitbox
	App->physics->CreateRectangle(69, 121, 4, 242, false); // Left border hitbox
	App->physics->CreateRectangle(233, 121, 4, 242, false); // Last right border hitbox

}
