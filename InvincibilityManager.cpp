#include "InvincibilityManager.h"

InvincibilityManager::InvincibilityManager()
{
    InvincibilityLimit = 2000;
}

InvincibilityManager::InvincibilityManager(CRuint numEnemies)
{
    resize(numEnemies);
}

InvincibilityManager::~InvincibilityManager()
{
    invincibleTimer.clear();
    isInvincible.clear();
}

void InvincibilityManager::resize(CRuint enemies)
{
    invincibleTimer.resize(enemies);
    isInvincible.resize(enemies);
}

int InvincibilityManager::size(){return invincibleTimer.size();}

bool InvincibilityManager::getIsInvincible(CRuint enemy){return isInvincible[enemy];}

void InvincibilityManager::setIsInvincible(CRuint enemy){setIsInvincible(enemy,true);}

void InvincibilityManager::setIsInvincible(CRuint enemy, CRbool status)
{
    isInvincible[enemy] = status;

    if(status)
    {
        invincibleTimer[enemy].setMode(MILLI_SECONDS);
        invincibleTimer[enemy].start();
    }
}

void InvincibilityManager::setInvincibilityLimit(CRuint limit){InvincibilityLimit = limit;}

void InvincibilityManager::update()
{
    for(int i = size() -1; i >=0; i--)
    {
        if(isInvincible[i])
        {
            if((uint)invincibleTimer[i].getScaledTicks() >= InvincibilityLimit)
            {
                setIsInvincible(i,false);
                invincibleTimer[i].start();
            }
        }
    }
}


