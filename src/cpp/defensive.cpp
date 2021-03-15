#include "../hpp/defensive.h"
#include "../hpp/exceptions.h"

const double CDefensive::HP_multiplier = 4.0;

//------------------------------------------------------

CDefensive::CDefensive(CNode *myNode)
    : CBasicPawn(myNode),
      m_Charged(false)
{
    this->InitHP();
}

CDefensive::CDefensive(CNode *myNode, std::ifstream &readFile)
    : CBasicPawn(myNode, readFile)
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

//------------------------------------------------------

std::ostream &CDefensive::SaveInfo(std::ostream &out) const
{
    CObject::SaveInfo(out);
    out << saveDelim << m_Charged;
    return out;
}

//------------------------------------------------------

char CDefensive::GetPrintChar() const
{
    return displayChar;
}

//------------------------------------------------------

void CDefensive::InitHP()
{
    CBasicPawn::InitHP();
    m_HP *= HP_multiplier;
}

//------------------------------------------------------

bool CDefensive::DoAction()
{
    if (m_Charged && CBasicPawn::DoAction())
    {
        m_Charged = false;
        return true;
    }

    m_Charged = true;
    return false;
}

//------------------------------------------------------