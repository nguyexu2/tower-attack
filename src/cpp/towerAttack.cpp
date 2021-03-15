#include "../hpp/towerAttack.h"
#include "../hpp/gameEngine.h"
//--------------------------------------------------------------------------------

#include <iostream>
#include <curses.h>
#include <sstream>
#include <iomanip>

//--------------------------------------------------------------------------------

WINDOW *InitScreen()
{
    WINDOW *ret;

    if ((ret = initscr()) == nullptr)
    {
        std::cerr << "Error initialising ncurses." << std::endl;
        return nullptr;
    }

    int heightSize = 0;
    int widthSize = 0;
    //reads size of map
    getmaxyx(ret, heightSize, widthSize);

    if (heightSize < MIN_HEIGHT || widthSize < MIN_WIDTH)
    {
        EndScreen(ret);
        std::cerr << "Window too small." << std::endl;
        std::cerr << "This window height: " << heightSize << " | Required: " << MIN_HEIGHT << std::endl;
        std::cerr << "This window width: " << widthSize << " | Required: " << MIN_WIDTH << std::endl;
        return nullptr;
    }
    //------------------------------------------------------------------
    /**
     * set no echo and turn off cursor
     */
    noecho();
    curs_set(0);
    
    /**
     * allow to use asynchronous input
     * if no input is given, getch returns nothing
     * */
    nodelay(stdscr, TRUE);

    return ret;
}

//--------------------------------------------------------------------------------

void EndScreen(WINDOW *winContext)
{
    wclear(winContext);
    delwin(winContext);
    endwin();
    refresh();
}

//--------------------------------------------------------------------------------

void InitGame(WINDOW *winContext)
{
    //creates game engine
    auto game = new CGameEngine(winContext);
    
    //starts game engine
    game->StartEngine();

    //ends game engine and deletes engine
    delete game;
}

//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------

void RemoveText(WINDOW *winContext, int yPosStart, int xPosStart, size_t amount)
{
    std::stringstream s;
    s << std::setw(amount - 1) << ' ';
    std::string remove = s.str();

    mvwaddstr(winContext, yPosStart, xPosStart, remove.c_str());
    refresh();
}

//--------------------------------------------------------------------------------

void DisplayNumber(WINDOW *winContext, int yPosStart, int xPosStart, int number)
{
    std::stringstream s;
    s << number;
    std::string printer;
    s >> printer;

    mvwaddstr(winContext, yPosStart, xPosStart, printer.c_str());
    refresh();
}