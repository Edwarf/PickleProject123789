// PickleProject.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <SFML/Graphics.hpp>
#include"LoopDefs.h"
int main()
{

	gameLoop loop;
	loop.Initialize();
	loop.run();
    return 0;
}


