#pragma once

#include "object.h"

//------------------------------------

#include <list>
#include <functional>

//------------------------------------

class CBasicPawn : public CObject
{
public:
    /**
     * @brief moves this pawn
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
     * @brief moves this pawn to newPlace
     * 
     * @param [in] newPlace node that this object should connect move to
     * 
     * @return true if succesfully moved, false otherwise
     * */
    bool MoveTo(CNode *newPlace);

    /**
     * @brief indicator that this object can move 
     * 
     * @returns true
     * */
    virtual bool isMoving() const override;

    /**
     * @brief returns char describing this pawn
     * 
     * @return a char - CBasicPawn::displayChar
     * */
    virtual char GetPrintChar() const override;

    const static char displayChar = '@';

protected:
    /**
     * @brief uses BFS algorithm to find route towards goal
     * 
     * this BFS variant doesnt allow going through walls and other non moving objects
     * first element in returned list is current node the object is on
     * last element is node of goal post
     * 
     * changes visiting state of nodes and sets m_Parents of nodes that were visited during the duration of searching
     * 
     * @return list of nodes determining route that this object should take
     * */
    std::list<CNode *> BFS();

    /**
     * @brief uses BFS algorithm to find route towards goal
     * 
     * takes comparator to determine whether this object and go to through each node
     * 
     * first element in returned list is current node the object is on
     * last element is node of goal post
     * 
     * changes visiting state of nodes and sets m_Parents of nodes that were visited during the duration of searching
     * 
     * @param [in] comparator function that checks whether said node can be gone on
     * 
     * @return list of nodes determining route that this object should take
     * */
    std::list<CNode *> BFS(std::function<bool(const CNode *)> comparator);

    /**
     * @brief sets route that this object should take
     * 
     * copies route
     * sets generated flag to true
     * 
     * @param [in] route list of nodes that should be visited
     * */
    void setRoute(const std::list<CNode *> & route);

    bool m_MoveGenerated; ///<generated flag
    std::list<CNode *> m_MoveList; ///<route that should be taken

    /**
     * @brief initializes HP value
     * 
     * HP value depends on baseHP
     * */
    virtual void InitHP() override;

private:
    const static int baseHP = 700;

public:
    /**
     * CTOR
     * 
     * sets generated to false
     * sets HP through InitHP method
     * connects this object to the node 
     * 
     * @param [in] myNode node that this object should connect to
     * */
    CBasicPawn(CNode *myNode);

    /**
     * CTOR
     * connects to a node where the object will be stored at
     * reads sufficient amount of information in readFile to initialize all other member variables
     * 
     * @param [in] node place where the object should start at
     * @param [in] readFile stream that has information needed to initialize other variables
     * 
     * */
    CBasicPawn(CNode *myNode, std::ifstream &readFile);
};