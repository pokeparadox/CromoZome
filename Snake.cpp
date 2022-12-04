#include "Snake.h"

Snake::Snake()
{
    nodes.push_back(Node(0,0));
    oldPos.push_back(Vector2df(0,0));
	childCreateLevel = 12;
	RandomClass randGen(10,230);
	waypoint.x = randGen.nextInt();
	waypoint.y = randGen.nextInt();
	redPower = greenPower = bluePower = 0;
}

Snake::Snake(Vector2df v)
{
    nodes.push_back(Node(v));
    oldPos.push_back(v);
	childCreateLevel = 12;
	RandomClass randGen(10,230);
	waypoint.x = randGen.nextInt();
	waypoint.y = randGen.nextInt();
	redPower = greenPower = bluePower = 0;
}

Snake::Snake(Node n)
{
   nodes.push_back(n);
   oldPos.push_back(n.getPos());
	childCreateLevel = 12;
	waypoint.x = 160;
	waypoint.y = 120;
	RandomClass randGen(10,230);
	waypoint.x = randGen.nextInt();
	waypoint.y = randGen.nextInt();
	redPower = greenPower = bluePower = 0;
}

void Snake::mutation()
{
	uint iMax = (uint)nodes.size();
	for(uint i = 0; i < iMax; i++)
	{
		nodes[i].randomiseColour();
	}
}

void Snake::disableBrain()
{
	brain.setMode(NONE);
}

void Snake::setHeadColour(CRuint colour)
{
	if(nodes.size())
		nodes[0].setColour(colour);
	redPower = greenPower = bluePower = 0;

	// assign power relative to colour
	if(colour == RED_NODE)
	{
		redPower = 1;
		//brain.setMode(AGRESSIVE);
	}
	else if(colour == GREEN_NODE)
	{
		greenPower = 1;
		//brain.setMode(NEUTRAL);
	}
	else if(colour == BLUE_NODE)
	{
		bluePower = 1;
		//brain.setMode(PASSIVE);
	}
}

void Snake::eat(Node* n)
{
	//	Set waypoint
	waypoint = n->getPos();
	//	Check if the food is the same type as the starting type of the snake
	uint tempColour = n->getColour();
	uint iMax = 1;
	if(n->getColour() == RED_NODE)
		iMax = 2;
	if(tempColour == nodes[0].getColour())
	{
		//	We add bonus for native type
		iMax = 2;

		////	BUT power get 1 more bonus
		//if(nodes[0].getColour() == RED_NODE)
		//	iMax = 3;

		//	AND GREEN only get one extra
		if(nodes[0].getColour() == GREEN_NODE)
			iMax = 1;
	}

	for (uint i = 0; i < iMax; i++)
	{
		//	Check colours and assign stats
		if(tempColour == RED_NODE)
		{
			redPower++;
			if(bluePower > 0)
				bluePower--;
		}
		else if(tempColour == GREEN_NODE)
		{
			greenPower++;
		}
		else if(tempColour == BLUE_NODE)
		{
			uint limit = 4;
			if(nodes[0].getColour() == BLUE_NODE)
			{
				limit = 6;
			}
			if(bluePower < limit)
				bluePower++;
		}
		//	We add nodes and stats
		if(nodes[0].getColour() != GREEN_NODE && tempColour == GREEN_NODE)
		{
			uint backColour = nodes.back().getColour();
			if(backColour != GREEN_NODE)
			{
				//	Check if it a red or blue and alter speed accordingly
				if(backColour == RED_NODE)
				{
					if(redPower>0)
						redPower--;
					bluePower++;
				}
				else if(backColour == BLUE_NODE)
				{
					if(bluePower>0)
						bluePower--;
					redPower++;
				}
				nodes[nodes.size()-1].setColour(GREEN_NODE);

			}
			else
			{
				nodes.pop_back();
				oldPos.pop_back();
			}
		}
		else
		{
			nodes.push_back(Node(n));
			oldPos.push_back(n->getPos());
		}
	}
}

void Snake::render(Sprite& red, Sprite& green, Sprite& blue)
{
    for(int i=nodes.size()-1; i >= 0; i--)
        nodes[i].render(red,green,blue);
}
void Snake::render(AnimatedSprite& red, AnimatedSprite& green, AnimatedSprite& blue)
{
    for(int i=nodes.size()-1; i >= 0; i--)
        nodes[i].render(red,green,blue);
}

uint Snake::getPower(CRuint colour)
{
	if(colour == RED_NODE)
	{
		return redPower;
	}
	else if(colour == GREEN_NODE)
	{
		return greenPower;
	}
return bluePower;
}


uint Snake::findStrongestType()
{
	uint strongest=0;
	uint bestColour=0;
	if(bluePower > strongest)
	{
		strongest = bluePower;
		bestColour = BLUE_NODE;
	}
	if(greenPower > strongest)
	{
		strongest = greenPower;
		bestColour = GREEN_NODE;
	}
	if(redPower > strongest)
	{
		strongest = redPower;
		bestColour = RED_NODE;
	}
	return bestColour;
}
