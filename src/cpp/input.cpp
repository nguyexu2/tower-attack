#include "../hpp/input.h"

MENU_STATES operator+(MENU_STATES a, int b)
{
    return MENU_STATES(((int)a + b) % MENU_STATES::LAST);
}

MENU_STATES operator-(MENU_STATES a, int b)
{
    return MENU_STATES(((int)a - b) < 0 ? (MENU_STATES::LAST - 1) : (int)a - b);
}
