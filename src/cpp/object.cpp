#include "../hpp/object.h"
#include "../hpp/nodes.h"
#include "../hpp/map.h"
#include "../hpp/exceptions.h"

//-----------------------------------------------------------------------

#include <climits>
#include <fstream>

//-----------------------------------------------------------------------

CObject::CObject(CNode *node)
    : m_MyNode(node)
{
    if (node->m_Object != nullptr)
        throw CreationObjectError();

    node->m_Object = this;
    this->InitHP();
}

CObject::CObject(CNode *node, std::ifstream &readFile)
    : m_MyNode(node)
{
    if (node->m_Object != nullptr)
        throw CreationObjectError();

    node->m_Object = this;

    if ((readFile >> m_HP).fail())
        throw CorruptedFile();
}

//-----------------------------------------------------------------------

void CObject::InitHP()
{
    m_HP = INT_MAX;
}

//-----------------------------------------------------------------------

void CObject::PrintSelf(WINDOW *winContext, const int offset) const
{
    //determines which char to print depending on ret value of GetPrintChar
    mvwaddch(winContext, offset + m_MyNode->m_Ypos, offset + m_MyNode->m_Xpos, this->GetPrintChar());
    refresh();
}

//-----------------------------------------------------------------------

int CObject::getHP() const
{
    return m_HP;
}

//-----------------------------------------------------------------------

void CObject::TakeDamage(int attackStat)
{
    m_HP -= attackStat;
}

//-----------------------------------------------------------------------

bool CObject::isMoving() const
{
    return false;
}

//-----------------------------------------------------------------------

bool CObject::isGoal() const
{
    return false;
}

//-----------------------------------------------------------------------

std::pair<int, int> CObject::GetPos() const
{
    //determines current position according to m_MyNode position
    return {m_MyNode->m_Ypos, m_MyNode->m_Xpos};
}

//-----------------------------------------------------------------------

CNode *CObject::FindNode(int xPos, int yPos) const
{
    if (xPos < 0 || yPos < 0)
        return nullptr;

    CNode *current = this->m_MyNode;

    while (current->m_Xpos != xPos)
    {
        //xPos m_Xpos :: need to move left -<
        if (current->m_Xpos > xPos)
            current = current->m_LeftNeighbour;
        else
            current = current->m_RightNeighbour;

        if (current == nullptr)
            return nullptr;
    }

    while (current->m_Ypos != yPos)
    {
        /**
         * yPos
         * 
         * m_Ypos :: need to move up
         * */
        if (current->m_Ypos > yPos)
            current = current->m_UpNeighbour;
        else
            current = current->m_DownNeighbour;

        if (current == nullptr)
            return nullptr;
    }

    return current;
}

//-----------------------------------------------------------------------

std::ostream &operator<<(std::ostream &out, const CObject &obj)
{
    obj.SaveInfo(out);
    return out;
}

//-----------------------------------------------------------------------

std::ostream &CObject::SaveInfo(std::ostream &out) const
{
    out << GetPrintChar() << saveDelim;
    out << '[' << GetPos().first << saveDelim;
    out << GetPos().second << ']' << saveDelim;
    out << getHP();

    return out;
}
