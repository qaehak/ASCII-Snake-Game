#include "objPosArrayList.h"

objPosArrayList::objPosArrayList()
{
    // Default constructor
    sizeList = 0; // empty list
    sizeArray = ARRAY_MAX_CAP; // default capacity
    aList = new objPos[ARRAY_MAX_CAP]; // heap data member of array containing objPos instances

    // Set all elements to be blank / invalid coordinates and blank character
    for(int index=0; index<ARRAY_MAX_CAP; index++)
    {
        // -1 is being used as an indicator of a blank object (so that its coords can't accidentally be used)
        aList[index].setObjPos(-1,-1,'\0');
    }
}

objPosArrayList::~objPosArrayList()
{
    // Destructor
    delete[] aList; // delete the heap allocation
    aList = 0; // prevent misuse of pointer; setting it to NULL does not work, but setting it to 0 does and has the same effect
}

int objPosArrayList::getSize()
{
    return sizeList;
}

void objPosArrayList::insertHead(objPos thisPos)
{
    // Safeguard: prevent inserting when at full capacity
    if(sizeList==sizeArray)
    {
        return; // do not insert anything, ending function
    }

    // Shuffle all elements down by 1
    for(int index=sizeList; index>0; index--)
    {
        aList[index] = aList[index-1]; // shuffle the previous element into the current slot
    }

    // Now insert the head at the start of the array
    aList[0] = thisPos; // using the copy constructor

    // Increment the size of the list
    ++sizeList;
}

void objPosArrayList::insertTail(objPos thisPos)
{
    // Safeguard: prevent inserting when at full capacity
    if(sizeList==sizeArray)
    {
        return; // do not insert anything
    }

    // Add tail to end of array (sizeList is the blank position after the last element currently in the array)
    aList[sizeList] = thisPos; // using the copy constructor

    // increment the size of the list so that the information is updated for next uses
    ++sizeList;
}

void objPosArrayList::removeHead()
{
    // Safeguard: prevent removal when there is nothing in the list
    if(sizeList<=0)
    {
        return; // do not do anything
    }

    // Suffle forward
    for(int index=0; index<(sizeList-1); index++)
    {
        aList[index] = aList[index+1]; // replace by following element
    }

    // reduce size to update information
    --sizeList; // no need to acutally delete last element (lazy delete)
}

void objPosArrayList::removeTail()
{
    // Safeguard: prevent removal when there is nothing in the list
    if(sizeList<=0)
    {
        return; // do not do anything
    }

    // reduce size to update information
    --sizeList; // no need to acutally delete last element (lazy delete)
}

void objPosArrayList::getHeadElement(objPos &returnPos)
{
    returnPos.setObjPos(aList[0]);      // Return by reference, acceepting an objPos item and writing the head element into the object
                                        // Writing into object, so no need to return (passed by reference, so directly modified)
}

void objPosArrayList::getTailElement(objPos &returnPos)
{
    returnPos.setObjPos(aList[sizeList-1]);     // Return by reference, acceepting an objPos item and writing the tail element into the object
                                                // Writing into object, so no need to return (passed by reference, so directly modified)
    // sizelist is the index of the next free slot
    // so sizelist-1 is the index of the tail element
}

void objPosArrayList::getElement(objPos &returnPos, int index)
{
    // Safeguard: do not grab anything for invalid index
    // sizelist is the index of the next free slot
    // so sizelist-1 is the index of the tail element
    if(index<0 || index>=sizeList) // checking for valid indexes from 0 to sizelist-1
    {
        return; // do not write anything into returnPos
    }

    // Set the returnPos to have the same parameters as the object at position index in the list
    returnPos.setObjPos(aList[index]);
    // Returns by reference; no need to return anything in the function as the original object is modified
}

void objPosArrayList::reverseList()
{
    // This function reverse the order of the list
    // It goes through each element up to the half-point mark, making swaps from one end and the other approaching inwards
    
    objPos tempPos;
    for(int i = 0; i < sizeList/2; i++)
    {
        // We want to swap as follows:
        // 0 1 2 3 4 5
        // 5 4 3 2 1 0
        // We will thus use the standard swapping procedure for indexes i and (sizeList-1)-i
        tempPos = aList[i];
        aList[i] = aList[(sizeList-1)-i];
        aList[(sizeList-1)-i] = tempPos;
    }
}