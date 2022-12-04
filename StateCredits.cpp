#include "StateCredits.h"

StateCredits::StateCredits()
{
	variables.resize(1);
	upOrDown = 0;
	txtUpOrDown = true;
	firstRun = true;
	renderParticles = true;
	startX = 158;
	startY = 118;
	w = 162;
	h = 122;
}

StateCredits::~StateCredits()
{
	clear();
}

void StateCredits::clear()
{
	//this->text.clear();
	this->camSprite.clearFrames();
	this->music.freeAll();
	for(int i = NUM_AVAS-1; i >= 0; i--)
		staticAvatars[i].clear();

    emit.clear();
}

void StateCredits::init()
{
    halfRes = (GFX::getXResolution()*0.5f) -10;
	//	We will use this to progress the current credit line.
	currentLine = 0;

	//	Setup timer for credits scrolling
    timer.start();
	timer.setMode(MILLI_SECONDS);

	//	Setup SDL Text Handler
	text.loadFont("font/chromo.ttf", 19);
	text.setRelativity(false);	//	We will control all positions of text manually
	textPos.x = 10;
	textPos.y = 110;
	text.setPosition(textPos);
	text.setColour(Colour(WHITE));

	//	Setup randomNumbers for colours
	randGen.setLimits(0,255);


	//	Load music
	string dir = "music/";
	music.loadMusic(dir+"okeanos-psychedelic_waves.xm");
	music.setLooping(true);
	music.play();

	//	Load sprites
	dir = "images/avatars/";
	string ext = ".png";

	//	setup cameleon animated sprite
    camSprite.loadFrames(dir + "cameleon"+ext,5,1);
	camSprite.setFrameRate(FIFTEEN_FRAMES);
	camSprite.setPosition(160,45);
	mariela.loadFrames(dir+"mariela"+ext,4,1);
	mariela.setFrameRate(FIFTEEN_FRAMES);
	mariela.setPosition(160,45);

	staticAvatars[0].loadSprite(dir+"pokeparadox"+ext);
	staticAvatars[1].loadSprite(dir+"pirategames"+ext);
	staticAvatars[2].loadSprite(dir+"a_sn"+ext);

	staticAvatars[4].loadSprite(dir+"drian"+ext);
	staticAvatars[5].loadSprite(dir+"guru"+ext);
	staticAvatars[6].loadSprite(dir+"pkostrze"+ext);
	staticAvatars[7].loadSprite(dir+"rokdcasbah"+ext);
	staticAvatars[8].loadSprite(dir+"tripmonkeyuk"+ext);
	staticAvatars[13].loadSprite(dir+"studiosonistar"+ext);
    staticAvatars[14].loadSprite(dir+"markoez"+ext);
    staticAvatars[15].loadSprite(dir+"penjin"+ext);
	staticAvatars[9].loadSprite(dir+"unknown"+ext);
	ext = ".gif";
	staticAvatars[10].loadSprite(dir+"parkydr"+ext);
    staticAvatars[11].loadSprite(dir+"yakumofuji"+ext);
    ext = ".jpg";
    staticAvatars[3].loadSprite(dir+"davidbeoulve"+ext);
    staticAvatars[12].loadSprite(dir+"magnar"+ext);

	for(int i = NUM_AVAS-1; i >= 0; i--)
		staticAvatars[i].setPosition(160,45);

    SDL_Rect bounds;
    bounds.x = bounds.y = 0;
    bounds.w = 320;
    bounds.h = 240;

    emit.resize(3);
	emit[0].setMax(10);
	emit[0].setLifeTime(400);
	emit[0].setPosition(Vector2df(160,120));
	emit[0].setMaxVelocity(Vector2df(1.5f,1.5f));
	emit[0].setColour(Colour(ORANGE));
	emit[0].setDeviation(0.2f);
	emit[0].setGravity(0.001f);
	emit[0].setBoundaries(bounds);

    emit[1].setMax(10);
    emit[1].setLifeTime(400);
	emit[1].setPosition(Vector2df(160,120));
	emit[1].setMaxVelocity(Vector2df(1.5f,1.5f));
	emit[1].setColour(Colour(ORANGE));
	emit[1].setDeviation(0.2f);
	emit[1].setGravity(0.001f);
	emit[1].setBoundaries(bounds);

    emit[2].setMax(40);
    emit[2].setLifeTime(500);
	emit[2].setPosition(Vector2df(140,90));
	emit[2].setMaxVelocity(Vector2df(1.2f,1.2f));
	emit[2].setColour(Colour(BLUE));
	emit[2].shouldFadeColour(true);
	emit[2].setFadeValue(-1);
	emit[2].setDeviation(0.1f);
	emit[2].setGravity(0.001f);
	emit[2].setBoundaries(bounds);
}

void StateCredits::userInput()
{
	//	Refresh inputs
	input->update();
    #ifdef PLATFORM_PC
        if(input->isQuit())
            nullifyState();
    #endif
	//	Skip current Credits item
	//if(input->IsB())
		//timer.incLimit(-timer.getRemaining());

	//	adjust volume
	#ifdef PLATFORM_GP2X
        uint volume = music.getVolume();
        if(input->isVolumeUp() && volume < 128)
            music.setVolume(volume+2);
        else if(input->isVolumeDown() && volume > 0)
            music.setVolume(volume-2);
    #endif
	//	skip credits
	if(input->isStart())
		setNextState(STATE_TITLE);
}

void StateCredits::render(SDL_Surface *screen)
{
	// Clear screen
	SDL_Rect filler;

	if(currentLine == CREDITS_SIZE-1)
		firstRun = true;

	if(text.getWidth() > halfRes || firstRun)
	{
		filler.x = 0;
		filler.y = 0;
		filler.w = 320;
		filler.h = 240;
		firstRun = false;
	}
	else
	{
		filler.x = 0;
		filler.y = 0;
		filler.w = 160;
		filler.h = 240;

	}
	SDL_FillRect(screen,&filler, 0);

	//  Render all particles
	if(renderParticles)
	{
        for(int e = emit.size()-1; e >= 0; --e)
            emit[e].render();
	}

	string tempString = CREDITS[currentLine];
	//	Now process avatars
	if(strstr(tempString.c_str(),"PokeParadox"))
	{
		staticAvatars[0].render();
	}
	else if(strstr(tempString.c_str(),"Pirate Games"))
	{
		staticAvatars[1].render();
	}
	else if(strstr(tempString.c_str(),"A SN"))
	{
		staticAvatars[2].render();
	}
	else if(strstr(tempString.c_str(),"David Beoulve"))
	{
		staticAvatars[3].render();
	}
	else if(strstr(tempString.c_str(),"Dr Ian"))
	{
		staticAvatars[4].render();
	}
	else if(strstr(tempString.c_str(),"Squidge"))
	{
		staticAvatars[5].render();
	}
	else if(strstr(tempString.c_str(),"pkostrze"))
	{
		staticAvatars[6].render();
	}
	else if(strstr(tempString.c_str(),"rokdcasbah"))
	{
		staticAvatars[7].render();
	}
	else if(strstr(tempString.c_str(),"Tripmonkey uk"))
	{
		staticAvatars[8].render();
	}
	else if(strstr(tempString.c_str(),"Jackd") || strstr(tempString.c_str(),"wiseguy109") || strstr(tempString.c_str(),"stressball") || strstr(tempString.c_str(),"Okeanos") || strstr(tempString.c_str(),"foxblock"))
	{
		staticAvatars[9].render();
	}
	else if(strstr(tempString.c_str(),"Parkydr"))
	{
		staticAvatars[10].render();
	}
	else if(strstr(tempString.c_str(),"YakumoFuji"))
	{
		staticAvatars[11].render();
	}
	else if(strstr(tempString.c_str(),"Magnar"))
	{
	    staticAvatars[12].render();
	}
	else if(strstr(tempString.c_str(),"Cameleon"))
	{
		camSprite.render();
	}
	else if(strstr(tempString.c_str(),"Sonistar"))
	{
	    staticAvatars[13].render();
	}
	else if(strstr(tempString.c_str(),"MarkoeZ"))
	{
	    staticAvatars[14].render();
    }
	else if(strstr(tempString.c_str(),"Penjin"))
	{
	    staticAvatars[15].render();
    }
	else if(strstr(tempString.c_str(),"Mariela"))
	{
	    mariela.render();
    }
	else if(strstr(tempString.c_str(),"[VERSION]"))
        tempString = "V" + (string)AutoVersion::FULLVERSION_STRING;


	if(currentLine != CREDITS_SIZE-1)
	{
		randGen.setLimits(3,4);
		#ifdef PENJIN_SDL
		GFX::renderStatic(screen,155,5,315,235,randGen.nextInt(),-1);
		GFX::borderColouring(screen,0,0,320,240,5,textColour);
		#endif
	}
	else
	{
		if(startX-4 > 0)
		{
			startX-=4;
			w+=4;
		}
		if(startY-2 > 0)
		{
			startY-=2;
			h+=2;
		}
		#ifdef PENJIN_SDL
		GFX::renderStatic(screen,startX,startY,w,h,4,-1);
		#endif
	}
	text.print(tempString);
}

void StateCredits::unlimitedUpdate()
{
	camSprite.update();
	mariela.update();
	if(timer.getScaledTicks() >= 3205)
	{
        txtUpOrDown = !txtUpOrDown;
	    timer.start();
		firstRun = true;
		randGen.setLimits(0,255);
		textColour.setColour((uchar)randGen.nextInt(),randGen.nextInt(),randGen.nextInt());
		while(textColour.red<=20 && textColour.green<=20 && textColour.blue<=20)
			textColour.setColour((uchar)randGen.nextInt(),randGen.nextInt(),randGen.nextInt());

		text.setColour(textColour);

		//	Get the latest credit line from the file by increasing currentLine
		if(currentLine < (uint)CREDITS_SIZE-1)
			++currentLine;
		else
			setNextState(STATE_TITLE);

        //	Check if it is the last credit item and begin to fade the music.
        if(currentLine == CREDITS_SIZE-1)
        {
            music.fade(3205);
            renderParticles= false;
        }
        else
        {
            //  set some particle propeties
            emit[0].setPosition(textPos);
            emit[1].setPosition(textPos + Vector2di(textPos.x+text.getWidth(),0));
            emit[2].setPosition(textPos + Vector2di(textPos.x+text.getWidth()*0.5f,0));
            randGen.setLimits(20,255);
            emit[0].setColour(Colour(randGen.nextInt(),randGen.nextInt(),randGen.nextInt()));
            emit[1].setColour(Colour(randGen.nextInt(),randGen.nextInt(),randGen.nextInt()));
        }
	}
}

void StateCredits::update()
{
    if(!txtUpOrDown)
		textPos.y += 2;
	else
		textPos.y -= 2;

	if(textPos.y+text.getHeight()>240)
		textPos.y = 1;
	if(textPos.y<0)
		textPos.y = 240-text.getHeight();

	text.setPosition(textPos);

	//	Move sprites?
	for(int i = NUM_AVAS-1; i >= 0; --i)
	{
		if(upOrDown)
		{
			++textColour.red;
			textColour.green+=2;
			textColour.blue+=3;
			staticAvatars[i].setY(staticAvatars[0].getY()-1);
			camSprite.setY(staticAvatars[0].getY()-1);
			//	check if we have moved to the top of the screen
			if(staticAvatars[0].getY() < 7)
				upOrDown = 0;
		}
		else
		{
			textColour.red-=2;
			textColour.green-=3;
			textColour.blue-=4;
			staticAvatars[i].setY(staticAvatars[0].getY()+1);
			camSprite.setY(staticAvatars[0].getY()+1);
			//	check if we have moved to the top of the screen
			if(staticAvatars[0].getY()+staticAvatars[0].getHeight() > 233)
				upOrDown = 1;
		}
	}
    //  Update all particles
    for(int e = emit.size()-1; e >= 0; --e)
    {
        emit[e].setPosition(Vector2df(emit[e].getPosition().x, textPos.y));
        if(emit[e].isFinished())
            emit[e].resetPlayback();
        emit[e].update();
    }
}
