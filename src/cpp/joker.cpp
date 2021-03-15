#include "../hpp/joker.h"

//--------------------------------------------------------

#include <cstdlib>
#include <ctime>

#include <list>

//--------------------------------------------------------
const double CJoker::HP_multiplier = 1.0;

CJoker::CJoker(CNode *myNode)
    : CBasicPawn(myNode)
{
}

CJoker::CJoker(CNode *myNode, std::ifstream &readFile)
    : CBasicPawn(myNode, readFile)
{
}

//--------------------------------------------------------

char CJoker::GetPrintChar() const
{
    return displayChar;
}

//------------------------------------------------------

void CJoker::InitHP()
{
    CBasicPawn::InitHP();
    m_HP *= HP_multiplier;
}

//--------------------------------------------------------

bool CJoker::DoAction()
{
    //if moves were generated then move like basic pawn and keep going down planned route
    if (m_MoveGenerated)
    {
        return CBasicPawn::DoAction();
    }
    else
    {
        auto list = BFS();

        if (!list.empty() && list.size() <= possibleMovesNormal)
        {
            //within range to goal, move like a basic pawn
            setRoute(list);
            return CBasicPawn::DoAction();
        }
        else
        {
            return GoToRandomPlace();
        }
    }
}

//------------------------------------------------------------------

bool CJoker::GoToRandomPlace()
{
    enum DIRECTION
    {
        UP,
        LEFT,
        DOWN,
        RIGHT,
        LAST
    };

    //---------------------------------------------------
    CNode *current = m_MyNode;
    
    //seeds to generate more random numbers
    srand(time(NULL));


    int amount = std::rand() % possibleMovesRandom;


    while (amount > 0 ? amount-- : 0 || current->m_Object != nullptr)
    {
        int direction = std::rand() % LAST;

        //checks if isnt moving out of bound, neighbour != nullptr
        switch (direction)
        {
        case UP:
            if (current->m_UpNeighbour)
                current = current->m_UpNeighbour;
            break;
        case LEFT:
            if (current->m_LeftNeighbour)
                current = current->m_LeftNeighbour;
            break;
        case DOWN:
            if (current->m_DownNeighbour)
                current = current->m_DownNeighbour;
            break;
        case RIGHT:
            if (current->m_RightNeighbour)
                current = current->m_RightNeighbour;
            break;
        }
    }

    return MoveTo(current);
}
