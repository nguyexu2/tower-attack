#include "../hpp/basic.h"
#include "../hpp/object.h"
#include "../hpp/nodes.h"
#include "../hpp/exceptions.h"

#include <queue>
#include <list>
#include <vector>

#include <functional>

CBasicPawn::CBasicPawn(CNode *myNode)
    : CObject(myNode),
      m_MoveGenerated(false)
{
    this->InitHP();
}

CBasicPawn::CBasicPawn(CNode *myNode, std::ifstream &readFile)
    : CObject(myNode, readFile),
      m_MoveGenerated(false)
{
}

//--------------------------------------------------

char CBasicPawn::GetPrintChar() const
{
    return displayChar;
}

//--------------------------------------------------

void CBasicPawn::InitHP()
{
    m_HP = baseHP;
}

//--------------------------------------------------

void CBasicPawn::setRoute(const std::list<CNode *> &route)
{
    m_MoveList = route;
    m_MoveGenerated = true;
}

//--------------------------------------------------

bool CBasicPawn::DoAction()
{
    if (!m_MoveGenerated)
    {
        std::list<CNode *> route;
        
        try
        {
            route = BFS([](const CNode *a) { return a->m_Object->isMoving(); });
        }
        catch (const NoRouteException &)
        {
            return false;
        }

        setRoute(route);

        //pop current position to move to next pos and not stay on the same place
        m_MoveList.pop_front();
    }
    //----------------------------------------------------------------------
    //empty list - dont do anything
    if (m_MoveList.empty())
        return false;
    //----------------------------------------------------------------------
    if (m_MoveList.front()->m_Object != nullptr)
    {
        //cant move to next square, check if there is another alternative route

        std::list<CNode *> tmpMoves;
        try
        {
            //do not allow going through other objects
            tmpMoves = BFS([](const CNode *a) { return false; });
        }
        catch (const NoRouteException &)
        {
            //no route exits, wait until a way is created
            return false;
        }

        //tmp Moves has only steps that dont go on any figures
        if (tmpMoves.empty())
            return false;
        else
            m_MoveList = tmpMoves;
    }
    //----------------------------------------------------------------------

    MoveTo(m_MoveList.front());

    //pop move that has been done already
    m_MoveList.pop_front();

    return true;
}

//-----------------------------------------------------------------------

std::list<CNode *> CBasicPawn::BFS()
{
    //allows going through moving objects in hope that by the time the object gets there, the route has been cleared
    return CBasicPawn::BFS([](const CNode *a) { return a->m_Object->isMoving(); });
}

//-----------------------------------------------------------------------

std::list<CNode *> CBasicPawn::BFS(std::function<bool(const CNode *)> comparator)
{
    CNode *start = m_MyNode;
    std::queue<CNode *> possibleMoves;
    possibleMoves.push(start);

    auto current = possibleMoves.front();

    //------------------------------------------------------------------
    //search queue
    while (!possibleMoves.empty())
    {
        //working with current node
        current = possibleMoves.front();
        possibleMoves.pop();

        //check if goal has been found or not
        if (current->m_LeftNeighbour != nullptr && current->m_LeftNeighbour->m_IsGoal)
            break;

        std::vector<CNode *> neighbours({current->m_DownNeighbour,
                                         current->m_UpNeighbour,
                                         current->m_LeftNeighbour,
                                         current->m_RightNeighbour});
        //visit all neighbours
        for (auto i : neighbours)
        {
            //cant go on a square that is out of bound and only add not visited nodes into the queue
            if (i != nullptr && i->m_State == CNode::STATES::FRESH)
            {
                if (i->m_Object == nullptr || (i->m_LeftNeighbour != nullptr && i->m_LeftNeighbour->m_IsGoal) || comparator(i))
                {

                    //adds to queue to be worked with later
                    possibleMoves.push(i);
                    //sets parent for backtracking
                    i->m_Parent = current;
                    i->m_State = CNode::STATES::OPENED;
                }
            }
        }

        //close this node
        current->m_State = CNode::STATES::CLOSED;
    }
    //------------------------------------------------------------------

    //got to end of queue but hasnt found goal
    if (current->m_LeftNeighbour == nullptr || !current->m_LeftNeighbour->m_IsGoal)
        throw NoRouteException();

    //------------------------------------------------------------------

    //creation of list
    std::list<CNode *> ret;
    ret.push_front(current->m_LeftNeighbour);
    CNode *it = current;
    while (it)
    {
        ret.push_front(it);
        it = it->m_Parent;
    }
    return ret;
}

//--------------------------------------------------

bool CBasicPawn::isMoving() const
{
    return true;
}

//--------------------------------------------------

bool CBasicPawn::MoveTo(CNode *newPlace)
{
    //do not allow moving to nonexistent node and do not overwirte the object that already stands on newPlace
    if (!newPlace || newPlace->m_Object)
        return false;

    m_MyNode->m_Object = nullptr;
    m_MyNode = newPlace;
    m_MyNode->m_Object = this;

    return true;
}
