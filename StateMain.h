#ifndef STATEMAIN_H_INCLUDED
#define STATEMAIN_H_INCLUDED

#include "PenjinStates.h"     // Make sure your state is listen in this include.
/*
This is the main game state where the game is actually played
*/
#include "Animation.h"
#include "Background.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Node.h"
#include "Snake.h"
#include "InvincibilityManager.h"
#include "Text.h"
#include "ScreenPartition.h"
#include "Score.h"
#include "Music.h"
#include "Sound.h"
#include "Menu.h"
#include "GFX.h"
#include "Rectangle.h"

class StateMain : public BaseState
{
    public:
        StateMain();
        virtual void init();
        virtual void userInput();
        virtual void render();
        virtual void update();
        //virtual void unlimitedUpdate();

        virtual void pauseScreen();
        virtual void pauseUpdate();
        virtual void pauseInput();
        //virtual void onPause();
        //virtual void onResume();

        virtual ~StateMain();


    private:
        void clear();
        void loadSounds();
        void loadMusic();
        void loadSprites();
        void loadMenu();
        void setupObjects();
        void setupSnakes();
        void setupText();

        void rebuildCollisionLists();
		void processCollisionLists();
		void newCollisions();
		void checkSubList(ScreenPartition& subList, CRuint quad);
		void distanceCollision(CRint currSnake, CRint enemySnake, CRint currIndex, CRint enemIndex, CRuint quad);
		void cleanDead();

		void checkLevelUp();

		Text text;
		AnimatedSprite background;
		Vector2di backPos[4];

		AnimatedSprite redSprite,greenSprite,blueSprite;	//	Animated sprites for snakes
		AnimatedSprite p1indicator;
		AnimatedSprite plagueSprite;
		AnimatedSprite mutationSprite;

		vector<Snake*> snakes;	//	All of the other snakes
		vector<Vector2df> headPos;    //  Stores the snakes head positions in the last update
		InvincibilityManager godManager;    //  Tracks invincibility status

		Node* food;       // a piece of food for the snakes to eat
		Node* food2;
		bool foodEaten;            // says if the food has been eaten by a snake or not
		bool foodEaten2;
		bool isPaused;
        bool playPlagueAnimation;
        bool playMutationAnimation;

		//	Other stuff
		uint widthSq;
		uint plagueRate;
		uint mutationRate;

		uint longSnake;
		Score score;
		RandomClass randGen;
		ScreenPartition partition;
		vector<int> collisionLists[4];      // Flag as -1 to represent a cleared node!
		Music music;
		Sound eatSound;
		Sound splitSound;
		Sound plagueSound;
		Menu menu;
		int choice;
		Rectangle pauseRect;
};


#endif // STATEMAIN_H_INCLUDED
