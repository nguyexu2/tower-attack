#include "../hpp/gameState.h"
#include "../hpp/towerAttack.h"
#include "../hpp/exceptions.h"

#include <vector>
#include <sstream>
#include <algorithm>

//*****************************************************************************
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

CGameState::CGameState(const std::string &username,
                       const std::vector<CGate *> &gates, int winCondition, unsigned int loseCondition,
                       int currGate, ATTACKERTYPE figure, unsigned turns, int score)
    : m_Username(username),
      m_Turns(turns),
      m_Score(score),
      m_CurrentGate(currGate),
      m_Figure(figure),
      m_Gates(gates),
      m_WinCondition(winCondition),
      m_LoseCondition(loseCondition)
{
}

//-----------------------------------------------------------------------------

CGameState::CGameState(std::ifstream &readFile, const std::vector<CGate *> &gates)
    : m_Gates(gates)
{
    std::string tmpString;

    if (!(getline(readFile, tmpString)) || tmpString != stateStartDelim)
        throw CorruptedFile();
    //********************************************************************
    //start reading information about the file
    if (!(getline(readFile, m_Username)))
        throw CorruptedFile();

    if (!(readFile >> m_Turns >> m_Score >> m_CurrentGate))
        throw CorruptedFile();

    int tmpFigure;
    readFile >> tmpFigure;
    m_Figure = ATTACKERTYPE(tmpFigure);

    if (!(readFile >> m_WinCondition >> m_LoseCondition))
        throw CorruptedFile();

    //end reading information about the file
    //********************************************************************
    readFile.ignore(1, '\n');
    if (!(getline(readFile, tmpString)) || tmpString != stateEndDelim)
        throw CorruptedFile();
}

//*****************************************************************************
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void CGameState::InputNewFigure()
{
    for (auto gate : m_Gates)
    {
        //only sommon from gates that have the same gate number
        //m_CurrentGate + 1 because index doesnt match display number
        if (gate->GetGateNum() == m_CurrentGate + 1)
            gate->Summon(m_Figure);
    }
}

//-----------------------------------------------------------------------------

void CGameState::ChangeChoice(int direction)
{
    switch (direction)
    {
    case INPUT_KEY_DOWN:
        m_CurrentGate = (m_CurrentGate + 1) % m_Gates.size();
        break;
    case INPUT_KEY_UP:
        m_CurrentGate = (m_CurrentGate + -1) < 0 ? m_Gates.size() - 1 : (m_CurrentGate + -1) % m_Gates.size();
        break;
    //---------------------------------------------------------//
    case INPUT_KEY_RIGHT:
        m_Figure = ATTACKERTYPE((m_Figure + 1) % ATTACKERTYPE::ATTACKER_STOPPER);
        break;
    case INPUT_KEY_LEFT:
        m_Figure = ATTACKERTYPE((m_Figure - 1) < 0 ? ATTACKERTYPE::ATTACKER_STOPPER - 1 : (m_Figure - 1));
        break;
    }
}

//-----------------------------------------------------------------------------

void CGameState::IncreaseTurns()
{
    m_Turns++;
}

//-----------------------------------------------------------------------------

std::pair<int, int> CGameState::GetSize() const
{
    return {boxHeight, boxWidth + distanceFromMap};
}

//-----------------------------------------------------------------------------

void CGameState::DisplayGameState(WINDOW *winContext, const int offset) const
{
    this->DisplayBorder(winContext, offset);

    int row = 2; //<starting at 2 for clearance from border
    this->DisplayTurns(winContext, offset, row);

    row++; //for space inbetween each display
    this->DisplayChoice(winContext, offset, row);

    row++;
    this->DisplayScore(winContext, offset, row);
}

//-----------------------------------------------------------------------------

void CGameState::DisplayBorder(WINDOW *winContext, const int offset) const
{
    //get the left most point of map
    std::pair<int, int> pos = m_Gates[0]->GetPos();
    int xOffset = pos.second + distanceFromMap;

    //->
    for (int i = 0; i <= boxWidth; i++)
    {
        //top layer
        mvwaddch(winContext, offset, xOffset + i, '#');
        //bottom layer
        mvwaddch(winContext, offset + boxHeight, xOffset + i, '#');
        refresh();
    }
    // |
    // v
    for (int i = 0; i <= boxHeight; i++)
    {
        //row on the right
        mvwaddch(winContext, offset + i, xOffset, '#');
        //row on the left
        mvwaddch(winContext, offset + i, xOffset + boxWidth, '#');
        refresh();
    }
}

//-----------------------------------------------------------------------------

void CGameState::DisplayTurns(WINDOW *winContext, const int offset, int &row) const
{
    std::pair<int, int> pos = m_Gates[0]->GetPos();
    int xOffset = pos.second + distanceFromMap + 2;
    int yOffset = offset + row;

    mvwaddstr(winContext, yOffset, xOffset, "turns: ");
    DisplayNumber(winContext, yOffset, xOffset + sizeof("turns: "), m_Turns);
    refresh();

    row++;
}

//-----------------------------------------------------------------------------

void CGameState::DisplayChoice(WINDOW *winContext, const int offset, int &row) const
{

    std::pair<int, int> pos = m_Gates[0]->GetPos();
    int xOffset = pos.second + distanceFromMap + 2;
    int yOffset = offset + row;

    mvwaddstr(winContext, yOffset, xOffset, "figure: ");

    std::vector<std::string> allFigures({"basic", "defensive", "joker"});

    std::string printer;
    switch (m_Figure)
    {
    case BASIC:
        printer = allFigures[0];
        break;
    case DEFENSIVE:
        printer = allFigures[1];
        break;
    case JOKER:
        printer = allFigures[2];
        break;
    default:
        break;
    }

    //removes previous string
    RemoveText(winContext, yOffset, xOffset + sizeof("figure: "), 
        std::max({allFigures[0].length(), allFigures[1].length(), allFigures[2].length()  }) + 1 );
    mvwaddstr(winContext, yOffset, xOffset + sizeof("figure: "), printer.c_str());
    refresh();

    row += 2;
    yOffset += 2;

    mvwaddstr(winContext, yOffset, xOffset, "gate: ");
    DisplayNumber(winContext, yOffset, xOffset + sizeof("gate: "), m_CurrentGate + 1); //gate +1 because index doesnt match numbering
    refresh();

    row++;
}

//-----------------------------------------------------------------------------

void CGameState::DisplayScore(WINDOW *winContext, const int offset, int &row) const
{
    std::pair<int, int> pos = m_Gates[0]->GetPos();
    int xOffset = pos.second + distanceFromMap + 2;
    int yOffset = offset + row;

    mvwaddstr(winContext, yOffset, xOffset, "WinCondition: ");
    DisplayNumber(winContext, yOffset, xOffset + sizeof("WinCondition: "), m_WinCondition);
    refresh();

    yOffset++;
    row++;

    mvwaddstr(winContext, yOffset, xOffset, "Score: ");
    DisplayNumber(winContext, yOffset, xOffset + sizeof("Score: "), m_Score);
    refresh();

    row++;
}

//-----------------------------------------------------------------------------

void CGameState::IncreaseScore(int amount)
{
    m_Score += amount;
}

//-----------------------------------------------------------------------------

bool CGameState::isWon() const
{
    return m_Score >= m_WinCondition;
}

//-----------------------------------------------------------------------------

bool CGameState::isLost() const
{
    return m_Turns >= m_LoseCondition;
}

//-----------------------------------------------------------------------------
std::ostream &operator<<(std::ostream &out, const CGameState &gameState)
{
    out << stateStartDelim << std::endl;

    out << gameState.m_Username << std::endl;
    out << gameState.m_Turns << std::endl;
    out << gameState.m_Score << std::endl;
    out << gameState.m_CurrentGate << std::endl;
    out << gameState.m_Figure << std::endl;
    out << gameState.m_WinCondition << std::endl;
    out << gameState.m_LoseCondition << std::endl;

    out << stateEndDelim;
    return out;
}
