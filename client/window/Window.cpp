/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Window
*/

#include "Window.hpp"
#include <raylib.h>


Window::Window(int width, int height, const char *title) : _width(width), _height(height), _title(title)
{
    InitClientWindow(width, height, title);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    _exitWindowRequested = false;
    _exitWindow = false;
}

Window::~Window()
{
}

void Window::InitClientWindow(int width, int height, const char *title)
{
    InitWindow(width, height, title);
    SetTargetFPS(60);
}


void Window::loop()
{
    while (!_exitWindow) {
        if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) 
            _exitWindowRequested = true;
        if (_exitWindowRequested)
            _exitWindow = true;
        BeginDrawing();
           ClearBackground(RAYWHITE);
        EndDrawing();
    }
    CloseWindow();
}