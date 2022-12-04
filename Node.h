#ifndef NODE_H_INCLUDED
#define NODE_H_INCLUDED

#include "Random.h"
#include "PenjinTypes.h"

enum NodeType
{
	RED_NODE = 1,
    BLUE_NODE,
	GREEN_NODE,
    CUSTOM_NODE // Used for pickups, adds to snakes dominant type
};
class Node
{
    public:
        Node()
        {
            pos.x = 0;
            pos.y = 0;
            colour = CUSTOM_NODE;
        }
        Node(Vector2df pos)
        {
            this->pos = pos;
            colour = CUSTOM_NODE;
        }
        Node(int X,int Y)
        {
            pos = Vector2df(X,Y);
            colour = CUSTOM_NODE;
        }
        Node(Node* n)
        {
            pos = n->getPos();
            colour = n->getColour();
        }
        void randomiseColour()
        {
            randGen.setLimits(RED_NODE,BLUE_NODE*4);
            colour = (uint)randGen.nextInt();
			if(colour<4)
				colour=BLUE_NODE;
			else if(colour >=4 && colour<8)
				colour=GREEN_NODE;
			else
				colour=RED_NODE;

        }
        uint getColour(){return colour;};
        void render(Sprite& red, Sprite& green,Sprite& blue)
        {
            if(colour == RED_NODE)
            {
                red.setPosition(pos.x, pos.y);
                red.render();
            }
            else if(colour == GREEN_NODE)
            {
                green.setPosition(pos.x, pos.y);
                green.render();
            }
            else if(colour == BLUE_NODE)
            {
                blue.setPosition(pos.x, pos.y);
                blue.render();
            }
        }
		void render( AnimatedSprite& red, AnimatedSprite& green,AnimatedSprite& blue)
        {
            if(colour == RED_NODE)
            {
                red.setPosition(pos.x, pos.y);
                red.render();
            }
            else if(colour == GREEN_NODE)
            {
                green.setPosition(pos.x, pos.y);
                green.render();
            }
            else if(colour == BLUE_NODE)
            {
                blue.setPosition(pos.x, pos.y);
                blue.render();
            }
        }
        void setColour(uint colour){this->colour = colour;};
        void updateXY(Vector2df velocity)
        {
            // Sort X first
            if(velocity.x < 0)
            {
                if(pos.x < 0 )
                {
                    pos.x = 320;
                }
            }
            else
            {
                if(pos.x > 312)
                {
                    pos.x = 0;
                }
            }
            pos.x += velocity.x;

            // Now handle y
            if(velocity.y < 0)
            {
                if(pos.y  < 0 )
                {
                    pos.y = 240;
                }
            }
            else
            {
                if(pos.y  > 232)
                {
                    pos.y = 0;
                }
            }
            pos.y += velocity.y;
        }
        void setX(float x){pos.x = x;};
        void setX(int x){pos.x = x;};
        void setY(float y){pos.y = y;};
        void setY(int y){pos.y = y;};
        void randomisePos()
        {
            randGen.setLimits(30,210);
            pos = Vector2df(randGen.nextInt(),randGen.nextInt());
        }
        void setPos(Vector2df pos){this->pos = pos;};
        Vector2df getPos(){return pos;};
        float getX(){return pos.x;};
        float getY(){return pos.y;};
    private:
        RandomClass randGen;
        Vector2df pos;
        uint colour;
};

#endif // NODE_H_INCLUDED
