#include "../hpp/tower.h"
#include "../hpp/nodes.h"

//---------------------------------------------------------------------------------------

#include <vector>
#include <queue>

//---------------------------------------------------------------------------------------

CTower::CTower(CNode *myNode)
    : CObject(myNode), m_Generated(false)
{
}

CTower::CTower(CNode *myNode, std::ifstream &readFile)
    : CObject(myNode, readFile), m_Generated(false)
{
}

//---------------------------------------------------------------------------------------

char CTower::GetPrintChar() const
{
    return CTower::displayChar;
}

//---------------------------------------------------------------------------------------

void CTower::InitHP()
{
    m_HP = CTower::baseHP;
}

//---------------------------------------------------------------------------------------

int CTower::GetAtk() const
{
    return CTower::baseAttack;
}

//---------------------------------------------------------------------------------------


int CTower::GetRange() const
{
    return CTower::range;
}

//---------------------------------------------------------------------------------------

void CTower::CreateTargets()
{
    //range to both sides + self
    int boxRangeSize = GetRange() * 2 + 1;

    //moves to upper left side of range box
    int xPosStart = m_MyNode->m_Xpos - GetRange();
    int yPosStart = m_MyNode->m_Ypos - GetRange();

    CNode *current = nullptr;

    //checks all nodes within this square
    for (int xIt = 0; xIt < boxRangeSize; xIt++)
        for (int yIt = 0; yIt < boxRangeSize; yIt++)
        {
            if ((current = FindNode(xPosStart + xIt, yPosStart + yIt)) != nullptr)
            {
                //target node has to be free or can only be occupied by an attacker
                if (current->m_Object == nullptr || current->m_Object->isMoving())
                {
                    m_PotentialTargets.push_back(current);
                }
            }
        }
}

//---------------------------------------------------------------------------------------

bool CTower::DoAction()
{
    return this->Attack();
}

//---------------------------------------------------------------------------------------

bool CTower::Attack()
{
    if (!m_Generated)
    {
        CreateTargets();
        m_Generated = true;
    }
    //----------------------------------------

    /**
     * find target and attack it
     * */
    std::vector<CNode *> targets = GetTarget();
    for (auto it : targets)
    {
        it->m_Object->TakeDamage(this->GetAtk());

        //delete object if it got "killed"
        if (it->m_Object->getHP() <= 0)
        {
            delete it->m_Object;
            it->m_Object = nullptr;
        }
    }

    return !targets.empty();
}

//---------------------------------------------------------------------------------------

std::vector<CNode *> CTower::GetTarget() const
{
    std::vector<CNode *> ret;
    for (auto potentialTargets : m_PotentialTargets)
    {
        if (potentialTargets->m_Object && potentialTargets->m_Object->isMoving())
        {
            //tower only attacks 1 pawn
            ret.push_back(potentialTargets);
            return ret;
        }
    }

    return ret;
}

//---------------------------------------------------------------------------------------

