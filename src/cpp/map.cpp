#include "../hpp/map.h"
#include "../hpp/object.h"
#include "../hpp/nodes.h"
#include "../hpp/all_static.h"
#include "../hpp/towerAttack.h"
#include "../hpp/exceptions.h"

//------------------------------------------------------------------------------------------

#include <cstdlib>
#include <ctime>
#include <curses.h>

#include <algorithm>

#include <chrono>
#include <thread>

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

CMap::CMap(int height, int width, int difficulty)
    : m_Height(height),
      m_Width(width),
      m_Difficulty(difficulty)
{
    if (m_Height <= 0 || m_Width <= 0)
        throw InvalidMapSizeFromInput();

    CreateNodes();
    GenerateNewMap();
}

//------------------------------------------------------------------------------------------

void CMap::CreateNodes()
{
    //creation of nodes
    m_Nodes = new CNode **[m_Height];

    //creating grid
    for (int yPos = 0; yPos < m_Height; yPos++)
    {
        m_Nodes[yPos] = new CNode *[m_Width];

        for (int xPos = 0; xPos < m_Width; xPos++)
        {
            m_Nodes[yPos][xPos] = new CNode(xPos, yPos);
        }
    }

    //----------------------------------------------------//
    //connecting nodes from top to bottom
    for (int yPos = 0; yPos < m_Height - 1; yPos++)
    {
        for (int xPos = 0; xPos < m_Width; xPos++)
        {
            m_Nodes[yPos][xPos]->m_DownNeighbour = m_Nodes[yPos + 1][xPos];
            m_Nodes[yPos + 1][xPos]->m_UpNeighbour = m_Nodes[yPos][xPos];
        }
    }
    //----------------------------------------------------//
    //connecting nodes from left to right
    for (int xPos = 0; xPos < m_Width - 1; xPos++)
    {
        for (int yPos = 0; yPos < m_Height; yPos++)
        {
            m_Nodes[yPos][xPos]->m_RightNeighbour = m_Nodes[yPos][xPos + 1];
            m_Nodes[yPos][xPos + 1]->m_LeftNeighbour = m_Nodes[yPos][xPos];
        }
    }
}

//------------------------------------------------------------------------------------------

void CMap::GenerateNewMap()
{
    //1st gates have to be created so walls and towers dont overwrite
    CreateGates();

    GenerateWalls();
    GenerateTowers();

    if (!IsLegalMap())
    {
        //failed to create a playable map, redo generating again
        this->ResetObjects();
        this->GenerateNewMap();
    }
}

//------------------------------------------------------------------------------------------

void CMap::CreateGates()
{
    //generate end goal
    new CGoal(m_Nodes[m_Height / 2][0]);
    m_GoalPost = m_Nodes[m_Height / 2][0];
    m_GoalPost->m_IsGoal = true;

    //-----------------------------------------------------------------

    CGate::ResetGates();

    //generate input gates
    new CGate(m_Nodes[m_Height / 2][m_Width - 1]);
    m_GatesPos.push_back(m_Nodes[m_Height / 2][m_Width - 1]);

    new CGate(m_Nodes[m_Height - 1][m_Width - 1]);
    m_GatesPos.push_back(m_Nodes[m_Height - 1][m_Width - 1]);

    new CGate(m_Nodes[0][m_Width - 1]);
    m_GatesPos.push_back(m_Nodes[0][m_Width - 1]);
}

//------------------------------------------------------------------------------------------

void CMap::GenerateWalls()
{
    srand(time(NULL));
    int amount = amountWalls * m_Difficulty;

    while (amount--)
    {
        int xPos = std::rand() % m_Width;
        int yPos = std::rand() % m_Height;

        if (m_Nodes[yPos][xPos]->m_Object == nullptr)
        {
            new CWall(m_Nodes[yPos][xPos]);
        }
        else
            amount++;
    }

}

//------------------------------------------------------------------------------------------

void CMap::GenerateTowers()
{
    srand(time(NULL));
    int amount = amountTowers * m_Difficulty;

    while (amount--)
    {
        int type = std::rand() % amountTypeTowers;
        int xPos = std::rand() % m_Width;
        int yPos = std::rand() % m_Height;

        if (m_Nodes[yPos][xPos]->m_Object == nullptr)
        {
            switch (type)
            {
            case 0:
                new CTower(m_Nodes[yPos][xPos]);
                m_Towers.push_back(m_Nodes[yPos][xPos]);
                break;
            case 1:
                new CChargeTower(m_Nodes[yPos][xPos]);
                m_Towers.push_back(m_Nodes[yPos][xPos]);
                break;
            default:
                amount++; ///< just in case user forgot to add tower
                break;
            }
        }
        else
            amount++;
    }
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

CMap::CMap(std::ifstream &readFile)
    : m_Height(0), m_Width(0), m_Difficulty(0), m_Nodes(nullptr), m_GoalPost(nullptr)
{
    std::string tmpString;

    if (!getline(readFile, tmpString) || tmpString != mapStartDelim)
        throw CorruptedFile();
    //*******************************************************************
    //start of reading map
    if (!(readFile >> m_Height >> m_Width >> m_Difficulty))
        throw CorruptedFile();

    this->CreateNodes();

    //ignore unused \n in stream
    readFile.ignore(1, '\n');
    if (!getline(readFile, tmpString) || tmpString != objectsStartDelim)
        throw CorruptedFile();

    if (!GenerateNewMap(readFile))
        throw CorruptedFile();

    //end of reading map
    //*******************************************************************
    if (!getline(readFile, tmpString) || tmpString != mapEndDelim)
        throw CorruptedFile();
    
    //last check
    if (!IsLegalMap())
        throw CorruptedFile();
}

//------------------------------------------------------------------------------------------

bool CMap::GenerateNewMap(std::ifstream &readFile)
{
    std::string tmpString;
    char printChar;
    char delim1, delim2;
    int yPos, xPos;

    //reset gate numbering
    CGate::ResetGates();

    while (true)
    {
        //-----------------------------------------------------------------------------------------
        //tests if end of objects reading, if no, return to where it was before trying to read
        auto currentStreamPos = readFile.tellg();
        readFile.ignore(1, '\n');
        getline(readFile, tmpString);

        if (tmpString == objectsEndDelim)
            return true;

        readFile.seekg(currentStreamPos);
        //-----------------------------------------------------------------------------------------
        //false format
        if ((readFile >> printChar >> delim1 >> yPos >> xPos >> delim2).fail() || delim1 != '[' || delim2 != ']')
            return false;

        //created object is out of bound
        if (yPos < 0 || xPos < 0 || yPos >= m_Height || xPos >= m_Width)
            return false;

        //created object lies on another object
        if (m_Nodes[yPos][xPos]->m_Object != nullptr)
            return false;

        switch (printChar)
        {
        //-------------------------------------------------------------//
        case CBasicPawn::displayChar:
            new CBasicPawn(m_Nodes[yPos][xPos], readFile);
            break;
        //-------------------------------------------------------------//
        case CDefensive::displayChar:
            new CDefensive(m_Nodes[yPos][xPos], readFile);
            break;
        //-------------------------------------------------------------//
        case CJoker::displayChar:
            new CJoker(m_Nodes[yPos][xPos], readFile);
            break;
        //-------------------------------------------------------------//
        case CWall::displayChar:
            new CWall(m_Nodes[yPos][xPos], readFile);
            break;
        //-------------------------------------------------------------//
        case CGate::displayChar:
            new CGate(m_Nodes[yPos][xPos], readFile);
            m_GatesPos.push_back(m_Nodes[yPos][xPos]);
            break;
        //-------------------------------------------------------------//
        case CGoal::displayChar:
            new CGoal(m_Nodes[yPos][xPos], readFile);
            m_GoalPost = m_Nodes[yPos][xPos];
            m_GoalPost->m_IsGoal = true;
            break;
        //-------------------------------------------------------------//
        case CTower::displayChar:
            new CTower(m_Nodes[yPos][xPos], readFile);
            m_Towers.push_back(m_Nodes[yPos][xPos]);
            break;
        //-------------------------------------------------------------//
        case CChargeTower::displayChar:
            new CChargeTower(m_Nodes[yPos][xPos], readFile);
            m_Towers.push_back(m_Nodes[yPos][xPos]);
            break;
        //-------------------------------------------------------------//
        default:
            //got unexpected characted as 1st char
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------------------

bool CMap::IsLegalMap() const
{
    if (m_Height <= 0 || m_Width <= 0)
        return false;

    if (m_Nodes == nullptr)
        return false;

    if (m_GoalPost == nullptr || m_GoalPost->m_RightNeighbour == nullptr || m_GoalPost->m_RightNeighbour->m_Object)
        return false;

    if (m_GatesPos.empty())
        return false;

    for (auto gates : m_GatesPos)
        if (gates == nullptr || gates->m_LeftNeighbour == nullptr || gates->m_LeftNeighbour->m_Object)
            return false;

    return true;
}

//------------------------------------------------------------------------------------------

void CMap::ResetObjects()
{
    for (int yPos = 0; yPos < m_Height; yPos++)
    {
        for (int xPos = 0; xPos < m_Width; xPos++)
        {
            if (m_Nodes[yPos][xPos]->m_Object != nullptr)
            {
                delete m_Nodes[yPos][xPos]->m_Object;
                m_Nodes[yPos][xPos]->m_Object = nullptr;
            }
        }
    }
    //------------------------------------------------------
    m_GoalPost->m_IsGoal = false;
    m_GoalPost = nullptr;
    m_GatesPos.clear();
    m_Towers.clear();
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

CMap::~CMap()
{
    for (int yPos = 0; yPos < m_Height; yPos++)
    {
        for (int xPos = 0; xPos < m_Width; xPos++)
        {
            //nodes delete the objects themself if they hold any
            delete m_Nodes[yPos][xPos];
        }

        delete[] m_Nodes[yPos];
    }

    delete[] m_Nodes;
    m_Nodes = nullptr;
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void CMap::DisplayBorder(WINDOW *winContext, int offset) const
{
    //#####->
    for (int xPos = 0; xPos <= m_Width + 1; xPos++)
    {
        mvwaddch(winContext, offset, offset + xPos, '#');
        refresh();

        mvwaddch(winContext, offset + m_Height + 1, offset + xPos, '#');
        refresh();
    }

    //walls vertically
    for (int yPos = 0; yPos <= m_Height + 1; yPos++)
    {
        mvwaddch(winContext, offset + yPos, offset, '#');
        mvwaddch(winContext, offset + yPos, offset + m_Width + 1, '#');
        refresh();
    }
}

//------------------------------------------------------------------------------------------

void CMap::DisplayMap(WINDOW *winContext, int offset) const
{
    //increment offset by 1 because at offset exactly is border displayed
    offset++;

    for (int xPos = 0; xPos < m_Width; xPos++)
    {
        for (int yPos = 0; yPos < m_Height; yPos++)
        {
            if (m_Nodes[yPos][xPos]->m_Object != nullptr)
                m_Nodes[yPos][xPos]->m_Object->PrintSelf(winContext, offset);
            else
                mvwaddch(winContext, offset + yPos, offset + xPos, ' ');

            refresh();
        }
    }
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

std::pair<int, int> CMap::GetSize() const
{
    return {m_Height, m_Width};
}

//------------------------------------------------------------------------------------------

std::vector<CBasicPawn *> CMap::GetAttackers() const
{
    std::vector<CBasicPawn *> ret;
    for (int xPos = 0; xPos < m_Width; xPos++)
        for (int yPos = 0; yPos < m_Height; yPos++)
        {
            if (m_Nodes[yPos][xPos]->m_Object != nullptr && m_Nodes[yPos][xPos]->m_Object->isMoving())
                ret.push_back((CBasicPawn *)m_Nodes[yPos][xPos]->m_Object);
        }
    return ret;
}

//------------------------------------------------------------------------------------------

CGoal *CMap::GetGoal() const
{
    return (CGoal *)m_GoalPost->m_Object;
}

//------------------------------------------------------------------------------------------

std::vector<CGate *> CMap::GetGates() const
{
    std::vector<CGate *> ret;
    for (auto gates : m_GatesPos)
        ret.push_back((CGate *)gates->m_Object);

    return ret;
}

//------------------------------------------------------------------------------------------

std::vector<CTower *> CMap::GetTowers() const
{
    std::vector<CTower *> ret;
    for (auto towers : m_Towers)
        ret.push_back((CTower *)towers->m_Object);

    return ret;
}

//------------------------------------------------------------------------------------------

void CMap::ResetNodes()
{
    for (int xPos = 0; xPos < m_Width; xPos++)
        for (int yPos = 0; yPos < m_Height; yPos++)
        {
            m_Nodes[yPos][xPos]->m_State = CNode::STATES::FRESH;
            m_Nodes[yPos][xPos]->m_Parent = nullptr;
        }
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

std::ostream &operator<<(std::ostream &out, const CMap &map)
{
    out << mapStartDelim << std::endl;
    //******************************************************
    out << map.m_Height << std::endl;
    out << map.m_Width << std::endl;
    out << map.m_Difficulty << std::endl;
    //******************************************************
    out << objectsStartDelim << std::endl;
    for (int xPos = 0; xPos < map.m_Width; xPos++)
        for (int yPos = 0; yPos < map.m_Height; yPos++)
        {
            CObject *currentObj = map.m_Nodes[yPos][xPos]->m_Object;
            if (currentObj != nullptr)
                out << *currentObj << std::endl;
        }
    out << objectsEndDelim << std::endl;
    //******************************************************
    out << mapEndDelim;

    return out;
}

//------------------------------------------------------------------------------------------
