#include "StateMain.h"

StateMain::StateMain()
{
    isPaused = false;
    plagueRate = 6;
    mutationRate = 4;
    //decayRate = 10;
    variables.resize(2);	//	We need 2 slots... one for AI true/false and the other for player colour
    // Slot 0 is for Colour
    choice = -1;
    longSnake = 0;
}

void StateMain::loadMenu()
{
    menu.clear();
    menu.setMenuStart(GFX::getXResolution()*0.151690141f,GFX::getYResolution()*0.333f);
    menu.loadFont("font/origami.ttf", 16);
    menu.setTextColour(Colour(YELLOW));
    menu.setSelection(1);

    menu.addItem(MENU_STRING_ITEM);
    menu.setMenuItemText("Pause Menu");
    menu.setIsSelectable(false);
    menu.addItem(MENU_STRING_ITEM);
    menu.setMenuItemText("Resume");
    menu.setIsSelectable(true);
    menu.addItem(MENU_STRING_ITEM);
    menu.setMenuItemText("Quit to Title");
    menu.setIsSelectable(true);
    menu.addItem(MENU_STRING_ITEM);
    menu.setMenuItemText("Quit Game");
    menu.setIsSelectable(true);

    pauseRect.setColour(Colour(0,0,0,64));
    pauseRect.setPosition(GFX::getXResolution()*0.151690141f,GFX::getYResolution()*0.333f);
    pauseRect.setDimensions(222,80);
}

void StateMain::loadMusic()
{
    string dir = "music/";
    string ext = ".ogg";
    music.loadMusic(dir+"water"+ext);
    music.setLooping(true);
    music.play();
}

void StateMain::loadSounds()
{
    string dir = "sounds/";
    string ext = ".wav";
    eatSound.loadSound(dir+"blurp" + ext);
    splitSound.loadSound(dir+"bloop" + ext);
    plagueSound.loadSound(dir+"plague" + ext);
}

void StateMain::loadSprites()
{
    // Setup sprites
    string dir = "images/";
    string ext = ".png";

    redSprite.loadFrames(dir + "redamoeba" + ext, 14, 1);
    redSprite.setFrameRate(FIFTEEN_FRAMES);
    redSprite.setLooping(true);


    //	Now we have loaded the sprite get the width squared
    widthSq = redSprite.getWidth();
    widthSq *= widthSq;

    string col = "p1/";

    for(uint i=1; i < 11; ++i)
    {
        string temp = dir;
        p1indicator.loadFrame(temp.append(col).append(StringUtility::leadingZero(i)).append(ext));
    }
    p1indicator.setFrameRate(FIFTEEN_FRAMES);
    p1indicator.setLooping(true);


    greenSprite.loadFrames(dir + "greenamoeba" + ext, 14, 1);
    greenSprite.setFrameRate(FIFTEEN_FRAMES);
    greenSprite.setLooping(true);

    blueSprite.loadFrames(dir + "blueamoeba" + ext, 14, 1);
    blueSprite.setFrameRate(FIFTEEN_FRAMES);
    blueSprite.setLooping(true);
    //	Setup background

    background.loadFrames(dir+"background" + ext, 8, 2);
    background.setFrameRate(FIFTEEN_FRAMES);
    background.setLooping(true);

    backPos[0] = Vector2di(0,0);
    backPos[1] = Vector2di(160,0);
    backPos[2] = Vector2di(0,120);
    backPos[3] = Vector2di(160,120);

    // Setup hazard animations
    ext=".gif";

    plagueSprite.loadFrames(dir + "plague" + ext,4,1);
    plagueSprite.setFrameRate(FIFTEEN_FRAMES);
    plagueSprite.setLooping(8);
    plagueSprite.setX(270);

    mutationSprite.loadFrames(dir + "mutation" + ext,7,1);
    mutationSprite.setFrameRate(FIFTEEN_FRAMES);
    mutationSprite.setLooping(4);
    mutationSprite.setX(290);
}

void StateMain::setupObjects()
{
    // Setup node
    food = food2 = NULL;
    food = new Node(50,50);
    food2 = new Node(50,50);
    foodEaten = false;
    foodEaten2 = false;
    food->randomisePos();
    food->randomiseColour();
    food2->randomiseColour();
    food2->randomisePos();

    setupSnakes();

    setupText();

    for (uint i = 0; i < 4; ++i)
        collisionLists[i].clear();

    godManager.setInvincibilityLimit(80);
    godManager.resize(snakes.size());
}

void StateMain::setupSnakes()
{
    //	Check for values passed from title screen
    int type = CUSTOM_NODE;
    if (variables.size())
    {
        if (variables[0].hasInt())      //  Get the colour
        {
            // We get the variables
            type = variables[0].getInt();
        }
    }
    // Setup Snakes
    snakes.push_back(NULL);
    snakes.at(0) = new Snake(Node(50,150));
    snakes.at(0)->setVelocityXY(0,0);
    snakes.at(0)->setHeadColour(type);

    //	Setup enemies
    snakes.push_back(NULL);
    snakes.at(1) = new Snake(Node(50,50));
    snakes.push_back(NULL);
    snakes.at(2) = new Snake(Node(150,150));

    //	Also depending on type we need to add more nodes.
    Node *temp = NULL;
    temp = new Node(100,100);
    temp->setColour(type);
    uint greenStart = 4;
    uint redStart = 1;
    uint blueStart = 3;
    if (type == RED_NODE)
    {
        //	Setup the green and blue enemies
        snakes.at(1)->setHeadColour(GREEN_NODE);
        snakes.at(1)->setVelocityXY(1,-1);
        temp->setColour(GREEN_NODE);
        for (uint i = 0; i < greenStart; ++i)
            snakes.at(1)->eat(temp);
        snakes.at(1)->setHeadColour(BLUE_NODE);
        snakes.at(1)->setVelocityXY(-1,-1);
        temp->setColour(BLUE_NODE);
        for (uint i = 0; i < blueStart; ++i)
            snakes.at(2)->eat(temp);
        //	Setup player
        temp->setColour(RED_NODE);
        for (uint i = 0; i < redStart; ++i)
            snakes.at(0)->eat(temp);
    }
    else if (type == GREEN_NODE)
    {
        //	Setup the red and blue enemies
        snakes.at(1)->setHeadColour(RED_NODE);
        snakes.at(1)->setVelocityXY(1,-1);
        temp->setColour(RED_NODE);
        for (uint i = 0; i < redStart; ++i)
            snakes.at(1)->eat(temp);
        snakes.at(2)->setHeadColour(BLUE_NODE);
        snakes.at(2)->setVelocityXY(-1,-1);
        temp->setColour(BLUE_NODE);
        for (uint i = 0; i < blueStart; ++i)
            snakes.at(2)->eat(temp);

        temp->setColour(GREEN_NODE);
        for (uint i = 0; i < greenStart; ++i)
            snakes.at(0)->eat(temp);
    }
    else if (type == BLUE_NODE)
    {
        //	Setup the red and green enemies
        snakes.at(1)->setHeadColour(GREEN_NODE);
        snakes.at(1)->setVelocityXY(1,-1);
        temp->setColour(GREEN_NODE);
        for (uint i = 0; i < greenStart; ++i)
            snakes.at(1)->eat(temp);

        snakes.at(2)->setHeadColour(RED_NODE);
        snakes.at(2)->setVelocityXY(-1,-1);
        temp->setColour(RED_NODE);
        for (uint i = 0; i < redStart; ++i)
            snakes.at(2)->eat(temp);

        temp->setColour(BLUE_NODE);
        for (uint i = 0; i < blueStart; ++i)
        {
            snakes.at(0)->eat(temp);
        }
    }

    //	Now set snake 0 to be player controlled
    if (variables[1].hasInt())
    {
        if(variables[1].getInt() != 1)
        {
            snakes.at(0)->disableBrain();
            snakes.at(0)->setMode(NONE);
            //	And position p1indicator to the snake[0]
            p1indicator.setPosition(snakes.at(0)->getNode(0).getPos().x-4,snakes.at(0)->getNode(0).getPos().y-4);
        }
    }
    else
    {
        switch (snakes.at(0)->getHeadColour())
        {
        case RED_NODE:
            snakes.at(0)->setMode(AGRESSIVE);
            break;
        case GREEN_NODE:
            snakes.at(0)->setMode(NEUTRAL);
            break;
        case BLUE_NODE:
            snakes.at(0)->setMode(PASSIVE);
            break;
        }
    }
    for(int i=0;i<3;i++)
    {
        if(i == 0)
        {
            if (variables[1].hasInt())
            {
                if(variables[1].getInt() != 1)
                {
                    snakes.at(i)->disableBrain();
                    snakes.at(i)->setMode(NONE);
                    //	And position p1indicator to the snake[0]
                    p1indicator.setPosition(snakes.at(i)->getNode(0).getPos().x-4,snakes.at(i)->getNode(0).getPos().y-4);
                }
            }
            else
            {
                switch (snakes.at(i)->getHeadColour())
                {
                case RED_NODE:
                    snakes.at(i)->setMode(AGRESSIVE);
                    break;
                case GREEN_NODE:
                    snakes.at(i)->setMode(NEUTRAL);
                    break;
                case BLUE_NODE:
                    snakes.at(i)->setMode(PASSIVE);
                    break;
                }
            }
        }
    }
    if (temp)
    {
        delete temp;
        temp = NULL;
    }
}

void StateMain::setupText()
{
    text.loadFont("font/origami.ttf", 12);
    text.setPosition(5,5);
    text.setRelativity(true);
    text.setColour(Colour(WHITE));
}

void StateMain::init()
{
    playPlagueAnimation = false;
    playMutationAnimation = false;

    loadMusic();
    loadMenu();
    loadSounds();
    loadSprites();

    setupObjects();
}

void StateMain::clear()
{
    if (food)
    {
        delete food;
        food = NULL;
    }
    if (food2)
    {
        delete food2;
        food2 = NULL;
    }
}

StateMain::~StateMain()
{
    clear();
    for(int i = snakes.size()-1; i>=0; --i)
    {
        if(snakes.at(i))
        {
            delete snakes.at(i);
        }
        snakes.at(i) = NULL;
    }
}

void StateMain::render()
{
    //	Draw the background first
    int currFrame = background.getCurrentFrame();
    for(int i = 3; i >= 0; i--)
    {
        background.setPosition(backPos[i]);
        background.setCurrentFrame(NumberUtility::wrapValue(currFrame + i, 16));
        background.render();
    }
    background.setCurrentFrame(currFrame);
    // We check if the food exists and if it does we draw it.
    if (!foodEaten)
        food->render(redSprite,greenSprite,blueSprite);
    if (!foodEaten2)
        food2->render(redSprite,greenSprite,blueSprite);

    //	draw all snakes
    uint numSnakes = (uint)snakes.size();
    for (int i = numSnakes-1; i >= 0; i--)
        snakes.at(i)->render(redSprite,greenSprite,blueSprite);
    if (variables[1].hasInt())        //  Check for AI mode
    {
        if(variables[1].getInt() != 1)
        {
            //	Display p1 indicator
            p1indicator.render();
            snakes.at(0)->disableBrain();
            snakes.at(0)->setMode(NONE);
        }
    }
    else
    {
        switch (snakes.at(0)->getHeadColour())
        {
        case RED_NODE:
            snakes.at(0)->setMode(AGRESSIVE);
            break;
        case GREEN_NODE:
            snakes.at(0)->setMode(NEUTRAL);
            break;
        case BLUE_NODE:
            snakes.at(0)->setMode(PASSIVE);
            break;
        }
    }
    //  Display Game Info
    text.setPosition(5,5);
    //text.print(screen,"CromoZomes:");
    //text.print(screen, (int)numSnakes);
    text.print((string)"Score:");
    text.print(score.getScore());
    text.print((string)" Mult: ");
    text.print(score.getMultiplier());
    text.print((string)"X");

    ///  EFFECT ANIMATIONS
    if (playPlagueAnimation)
    {
        plagueSprite.render();
        text.setPosition(110,115);
        text.setColour(Colour(GREEN));
        text.print((string)"THE PLAGUE!");
        text.setColour(Colour(WHITE));
    }
    if (playMutationAnimation)
    {
        mutationSprite.render();
        text.setPosition(110,115);
        text.setColour(Colour(GREEN));
        text.print((string)"MU");
        text.setColour(Colour(RED));
        text.print((string)"TA");
        text.setColour(Colour(BLUE));
        text.print((string)"TI");
        text.setColour(Colour(PURPLE));
        text.print((string)"ON");
        text.setColour(Colour(ORANGE));
        text.print((string)"!!");
        text.setColour(Colour(WHITE));
    }
}

void StateMain::update()
{
    if(!isPaused)
        godManager.update();

    //	animate sprite
    redSprite.update();
    blueSprite.update();
    greenSprite.update();
    p1indicator.update();

    for(int i = 3; i >= 0; i--)
        background.update();

    if (playPlagueAnimation)
        plagueSprite.update();

    if (playMutationAnimation)
        mutationSprite.update();

    if (plagueSprite.hasFinished())
    {
        playPlagueAnimation = false;
        plagueSprite.rewind();
    }
    if (mutationSprite.hasFinished())
    {
        playMutationAnimation = false;
        mutationSprite.rewind();
    }

    //staleMate();    //  Workaround for stalemate situations
    uint numSnakes = (uint)snakes.size();
    if(numSnakes==0)
        return;
    //  We need to find the longest snake... we should do it in these loops as we are already running through them
    int snakeLength = 0;
    longSnake = 0; //  Default to snake 0!

    for (uint i = 0; i < numSnakes; ++i)
    {
        uint snSize = snakes.at(0)->size();
        if (snSize > snakeLength)
        {
            snakeLength=snSize;
            longSnake = i;
        }
        if (i>0)
        {
            uint col = snakes.at(0)->getHeadColour();
            Vector2df closest(0,0);
            uint part;
            uint cLSize;
            randGen.setLimits(0,snakes.size() - 1);

            //  Get the partition of the current AI snake
            part = partition.getPartition(snakes.at(0)->getXY());
            cLSize = collisionLists[part].size();
            switch (col)
            {
            case RED_NODE:
                if (snSize >= 5) //if the snake is longer than 5 nodes
                {
                    if (cLSize > 1) //if it is still in the colission lists
                    {
                        for (int j=0;j<cLSize-1;j++)
                        {
                            if (i == j) //if we are checking it against itself
                            {
                                closest = food->getPos();
                                continue;
                            }else
                            {
                                uint snake = collisionLists[part][j]; //find the closest snake
                                if (snake > snakes.size() - 1)
                                {
                                    cLSize--; //this one has already been processed
                                    continue;
                                }
                                if (snakes.at(snake)->size() < 1)
                                {
                                    cLSize--; //as has this one
                                    continue;
                                }
                                //  Get the tail segment of the closest snake
                                Vector2df snakePos = snakes.at(snake)->getNode(snakes.at(snake)->size()-1).getPos();

                                //get the distance between this snakes head and the closests tail
                                Vector2df difference(snakePos - closest);
                                if (difference.lengthSquared() < widthSq)
                                {
                                    //tell this snake that another is close
                                    closest = snakePos;
                                }
                            }
                        }
                    }
                    else
                    {
                        //or tell it to hunt for food
                        closest = food->getPos();
                    }
                }
                else
                {
                    //if the snake is still too small, tell it to hunt for food
                    closest = food->getPos();
                }
                if(i>= snakes.size())
                    continue;
                snakes.at(i)->setMode(AGRESSIVE);
                snakes.at(i)->setWaypoint(closest);
                break;
            case GREEN_NODE:
                //Green snakes always hunt for food
                if(i>= snakes.size())
                    continue;
                snakes.at(i)->setMode(NEUTRAL);
                snakes.at(i)->setWaypoint(food->getPos());
                break;
            case BLUE_NODE:
                if (snSize < 8) //if the snake is less than 8 segments long
                {
                    if (cLSize > 1)//and its on the colission list
                    {
                        for (int j=0;j<cLSize-1;j++)
                        {
                            if (i == j)
                            {
                                //do nothing, have the sanke continue on its way
                                continue;
                            }else
                            {
                                uint snake = collisionLists[part][j];
                                if (snake > snakes.size() - 1)
                                {
                                    cLSize--;
                                    continue;
                                }
                                if (snakes.at(snake)->size() < 1)
                                {
                                    cLSize--;
                                    continue;
                                }
                                //  We want the blue guys to run away from the head segment!
                                Vector2df snakePos = snakes.at(snake)->getXY();
                                snakePos -= closest;
                                if (snakePos.lengthSquared() < widthSq)
                                {
                                    closest = snakePos; //run away from the nearest snake
                                    //blue snakes are passive when other snakes are around;
                                    if(i>= snakes.size())
                                        continue;

                                    snakes.at(i)->setMode(PASSIVE);
                                    snakes.at(i)->setWaypoint(closest);
                                }
                            }
                        }
                    }

                }
                    else
                    {
                        //but they can also hunt for food
                        closest = food->getPos();
                        if(i>= snakes.size())
                            continue;

                        snakes.at(i)->setMode(NEUTRAL);
                        snakes.at(i)->setWaypoint(closest);
                    }
                break;
            }
        }

        if(i>= snakes.size())
            continue;

        snakes.at(i)->update();
        //processCollisionLists();
        newCollisions();
    }//END FOR

    //	And position p1indicator to the snake[0]
    p1indicator.setPosition(snakes.at(0)->getNode(0).getX()-4,snakes.at(0)->getNode(0).getY()-4);

    //  If any of the snakes get too long we trigger a decay
    //  The longest snake is worked in the earlier AI loop to avoid having to traverse the array again!

    // update effects
    numSnakes = snakes.size();
    if (numSnakes > plagueRate)
    {
        //	 reset score multiplier
        plagueSound.play();
        //score.setMultiplier(1);
        ++plagueRate;
        uint halfRate = plagueRate*0.5f;
        for (uint i = 0; i < halfRate; ++i)
        {
            delete snakes.back();
            snakes.back() = NULL;
            snakes.pop_back();
            --numSnakes;
        }
        playPlagueAnimation = true;
    }
    else if (numSnakes > mutationRate)
    {
        plagueSound.play();
        //score.changeScore(10);
        mutationRate+=2;

        uint iMax = (uint)snakes.size();
        //	kill off some snakes if we can
//        if (iMax > 4)
//        {
//            //	kill three snakes
//            for (uint i =0; i < 3; i++)
//                snakes.pop_back();
//
//            iMax = (uint)snakes.size();
//        }
        for (uint i = 0; i < iMax; i++)
        {
            snakes.at(i)->mutation();
            if(snakes.at(i)->getAIMode() != NONE)
            {
                uint col = snakes.at(i)->getHeadColour();
                switch(col)
                {
                    case RED_NODE:
                        snakes.at(i)->setMode(AGRESSIVE);
                        break;
                    case BLUE_NODE:
                        snakes.at(i)->setMode(PASSIVE);
                        break;
                    case GREEN_NODE:
                        snakes.at(i)->setMode(NEUTRAL);
                        break;
                }
            }
        }
        playMutationAnimation = true;
    }
    rebuildCollisionLists();
}

void StateMain::userInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    //	Check for start button to pause game
    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }

    if (variables[1].hasInt())        //  Check for AI mode
    {
        if(variables[1].getInt() != 1)
        {
            if (input->isUp())
                snakes.at(0)->setVelocityXY(0,-1);
            if (input->isDown())
                snakes.at(0)->setVelocityXY(0,1);
            if (input->isLeft())
                snakes.at(0)->setVelocityXY(-1,0);
            if (input->isRight())
                snakes.at(0)->setVelocityXY(1,0);
#ifndef PLATFORM_WII
            // Check diagonals
            if (input->isUpLeft())
                snakes.at(0)->setVelocityXY(-1,-1);
            if (input->isUpRight())
                snakes.at(0)->setVelocityXY(1,-1);
            if (input->isDownLeft())
                snakes.at(0)->setVelocityXY(-1,1);
            if (input->isDownRight())
                snakes.at(0)->setVelocityXY(1,1);
#endif
            //	Check for DPAD "Diagonals"
            if (input->isUp() && input->isLeft())
                snakes.at(0)->setVelocityXY(-1,-1);
            if (input->isUp() && input->isRight())
                snakes.at(0)->setVelocityXY(1,-1);
            if (input->isDown() && input->isLeft())
                snakes.at(0)->setVelocityXY(-1,1);
            if (input->isDown() && input->isRight())
                snakes.at(0)->setVelocityXY(1,1);
        }
    }
#ifdef PLATFORM_GP2X
    //	adjust volume
    uint volume = music.getVolume();
    if (input->isVolumeUp() && volume < 128)
        music.setVolume(volume+2);
    else if (input->isVolumeDown() && volume > 0)
        music.setVolume(volume-2);
#endif
}

void StateMain::rebuildCollisionLists()
{
    // Clear collision lists
    for (uint i = 0; i < 4; ++i)
        collisionLists[i].clear();

    //	work out which quarter of the screen the food is in
    Vector2df foodPos = food->getPos();
    Vector2df foodPos2 = food2->getPos();

    uint foodQuarter = partition.getPartition(foodPos);
    uint foodQuarter2 = partition.getPartition(foodPos2);

    uint currentPartition = UNKNOWN_PARTITION;
    uint iMax = snakes.size();

    godManager.resize(iMax);

    //  Now rebuild collisionLists
    for (uint i = 0; i < iMax; ++i)
    {
        Vector2df currPos = snakes.at(i)->getXY();
        currentPartition = partition.getPartition( currPos);
        //	Do food check
        if (currentPartition == foodQuarter)
        {
            currPos -= foodPos;
            if (currPos.lengthSquared() <= widthSq)
            {
                foodEaten = true;
            }
            if (foodEaten)
            {
                //	check for snake[0]
                if (i == 0)
                {
                    score.changeScore(food->getColour()+1);
                }
                snakes.at(i)->eat(food);
                eatSound.play();
                food->randomisePos();
                foodPos = food->getPos();
                foodQuarter = partition.getPartition(foodPos);
                food->randomiseColour();
                foodEaten = false;
            }
        }
        else if (currentPartition == foodQuarter2)
        {
            currPos -= foodPos2;
            if (currPos.lengthSquared() <= widthSq)
            {
                foodEaten2= true;
            }
            if (foodEaten2)
            {
                //	check for snake[0]
                if (i == 0)
                {
                    score.changeScore(food2->getColour()+1);
                }
                snakes.at(i)->eat(food2);
                eatSound.play();
                food2->randomisePos();
                foodPos2 = food2->getPos();
                foodQuarter2 = partition.getPartition(foodPos2);
                food2->randomiseColour();
                foodEaten2 = false;
            }
        }
        //	Partition snakes
        if (currentPartition == TOP_LEFT)
            collisionLists[TOP_LEFT].push_back(i);
        else if (currentPartition == TOP_RIGHT)
            collisionLists[TOP_RIGHT].push_back(i);
        else if (currentPartition == BOTTOM_LEFT)
            collisionLists[BOTTOM_LEFT].push_back(i);
        else
            collisionLists[BOTTOM_RIGHT].push_back(i);	//BOTTOM_RIGHT
    }
}

void StateMain::checkSubList(ScreenPartition& subList, CRuint quad)
{
    int maxSnakes = snakes.size();

    /// Run through the available snakes in this quadrant
    int CLmax = collisionLists[quad].size()-1;
    for(int CL = CLmax; CL >= 0; --CL)
    {
        int currSnake = collisionLists[quad][CL];
        if(currSnake >= maxSnakes)
            collisionLists[quad][CL] = -1;      //  Snake out of bounds, must be dead.
        if(collisionLists[quad][CL] == -1)      //  If snake is dead, we don't need to do further tests!
            continue;

        /// Get snakes to test in this sub-quadrant
        Vector2df currPos = snakes.at(currSnake)->getXY();
        int currSubQuad = subList.getPartition(currPos);
        for(int sQ = CLmax; sQ >= 0; --sQ)
        {
            if(sQ == CL)    //  Don't test against self
                continue;

            int enemySnake = collisionLists[quad][sQ];
            if(enemySnake == -1)    // snake is ded snake
                continue;
            if(currSnake == enemySnake)
                continue;
            if(currSubQuad == subList.getPartition(snakes.at(enemySnake)->getXY()))
            {
                /// snakes are in the same quad, do distance collision tests
                distanceCollision(currSnake,enemySnake,CL,sQ,quad);
            }
        }
    }
}

void StateMain::cleanDead()
{
    for(int currSnake = snakes.size()-1; currSnake >= 0; --currSnake)
    {
        if(snakes.at(currSnake)->size() == 0)
        {
            /// Check if player is dead
            if(currSnake == 0)
            {
                /// if it is a human player!
                if (variables[1].hasInt() && variables[1].getInt() != 1)
                {
                       variables[1] = 0;
                        //	push score through and add it to variable stack
                        //  We use the same slot as AI true/false since AI true means no high score!
                        variables[1].setInt((int)score.getScore());
                        //	Now change to gameover state
                        setNextState(STATE_GAMEOVER);
                }
            }
            delete snakes.at(currSnake);
            snakes.at(currSnake) = NULL;
            snakes.erase(snakes.begin() + currSnake);
        }
    }
}

void StateMain::distanceCollision(CRint curr, CRint enem,  CRint currIndex, CRint enemIndex, CRuint quad)
{
    /// first check if either snake is actually dead
    int currMax = snakes.at(curr)->size();
    int enemMax = snakes.at(enem)->size();
    if(currMax <= 0)    //  flag as dead - can't eat or be eaten!
        collisionLists[quad][currIndex] = -1;
    if(enemMax <= 0)    //  flag as dead
        collisionLists[quad][enemIndex] = -1;
    if(collisionLists[quad][currIndex] == -1 || collisionLists[quad][enemIndex] == -1)
        return;

    /// Check invincibility status of eatee (can't eat if invincible)
    if(godManager.getIsInvincible(collisionLists[quad][enemIndex]))
        return;

    /// If the snake is alive and the eatee is not invincible then we will see if we can eat him
    // Get eater's head position
    Vector2di diff = snakes.at(curr)->getXY();
    // compare head position to all nodes on eatee
    for(int n = snakes.at(enem)->size()-1; n >=0; --n)
    {
        Vector2di temp(diff.x,diff.y);
        temp -= snakes.at(enem)->getNode(n).getPos();
        if (temp.lengthSquared() <= widthSq-32)
        {
            if (curr == 0) // If the eater is the player
                score.changeScore(snakes.at(enem)->getNode(n).getColour()+1);
            if (enem == 0) // If the eatee is the player
                score.setMultiplier(1);


            eatSound.play();
            snakes.at(curr)->eat(&snakes.at(enem)->getNode(n));
            snakes.at(enem)->eraseNode(n);
            if(snakes.at(enem)->size() <= 0)
                collisionLists[quad][enemIndex] = -1;
            else
                godManager.setIsInvincible(enem);
            //  We don't want to eat multiple nodes simultaneously.

            if (n == 0)     // If the head is eaten, you're dead!
            {
                //  compare lengths of snakes to see which one should die
                if(currMax > enemMax)
                {
                    if (enem == 0)
                    {
                        if (variables[1].hasInt() && variables[1].getInt() != 1)
                        {
                           variables[1] = 0;
                            //	push score through and add it to variable stack
                            //  We use the same slot as AI true/false since AI true means no high score!
                            variables[1].setInt((int)score.getScore());
                            //	Now change to gameover state
                            setNextState(STATE_GAMEOVER);
                        }
                    }
                }
            }
            break;
        }
    }
}

void StateMain::newCollisions()
{
    // Check through each list
    for(int l = 3; l>= 0; --l)
    {
        ScreenPartition tempSP;
        if(l == BOTTOM_LEFT)
            tempSP.setPartition(Vector2di(80,60));
        else if(l == BOTTOM_RIGHT)
            tempSP.setPartition(Vector2di(240,60));
        else if(l == TOP_RIGHT)
            tempSP.setPartition(Vector2di(240,180));
        else    //  TOP_LEFT
            tempSP.setPartition(Vector2di(80,180));
        checkSubList(tempSP, l);
    }

    /// Once all collisions have been tested, clean out all snakes marked as dead
    cleanDead();
    checkLevelUp();
}

void StateMain::processCollisionLists()
{
    Vector2di diff;
    uint iMax = (uint)snakes.size();
    godManager.resize(iMax);

    //	run through all lists
    for (uint i = 0; i < 4; i++)
    {
        //	run through snakes in current list
        uint jMax = (uint)collisionLists[i].size();
        for (uint j = 0; j < jMax; j++)
        {
            //comparing against snakes in same partition
            uint jMax = (uint)collisionLists[i].size();
            for (uint k = 0; k < jMax; k++)
            {
                //	Guard against self collisions
                if (j == k)
                    continue;
                //	Guard against j and k being out of bounds
                if (k >= collisionLists[i].size() || j >= collisionLists[i].size())
                    continue;
                if (collisionLists[i][k] >= snakes.size() || collisionLists[i][j] >= snakes.size())
                    continue;
                //  Guard against snakes already removed from collision lists
                if (collisionLists[i][j] == -1 || collisionLists[i][k] == -1)
                    continue;
                //  Check if a snake is temprorarily invincible
                if(godManager.getIsInvincible(collisionLists[i][k]))
                    continue;

                uint numNodes = snakes.at(collisionLists[i][k])->size();

                /*  if the number of nodes on any snakes is greater than the trigger we should randomly reduce the current
                number of nodes in all snakes to keep tabs on game speed.

                then increase the trigger by one.

                */

                //collisionLists[i][j]
                diff = Vector2di(snakes.at(collisionLists[i][j])->getNode(0).getPos());

                //	now compare this snake's head to all the nodes in the enemy snake
                for (uint l=0; l < numNodes; l++)
                {
                    Vector2di temp(diff.x,diff.y);
                    temp -= snakes.at(collisionLists[i][k])->getNode(l).getPos();
//					uint tempSize = snakes[collisionLists[i][k]]->size();
                    if (temp.lengthSquared() <= widthSq-32)
                    {
                        bool isPlayer = false;
                        if (collisionLists[i][j] == 0) // If the eater is the player
                        {
                            score.changeScore(snakes.at(collisionLists[i][k])->getNode(l).getColour()+1);
                        }
                        if (collisionLists[i][k] == 0) // If the eatee is the player
                        {
                            isPlayer = true;
                            score.setMultiplier(1);
                        }
                        eatSound.play();
                        snakes.at(collisionLists[i][j])->eat(&snakes.at(collisionLists[i][k])->getNode(l));
                        snakes.at(collisionLists[i][k])->eraseNode(l);
                        godManager.setIsInvincible(collisionLists[i][k]);
                        numNodes--;
                        //	This break means we can only remove node per update loop.

                        // We should check if the snake to be eaten is the player... and if so check all nodes... otherwise do this break
                        if (!isPlayer)
                            break;
                    }
                }
                if (numNodes <= 0)
                {
                    if (variables[1].hasInt() && variables[1].getInt() != 1)
                    {
                           variables[1] = 0;
                            //	push score through and add it to variable stack
                            //  We use the same slot as AI true/false since AI true means no high score!
                            variables[1].setInt((int)score.getScore());
                            //	Now change to gameover state
                            setNextState(STATE_GAMEOVER);
                    }
                    delete snakes.at(collisionLists[i][k]);
                    snakes.at(collisionLists[i][k]) = NULL;
                    snakes.erase(snakes.begin() + collisionLists[i][k]);
                    collisionLists[i][k] = -1;
                    jMax--;
                }
            }
        }
    }
}

void StateMain::checkLevelUp()
{
    //	run through the lists of snakes
    int iMax = snakes.size();
    for (int i = 0; i < iMax; ++i)
    {
        //	check if their length is longer than their levelup number
        if (snakes.at(i)->size() > snakes.at(i)->getChildCreateLevel())
        {
            if(i == 0 && variables[1].hasInt())
            {
                if(variables[1].getInt() != 1)
                    snakes.at(i)->setMode(NONE);
            }
            splitSound.play();
            // increase multiplier for score
            //if(i == 0)
            score.increaseMultiplier();

            //	We create a new snake based on the best abilities of this snake
            //Snake *tempSnake = new Snake(Node(snakes[i]->getX(), snakes[i]->getY()));
            //tempSnake->setHeadColour(snakes[i]->findStrongestType());
            //	We then half the size of the players current snake
            int halfSize = snakes.at(i)->size()*0.5f;
            for (int j = halfSize-1;j >=1; --j )
                snakes.at(i)->eraseNode(j);
            int sSize = snakes.at(i)->size();
            //	Swap the snakes around so that the player is in control of the newly born snake now
            snakes.push_back(NULL);
            Vector2df pos(snakes.at(i)->getNode(sSize-1).getX(), snakes.at(i)->getNode(sSize-1).getY());
            pos -= snakes.at(i)->getVelocity();
            snakes.back() = new Snake(pos);
            snakes.back()->setHeadColour(snakes.at(i)->getHeadColour());
            snakes.back()->setChildCreateLevel(20);
            ++iMax;

            /*godManager.resize(iMax);
            godManager.setIsInvincible(sSize-1);
            godManager.setIsInvincible(i);*/
            halfSize = snakes.at(i)->size()/5;
            for (uint j = 0;j < halfSize; j++  )
                snakes.back()->eat(&snakes.at(i)->getNode(j));

            Vector2di tempV(-snakes.at(i)->getVelocity());
            if (tempV.x == 0)
                tempV.x=1;
            if (tempV.y == 0)
                tempV.y=1;
            snakes.back()->setVelocityXY(tempV.x,tempV.y);
            snakes.back()->setMode(snakes.at(i)->getAIMode());

            if (i == 0)
            {
                if(variables[1].hasInt())
                {
                    if(variables[1].getInt() == 1)
                    {
                        uint AIMode = snakes.at(i)->getAIMode();
                        switch(AIMode)
                        {
                            case NONE:
                            {
                                snakes.at(i)->disableBrain();
                                godManager.setIsInvincible(i);
                                break;
                            }
                            case AGRESSIVE:
                            {
                                snakes.at(i)->setMode(AGRESSIVE);
                                break;
                            }
                            case NEUTRAL:
                            {
                                snakes.at(i)->setMode(NEUTRAL);
                                break;
                            }
                            case PASSIVE:
                            {
                                snakes.at(i)->setMode(PASSIVE);
                                break;
                            }
                        }
                    }else
                    {
                        snakes.at(i)->disableBrain();
                        godManager.setIsInvincible(i);
                    }
                }
            }else //update AI for all other snakes
            {           uint AIMode = snakes.at(i)->getAIMode();
                        switch(AIMode)
                        {
                            case NONE:
                            {
                                //there can only be 1 player
                                snakes.at(i)->setMode(NEUTRAL);
                                break;
                            }
                            case AGRESSIVE:
                            {
                                snakes.at(i)->setMode(AGRESSIVE);
                                break;
                            }
                            case NEUTRAL:
                            {
                                snakes.at(i)->setMode(NEUTRAL);
                                break;
                            }
                            case PASSIVE:
                            {
                                snakes.at(i)->setMode(PASSIVE);
                                break;
                            }
                        }
            }
            snakes.at(i)->setHeadColour(snakes.at(i)->findStrongestType());
            // Increase the players ChildCreateLevel by 5 points
            snakes.at(i)->setChildCreateLevel(snakes.at(i)->getChildCreateLevel()+5);
            tempV = snakes.at(i)->getVelocity();
            if (tempV.x == 0)
                tempV.x=-1;
            if (tempV.y == 0)
                tempV.y=-1;
            snakes.at(i)->setVelocityXY(tempV.x,tempV.y);
        }
    }
}

void StateMain::pauseScreen()
{
    //GFX::renderStatic(45,90,255,190,0,10);
    pauseRect.render();
    menu.render();
}

void StateMain::pauseUpdate()
{
    menu.update();
}

void StateMain::pauseInput()
{
    input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
    //	Check for start button to pause game
    if (input->isStart())
    {
        pauseToggle();
        input->resetKeys();
    }
    if (input->isUp())
    {
        menu.menuUp();
        input->resetKeys();
    }
    if (input->isDown())
    {
        menu.menuDown();
        input->resetKeys();
    }

    if (input->isB())
    {
        choice = menu.getSelection();
        if (choice == 1)
        {
            isPaused = false;
        }
        else if (choice == 2)
        {
            setNextState(STATE_TITLE);
        }
        else if (choice == 3)
        {
            nullifyState();
        }
        input->resetKeys();
    }
#ifdef PLATFORM_GP2X
    //	adjust volume
    uint volume = music.getVolume();
    if (input->isVolumeUp() && volume < 128)
        music.setVolume(volume+2);
    else if (input->isVolumeDown() && volume > 0)
        music.setVolume(volume-2);
#endif
}
