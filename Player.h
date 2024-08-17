#ifndef PLAYER_H
#define PLAYER_H

#include "objPos.h"
#include "objPosArrayList.h"
#include "GameMechs.h"
#include "Food.h"

// Class definitions of other classes included to allow for compilation due to the way the code was constructed (due to self-referencing property of classes)
class GameMechs;
class objPosArrayList;
class Food;

class Player
{
    // Construct the remaining declaration from the project manual.

    // Only some sample members are included here

    // You will include more data members and member functions to complete your design.

    public:
        enum Dir {UP, DOWN, LEFT, RIGHT, STOP};  // This is the direction state

        Player(GameMechs* thisGMRef);
        ~Player();

        objPosArrayList* getPlayerPos();
        void updatePlayerDir();
        void movePlayer(Food* food);
        bool checkFoodConsumption(Food* food, char &foodType);
        bool checkSelfCollision();
        
        void turnAround(); // Used for bonus feature

    private:
        // References the objPosArrayList class for a local (to class) instance
        objPosArrayList* playerPosList;       
        
        enum Dir myDir;

        // Need a reference to the Main Game Mechanisms
        GameMechs* mainGameMechsRef;
};

#endif