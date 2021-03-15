#pragma once

/**
 * forward declaration to prevent recursive definition
 * */
class CObject;

struct CNode
{
    /**
     * 3 states for BFS searching
     * */
    enum STATES
    {
        FRESH,  //< node hasnt been visited or seen
        OPENED, //< node has been seen but hasnt been proccessed
        CLOSED  //< node has been seen, proccessed and musnt be visited again
    };

//---------------------------------------//
    STATES m_State;
    CNode * m_Parent;

    int m_Xpos; 
    int m_Ypos;

    CObject * m_Object;
    bool m_IsGoal; ///<detection whether this node holds goal or not
//---------------------------------------//
    /**
     * All neighbours
     * */
    CNode * m_UpNeighbour;
    CNode * m_DownNeighbour;
    CNode * m_LeftNeighbour;
    CNode * m_RightNeighbour;
//---------------------------------------//
    CNode(int xPos = 0, int yPos = 0);
    CNode(CObject * object);

    /**
     * DTOR
     * 
     * deletes object that this node holds
     * */
    ~CNode();
};