#include "Player.h"
#include "App.h"
#include "Textures.h"
#include "Audio.h"
#include "Input.h"
#include "Render.h"
#include "Scene.h"
#include "Log.h"
#include "Point.h"
#include "Physics.h"
#include "Animation.h"
#include "Fonts.h"

Player::Player() : Entity(EntityType::PLAYER)
{
	name.Create("Player");
}

Player::~Player() {

}

bool Player::Awake() {

	idleAnim.PushBack({ 0, 0, 140, 140 });
	idleAnim.loop = true;

	for (int i = 0; i < 10; i++) //penutlima:cabezon
	{
		walkDownAnim.PushBack({ (i*150), 150, 150, 150 });
	}
	walkDownAnim.loop = true;
	walkDownAnim.speed = 0.30f;

	for (int i = 0; i < 10; i++)
	{
		walkUpAnim.PushBack({ (i * 150), 600, 150, 150 });
	}
	walkUpAnim.loop = true;
	walkUpAnim.speed = 0.30f;

	for (int i = 0; i < 10; i++)
	{
		walkRightAnim.PushBack({ (i * 150), 450, 150, 150 });
	}
	walkRightAnim.loop = true;
	walkRightAnim.speed = 0.30f;

	for (int i = 0; i < 10; i++)
	{
		walkLeftAnim.PushBack({ (i * 150), 300, 150, 150 });
	}
	walkLeftAnim.loop = true;
	walkLeftAnim.speed = 0.30f;

	
	position.x = parameters.attribute("x").as_int();
	position.y = parameters.attribute("y").as_int();
	texturePath = parameters.attribute("texturepath").as_string();
	
	speed = 10;
	vel = b2Vec2(0, 0);

	return true;
}

bool Player::Start() {

	texture = app->tex->Load(texturePath);
	currentAnimation = &idleAnim;

	pbody = app->physics->CreateRectangle(position.x,position.y,70,70, bodyType::DYNAMIC);
	pbody->body->SetFixedRotation(true);
	pbody->listener = this;
	pbody->ctype = ColliderType::PLAYER;


	// Bool variables
	npcInteractAvailable = false;
	itemInteractAvailable = false;
	movementRestringed = false;
	playerState = PlayerState::MOVING;
	playerPrevState = PlayerState::MOVING;

	lastCollision = ColliderType::UNKNOWN;

	godMode = false;
	

	return true;
}

bool Player::Update()
{
	currentAnimation->Update();

	std::cout << "Player pos X: " << position.x << std::endl;
	std::cout << "Player pos Y: " << position.y << std::endl;

	switch (playerState)
	{
	case PAUSE:
		movementRestringed = true;
		/*app->fonts->DrawText("PLAYER STATE: PAUSE", position.x + 100, position.y + 100,
			100, 100, { 255,255,255,255 }, app->fonts->gameFont);*/
		break;
	case INVENTORY:
		movementRestringed = true;
		/*app->fonts->DrawText("PLAYER STATE: INVENTORY", position.x + 100, position.y + 100,
			100, 100, { 255,255,255,255 }, app->fonts->gameFont);*/

		break;
	case MOVING:
		movementRestringed = false;
		/*app->fonts->DrawText("PLAYER STATE: MOVING", position.x + 100, position.y + 100,
			100, 100, { 255,255,255,255 }, app->fonts->gameFont);*/
		break;
	case BATTLE:
		break;
	case NPC_INTERACT:
		LOG("TALKING TO NPC1");
		movementRestringed = true;
		TriggerDialogueTree(lastCollision);
		InteractWithTree();

		/*app->fonts->DrawText("PLAYER STATE: NPC_INTERACT", position.x + 100, position.y + 100,
			100, 100, { 255,255,255,255 }, app->fonts->gameFont);*/
		break;
	case ITEM_INTERACT:
		LOG("INTERACTING WITH ITEM");
		movementRestringed = true;
		/*app->fonts->DrawText("PLAYER STATE: ITEM_INTERACT", position.x + 100, position.y + 100,
			100, 100, { 255,255,255,255 }, app->fonts->gameFont);*/
		break;
	case NONE:
		break;
	}

	//Movement
	if (!movementRestringed)
	{
		Movement();
	}


	if (app->input->GetKey(SDL_SCANCODE_E) == KEY_DOWN)
	{
		InteractWithEntities();
	}

	GodMode();


	//Update player position in pixels
	position.x = METERS_TO_PIXELS(pbody->body->GetTransform().p.x) - 16;
	position.y = METERS_TO_PIXELS(pbody->body->GetTransform().p.y) - 16;



	

	return true;
}

bool Player::PostUpdate() {

	SDL_Rect rect = currentAnimation->GetCurrentFrame();
	app->render->DrawTexture(texture, position.x - 55, position.y-75, &rect);

	return true;
}

bool Player::CleanUp()
{
	app->tex->UnLoad(texture);

	if (pbody != NULL)
	{
		pbody->body->GetWorld()->DestroyBody(pbody->body);
	}

	return true;
}

void Player::OnCollision(PhysBody* physA, PhysBody* physB) {

	switch (physB->ctype)
	{
		case ColliderType::ITEM:
			LOG("Collision ITEM");
			itemInteractAvailable = true;
			break;
		case ColliderType::BARRIER:
			LOG("Collision BARRIER");
			break;
		case ColliderType::DOOR:
			LOG("Collision DOOR");

			break;
		case ColliderType::UNKNOWN:
			LOG("Collision UNKNOWN");
			break;
		case ColliderType::ANGRYVILLAGER:
			LOG("Collision 	ANGRYVILLAGER");
			npcInteractAvailable = true;
			lastCollision = ColliderType::ANGRYVILLAGER;
			break;
		case ColliderType::TALISMANVILLAGER:
			LOG("Collision 	TALISMANVILLAGER");
			npcInteractAvailable = true;
			lastCollision = ColliderType::TALISMANVILLAGER;
			break;
		case ColliderType::GRANDMA:
			LOG("Collision 	GRANDMA");
			npcInteractAvailable = true;
			lastCollision = ColliderType::GRANDMA;
			break;
		case ColliderType::LRRH:
			LOG("Collision 	LRRH");
			npcInteractAvailable = true;
			lastCollision = ColliderType::LRRH;
			break;
	}
	
}

//This function toggles the player's god mode when the F10 key is pressed
void Player::GodMode()
{
	if (app->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN)
	{
		if (godMode)
		{
			godMode = false;
		}
		else
		{
			godMode = true;
		}
	}
}

//This function checks for input from the player's keyboard and updates the dialogue tree in the game's scene accordingly. The function checks if any button is being pressed, and if so, it calls the UpdateDialogueTree() function in the scene and passes it an integer value from 1 to 4, depending on which button was pressed.
void Player::InteractWithTree()
{
	if (app->input->GetKey(SDL_SCANCODE_Z) == KEY_DOWN)
	{
		app->scene->UpdateDialogueTree(1);
	}
	else if (app->input->GetKey(SDL_SCANCODE_X) == KEY_DOWN)
	{
		app->scene->UpdateDialogueTree(2);
	}
	else if (app->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{
		app->scene->UpdateDialogueTree(3);
	}
	else if (app->input->GetKey(SDL_SCANCODE_V) == KEY_DOWN)
	{
		app->scene->UpdateDialogueTree(4);
	}


}

//This function takes a ColliderType parameter and runs the corresponding dialogue tree in the game's scene
void Player::TriggerDialogueTree(ColliderType NPC)
{
	switch (NPC)
	{
	case ColliderType::ANGRYVILLAGER:
		app->scene->RunDialogueTree(ColliderType::ANGRYVILLAGER);
		break;
	case ColliderType::TALISMANVILLAGER:
		app->scene->RunDialogueTree(ColliderType::TALISMANVILLAGER);
		break;
	case ColliderType::GRANDMA:
		app->scene->RunDialogueTree(ColliderType::GRANDMA);
		break;
	case ColliderType::LRRH:
		app->scene->RunDialogueTree(ColliderType::LRRH);
		break;
	default:
		break;
	}
}

//This function handles the player's movement in the game. The function also sets the player's animation based on their direction of movement.
void Player::Movement()
{
	vel = b2Vec2(0, 0);

	bool isMovingH, isMovingV;

	isMovingH = HorizontalMovement();
	isMovingV = VerticalMovement();
	bool isRunning = SprintMovement();


	if (!isMovingH && !isMovingV)
		currentAnimation = &idleAnim;

	if (godMode)
	{
		if(!isRunning)
		vel *= 2;
	}

	pbody->body->SetLinearVelocity(vel);

}

//This function checks if the player is holding down the left shift key to sprint. If the key is being held down, the function increases the player's velocity and sets the animation speed to a faster value
bool Player::SprintMovement()
{
	if (app->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		vel *= 2;

		walkDownAnim.speed = 0.45f;
		walkUpAnim.speed = 0.45f;
		walkRightAnim.speed = 0.45f;
		walkLeftAnim.speed = 0.45f;

		return true;
	}
	else
	{
		walkDownAnim.speed = 0.30f;
		walkUpAnim.speed = 0.30f;
		walkRightAnim.speed = 0.30f;
		walkLeftAnim.speed = 0.30f;

		return false;
	}
}

//Handles the player vertical movement.
bool Player::VerticalMovement()
{
	if (app->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) {
		vel.y = -speed;
		currentAnimation = &walkUpAnim;
		return true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) {
		vel.y = speed;
		currentAnimation = &walkDownAnim;
		return true;
	}

	return false;
}

//Handles the player horizontal movement.
bool Player::HorizontalMovement()
{
	if (app->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		vel.x = -speed;
		currentAnimation = &walkLeftAnim;
		return true;
	}
	else if (app->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		vel.x = speed;
		currentAnimation = &walkRightAnim;
		return true;
	}

	return false;

}

//This function handles the player's interaction with other entities in the game. It checks if the player is near an NPC or item that can be interacted with, and if so, it sets the player's state to either NPC_INTERACT or ITEM_INTERACT. If the player is already in one of these states, the function resets the player's state to MOVING.
void Player::InteractWithEntities()
{
	if (playerState != PlayerState::PAUSE)
	{
		if (npcInteractAvailable)
		{
			if (playerState == NPC_INTERACT)
			{
				playerState = MOVING;
				npcInteractAvailable = false;
			}
			// Moving
			else
			{
				playerPrevState = playerState;
				playerState = NPC_INTERACT;
				StopVelocity();
			}
		}

		else if (itemInteractAvailable)
		{
			if (playerState == ITEM_INTERACT)
			{
				playerState = MOVING;
				itemInteractAvailable = false;
			}
			// Moving
			else
			{
				playerPrevState = playerState;
				playerState = ITEM_INTERACT;
				StopVelocity();
			}
		}
	}
}

//This function sets the player's velocity to zero and sets the player's animation to the idle animation. This is used to stop the player's movement when they are interacting with an NPC or item.
void Player::StopVelocity()
{
	vel = b2Vec2(0, 0);
	pbody->body->SetLinearVelocity(vel);
	currentAnimation = &idleAnim;
}
