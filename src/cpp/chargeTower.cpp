#include "../hpp/chargeTower.h"
#include "../hpp/exceptions.h"
#include "../hpp/nodes.h"

//--------------------------------------------------------------------

#include <fstream>

//--------------------------------------------------------------------

const double CChargeTower::AttackMultiplier = 3.0;

//--------------------------------------------------------------------

CChargeTower::CChargeTower(CNode *myNode)
    : CTower(myNode),
      m_Charged(false)
{
}

CChargeTower::CChargeTower(CNode *myNode, std::ifstream &readFile)
    : CTower(myNode, readFile)
{
    int charge;
    if (!(readFile >> charge))
        throw CorruptedFile();

    if (charge == 1)
        m_Charged = true;
    else if(charge == 0)
        m_Charged = false;
    else
        throw CorruptedFile();

}

//--------------------------------------------------------------------

char CChargeTower::GetPrintChar() const
{
    return displayChar;
}

//--------------------------------------------------------------------

int CChargeTower::GetAtk() const
{
    return CTower::GetAtk() * AttackMultiplier;
}

//--------------------------------------------------------------------

int CChargeTower::GetRange() const
{
    return CChargeTower::range;
}

//--------------------------------------------------------------------

bool CChargeTower::Attack()
{
    if (m_Charged)
    {
        if (CTower::Attack())
        {
            m_Charged = false;
            return true;
        }
    }
    else
    {
        m_Charged = true;
    }
    return false;
}

//--------------------------------------------------------------------

std::vector<CNode *> CChargeTower::GetTarget() const
{
    std::vector<CNode *> ret;
    for (auto potentialTargets : m_PotentialTargets)
    {
        if (potentialTargets->m_Object && potentialTargets->m_Object->isMoving())
        {
            ret.push_back(potentialTargets);
        }
    }

    return ret;
}

//--------------------------------------------------------------------

std::ostream &CChargeTower::SaveInfo(std::ostream &out) const
{
    CObject::SaveInfo(out);
    out << saveDelim << m_Charged;
    return out;
}
