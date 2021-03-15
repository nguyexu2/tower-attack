#pragma once

#include "object.h"

//-------------------------------------------------------------

#include <vector>
#include <fstream>

//-------------------------------------------------------------

class CTower : public CObject
{
public:
    /**
     * checks all nodes within range and attack nodes that hold a pawn
     * 
     * calls Attack();
     * 
     * @return true if attacked a piece, false if didnt attack any pieces
     * */
    virtual bool DoAction();

    /**
     * @brief attack a piece within range
     * 
     * create vector of possible targets if it hasnt been generated yet
     * gets all targerts that should be attacked
     * itterates through the list and attack every pawn
     * 
     * deletes the pawn if it has <= 0 HP
     * 
     * @return true if attacked a piece, false if didnt attack any pieces
     * */
    virtual bool Attack();

    /**
     * @brief returns char describing this tower
     * 
     * @return a char - CTower::displayChar
     * */
    virtual char GetPrintChar() const override;
    const static char displayChar = 'I';

protected:
    /**
     * initializes HP to custum value
     * HP value depends on baseHP
     * */
    virtual void InitHP();

    /**
     * creates vector of targets that should be checked everytime this->Attack() method is called
     * 
     * check all nodes that are within range (radius with m_MyNode in middle)
     * fill up m_PotentialTargets with nodes that dont have walls or towers
     * */
    virtual void CreateTargets();

    /**
     * returns the range of tower with this node in the center
     * 
     * @return int representing range
     * */
    virtual int GetRange() const;

    /**
     * @return attack stat of tower
     * */
    virtual int GetAtk() const;

    /**
     * method of getting nodes that should be attacked per turn
     * 
     * returns 1 target, the one that is 1st in m_PotentialTargets
     * 
     * @return vector of targets that should be attacked
     * */
    virtual std::vector<CNode *> GetTarget() const;

    bool m_Generated; ///<flag that indicates whether m_PotentialTargets has been initialized

    std::vector<CNode *> m_PotentialTargets; ///<all targets that can be attacked

private:
    const static int range = 2;
    const static int baseHP = 10000;
    const static int baseAttack = 50;

public:
    /**
     * CTOR
     * node where the object will be stored at
     * 
     * @param [in] node place where the object should start at
     * */
    CTower(CNode *myNode);

    /**
     * CTOR
     * connects to a node where the object will be stored at
     * reads sufficient amount of information in readFile to initialize all other member variables
     * 
     * @param [in] node place where the object should start at
     * @param [in] readFile stream that has information needed to initialize other variables
     * */
    CTower(CNode *myNode, std::ifstream &readFile);
};