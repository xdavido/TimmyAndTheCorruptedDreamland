#ifndef __SCENE_H__
#define __SCENE_H__

#include "Module.h"
#include "Player.h"
#include "Door.h"
#include "Npc.h"

#include "UIModule.h"
#include "GuiButton.h"
#include "DialogueTree.h"
#include "UriBehaviorTree.h"
#include "Physics.h"
#include "SDL_Timer.h"

struct SDL_Texture;

class Scene : public Module
{
public:

	Scene(bool isActive);

	// Destructor
	virtual ~Scene();

	// Called before render is available
	bool Awake(pugi::xml_node& config);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// Define multiple Gui Event methods
	bool OnGuiMouseClickEvent(GuiControl* control);

	void CreateDialogue();

	void RunDialogueTree(ColliderType NPC);

	void UpdateDialogueTree(int opt);

	std::vector<std::string> GetDialogue() { return dialogue; }

	std::string LastTextNPC(ColliderType NPC);

	void AppearDialogue();

	ColliderType GetPlayerLastCollision() { return player->lastCollision; }

private:

	void Camera();
	void GodMode();
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&);
	void MoveToBattleFromDialogue();

	int pruebaXML = 0;
public:

	Player* player;
	Npc* npc1;
	List<Door*> doors;


	// Load things
	int loadPlayerPosX;
	int loadPlayerPosY;

	// New Game things
	bool isNewGame;

	SDL_Texture* npcPopUpTexture;
	SDL_Texture* uiSpriteTexture;

	bool angryVillagerDefeated, LRRHDefeated;

	int numTimesAVDialogueTriggered, numTimesLRRHDialogueTriggered;
	SDL_Timer timerToSceneBattle, timerToLRRHCombat;
	

private:

	SString mapName;
	SString mapFolder;


	// L12: Debug pathfing
	iPoint origin;
	bool originSelected = false;

	//Fonts
	int font;

	bool pauseMenuActive;
	bool exitButtonBool;

	std::shared_ptr<DialogueTree> angryVillagerTree, angryVillagerTreePT, 
		talismanVillagerTree, littleRedTree, grandmaTree, littleRedACTree;

	std::vector<std::string> dialogue;

	bool godMode;


};

#endif // __SCENE_H__