#pragma once

/**
 * possible states gameEngine can be in
 * */
enum MENU_STATES
{
    NEW_GAME,
    CONTINUE,
    CREDIT,
    LAST,
    QUIT,
    ERROR
};

/**
 * modular addition
 * mod MENU_STATES::LAST
 * */
MENU_STATES operator+(MENU_STATES a, int b);

/**
 * modular subtraction
 * mod MENU_STATES::LAST
 * */
MENU_STATES operator-(MENU_STATES a, int b);

//------------------------------------------------------------------------------------

#define INPUT_KEY_UP 'w'
#define INPUT_KEY_DOWN 's'
#define INPUT_KEY_LEFT 'a'
#define INPUT_KEY_RIGHT 'd'

/*
#define INPUT_KEY_UP 72
#define INPUT_KEY_DOWN 80
#define INPUT_KEY_LEFT 75
#define INPUT_KEY_RIGHT 77
*/
