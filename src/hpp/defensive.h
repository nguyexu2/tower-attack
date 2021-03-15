#pragma once

#include "basic.h"

//----------------------------------------------------------

class CDefensive : public CBasicPawn
{
public:
    /**
     * @brief moves this pawn once every 2 turns
     * 
     * moves as CBasicPawn when m_Charged
     * else stands and charges
     * 
     * creates a route plan if plan hasnt been generated
     *  
     * if planned next place is occupied, generates a new route and executes new plan
     * move to new position according to plan
     * 
     * @return true if moved succesfully, false if not moved this turn
     * */
    virtual bool DoAction() override;

    /**
     * @brief returns char describing this pawn
     * 
     * @return a char - CDefensive::displayChar
     * */
    virtual char GetPrintChar() const override;
    const static char displayChar = '%';

private:
    /**
     * @brief initializes HP value
     * 
     * HP value depends on CBasicPawn::baseHP * HP_multiplier
     * */
    virtual void InitHP() override;
    const static double HP_multiplier;

    bool m_Charged;

public:
    /**
     * CTOR
     * 
     * sets m_Charged to false - has to wait a turn after creation to move
     * sets HP through InitHP method
     * 
     * @param [in] myNode node that this object should connect to
     * */
    CDefensive(CNode *myNode);

    /**
     * CTOR
     * connects to a node where the object will be stored at
     * reads sufficient amount of information in readFile to initialize all other member variables
     * 
     * @param [in] node place where the object should start at
     * @param [in] readFile stream that has information needed to initialize other variables
     * 
     * @throw CorruptedFile if reading for m_Charged was unsuccesful
     * */
    CDefensive(CNode *myNode, std::ifstream &readFile);
    
    /**
     * calls CBasicPawn::SaveInfo
     * saves m_Charged state afterwards
     * 
     * @param [in] out stream that the information about this object should be stored in
     * */
    virtual std::ostream &SaveInfo(std::ostream &out) const override;
};