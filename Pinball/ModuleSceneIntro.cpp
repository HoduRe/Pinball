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
	ray_on = false;
	sensed = false;
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

	scene_rect.x = 0;
	scene_rect.y = 0;
	scene_rect.w = SCREEN_WIDTH;
	scene_rect.h = SCREEN_HEIGHT;
	stage = ST_TITLE_SCREEN;

	sensor = App->physics->CreateRectangleSensor(SCREEN_WIDTH / 2, SCREEN_HEIGHT, SCREEN_WIDTH, 50);

	

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
	/*if(App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}

	if(App->input->GetKey(SDL_SCANCODE_1) == KEY_DOWN)
	{
		circles.add(App->physics->CreateCircle(App->input->GetMouseX(), App->input->GetMouseY(), 25));
		circles.getLast()->data->listener = this;
	}

	if(App->input->GetKey(SDL_SCANCODE_2) == KEY_DOWN)
	{
		boxes.add(App->physics->CreateRectangle(App->input->GetMouseX(), App->input->GetMouseY(), 100, 50));
	}

	if(App->input->GetKey(SDL_SCANCODE_3) == KEY_DOWN)
	{
		// Pivot 0, 0
		int rick_head[64] = {
			14, 36,
			42, 40,
			40, 0,
			75, 30,
			88, 4,
			94, 39,
			111, 36,
			104, 58,
			107, 62,
			117, 67,
			109, 73,
			110, 85,
			106, 91,
			109, 99,
			103, 104,
			100, 115,
			106, 121,
			103, 125,
			98, 126,
			95, 137,
			83, 147,
			67, 147,
			53, 140,
			46, 132,
			34, 136,
			38, 126,
			23, 123,
			30, 114,
			10, 102,
			29, 90,
			0, 75,
			30, 62
		};

		ricks.add(App->physics->CreateChain(App->input->GetMouseX(), App->input->GetMouseY(), rick_head, 64));
	}*/

	// Prepare for raycast ------------------------------------------------------
	
	/*iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);*/

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------
	p2List_item<PhysBody*>* c = circles.getFirst();


	// Stage Print
	if (stage == ST_TITLE_SCREEN) {
		TitleBlit();
	}
	else if (stage == ST_LOW_STAGE) {
		LowStageBlit();
	}
	else if (stage == ST_HIGH_STAGE) {
		HighStageBlit();
	}

/*	// Figures Print
	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		if(c->data->Contains(App->input->GetMouseX(), App->input->GetMouseY()))
			App->renderer->Blit(circle, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	c = boxes.getFirst();

	//Provisional key for changing stage_state
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->scene_intro->stage == ST_TITLE_SCREEN) {
		App->scene_intro->stage == ST_LOW_STAGE;
	}

/*	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(box, x, y, NULL, 1.0f, c->data->GetRotation());
		if(ray_on)
		{
			int hit = c->data->RayCast(ray.x, ray.y, mouse.x, mouse.y, normal.x, normal.y);
			if(hit >= 0)
				ray_hit = hit;
		}
		c = c->next;
	}
*/
/*	c = ricks.getFirst();

	while(c != NULL)
	{
		int x, y;
		c->data->GetPosition(x, y);
		App->renderer->Blit(rick, x, y, NULL, 1.0f, c->data->GetRotation());
		c = c->next;
	}

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}
*/
	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;

	App->audio->PlayFx(bonus_fx);

	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
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