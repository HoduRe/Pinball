#include "Globals.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleSceneIntro.h"

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
	if (position.y < 0 && App->scene_intro->stage == ST_LOW_STAGE) {
		App->scene_intro->stage == ST_HIGH_STAGE;
	}
	if (position.y > SCREEN_HEIGHT && App->scene_intro->stage == ST_HIGH_STAGE) {
		App->scene_intro->stage == ST_LOW_STAGE;
	}
	return UPDATE_CONTINUE;
}


