#pragma once

#include "map.h"
#include "gameState.h"
#include "input.h"

//------------------------------------------------------------------------------------

#include <curses.h>
#include <vector>

//------------------------------------------------------------------------------------

const int mapHeight = 7;
const int mapWidth = 49;
const int winCondition = 10;
const unsigned int loseCondition = 200;

//duration of sleep time in milliseconds after each cycle
const unsigned int sleepTime = 100;

const std::string user("nguyexu2");

/**
 * if you decide to add another file into loadFile, make sure to also change input reading choices in CGameEngine::ChooseFile
 * */
const std::vector<std::string> loadFile({"./examples/game0.txt", "./examples/game1.txt", "./examples/game2.txt"});
const std::string saveFile("./examples/save1.txt");

//------------------------------------------------------------------------------------

class CGameEngine
{
public:
    /**
     * starts engine and prompts the user for action in menu screen
     * starts a game according to state or displays credit
     * 
     * ending the engine doesnt free allocated memory
     * */
    void StartEngine();

    /**
     * resets engine to starting state, frees up ALL allocated memory
     * 
     * frees map and gamestate
     * resets m_CurrentState
     * */
    void Reset();

private:
    //-----------------------------------------------------------------------------------------------
    /**
     * displays the menu and and prompts the user for further action
     * changes m_CurrentState of the engine
     * */
    void DisplayMenu();

    /**
     * displays which option the user is currently selecting
     * 
     * @param [in] possiblePos .first is the x-position, .second are possible y-position
     *      inputted .second has to be sorted
     * */
    void DisplayMenuChoice(const std::pair<int, const std::vector<int>> & possiblePos) const;

    /**
     * reads input from user and changes state
     * displays which option the user is currently selecting
     * 
     * @param [in] .second are possible y-position, inputted .second has to be sorted
     *      possiblePos .second is the x-position
     *      used to know where to display choice
     * */
    void HandleMenuInput(const std::pair<std::vector<int>, int> possiblePos);
    //-----------------------------------------------------------------------------------------------
    /**
     * displays information about the game until 'q' is pressed
     * */
    void DisplayCredit() const;
    //-----------------------------------------------------------------------------------------------
    /**
     * @return true if game fits on screen, false if doesnt fit
     * */
    bool CheckScreenSize() const;

    /**
     * displays error message in top left corner until user doesnt press 'q'
     * 
     * @param [in] message message of error to be displayed
     * */
    void ErrorHandle(const std::string &message) const;
    //-----------------------------------------------------------------------------------------------

    /**
     * starts a new game
     * 
     * size of map is always the same, layout of map is generated randomly
     * 
     * fails to start if screen size is insufficient to display the game
     * */
    void NewGame();
    //-----------------------------------------------------------------------------------------------

    /**
     * selects a file to load up a game
     * loads the game and starts it
     * 
     * fails to start if game file reading wasnt succesful until the end
     * fails to start if screen if screen size if insufficient to display the game
     * */
    void LoadGame();

    /**
     * prompts the user to choose a file to load from given list
     * 
     * 
     * the list is defined as saveFile from this file
     * @return name of file to load from
     * */
    std::string ChooseFile();

    /**
     * creates instance of map from file
     * creates instance of gamestate from the rest of the file
     * if failure occures, deletes all allocated memory during this method
     * 
     * @param [in] readFile stream of file to read from and initialize map and gamestate
     * @return true if reading went completly and without problem, true if corrupted file (incorrect format, missing elements...)
     * */
    bool LoadGame(std::ifstream &readFile);
    //-----------------------------------------------------------------------------------------------

    /**
     * displays map and starts game cycle
     * */
    void StartGame();

    /**
     * handles cycles of game
     * 
     * checks for new inputs and starts action of every piece on map
     * prepares for next round
     * */
    void GameCycle();

    /**
     * checks for user input and does corresponding action
     * 
     * the user can pause, change gate nad piece to summon, quit
     * 
     * changes m_CurrentState if the user decides to quit
     * */
    void HandleGameInput();

    /**
     * starts round and gets every piece on map to do an action
     * 
     * 1st does action goal
     * 2nd attackers
     * 3rd towers
     * */
    void StartRound();
    //-----------------------------------------------------------------------------------------------

    /**
     * stops game and prompts user for input
     * 
     * during stopped state, the user can save the game or quit the game
     * */
    void StopGame();

    /**
     * displays end of game
     * 
     * does not free up allocated space, reset is called after return to (Startgame =>) StartEngine
     * */
    void EndGame(const std::string & endText);
    //-----------------------------------------------------------------------------------------------

    /**
     * chooses file and and saves all information
     * 
     * open ups file of choosing, checks for writing permission and writes into the file
     * fails to save game if file does not exist or writing interrupted midway
     * */
    void SaveGame();

    /**
     * checks for given file if it exists
     * prompts the user for rewriting if file is not empty
     * 
     * @param [in] destination name of file to save into
     * */
    bool ApproveSaving(const std::string &destination) const;

    //-----------------------------------------------------------------------------------------------

    WINDOW *m_WinContext;

    MENU_STATES m_CurrentState;

    bool m_Generated;
    CMap *m_Map;
    CGameState *m_GameState;

    const static int TEXT_DISTANCE = 3;

public:
    /**
     * CTOR
     *  
     * @param [in] winContext window context where the whole game will take place on
     * */
    CGameEngine(WINDOW *winContext);

    /**
     * 1st saves map information
     * 2nd saves gamestate information
     * */
    friend std::ostream &operator<<(std::ostream &out, const CGameEngine &gameEngine);

    /**
     * CTOR
     * 
     * cleans up engine, frees up map and gamestate
     * */
    ~CGameEngine();
};