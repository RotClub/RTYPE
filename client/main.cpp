/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** CLIENT main
*/

#include "raylib.h"
#include "./window/Window.hpp"
#include <iostream>

void initLittleWindow(void)
{
	int width = 800;
	int height = 450;
	const char *title = "R-TYPE";
	
	Window window(width, height, title);
	
	window.loop();
}

int main(void) {
	return 0;
}
