#ifndef INVINCIBILITYMANAGER_H_INCLUDED
#define INVINCIBILITYMANAGER_H_INCLUDED

#include "PenjinTypes.h"
#include "Timer.h"
#include <vector>
using namespace std;

/*
Class manages which enemies have temporary invincibility
and stores timers for each enemy.
*/

class InvincibilityManager
{
    public:
        InvincibilityManager();
        InvincibilityManager(CRuint numberEnemies);
        ~InvincibilityManager();

        void resize(CRuint numEnemies);               //  Set number of enemies
        int size();                                 //  Get the number of enemies this is handling.

        bool getIsInvincible(CRuint enemy);           //  Get an enemies invincibility status
        void setIsInvincible(CRuint enemy);           //  Sets an enemy to be invincible.
        void setIsInvincible(CRuint enemy, CRbool isInvincible);    //  Sets invincibility status of enemy

        void setInvincibilityLimit(CRuint limit);     //  Sets the limit that an enemy is invincible for.

        void update();                              //  Update timers.

    private:
        vector <bool> isInvincible;
        vector <Timer> invincibleTimer;
        uint InvincibilityLimit;                       //  the limit that an enemy is invincible for
};

#endif // INVINCIBILITYMANAGER_H_INCLUDED
