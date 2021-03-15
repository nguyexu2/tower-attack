#include "../hpp/gameEngine.h"
#include "../hpp/towerAttack.h"

#include "../hpp/all_moving.h"
#include "../hpp/all_static.h"
#include "../hpp/exceptions.h"

//------------------------------------------------------------------------------------------

#include <chrono>
#include <thread>

#include <iostream>
#include <vector>
#include <fstream>

//------------------------------------------------------------------------------------------

CGameEngine::CGameEngine(WINDOW *winContext)
    : m_WinContext(winContext), m_CurrentState(NEW_GAME), m_Generated(false),
      m_Map(nullptr), m_GameState(nullptr)
{
}

CGameEngine::~CGameEngine()
{
    this->Reset();
}

//------------------------------------------------------------------------------------------

void CGameEngine::Reset()
{
    delete m_Map;
    m_Map = nullptr;
    delete m_GameState;
    m_GameState = nullptr;

    m_Generated = false;
    m_CurrentState = NEW_GAME;
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void CGameEngine::StartEngine()
{
    //changes m_CurrentState
    this->DisplayMenu();

    switch (m_CurrentState)
    {
    case MENU_STATES::NEW_GAME:
        this->NewGame();
        break;
    case MENU_STATES::CONTINUE:
        this->LoadGame();
        break;
    case MENU_STATES::CREDIT:
        this->DisplayCredit();
        break;
    case MENU_STATES::QUIT:
        return;
    default:
        break;
    }

    //resets for preparation for next round
    this->Reset();

    //goes back to display menu again if not quitting
    this->StartEngine();
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void CGameEngine::DisplayMenu()
{
    wclear(m_WinContext);

    int heightSize = 0;
    int widthSize = 0;
    getmaxyx(m_WinContext, heightSize, widthSize);
    int middle_Y = heightSize / 2;
    int middle_X = widthSize / 2;

    //displays the menu screen and choices
    std::vector<int> possiblePosY = {middle_Y - TEXT_DISTANCE, middle_Y, middle_Y + TEXT_DISTANCE};
    mvwaddstr(m_WinContext, possiblePosY[0], middle_X, "New Game");
    mvwaddstr(m_WinContext, possiblePosY[1], middle_X, "Load Game");
    mvwaddstr(m_WinContext, possiblePosY[2], middle_X, "Credit");
    refresh();

    //shows cursor to current position
    mvwaddch(m_WinContext, possiblePosY[m_CurrentState], middle_X - 5, '>');

    this->HandleMenuInput({possiblePosY, middle_X - 5});

    wclear(m_WinContext);
}

//------------------------------------------------------------------------------------------

void CGameEngine::HandleMenuInput(const std::pair<std::vector<int>, int> possiblePos)
{
    char readChar;

    //jump out of loop when quitting or enter(confirmation is pressed)
    while ((readChar = getch()) != 'q' && readChar != '\n')
    {
        //change state of cursor if moving key is pressed
        switch (readChar)
        {
        case INPUT_KEY_UP:
            //removes previous char
            mvwaddch(m_WinContext, possiblePos.first[m_CurrentState], possiblePos.second, ' ');

            m_CurrentState = m_CurrentState - 1; //overloaded operator- is modular
            mvwaddch(m_WinContext, possiblePos.first[m_CurrentState], possiblePos.second, '>');
            break;
        //-----------------------------------------------------------//
        case INPUT_KEY_DOWN:
            //removes previous char
            mvwaddch(m_WinContext, possiblePos.first[m_CurrentState], possiblePos.second, ' ');

            m_CurrentState = m_CurrentState + 1; //overloaded operator+ is modular
            mvwaddch(m_WinContext, possiblePos.first[m_CurrentState], possiblePos.second, '>');
            break;
        //-----------------------------------------------------------//
        default:
            break;
        }
        refresh();
    }

    if (readChar == 'q')
        m_CurrentState = MENU_STATES::QUIT;
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void CGameEngine::DisplayCredit() const
{
    wclear(m_WinContext);

    int heightSize = 0;
    int widthSize = 0;
    getmaxyx(m_WinContext, heightSize, widthSize);
    int displayY = heightSize / 2;
    int displayX = widthSize / 2;

    mvwaddstr(m_WinContext, displayY - TEXT_DISTANCE, displayX - sizeof("Tower attack") / 2, "Tower attack");
    mvwaddstr(m_WinContext, displayY, displayX - sizeof("Game created by: Nguyen Xuan Thang") / 2, "Game created by: Nguyen Xuan Thang");
    mvwaddstr(m_WinContext, displayY + 2 * TEXT_DISTANCE, displayX - sizeof("quit by pressing 'q'") / 2, "quit by pressing 'q'");
    refresh();

    //displays until 'q' is pressed
    while (getch() != 'q')
        ;

    wclear(m_WinContext);
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

bool CGameEngine::CheckScreenSize() const
{
    int heightSize = 0;
    int widthSize = 0;
    getmaxyx(m_WinContext, heightSize, widthSize);

    auto mapSize = m_Map->GetSize();
    auto stateSize = m_GameState->GetSize();

    if (std::max(mapSize.first, stateSize.first) + TEXT_DISTANCE * 2 >= heightSize || TEXT_DISTANCE * 2 + mapSize.second + stateSize.second >= widthSize)
    {
        return false;
    }

    return true;
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void CGameEngine::NewGame()
{
    if (m_Generated)
        this->Reset();

    m_Map = new CMap(mapHeight, mapWidth);
    m_GameState = new CGameState(user, m_Map->GetGates(), winCondition, loseCondition);

    if (!this->CheckScreenSize())
    {
        wclear(m_WinContext);
        this->ErrorHandle("Window screen too small to initialize this game");
        this->Reset();
        return;
    }

    m_Generated = true;

    this->StartGame();
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void CGameEngine::LoadGame()
{
    if (m_Generated)
        this->Reset();

    std::string file = ChooseFile();
    if (file.empty())
        return;

    wclear(m_WinContext);
    std::ifstream readFile(file);

    if (readFile.fail())
    {
        wclear(m_WinContext);
        this->ErrorHandle("Failed to open file");
        return;
    }

    if (!LoadGame(readFile))
    {
        this->ErrorHandle("Corrupted or incorrect format of file");
        return;
    }

    if (!this->CheckScreenSize())
    {
        this->ErrorHandle("Window screen too small to initialize this game");
        this->Reset();
        return;
    }

    m_Generated = true;
    StartGame();
}

//------------------------------------------------------------------------------------------

std::string CGameEngine::ChooseFile()
{
    wclear(m_WinContext);

    int heightSize = 0;
    int widthSize = 0;
    getmaxyx(m_WinContext, heightSize, widthSize);
    int displayY = heightSize / 2;
    int displayX = widthSize / 2;

    int xOffset = -2;
    mvwaddstr(m_WinContext, displayY + xOffset++, displayX - sizeof("press a number to choose a file") / 2, "press a number to choose a file");

    int it = 1;
    for (const auto &files : loadFile)
    {
        DisplayNumber(m_WinContext, displayY + xOffset, displayX - (sizeof(files) / 2) - TEXT_DISTANCE, it++ );
        mvwaddstr(m_WinContext, displayY + xOffset++, displayX - sizeof(files) / 2, files.c_str());
    }

    DisplayNumber(m_WinContext, displayY + xOffset, displayX - (sizeof(saveFile) / 2) - TEXT_DISTANCE, it++ );
    mvwaddstr(m_WinContext, displayY + xOffset++, displayX - sizeof(saveFile) / 2, saveFile.c_str());

    refresh();

    while (true)
    {
        switch (getch())
        {
        case '1':
            return loadFile[0];
        case '2':
            return loadFile[1];
        case '3':
            return loadFile[2];
        case '4':
            return saveFile;
        case 'q':
            return std::string();
        default:
            break;
        }
    }
}

//------------------------------------------------------------------------------------------

bool CGameEngine::LoadGame(std::ifstream &readFile)
{
    try
    {
        m_Map = new CMap(readFile);
        m_GameState = new CGameState(readFile, m_Map->GetGates());
    }
    catch (const CorruptedFile &)
    {
        this->Reset();
        return false;
    }

    return true;
}

//******************************************************************************************
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

void CGameEngine::StartGame()
{
    wclear(m_WinContext);

    if (!m_Generated)
    {
        this->ErrorHandle("Map not Generated");
        return;
    }

    //display the edges
    m_Map->DisplayBorder(m_WinContext, TEXT_DISTANCE);

    //display gamestate box
    m_GameState->DisplayGameState(m_WinContext, TEXT_DISTANCE);

    //display actual map
    m_Map->DisplayMap(m_WinContext, TEXT_DISTANCE);

    //starts game for real now
    this->GameCycle();

    wclear(m_WinContext);
}

//------------------------------------------------------------------------------------------

void CGameEngine::GameCycle()
{
    while (true)
    {
        //changes m_CurrentState
        this->HandleGameInput();

        if (m_CurrentState == QUIT)
            break;

        //---------------------------------------------------------------

        this->StartRound();

        //checks for end of game
        if (m_GameState->isWon())
        {
            this->EndGame("You won!");
            return;
        }
        else if (m_GameState->isLost())
        {
            this->EndGame("You Lost.");
            return;
        }

        //---------------------------------------------------------------
        //only display after every piece moved
        m_GameState->IncreaseTurns();
        m_Map->DisplayMap(m_WinContext, TEXT_DISTANCE);
        m_GameState->DisplayGameState(m_WinContext, TEXT_DISTANCE);
        std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));
    }
}

//------------------------------------------------------------------------------------------

void CGameEngine::HandleGameInput()
{
    auto input = getch();
    switch (input)
    {
    case '\n':
        m_GameState->InputNewFigure();
        m_Map->DisplayMap(m_WinContext, TEXT_DISTANCE);
        break;
    //-------------------------------------------------------------------------
    case INPUT_KEY_UP:
    case INPUT_KEY_DOWN:
    case INPUT_KEY_RIGHT:
    case INPUT_KEY_LEFT:
        m_GameState->ChangeChoice(input);
        break;
    //-------------------------------------------------------------------------
    case 'p':
        StopGame();
        break;
    case 'q':
        m_CurrentState = QUIT;
        break;
    default:
        break;
    }
}

//------------------------------------------------------------------------------------------

void CGameEngine::StartRound()
{
    if (m_Map->GetGoal()->DoAction())
        m_GameState->IncreaseScore(1);

    //---------------------------------------------------------------

    std::vector<CBasicPawn *> attackers = m_Map->GetAttackers();

    for (auto att : attackers)
    {
        att->DoAction();

        //reset nodes for next pawns that need to search a path
        m_Map->ResetNodes();
    }
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));

    //---------------------------------------------------------------

    std::vector<CTower *> defenders = m_Map->GetTowers();
    for (auto &def : defenders)
    {
        def->DoAction();
    }
}

//------------------------------------------------------------------------------------------

void CGameEngine::ErrorHandle(const std::string &message) const
{
    mvwaddstr(m_WinContext, 0, 0, message.c_str());
    mvwaddstr(m_WinContext, 1, 0, "press 'q' to go back");
    refresh();
    while (getch() != 'q')
        ;

    RemoveText(m_WinContext, 0, 0, message.length());
    RemoveText(m_WinContext, 1, 0, sizeof("press 'q' to go back"));
}

//------------------------------------------------------------------------------------------

void CGameEngine::StopGame()
{
    std::string pauseText = "GAME PAUSED";
    std::string saveText = "press 's' to Save the Game";
    std::string continueText = "press 'c' to continue";

    //-------------------------------------------------------

    std::pair<int, int> stopTextPos(m_Map->GetSize().first + TEXT_DISTANCE + 3, TEXT_DISTANCE + m_Map->GetSize().second / 2);

    mvwaddstr(m_WinContext, stopTextPos.first, stopTextPos.second - pauseText.size() / 2, pauseText.c_str());
    mvwaddstr(m_WinContext, stopTextPos.first + 1, stopTextPos.second - saveText.size() / 2, saveText.c_str());
    mvwaddstr(m_WinContext, stopTextPos.first + 2, stopTextPos.second - continueText.size() / 2, continueText.c_str());
    refresh();

    //-------------------------------------------------------

    bool exitFlag = false;
    while (true)
    {
        switch (getch())
        {
        case 'q':
            m_CurrentState = QUIT;
            exitFlag = true;
            break;
        case 'c':
            exitFlag = true;
            break;
        case 's':
            this->SaveGame();
            break;
        }

        if (exitFlag)
            break;
    }

    //-------------------------------------------------------

    RemoveText(m_WinContext, stopTextPos.first, stopTextPos.second - pauseText.size() / 2, pauseText.length());
    RemoveText(m_WinContext, stopTextPos.first + 1, stopTextPos.second - saveText.size() / 2, saveText.length());
    RemoveText(m_WinContext, stopTextPos.first + 2, stopTextPos.second - continueText.size() / 2, continueText.length());
}

//------------------------------------------------------------------------------------------

void CGameEngine::SaveGame()
{
    std::string destination(saveFile);

    //check if can write/rewrite into destination file
    if (!ApproveSaving(destination))
    {
        this->ErrorHandle("Cant open save file");
        return;
    }

    std::ofstream out(destination, std::ios::out);

    if ((out << *this).fail())
    {
        this->ErrorHandle("Error while Saving game");
        return;
    }
}

//------------------------------------------------------------------------------------------

bool CGameEngine::ApproveSaving(const std::string &destination) const
{
    std::ifstream in(destination, std::ios::in);

    if (in.fail())
    {
        return false;
    }

    //1st char in file is EOF - blank file - ok for writing into
    if (in.peek() == std::ifstream::traits_type::eof())
        return true;

    mvwaddstr(m_WinContext, 0, 0, "File not empty, do you want to overwrite?");
    mvwaddstr(m_WinContext, 1, 0, "Press y/n");
    refresh();

    bool ret = false;
    bool exitFlag = false;

    while (true)
    {
        switch (getch())
        {
        case 'y':
            ret = true;
            //no break - leaving while cycle
        case 'n':
        case 'q':
            exitFlag = true;
            break;
        
        default:
            break;
        }
        if (exitFlag)
            break;
    }

    RemoveText(m_WinContext, 0, 0, sizeof("File not empty, do you want to overwrite?"));
    RemoveText(m_WinContext, 1, 0, sizeof("Press y/n"));
    refresh();

    return ret;
}

//------------------------------------------------------------------------------------------

std::ostream &operator<<(std::ostream &out, const CGameEngine &gameEngine)
{
    if (gameEngine.m_Generated)
    {
        out << *(gameEngine.m_Map) << std::endl;

        out << *(gameEngine.m_GameState);
    }

    return out;
}

//------------------------------------------------------------------------------------------

void CGameEngine::EndGame(const std::string &endText)
{
    m_Map->DisplayMap(m_WinContext, TEXT_DISTANCE);
    m_GameState->DisplayGameState(m_WinContext, TEXT_DISTANCE);
    this->ErrorHandle(endText);
}
