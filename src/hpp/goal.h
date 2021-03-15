#pragma once

#include "object.h"

//-------------------------------------------------------

class CGoal : public CObject
{
public:
    /**
     * @brief prints current object on window
     * 
     * prints self on map and a charater on the wall (offset - 1)
     * 
     * @param [in] winContext window where the object should be showed on
     * @param [in] offset offset from top and left side 
     * */
    virtual void PrintSelf(WINDOW *windowContext, const int offset) const override;

    /**
     * @brief returns char describing this pawn
     * 
     * @return CGoal::displayChar
     * */
    virtual char GetPrintChar() const override;
    const static char displayChar = '=';

    /**
     * @brief removes a pawn if one exists on the right of this goal
     * 
     * @return true if succesfully removed pawn, false otherwise
     * */
    virtual bool DoAction() override;

    /**
     * @return true indicating that this is a goal
     * */
    virtual bool isGoal() const override;

private:
    const static char displayBorder = '<';

public:
    /**
     * CTOR
     * 
     * connects this object to myNode
     * 
     * @param [in] myNode node that this object should connect to
     * */
    CGoal(CNode *myNode);

    /**
     * CTOR
     * connects to a node where the object will be stored at
     * 
     * @param [in] node place where the object should start at
     * @param [in] readFile stream that has information needed to initialize other variables
     * */
    CGoal(CNode *myNode, std::ifstream & readFile);
};