#include "Screen.h"

Screen::Screen() {
    m_size = std::pair<unsigned short, unsigned short>(64, 32);
    p_window = nullptr;
    init_SDL();
}

Screen::~Screen() {
    delete p_window;
    SDL_Quit();
}

bool Screen::init_SDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "ERROR: can't initialise SDL (" << SDL_GetError() << ")" << std::endl;
        return false;
    }

    p_window = SDL_CreateWindow("CHIP8 EMULATOR", SDL_WINDOWPOS_UNDEFINED,
                                                  SDL_WINDOWPOS_UNDEFINED,
                                                  m_size.first * 32,
                                                  m_size.second * 32,
                                                  SDL_WINDOW_SHOWN);

    if (!p_window) {
        std::cout << "ERROR: can't create window (" << SDL_GetError() << ")" << std::endl;
        return false;
    }

    return true;
}
