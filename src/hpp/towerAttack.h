#pragma once

#include <curses.h>

//-----------------------------------------------------------------------------------------

const int MIN_WIDTH = 120;
const int MIN_HEIGHT = 30;

//-----------------------------------------------------------------------------------------

/**
 * intializes ncurses screen and checks screen size
 * sets input reading mode and cursor setting
 * the user is responsible for ending the screen
 * 
 * @return nullptr if terminal screen too small (failure), a window context on success
 * */
WINDOW * InitScreen();
//-----------------------------------------------------------------------------------------

/**
 * closes window
 * 
 * goes through all necessary procedures to close ncurses window
 * 
 * @param [in] winContext window that gets closed
 * */
void EndScreen(WINDOW *winContext);
//-----------------------------------------------------------------------------------------

/**
 * creates a game engine for tower attack and starts the engine
 * when the game ends, deletes engine
 * 
 * @param [in] winContext window context where game will be displayed on
 * */
void InitGame(WINDOW *winContext);
//-----------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------

/**
 * removes all characters of len amount from given position on given window
 * an extension of ncurses 
 * 
 * @param [in] winContext window context where text will be removed
 * @param [in] yPosStart y-axis position of 1st character that should be removed
 * @param [in] xPosStart x-axis position of 1st character that should be removed
 * @param [in] amount - amount of characters that should be removed
 * */
void RemoveText(WINDOW *winContext, int yPosStart, int xPosStart, size_t amount);

/**
 * displays number on given position
 * 
 * 
 * @param [in] winContext window context where text will be removed
 * @param [in] yPosStart y-axis position of given number
 * @param [in] xPosStart x-axis position of given number
 * @param [in] number - number that should be displayed
 * */
void DisplayNumber(WINDOW *winContext, int yPosStart, int xPosStart, int number);