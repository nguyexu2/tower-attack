#pragma once

#include "nodes.h"

//-----------------------------------------------

#include <curses.h>
#include <list>
#include <fstream>

//-----------------------------------------------

const char saveDelim = ' ';

//-----------------------------------------------

class CObject
{
public:
    /**
     * @brief prints current object on window
     * 
     * gates print differently than walls, towers or pawns
     * 
     * @param [in] winContext window where the object should be showed on
     * @param [in] offset offset from top and left side 
     * */
    virtual void PrintSelf(WINDOW *winContext, int offset) const;

    /**
     * @brief object does an action upon calling this method
     * 
     * abstract method for executing action of derived object
     * 
     * @return true if an action was done, false if nothing happened
     * */
    virtual bool DoAction() = 0;

    /**
     * @brief changes HP value
     * 
     * calculates how much damage the attack deals and changes HP value
     * 
     * @param [in] attackStat attack power of attack
     * */
    void TakeDamage(int attackStat);
    //----------------------------------------------------------//

    /**
     * @brief returns current HP value
     * 
     * @return integer value of current HP value, can be negative
     * */
    int getHP() const;

    /**
     * @brief returns current Y and X position of this object on the map 
     * 
     * @return pair of integers where the first value is y-axis position and second value is x-axis position
     * */
    std::pair<int, int> GetPos() const;

    /**
     * @brief returns char indentifying the object
     * 
     * @return a char, each object has a char indentifying the object
     * */
    virtual char GetPrintChar() const = 0;
    //----------------------------------------------------------//

    /**
     * @brief identifies if an object can move
     * 
     * @return true if object can move(pawns), false otherwise(static objects)
     * */
    virtual bool isMoving() const;

    /**
     * @brief returns whether object is a goalPost
     * 
     * @return true if instance of CGoal, false otherwise
     * */
    virtual bool isGoal() const;
    //----------------------------------------------------------//

    /**
     * returns node at given param position
     * 
     * @param [in] xPos x position of node
     * @param [in] ypos y position of node
     * 
     * @return node if param values are on map, nullptr if out of bound
     * */
    CNode * FindNode(int xPos, int yPos) const;
protected:
    /**
     * upon calling this method, the object saves all information about self into stream
     * 
     * @param [in] out stream that the information about this object should be stored in
     * */
    virtual std::ostream& SaveInfo(std::ostream& out) const;

    /**
     * @brief Initializes HP value
     * 
     * each object has an amount of HP, default value is INT_MAX
     * */
    virtual void InitHP();

    CNode *m_MyNode;
    int m_HP;

public:
    /**
     * CTOR of CObject
     * needs a node where the object will be stored at
     * calls InitHP
     * 
     * @param [in] node place where the object should start at
     * 
     * @throw CreationObjectError if object overwrites existing object
     * */
    CObject(CNode *node);

    /**
     * CTOR
     * connects to a node where the object will be stored at
     * reads sufficient amount of information in readFile to initialize all other member variables
     * 
     * @param [in] node place where the object should start at
     * @param [in] readFile stream that has information needed to initialize other variables
     * 
     * @throw CorruptedFile when reading from readFile was unsuccesful (HP initialization)
     * @throw CreationObjectError if object overwrites existing object
     * */
    CObject(CNode *node, std::ifstream & readFile);
    virtual ~CObject() = default;
    
    CObject() = default;
    
    /**
     * calls SaveInfo method
     * */
    friend std::ostream &operator<<(std::ostream &out, const CObject &obj);
};
