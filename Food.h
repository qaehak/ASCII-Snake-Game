#ifndef FOOD_H
#define FOOD_H

#include "objPos.h"
#include "objPosArrayList.h"
#include "GameMechs.h"
#include "Player.h"

// Class definitions of other classes included to allow for compilation due to the way the code was constructed (due to self-referencing property of classes)
class GameMechs;
class objPosArrayList;
class Player;

class Food
{
    public:
        // Constructors & Destructor
        Food();
        ~Food();
        Food(Player* player, GameMechs* GM, int num);

        // Operation functions
        void generateFood();

        // Getters
        int getAmountToGenerate();
        objPosArrayList* getFoodPos();


    private:
        int amountToGenerate;
        objPosArrayList* foodBucket;

        // Pointer linking the object to other classes
        Player* myPlayer;
        GameMechs* myGM;

};

#endif