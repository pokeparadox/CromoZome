#ifndef SNAKEBRAIN_H
#define SNAKEBRAIN_H

#include "PenjinTypes.h"
#include "Random.h"

//	These values relate to the behaviour of the snake
enum Modes
{
	AGRESSIVE,		//	Snake will actively hunt other snakes
	NEUTRAL,		//	Snake will just go wherever but randomly choose to attack or retreat if too close
	PASSIVE,		//	Snake will cower from other snakes
	NONE			//	AI will be disabled.
};

//	These values relate to the directions the snake can decide to go.
enum Directions
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	UP_LEFT,		//	These are the diagonal directions
	UP_RIGHT,
	DOWN_LEFT,
	DOWN_RIGHT
};

class SnakeBrain
{
    public:
        SnakeBrain();
        SnakeBrain(CRuint mode);
        ~SnakeBrain();

        void changeTreat(CRuint treat)	//	modifies the treat for a nuetral snake
        {
            this->treat += treat;
        }
        void setMode(CRuint mode)			//	Sets the AI profile of the snake
        {
            this->mode = mode;
        }
        uint getMode()					//	Tells us the AI profile of the snake
        {
            return mode;
        }
        uint getDirection()		//	Tells us the current direction of the snake
        {
            return direction;
        }
        void update(const Vector2di& fPos,const Vector2di& cPos,const Vector2di& oldPos);
        void update(const Vector2df& fPos,const Vector2df& cPos,const Vector2df& oldPos)
        {
            Vector2di tfPos(fPos.x, fPos.y);
            Vector2di tcPos(cPos.x, cPos.y);
            Vector2di toldPos(oldPos.x, oldPos.y);
            update(tfPos,tcPos,toldPos);
        }
    private:
        void init();
        void assessSituation(const Vector2di& fPos,const Vector2di& currPos);			//	The snake makes it's next move
        uint distanceSq;		//	Stores the distance to the nearest snake to help snake decide
        uint mode;				//	Stores the AI profile
        uint direction;			//	Stores the current direction of the snake
        int treat;				//	We use the treat to increase the chance that the best direction will be picked.
        RandomClass randGen;			//	Random number generator
};
#endif	//	SNAKEBRAIN_H

