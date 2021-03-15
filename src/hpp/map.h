#pragma once

#include "nodes.h"
#include "basic.h"
#include "all_static.h"

//---------------------------------------------------------------------------------------------

#include <curses.h>
#include <vector>
#include <set>
#include <fstream>

//---------------------------------------------------------------------------------------------

const std::string mapStartDelim = "map start";
const std::string mapEndDelim = "map end";

const std::string objectsStartDelim = "objects start";
const std::string objectsEndDelim = "objects end";

const int amountWalls = 15;
const int amountTowers = 5;

//---------------------------------------------------------------------------------------------

class CMap
{
public:
    /**
     * generates a new map
     * internally generates positions of towers and walls
     * generates position and amounts of gates
     * generates goal post
     * */
    void GenerateNewMap();

    /**
     * generates a new map from file
     * 
     * @param [in] readFile file to read from, has information about the map that will be generated
     * 
     * @return true if map is correct and fits on map, false if incorrect information in file
     * */
    bool GenerateNewMap(std::ifstream &readFile);
    //-----------------------------------------------------------------------------------

    /**
     * prints all existing objects in map (walls, attackers, towers, gates, goal post...)
     * 
     * @param [in] winContext window where objects will be printed
     * @param [in] offset offset from top and left side for printing
     * */
    void DisplayMap(WINDOW *winContext, const int offset) const;

    /**
     * displays border of map
     * 
     * @param [in] winContext window where objects will be printed
     * @param [in] offset offset from top and left side for printing
     * */
    void DisplayBorder(WINDOW *winContext, const int offset) const;

    //-----------------------------------------------------------------------------------

    /**
     * use after each call when a pawn was moved to reset nodes' state
     * 
     * reset all nodes to state::fresh
     * set all nodes back parent as nullptr (for backtracking)
     * */
    void ResetNodes();

    /**
     * deletes all objects on map but doesnt destroy nodes of map
     * */
    void ResetObjects();
    //-----------------------------------------------------------------------------------
    /**
     * @return std::vector of all attackers that exist on the map
     * */
    std::vector<CBasicPawn *> GetAttackers() const;

    /**
     * @return std::vector of all towers that exist on the map
     * */
    std::vector<CTower *> GetTowers() const;

    /**
     * @return pointer to goal object
     * */
    CGoal *GetGoal() const;

    /**
     * @return std::vector of all gates that exist on the map
     * */
    std::vector<CGate *> GetGates() const;

    /**
     * returns size of current map where .first is height .second is width
     * 
     * @return height and width of map
     * */
    std::pair<int, int> GetSize() const;

private:
    /**
     * checks for state of map and its problems
     * 
     * @return false if there exists a problem with its gate or goal, true if everything is OK
     * */
    bool IsLegalMap() const;

    /**
     * create nodes that represents the grid of map
     * 
     * DTOR responsible for freeing up space of these nodes
     * */
    void CreateNodes();

    /**
     * generates position of goal and gates on map
     * connects them to node
     * */
    void CreateGates();

    /**
     * generates random walls on the map
     * connects them to node
     * */
    void GenerateWalls();

    /**
     * generates random types of towers on map and stores them separately for faster access
     * connects them to node
     * */
    void GenerateTowers();

    //---------------------------------------------------------------------
    //---------------------------------------------------------------------

    int m_Height;     //< height of map
    int m_Width;      //< width of map
    int m_Difficulty; //< difficulty of map

    //---------------------------------------------------------------------

    CNode ***m_Nodes;

    CNode *m_GoalPost;
    std::vector<CNode *> m_GatesPos;
    std::vector<CNode *> m_Towers;

public:
    /**
     * CTOR
     * creates a map of size that is given as params
     * generates objects randomly
     * 
     * @param [in] height height of map
     * @param [in] width wdith of map
     * @param [in] difficulty difficulty of map
     * 
     * @throw InvalidMapSizeFromInput thrown when mapsize is negative or 0
     * */
    CMap(int height, int width, int difficulty = 1);

    /**
     * CTOR
     * creates a map from information available from stream
     * 
     * @param [in] readFile stream where information is extracted from
     * 
     * @throw CorruptedFile thrown when reading was not succesfull or information is incorrect
     * */
    CMap(std::ifstream &readFile);

    /**
     * DTOR
     * deletes all nodes and objects conencted to them
     * */
    ~CMap();
    //---------------------------------------------------------------------

    friend std::ostream &operator<<(std::ostream &out, const CMap &map);
};
