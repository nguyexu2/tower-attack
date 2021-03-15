#pragma once

#include "basic.h"

//---------------------------------------------------------------------

class CJoker : public CBasicPawn
{
public:
    /**
     * @brief moves this pawn
     * 
     * checks how many moves away is this pawn from goal
     *      if the amount of steps is less than possibleMovesNormal
     *          moves like a pawn
     *      else
     *          moves randomly
     * 
     * makes random amount of moves into random directions
     * if the new square is occupied, continues to generate next positions until a free square is found
     * 
     * @return true if moved succesfully, false if not moved this turn
     * */
    virtual bool DoAction() override;

    /**
     * @brief returns char describing this pawn
     * 
     * @return a char - CJoker::displayChar
     * */
    virtual char GetPrintChar() const override;
    const static char displayChar = '&';

private:
    /**
     * @brief moves to randomly generated place
     * 
     * generates direction each time and moves into that direction
     * makes random amount of moves but not more than possibleMovesRandom
     * turns that move out of bound of map are ignored
     * 
     * @return true
     * */
    bool GoToRandomPlace();

    /**
     * @brief Initializes HP value
     * 
     * value depends on CBasicPawn::baseHP and HP_multiplier
     * */
    virtual void InitHP() override;
    const static double HP_multiplier;

    const static int possibleMovesNormal = 10;
    const static int possibleMovesRandom = possibleMovesNormal * 3;

public:
    /**
     * CTOR of CObject
     * needs a node where the object will be stored at
     * calls InitHP
     * 
     * @param [in] node place where the object should start at
     * */
    CJoker(CNode *myNode);

    /**
     * CTOR
     * connects to a node where the object will be stored at
     * reads sufficient amount of information in readFile to initialize all other member variables
     * 
     * @param [in] node place where the object should start at
     * @param [in] readFile stream that has information needed to initialize other variables
     * */
    CJoker(CNode *myNode, std::ifstream & readFile);
};