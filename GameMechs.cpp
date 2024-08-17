#include "GameMechs.h"
#include "MacUILib.h" // added so that we can do print operations in here already
#include <stdlib.h> // for random
#include <time.h> // for seeding

GameMechs::GameMechs()
{
    input = 0; // NULL

    exitFlag = false; // If true, triggers game close/exit
    loseFlag = false; // If true, triggers game loss

    boardSizeX = 30; // default board size for X and Y
    boardSizeY = 15;

    score = 1; // Initializes to 1, since snake starts with length 1

    // generate a seed for the game
    srand(time(NULL));

    // Creating a 2D char array to contain the board to print
    // (to simplify printing)
    // Only gets updated when the drawBoard function is called; at this point it will just contain the blank board and border pieces
    // Allocates the row lookup table (the Y coords)
    board = new char*[boardSizeY]; // row lookup table

    // Allocates the columns (the X coords) using data from row lookup table just allocated
    for(int index=0; index<boardSizeY; index++) 
    {
        board[index] = new char[boardSizeX+1]; // need 1 additional x value for '\0' at the end of each row of chars
                                               // This is because we will be using MacUILib_printf to print each row as a string
    }    

    // Draw default board; go through each index and print characters based on coordinates
    for(int yCoord=0; yCoord<boardSizeY; yCoord++)
    {
        for(int xCoord=0; xCoord<(boardSizeX+1); xCoord++)
        {
            // Border characters
            // left side and right borders
            if(xCoord==0 || xCoord==(boardSizeX-1)) 
            {
                board[yCoord][xCoord] = '#';
            }
            // top and bottom borders
            else if((yCoord==0 && xCoord>0 && xCoord<(boardSizeX-1)) || (yCoord==(boardSizeY-1) && xCoord>0 && xCoord<(boardSizeX-1)))
            {
                board[yCoord][xCoord] = '#';
            }

            // Null character (for end of row)
            else if(xCoord==boardSizeX)
            {
                board[yCoord][xCoord] = '\0';
            }

            // empty characters
            else
            {
                board[yCoord][xCoord] = ' ';
            }
        }
    }
}


GameMechs::GameMechs(int boardX, int boardY)
{
    input = 0; // NULL

    exitFlag = false;
    loseFlag = false;

    // Only use the given board sizes when they are valid sizes!
    // Set a minimum board size of 10x5
    // Otherwise use the default ones
    if(boardX>=10){boardSizeX = boardX;}
    else{boardSizeX = 30;}
    
    if(boardY>=5){boardSizeY = boardY;}
    else{boardSizeY = 15;}

    score = 1; // Initializes to 1, since snake starts with length 1

    
    // generate a seed for the game
    srand(time(NULL));

    // Creating a 2D char array to contain the board to print
    // (to simplify printing)
    // BUT not the up-to-date positions of objects on the board
    board = new char*[boardSizeY]; // row lookup table

    for(int index=0; index<boardSizeY; index++) 
    {
        board[index] = new char[boardSizeX+1]; // need 1 additional x value for '\0'
    }    

    // Draw default board
    // Same logic as in the default constructor (refer to there for comments explaining logic)
    for(int yCoord=0; yCoord<boardSizeY; yCoord++)
    {
        for(int xCoord=0; xCoord<(boardSizeX+1); xCoord++)
        {
            // Boarder characters
            // left side and right sides
            if(xCoord==0 || xCoord==(boardSizeX-1)) 
            {
                board[yCoord][xCoord] = '#';
            }
            // top and bottom
            else if((yCoord==0 && xCoord>0 && xCoord<(boardSizeX-1)) || (yCoord==(boardSizeY-1) && xCoord>0 && xCoord<(boardSizeX-1)))
            {
                board[yCoord][xCoord] = '#';
            }

            // Null character
            else if(xCoord==boardSizeX)
            {
                board[yCoord][xCoord] = '\0';
            }

            // empty characters
            else
            {
                board[yCoord][xCoord] = ' ';
            }
        }
    }
}

GameMechs::~GameMechs()
{
    // Delete each row
    for(int yCoord=0; yCoord<boardSizeY; yCoord++)
    {
        delete[] board[yCoord];
    }

    // Delete row-lookup table
    delete[] board;

    // Prevent mis-use of pointer
    board = NULL;
}


bool GameMechs::getExitFlagStatus()
{
    return exitFlag;
}

bool GameMechs::getLoseFlagStatus()
{
    return loseFlag;
}

char GameMechs::getInput()
{
    return input;
}

int GameMechs::getBoardSizeX()
{
    return boardSizeX;
}

int GameMechs::getBoardSizeY()
{
    return boardSizeY;
}

int GameMechs::getScore()
{
    return score;
}

void GameMechs::setExitTrue()
{
    exitFlag = true;
}

void GameMechs::setLoseFlag()
{
    loseFlag = true;
}

void GameMechs::setInput(char this_input)
{
    input = this_input;
}

void GameMechs::clearInput()
{
    input = 0;
}

void GameMechs::incrementScore()
{
    ++score;
}

void GameMechs::printBoard()
{

    for(int yCoord=0; yCoord<boardSizeY; yCoord++)
    {
        // Each row of the gameboard
        MacUILib_printf("%s", board[yCoord]);   // Prints the data saved for the entire row

        // Print the controls for good UI design
        if(yCoord==0){MacUILib_printf("\tControls:\tw-UP, a-LEFT, s-DOWN, d-RIGHT, spacebar-QUIT");}
        else if(yCoord==1){MacUILib_printf("\tScore:\t\t%d",score);}
        else if(yCoord==3){MacUILib_printf("\t\t o\t\t  B");}
        else if(yCoord==4){MacUILib_printf("\t     +1 Score     Bounce, +2 Score");}


        MacUILib_printf("\n");
    }
}

void GameMechs::drawBoard(Player *player, Food *food)
{
    // Values to be used in this function locally
    char symbol;
    objPos tempPos;
    objPosArrayList* foodPosList;
    objPosArrayList* playerPosList;

    // Draw default board (empty)
    for(int yCoord=0; yCoord<boardSizeY; yCoord++)
    {
        for(int xCoord=0; xCoord<(boardSizeX+1); xCoord++)
        {
            tempPos.setObjPos(xCoord, yCoord, '\0');
            
            // Border characters
            // left side and right sides
            if(xCoord==0 || xCoord==(boardSizeX-1)) 
            {
                board[yCoord][xCoord] = '#';
            }
            // top and bottom
            else if((yCoord==0 && xCoord>0 && xCoord<(boardSizeX-1)) || (yCoord==(boardSizeY-1) && xCoord>0 && xCoord<(boardSizeX-1)))
            {
                board[yCoord][xCoord] = '#';
            }

            // Null character
            else if(xCoord==boardSizeX)
            {
                board[yCoord][xCoord] = '\0';
            }

            // empty characters
            else
            {
                board[yCoord][xCoord] = ' ';
            }
        }
    }

    // Populate the board for Player and Food
    
    // Player
    // extract the player position list
    playerPosList = player->getPlayerPos();

    // Loop through the array and add in all the elements
    for(int index=0; index<(playerPosList->getSize()); index++)
    {
        playerPosList->getElement(tempPos,index); // saving the object at the index to a temporary objPos instance

        board[tempPos.y][tempPos.x] = tempPos.symbol;   // Insert object symbol into board
    }

    // Food
    // extracting the list of food
    foodPosList = food->getFoodPos();

    // Loop through the array and add in all the elements
    for(int index=0; index<(foodPosList->getSize()); index++)
    {
        foodPosList->getElement(tempPos,index); // saving the object at the index to a temporary objPos instance

        board[tempPos.y][tempPos.x] = tempPos.symbol;   // Insert object symbol into board
    }
}