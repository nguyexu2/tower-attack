#pragma once

#include "object.h"
#include "all_moving.h"

//--------------------------------------------------

class CGate : public CObject
{
public:
    /**
     * @brief resets gate numbering to 1
     * 
     * sets getNum = 1
     * should be called once, and that when the game starts to give gate a fresh start
     * */
    void static ResetGates();

    /**
     * returns gate number of this instance
     * 
     * @return an int representing gate number
     * */
    int GetGateNum() const;

    /**
     * prints self normally on map + prints an extra character on the border
     * also prints gate number behind (on the right) of the gate
     * 
     * @param [in] winContext window where the object should be showed on
     * @param [in] offset offset from top and left side 
     * */
    virtual void PrintSelf(WINDOW *windowContext, const int offset) const override;

    /**
     * @brief returns char indentifying the object
     * 
     * @return a char to be printed on map
     * */
    virtual char GetPrintChar() const override;
    const static char displayChar = '<';

    /**
     * @brief summons a basic pawn
     * 
     * summons default attacker - CBasicPawn
     * if the player wants to choose another piece, use Summon(ATTACKERTYPE type) method
     * 
     * @return true if attacker was succesfully created
     * */
    virtual bool DoAction() override;

    /**
     * @brief tries to summon an attacker of type type
     * 
     * an attacker is succesfully created if theres no object on the right of the gate
     * 
     * @return true if attacker was succesfully created
     * */
    bool Summon(ATTACKERTYPE type);

private:
    /**
     * saves basic information about the gate and gate number
     * 
     * @param [in] out stream that the information about this object should be stored in
     * */
    virtual std::ostream &SaveInfo(std::ostream &out) const override;

    const static char displayBorder = '=';

    static int gateNum;
    int m_GateNum;

public:
    /**
     * CTOR
     * attaches self to myNode
     * give self a get number (used from gateNum) and increments gatenum
     * 
     * @param [in] node place where the object should start at
     * */
    CGate(CNode *myNode);

    /**
     * CTOR
     * connects to a node where the object will be stored at
     * reads sufficient amount of information in readFile to initialize all other member variables
     * 
     * if sets gateNum to maximum of the current read gate
     * 
     * @param [in] node place where the object should start at
     * @param [in] readFile stream that has information needed to initialize other variables
     * 
     * @throw CorruptedFile when reading from readFile was unsuccesful (gate number)
     * */
    CGate(CNode *myNode, std::ifstream &readFile);
};
