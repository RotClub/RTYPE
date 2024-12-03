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
    initClientWindow(width, height, title);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    _exitWindowRequested = false;
    _exitWindow = false;
}

Window::Window(): _width(1920), _height(1080), _title("R-TYPE")
{
    initClientWindow(_width, _height, _title);
    SetTargetFPS(60);
    SetExitKey(KEY_NULL);
    _exitWindowRequested = false;
    _exitWindow = false;
}


Window::~Window()
{
}

void Window::initClientWindow(int width, int height, const char *title)
{
    InitWindow(width, height, title);
    SetTargetFPS(60);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    loop();
}


void Window::resizeClientWindow(int width, int height)
{
    SetWindowSize(width, height);
    _width = width;
    _height = height;
}

void Window::handleResize()
{
    if (IsWindowResized()) {
        _width = GetScreenWidth();
        _height = GetScreenHeight();
    }
}

void Window::loop()
{
    while (!_exitWindow) {
        if (WindowShouldClose() || IsKeyPressed(KEY_ESCAPE)) 
            _exitWindowRequested = true;
        if (_exitWindowRequested)
            _exitWindow = true;
        handleResize();
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText(TextFormat("Window size: %dx%d", GetScreenWidth(), GetScreenHeight()), 10, 10, GetScreenHeight() / 100, DARKGRAY);
        EndDrawing();
    }
    CloseWindow();
}