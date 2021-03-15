#pragma once

#include "object.h"

#include <fstream>

//-----------------------------------------------------------

class CWall : public CObject
{
public:
    /**
     * @brief do action of wall
     * 
     * walls dont do anything
     * 
     * @return false
     * */
    virtual bool DoAction() override;

    /**
     * @brief returns char describing walls
     * 
     * @return a char - CWall::displayChar
     * */
    virtual char GetPrintChar() const override;
    const static char displayChar = '#';

private:
    /**
     * no private variables or methods
     * */
public:
    /**
     * CTOR
     * needs a node where the object will be stored at
     * 
     * @param [in] node place where the object should start at
     * */
    CWall(CNode *myNode = nullptr);


    /**
     * CTOR
     * connects to a node where the object will be stored at
     * 
     * @param [in] node place where the object should start at
     * @param [in] readFile stream that has information needed to initialize other variables (mainly CObject)
     * */
    CWall(CNode *myNode, std::ifstream &readFile);
};