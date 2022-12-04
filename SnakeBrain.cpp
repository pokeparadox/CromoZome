#include "SnakeBrain.h"

SnakeBrain::SnakeBrain()
{
	mode = NONE;
	init();
}
SnakeBrain::SnakeBrain(CRuint mode)
{
	this->mode = mode;
	init();
}

SnakeBrain::~SnakeBrain()
{
}

inline void SnakeBrain::init()
{
	randGen.setLimits(UP,DOWN_RIGHT);	//	Set limits to directions
	direction = (uint)randGen.nextInt();
	distanceSq = 99999;
	//this->treat = 0;
}
void SnakeBrain::update(const Vector2di& fPos, const Vector2di& cPos, const Vector2di& oldPos)
{
	if(mode == NONE)
		return;

	Vector2di temp;
	temp = fPos - oldPos;
	distanceSq = temp.lengthSquared();
	assessSituation(fPos,cPos);
}

void SnakeBrain::assessSituation(const Vector2di& fPos, const Vector2di& currPos)
{
	if(mode == NONE)
		return;

	randGen.setLimits(UP,DOWN_RIGHT);	//	Set limits to directions
	//uint ranDir = randGen.nextInt();
	Vector2di temp;
	temp = fPos - currPos;
	uint dist = temp.lengthSquared();
	switch(mode)
	{
	    case AGRESSIVE:
	    {
            //	check if closer
            if( dist < distanceSq)
            {
                return;
            }
            else
            {
                //	If we haven't got any closer we just pick a direction at random
                direction = (uint)randGen.nextInt();
            }
	    }
	    break;
        case NEUTRAL:
        {
            //	Choose random number based on treat and either continue or pick random direction
            if( dist < distanceSq)
            {
                return;
            }
            else
            {
                //	If we haven't got any closer we just pick a direction at random
                direction = (uint)randGen.nextInt();
            }
        }
        break;
        case PASSIVE:
        {
            //	check if further
            if(dist > distanceSq)
            {
                return;
            }
            else
            {
                //	If we are closer then we decrease treat and pick a random direction
                direction = (uint)randGen.nextInt();
            }
        }
        break;
    }
}

