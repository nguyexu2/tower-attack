#include "../hpp/gate.h"
#include "../hpp/nodes.h"
#include "../hpp/all_moving.h"
#include "../hpp/exceptions.h"

//---------------------------------------------------------

#include <sstream>

#include <curses.h>

//---------------------------------------------------------

#define PAWN_X_OFFSET 2

int CGate::gateNum = 1;

//---------------------------------------------------------

void CGate::ResetGates()
{
    CGate::gateNum = 1;
}

//---------------------------------------------------------

CGate::CGate(CNode *myNode)
    : CObject(myNode),
      m_GateNum(CGate::gateNum++)
{
}

CGate::CGate(CNode *myNode, std::ifstream &readFile)
    : CObject(myNode, readFile)
{
    if (!(readFile >> m_GateNum))
        throw CorruptedFile();

    if (m_GateNum > CGate::gateNum)
        CGate::gateNum = m_GateNum;
}

//---------------------------------------------------------

int CGate::GetGateNum() const
{
    return m_GateNum;
}

//---------------------------------------------------------

char CGate::GetPrintChar() const
{
    return displayChar;
}

//---------------------------------------------------------

void CGate::PrintSelf(WINDOW *windowContext, const int offset) const
{
    CObject::PrintSelf(windowContext, offset);

    std::stringstream printer;
    printer << m_GateNum;
    std::string str;
    printer >> str;

    mvwaddch(windowContext, offset + m_MyNode->m_Ypos, offset + m_MyNode->m_Xpos + 1, displayBorder);
    mvwaddstr(windowContext, offset + m_MyNode->m_Ypos, offset + m_MyNode->m_Xpos + 2, str.c_str());
    refresh();
}

//---------------------------------------------------------

bool CGate::DoAction()
{
    return this->Summon(ATTACKERTYPE::BASIC);
}

//---------------------------------------------------------

bool CGate::Summon(ATTACKERTYPE type)
{
    if (this->m_MyNode->m_LeftNeighbour->m_Object != nullptr)
    {
        return false;
    }
    try
    {
        switch (type)
        {
        case BASIC:
            new CBasicPawn(m_MyNode->m_LeftNeighbour);
            break;
        case JOKER:
            new CJoker(m_MyNode->m_LeftNeighbour);
            break;
        case DEFENSIVE:
            new CDefensive(m_MyNode->m_LeftNeighbour);
            break;
        default:
            new CJoker(m_MyNode->m_LeftNeighbour);
            break;
        }
    }
    catch (const CreationObjectError &)
    {
        return false;
    }
    return true;
}

//---------------------------------------------------------

std::ostream &CGate::SaveInfo(std::ostream &out) const
{
    CObject::SaveInfo(out);
    out << saveDelim << m_GateNum;
    return out;
}
