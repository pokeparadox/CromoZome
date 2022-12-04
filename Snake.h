#ifndef SNAKE_H_INCLUDED
#define SNAKE_H_INCLUDED
#include "Random.h"

#include "SnakeBrain.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "Node.h"

//#include <iostream>
//using namespace std;
class Snake
{
    public:
        Snake();
        Snake(Node node);
        Snake(Vector2df vec);

        void eat(Node* segment);

		void mutation();	//	Mutate the nodes of this snake.

        void setHeadColour(CRuint colour);
        uint getHeadColour(){return nodes[0].getColour();};

        void setVelocityX(CRint x){velocity.x = x;};
        void setVelocityX(const float& x){velocity.x = x;};
        void setVelocityY(CRint y){velocity.y = y;};
        void setVelocityY(const float& y){velocity.y = y;};
        void setVelocityXY(CRint x, CRint y)
        {
            velocity.x = x;
            velocity.y = y;
        }
        void setVelocityXY(const Vector2df& vel){velocity = vel;};
        Vector2df getXY(){return nodes[0].getPos();};
        Fixed getX(){return nodes[0].getX();};
        Fixed getY(){return nodes[0].getY();};
		void eraseNode(CRuint i)
		{
			if(i >= nodes.size())
				return;
			nodes.erase(nodes.begin() + i);
			oldPos.erase(oldPos.begin() + i);
		}
		Node& getNode(CRuint i){return nodes[i];};
        void render(Sprite &red, Sprite &green, Sprite &blue);
		void render(AnimatedSprite &red, AnimatedSprite &green, AnimatedSprite &blue);
        void update()
        {
			if(nodes.size() == 0)
				return;
			//	Update oldpositions
			uint iMax = (uint)oldPos.size();
			for (int i = iMax-1; i >= 0; i--)
				oldPos[i] = nodes[i].getPos();

			iMax = (uint)nodes.size();

			//	Adjust the positions of the nodes

			//	Use powers to adjust speed
			if(bluePower > 1)
			{
				//	Check x direction
				float vel = velocity.x;
				if(vel == -1 || vel == 1)
					velocity.x = vel * bluePower;
				//	Check y direction
				vel = velocity.y;
				if(vel == -1 || vel == 1)
					velocity.y = vel * bluePower;

                velocity *= 0.8f;
			}

            for(int i = iMax-1; i >= 1; i--)
                nodes[i].updateXY(velocity);

			nodes[0].updateXY(velocity);

            //	Then shuffle the positions along
            for(uint i=1; i < iMax; i++)
            {
                nodes[i].setX(oldPos[i-1].x );
                nodes[i].setY(oldPos[i-1].y );
            }

			//	Check AI stuff
			if(brain.getMode() != NONE)
			{
				brain.update(waypoint, nodes[0].getPos(), oldPos[0]);
				calcNewVelocity();
			}
        }
		void clear()
		{
			nodes.clear();
			oldPos.clear();
			redPower = 0;
			greenPower = 0;
			bluePower = 0;
			Vector2df v(0,0);
			nodes.push_back(Node(v));
			oldPos.push_back(v);
		}
        size_t size()const{return nodes.size();};
		uint getChildCreateLevel(){return childCreateLevel;};
		void setChildCreateLevel(uint lvl){childCreateLevel = lvl;};
		Vector2df getVelocity(){return velocity;};
		uint findStrongestType();
		uint getPower(CRuint colour);
		void setWaypoint(const Vector2df& p){waypoint = p;};
		void setWaypoint(Vector2di p){waypoint = p;};
		void setMode(CRuint m){brain.setMode(m);};
		void disableBrain();
		uint getAIMode(){return brain.getMode();};
		void setPosition(Vector2df v){nodes.at(0).setPos(v);}

   private:
        // Disabled to revent double freed pointers.
        Snake(const Snake& BlingRef);
        Snake& operator=(const Snake& BlingRef);
		void calcNewVelocity()
		{
			uint dir = brain.getDirection();
            if(dir == UP_LEFT)
            {
				velocity.x = -1;
				velocity.y = -1;
            }
			else if(dir == UP_RIGHT)
			{
				velocity.x = 1;
				velocity.y = -1;
			}
			else if(dir == DOWN_LEFT)
			{
				velocity.x = -1;
				velocity.y = 1;
			}
			else if(dir == DOWN_RIGHT)
			{
				velocity.x = 1;
				velocity.y = 1;
			}
			else if(dir == UP)
				velocity.y = -1;
			else if(dir == DOWN)
				velocity.y = 1;
			else if(dir == LEFT)
				velocity.x =-1;
			else if(dir == RIGHT)
				velocity.x = 1;
		}

        SnakeBrain brain;
        Vector2df velocity;
        Vector2df waypoint;
        vector<Node> nodes;
        vector<Vector2df> oldPos;
        uint redPower;
        uint greenPower;
        uint bluePower;
        uint childCreateLevel;	//	This is the level needed to create a baby snake
};

#endif // SNAKE_H_INCLUDED
