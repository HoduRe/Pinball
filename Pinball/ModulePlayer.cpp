#include "Globals.h"
#include "ModuleSceneIntro.h"
#include "Application.h"
#include "ModulePlayer.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"

ModulePlayer::ModulePlayer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModulePlayer::~ModulePlayer()
{}

// Load assets
bool ModulePlayer::Start()
{
	LOG("Loading player");
	player = App->textures->Load("pinball/player.png");
	position.x = 10;
	position.y = 10;
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
	if(App->scene_intro->stage != ST_TITLE_SCREEN)
		App->renderer->Blit(player, position.x, position.y);

	return UPDATE_CONTINUE;
}

update_status ModulePlayer::PreUpdate() {
	
	// Stage calculation
	if (position.y < 0 && App->scene_intro->stage == ST_LOW_STAGE) {
		App->scene_intro->stage = ST_HIGH_STAGE;
	}
	if (position.y > SCREEN_HEIGHT /*&& App->scene_intro->stage == ST_HIGH_STAGE*/) {
		App->scene_intro->stage = ST_LOW_STAGE;
	}
  	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && App->scene_intro->stage != ST_LOW_STAGE) {
		App->scene_intro->stage = ST_LOW_STAGE;
	}
	if (App->input->GetKey(SDL_SCANCODE_J) == KEY_DOWN && App->scene_intro->stage != ST_HIGH_STAGE) {
		App->scene_intro->stage = ST_HIGH_STAGE;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) {
		position.x--;

	}	// LEFT FLIPPER
	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) {
		position.x++;

	}	// RIGHT FLIPPER
	else if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		position.y--;
	}
	else if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
		position.y++;
	}	// KICKER
	return UPDATE_CONTINUE;
}


