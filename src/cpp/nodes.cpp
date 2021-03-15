#include "../hpp/nodes.h"
#include "../hpp/object.h"

CNode::CNode(CObject * object)
    : m_State (STATES::FRESH),
    m_Parent(nullptr),
    m_Xpos(0), m_Ypos(0),
    m_Object(object),
    m_IsGoal(false),
    m_UpNeighbour(nullptr),
    m_DownNeighbour(nullptr),
    m_LeftNeighbour(nullptr),
    m_RightNeighbour(nullptr)
{
}

//--------------------------------------------------------//

CNode::CNode(int xPos, int yPos)
    : m_State (STATES::FRESH),
    m_Parent(nullptr),
    m_Xpos(xPos), m_Ypos(yPos),
    m_Object(nullptr),
    m_IsGoal(false),
    m_UpNeighbour(nullptr),
    m_DownNeighbour(nullptr),
    m_LeftNeighbour(nullptr),
    m_RightNeighbour(nullptr)
{
}

//--------------------------------------------------------//

CNode::~CNode()
{
    delete m_Object;
    m_Object = nullptr;
}

//--------------------------------------------------------//