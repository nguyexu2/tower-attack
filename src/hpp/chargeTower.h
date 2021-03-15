#pragma once

#include "tower.h"

#include <fstream>

class CChargeTower : public CTower
{

public:
    /**
     * @brief attacks invaders within its vicinity
     * 
     * only attacks when tower is charged
     * when charged, attacks as normal tower but with different attack stat and range
     * 
     * @return true if attacked a piece, false otherwise
     * */
    virtual bool Attack() override;

    /**
     * @return range of charged tower
     * */
    virtual int GetRange() const override;

    /**
     * @return all targets that are within range of this tower
     * */
    virtual std::vector<CNode *> GetTarget() const override;

    /**
     * @return attack power of this tower
     * */
    virtual int GetAtk() const override;

    /**
     * @brief returns char describing this tower
     * 
     * @return a char - CChargeTower::displayChar
     * */
    virtual char GetPrintChar() const override;
    const static char displayChar = 'O';

    /**
     * @brief saves additional information about charge flag
     * 
     * save all normal information from CObject part + m_Charged flag
     * 
     * @param [in|out] stream where information will be saved into
     * 
     * @return ostream that came in
     * */
    virtual std::ostream &SaveInfo(std::ostream &out) const;

private:
    bool m_Charged;

    const static double AttackMultiplier;
    const static int range = 1;

public:
    /**
     * CTOR
     * attaches self to myNode
     * give self a get number (used from gateNum) and increments gatenum
     * 
     * @param [in] node place where the object should start at
     * */
    CChargeTower(CNode *myNode);

    /**
     * CTOR
     * connects to a node where the object will be stored at
     * reads sufficient amount of information in readFile to initialize all other member variables
     * 
     * @param [in] node place where the object should start at
     * @param [in] readFile stream that has information needed to initialize other variables
     * 
     * @throw CorruptedFile when reading from readFile was unsuccesful (charged state)
     * */
    CChargeTower(CNode *myNode, std::ifstream &readFile);
};