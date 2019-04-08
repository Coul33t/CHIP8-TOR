#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <utility>

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

class Screen {
    public:
        Screen();
        virtual ~Screen();
    protected:
    private:
        bool init_SDL(void);

        std::pair<unsigned short, unsigned short> m_size;
        SDL_Window* p_window;
};

#endif // SCREEN_H
