#include "../hpp/towerAttack.h"

#include <curses.h>
#include <string>

int main(int argc, char *argv[])
{
    if (argc == 2 && std::string(argv[1]) == "test_screen")
    {
        WINDOW *myWin = InitScreen();
        EndScreen(myWin);
    }
    else
    {
        WINDOW *myWin;
        if ((myWin = InitScreen()) == nullptr)
            return 1;

        InitGame(myWin);

        EndScreen(myWin);
    }
    return 0;
}