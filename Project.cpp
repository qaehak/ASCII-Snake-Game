#include <iostream>
#include "MacUILib.h"
#include "objPos.h"
#include "GameMechs.h"
#include "Player.h"
#include "Food.h"

using namespace std;

#define DELAY_CONST 100000

GameMechs* Game;
Player* playerPos;
Food* foodBin;



void Initialize(void);
void GetInput(void);
void RunLogic(void);
void DrawScreen(void);
void LoopDelay(void);
void CleanUp(void);



int main(void)
{

    Initialize();

    while(Game->getExitFlagStatus() == false)  
    {
        GetInput();
        RunLogic();
        DrawScreen();
        LoopDelay();
    }

    CleanUp();

}


void Initialize(void)
{
    MacUILib_init();
    MacUILib_clearScreen();

    Game = new GameMechs(30,15); // On heap

    playerPos = new Player(Game);   // Linked directly to Game

    foodBin = new Food(playerPos, Game, 5); // food bin for 5 items
                                            // Linked directly to the player and to Game

    // Generate food
    foodBin->generateFood();

}

void GetInput(void)
{
    // Check if there is an new input before returning the input
    if(MacUILib_hasChar())
    {Game->setInput(MacUILib_getChar());}
}

void RunLogic(void)
{
    // If there is an input, then process it
    if(Game->getInput() != 0)
    {
        switch (Game->getInput())
        {
            // exit key
            case ' ': 
                Game->setExitTrue();
                break;
            
            // Movement keys
            case 'w':
            case 'W':
            case 's':
            case 'S':
            case 'd':
            case 'D':
            case 'a':
            case 'A':
                playerPos->updatePlayerDir();
                break;
            
            // Invalid key
            default:
                break;
        }
        Game->clearInput(); // reset the input once processed
    }

    
    // Moving the player based on updated movement directions
    // And performing any actions associated with collisions caused by the movement
    // like consuming food, or self-collision, i.e., increasing score, snake length, special food features, etc.
        // It thus requires the foodBin to be taken so that it knows what is going on around it as it updates movement (since Player was not directly linked to foodBin)
    playerPos->movePlayer(foodBin);
}

void DrawScreen(void)
{
    MacUILib_clearScreen();    

    // Print the gameboard
    Game->drawBoard(playerPos, foodBin); // update the board with most recent player and food data
    Game->printBoard();

    // Print a lose or exit message
    if(Game->getExitFlagStatus() && Game->getLoseFlagStatus())
    {
        MacUILib_printf("\nGame Over!\nScore: %d", Game->getScore());
    }
    else if(Game->getExitFlagStatus() && !(Game->getLoseFlagStatus()))
    {
        MacUILib_printf("\nQuitting...");
    }

}

void LoopDelay(void)
{
    MacUILib_Delay(DELAY_CONST); // 0.1s delay
}


void CleanUp(void)
{
    //MacUILib_clearScreen();    // commented out so that we can see the screen when program ends for necessary information
    
    // Delete all heap instances of objects/classes to prevent memory leakage
    delete Game;
    delete playerPos;
    delete foodBin;

    MacUILib_uninit();
}
