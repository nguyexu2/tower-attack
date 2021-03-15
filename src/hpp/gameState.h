#pragma once

#include "nodes.h"
#include "gate.h"
#include "input.h"

//---------------------------------------------------------

#include <curses.h>
#include <string>
#include <vector>
#include <fstream>

//---------------------------------------------------------

const std::string stateStartDelim = "gameState start";
const std::string stateEndDelim = "gameState end";

//---------------------------------------------------------

class CGameState
{
public:
    /**
     * displays all information of game state onto terminal
     * is displayed on the left of the map
     * 
     * needs at least mapwidth + offset + distanceFromMap + boxwidth size from terminal width
     * 
     * @param [in] winContext window where box will be printed
     * @param [in] offset offset from top and left side for printing
     * */
    void DisplayGameState(WINDOW *winContext, const int offset) const;
    //---------------------------------------------------------------------------------

    /**
     * depending on direction
     * changes gate number by 1, gate number change is modular (mod amount of gates)
     * or
     * change figure to be summoned
     * 
     * @param [in] direction up, right, down, left
     * */
    void ChangeChoice(const int direction);

    /**
     * summons the selected figure from selected gate
     * */
    void InputNewFigure();
    //---------------------------------------------------------------------------------

    /**
     * increases turn counter without displaying it
     * */
    void IncreaseTurns();

    /**
     * increases score counter by given amount without displaying it
     * 
     * @param [in] amount increase score by given amount
     * */
    void IncreaseScore(const int amount);
    //---------------------------------------------------------------------------------

    /**
     * @return true if player has enough score, false otherwise
     * */
    bool isWon() const;

    /**
     * @return true if turn counter went over treshold, false if less than treshold
     * */
    bool isLost() const;

    /**
     * returns size of box needed for displaying game state
     * 
     * .first is height, .second is width + distance from map needed
     * 
     * @return height and width of box 
     * */
    std::pair<int, int> GetSize() const;

private:
    /**
     * displays box border of gamestate
     * 
     * @param [in] winContext window where box will be printed
     * @param [in] offset offset from top and left side for printing
     * */
    void DisplayBorder(WINDOW *winContext, const int offset) const;

    /**
     * displays turn counter
     * 
     * @param [in] winContext window where box will be printed
     * @param [in] offset offset from top and left side for printing
     * @param [in|out] row that the text was displayed on and shouldnt be overwritten
     * */
    void DisplayTurns(WINDOW *winContext, const int offset, int & row) const;

    /**
     * displays chosen gate and chosen figure to be summoned
     * 
     * @param [in] winContext window where box will be printed
     * @param [in] offset offset from top and left side for printing
     * @param [in|out] row that the text was displayed on and shouldnt be overwritten
     * */
    void DisplayChoice(WINDOW *winContext, const int offset, int & row) const;

    /**
     * displays current score
     * 
     * @param [in] winContext window where box will be printed
     * @param [in] offset offset from top and left side for printing
     * @param [in|out] row that the text was displayed on and shouldnt be overwritten
     * 
     * */
    void DisplayScore(WINDOW *winContext, const int offset,  int & row) const;

    //---------------------------------------------------------------------------------
    std::string m_Username;

    unsigned int m_Turns;
    int m_Score;
    int m_CurrentGate;

    ATTACKERTYPE m_Figure; ///< type of figure that is currently selected
    std::vector<CGate *> m_Gates;

    int m_WinCondition;
    unsigned int m_LoseCondition;

    const static int boxHeight = 15;
    const static int boxWidth = 25;
    const static int distanceFromMap = 15;

public:
    /**
     * CTOR
     * creates a gamestate from given parameters
     * 
     * @param [in] username username of player
     * @param [in] gates std::vector of gates on map
     * @param [in] winCondition how many figures have to go through gate for the player to win
     * @param [in] loseCondition how many turns have to pass by for the player to lose
     * @param [in] figure starting figure
     * @param [in] turns turn counter
     * @param [in] score starting score
     * */
    CGameState(const std::string &username,
               const std::vector<CGate *> &gates,
               int winCondition,
               unsigned int loseCondition,
               int currGate = 0,
               ATTACKERTYPE figure = BASIC,
               unsigned int turns = 0,
               int score = 0);

    /**
     * CTOR
     * create a gamestate from file
     * 
     * @param [in] readFile stream where information is extracted from
     * @param [in] gates gates that will be stored for summoning
     * 
     * @throw CorruptedFile thrown when reading was not succesfull or information is incorrect
     * */
    CGameState(std::ifstream &readFile, const std::vector<CGate *> &gates);

    friend std::ostream &operator<<(std::ostream &out, const CGameState &gameState);
};