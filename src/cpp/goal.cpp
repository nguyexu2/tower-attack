#include "../hpp/goal.h"
#include "../hpp/nodes.h"

CGoal::CGoal(CNode *myNode)
    : CObject(myNode)
{
}

CGoal::CGoal(CNode *myNode, std::ifstream &readFile)
    : CObject(myNode, readFile)
{
}

//---------------------------------------------------------

char CGoal::GetPrintChar() const
{
    return displayChar;
}

//---------------------------------------------------------

void CGoal::PrintSelf(WINDOW *windowContext, int offset) const
{
    //prints indicator on map
    CObject::PrintSelf(windowContext, offset);
    
    //prints indicator on border
    mvwaddch(windowContext, offset + m_MyNode->m_Ypos, offset + m_MyNode->m_Xpos - 1, displayBorder);
    refresh();
}

//---------------------------------------------------------

bool CGoal::DoAction()
{
    //no object on the right
    if (m_MyNode->m_RightNeighbour->m_Object == nullptr)
        return false;

    //only remove moving pieces - attackers
    if( ! m_MyNode->m_RightNeighbour->m_Object->isMoving())
        return false;
    
    //let pawn in - figuratively
    delete m_MyNode->m_RightNeighbour->m_Object;
    m_MyNode->m_RightNeighbour->m_Object = nullptr;
    
    return true;
}

//---------------------------------------------------------

bool CGoal::isGoal() const
{
    return true;
}
