#include "../hpp/wall.h"

CWall::CWall(CNode * myNode)
    : CObject(myNode)
{
}

CWall::CWall(CNode *myNode, std::ifstream & readFile)
    : CObject(myNode, readFile)
{
}

//----------------------------------------------------------

char CWall::GetPrintChar() const
{
    return displayChar;
}

//----------------------------------------------------------

bool CWall::DoAction()
{
    return false;
}
