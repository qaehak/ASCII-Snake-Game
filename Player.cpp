#include "Player.h"


Player::Player(GameMechs* thisGMRef)
{
    mainGameMechsRef = thisGMRef;
    myDir = STOP;

    // Create the array list for the player
    playerPosList = new objPosArrayList();
    
    // Create a temporary objPos to pass into the insertHead function
    objPos snakeHead;

    // At the temporary objPos, set the coordinates to be the middle of the board and the symbol to be '*'
    snakeHead.setObjPos((mainGameMechsRef->getBoardSizeX()-1) / 2, (mainGameMechsRef->getBoardSizeY()-1) / 2, '*');

    // Insert the snake head
    playerPosList->insertHead(snakeHead);
}


Player::~Player()
{
    // delete any heap members here
    delete playerPosList;

    // Prevent misuse of pointer
    playerPosList = NULL;
}

objPosArrayList* Player::getPlayerPos()
{
    // return the reference to the playerPos arrray list
    return playerPosList;
}

void Player::updatePlayerDir()
{
    // PPA3 input processing logic   

    char input = mainGameMechsRef->getInput();

    // Based on current directions, defines allowed inputs, and what those inputs will do (which directions they will change)
    switch(myDir)
    {
        case STOP:
            if(input == 'W' || input == 'w'){
                myDir = UP;
            }
            else if(input == 'S' || input == 's'){
                myDir = DOWN;
            }
            else if(input == 'A' || input == 'a'){
                myDir = LEFT;
            }
            else if(input == 'D' || input == 'd'){
                myDir = RIGHT;
            }
            break;
        
        case LEFT:
            if(input == 'W' || input == 'w'){
                myDir = UP;
            }
            else if(input == 'S' || input == 's'){
                myDir = DOWN;
            }
            break;
        
        case RIGHT:
            if(input == 'W' || input == 'w'){
                myDir = UP;
            }
            else if(input == 'S' || input == 's'){
                myDir = DOWN;
            }
            break;
        
        case UP:
            if(input == 'A' || input == 'a'){
                myDir = LEFT;
            }
            else if(input == 'D' || input == 'd'){
                myDir = RIGHT;
            }
            break;
        
        case DOWN:
            if(input == 'A' || input == 'a'){
                myDir = LEFT;
            }
            else if(input == 'd' || input == 'D'){
                myDir = RIGHT;
            }
            break;
        
        default:
            break;
    }

    mainGameMechsRef->clearInput();
}

bool Player::checkFoodConsumption(Food* food, char &foodType)
{
    // Write the position of the snake head into a temporary playerPos object (type objPos)
    objPos playerPos;
    playerPosList->getHeadElement(playerPos);

    // Get the array of food objects
    objPosArrayList* foodBin;
    foodBin = food->getFoodPos();
    
    // Temp object for each food
    objPos foodItem;

    // Check for collision to all food items in bin
    for(int index=0; index<(foodBin->getSize()); index++)
    {
        // Extract the food item
        foodBin->getElement(foodItem, index);

        // Check if it overlaps with the head
        if(foodItem.isPosEqual(&playerPos))
        {
            foodType = foodItem.symbol;
            return true; // if it does overlap, return true (exits function)
        }
    }

    // if none of the body pieces overlapped, return false
    return false;
}

bool Player::checkSelfCollision()
{
    // If the snake less than 5 tiles long there is no need to check for self collision as it will be impossible
    if((playerPosList->getSize())<5)
    {
        return false; // no collision is possible if the snake is 4 tiles or less
        // doing this for efficiency
    }

    // Write the position of the snake head into a temporary playerPos object (type objPos)
    objPos playerPos;
    playerPosList->getHeadElement(playerPos);

    // Temporary object for each player body
    objPos playerBody;

    // Loop through all the tiles of the snake body AFTER the head
    // Note: the 5th element (index 4) is the first element that can have a collision with the head
    // Once again, not using index 1 for efficiency sake
    for(int index=4; index<(playerPosList->getSize()); index++)
    {
        // Extract the player body
        playerPosList->getElement(playerBody, index);

        // Check if it overlaps with the head
        if(playerPos.isPosEqual(&playerBody))
        {
            return true; // if it does overlap, return true (exits function)
        }
    }

    // if none of the body pieces overlapped, return false
    return false;
}

void Player::movePlayer(Food* food)
{
    // Write the position of the snake head into a temporary playerPos object (type objPos)
    objPos playerPos;
    playerPosList->getHeadElement(playerPos);
    
    char foodType;  // define local char for changing code based on type of food consumed
    
    // PPA3 Finite State Machine logic
    // Calculating the next position of the snake
    // Incrementing in current direction
    switch(myDir)
    {
        case STOP:
            break;
        case LEFT:
            playerPos.x--;
            break;
        case RIGHT:
            playerPos.x++;
            break;
        case UP:
            playerPos.y--;
            break;
        case DOWN:
            playerPos.y++;
            break;
        default:
            break;
    }

    // Wrap-around
    if( playerPos.y == (mainGameMechsRef->getBoardSizeY()-1)  || playerPos.y == 0 ){
        if( playerPos.y == (mainGameMechsRef->getBoardSizeY()-1) ){
            playerPos.y = 1;
        }else{
            playerPos.y = (mainGameMechsRef->getBoardSizeY()-2);
        }
    }if( playerPos.x == (mainGameMechsRef->getBoardSizeX()-1) || playerPos.x == 0 ){
        if( playerPos.x == (mainGameMechsRef->getBoardSizeX()-1) ){
            playerPos.x = 1;
        }else{
            playerPos.x = (mainGameMechsRef->getBoardSizeX()-2);
        }
    }

    // Add the head at the new location
    playerPosList->insertHead(playerPos);

    // For the tail: need to know if there was food that was consumed
    // Check food collision
    // If yes, dont remove tail to essentially "add" a tail
    // Also, generate a new food and increment the score
    if(!checkFoodConsumption(food, foodType)) {playerPosList->removeTail();}
    else
    {
        // Add bonus condition if the food collected is the bonus food
        if(foodType == 'B')
        {
            turnAround();
            mainGameMechsRef->incrementScore(); // This thus increments the score by 2 when B is picked up (score is incremented again after this if statement)
        }
        
        // Tail "added" by not removing tail during movement
        // And score will be incremented (tracking score seperately from length for above and beyond features)
        mainGameMechsRef->incrementScore();

        // Now, generate new food!
        food->generateFood(); // generate new food items
        // initially had 'this' as a parameter
    }

    // Endgame condition: Snake suicide
    // Note: both the food and the self collision cant happen at the same time, so this is all OK
    if(checkSelfCollision()) 
    {
        // Set exit and lose flags to true to quit the game
        mainGameMechsRef->setLoseFlag();
        mainGameMechsRef->setExitTrue();
        
        return;
    }

}

void Player::turnAround()
{
    objPos tempPos;
    
    // x and y coordinates of tail and piece-before-tail, where x2 and y2 are the tail's coordinates
    playerPosList->getElement(tempPos, playerPosList->getSize()-2); // Extract data of second-to-last element into tempPos, assign to x and y
    int x1 = tempPos.x;  
    int y1 = tempPos.y;

    playerPosList->getTailElement(tempPos); // Extract data of tail into tempPos, assign to x and y
    int x2 = tempPos.x;
    int y2 = tempPos.y; 
    
    if(playerPosList->getSize() <= 1)
    {
        // If the snake is only one in length, reverse direction
        // Given current direction, turn around
        switch(myDir)
        {
            case LEFT:
                myDir = RIGHT;
                break;
            case RIGHT:
                myDir = LEFT;
                break;
            case UP:
                myDir = DOWN;
                break;
            case DOWN:
                myDir = UP;
                break;
            default:
                break;
        }
    }
    
    else
    {
        if(y1 > y2) {myDir = UP;}           // If next direction of tail is down, make it go up
        else if (y2 > y1) {myDir = DOWN;}   // If next direction of tail is up, make it go down
        else if (x1 > x2) {myDir = LEFT;}   // If next direction of tail is right, make it go left
        else if (x2 > x1) {myDir = RIGHT;}  // If next direction of tail is left, make it go right
        playerPosList->reverseList();
    }
    
}

