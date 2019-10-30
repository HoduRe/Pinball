#include "Globals.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	return true;
}

// Unload assets
bool ModulePlayer::CleanUp()
{
	LOG("Unloading player");

	return true;
}

// Update: draw background
update_status ModulePlayer::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PreUpdate() {
	
	// Stage calculation
	if (position.y < 0 && App->scene_intro->stage == ST_LOW_STAGE) {
		App->scene_intro->stage == ST_HIGH_STAGE;
	}
	if (position.y > SCREEN_HEIGHT && App->scene_intro->stage == ST_HIGH_STAGE) {
		App->scene_intro->stage == ST_LOW_STAGE;
	}
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->scene_intro->stage == ST_TITLE_SCREEN) {
		App->scene_intro->stage == ST_LOW_STAGE;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN) {

	}	// LEFT FLIPPER
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN) {

	}	// RIGHT FLIPPER
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {

	}	// KICKER
	return UPDATE_CONTINUE;
}


