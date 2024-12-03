/*
** EPITECH PROJECT, 2024
** RTYPE
** File description:
** Window
*/

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <raylib.h>

class Window {
    public:
        Window(int width, int height, const char *title);
        ~Window();
        
        void InitClientWindow(int width, int height, const char *title);

        void loop();

    private:
        int _width;
        int _height;
        const char *_title;

        bool _exitWindowRequested;   // Flag to request window to exit
        bool _exitWindow;            // Flag to set window to exit
};



#endif /* !WINDOW_HPP_ */
