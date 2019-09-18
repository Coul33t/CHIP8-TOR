#include "Screen.h"

Screen::Screen() {
    m_size = std::pair<unsigned short, unsigned short>(64, 32);
    p_window = nullptr;
    p_renderer = nullptr;
    upscale_value = 100;
    init_SDL();
}

Screen::~Screen() {
    SDL_DestroyWindow(this->p_window);
    SDL_DestroyRenderer(this->p_renderer);
    SDL_Quit();
}

bool Screen::init_SDL() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cout << "ERROR: can't initialise SDL (" << SDL_GetError() << ")" << std::endl;
        return false;
    }

    p_window = SDL_CreateWindow("CHIP8 EMULATOR", SDL_WINDOWPOS_UNDEFINED,
                                                  SDL_WINDOWPOS_UNDEFINED,
                                                  m_size.first * 8,
                                                  m_size.second * 8,
                                                  SDL_WINDOW_SHOWN);

    if (!p_window) {
        std::cout << "ERROR: can't create window (" << SDL_GetError() << ")" << std::endl;
        return false;
    }

    p_renderer = SDL_CreateRenderer(this->p_window, -1, SDL_RENDERER_ACCELERATED);

    if (!p_renderer) {
        std::cout << "ERROR: can't create renderer (" << SDL_GetError() << ")" << std::endl;
        return false;
    }

    return true;
}

void Screen::test_display(void) {
    uint8_t array[80] = {
      0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
      0x20, 0x60, 0x20, 0x20, 0x70, // 1
      0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
      0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
      0x90, 0x90, 0xF0, 0x10, 0x10, // 4
      0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
      0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
      0xF0, 0x10, 0x20, 0x40, 0x40, // 7
      0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
      0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
      0xF0, 0x90, 0xF0, 0x90, 0x90, // A
      0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
      0xF0, 0x80, 0x80, 0x80, 0xF0, // C
      0xE0, 0x90, 0x90, 0x90, 0xE0, // D
      0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
      0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

    // Setting the background colour
    SDL_SetRenderDrawColor(this->p_renderer, 0, 0, 0, 255);
    SDL_RenderClear(this->p_renderer);

    unsigned int y = 0;
    std::vector<uint8_t> line_to_draw;

    SDL_SetRenderDrawColor(this->p_renderer,255, 255, 255, 255);
    for (int i=0; i<80/5; i += 5) {
        //std::cout << "x: " << i%5 << std::endl;
        //std::cout << "y: " << (int)(i/5) << std::endl;
        line_to_draw.clear();

        for (int j=i, j<i+5; j++)
            line_to_draw.push_back(array[j]);
        draw_hexa_line(i, y, line_to_draw);

        if (i% == 0)
            y += 5;
    }

    SDL_RenderPresent(this->p_renderer);
    std::cin.get();
}

void Screen::render_frame(uint8_t* to_render) {

}

    for (unsigned int i = 0; i < codes.size(); i++) {
        //std::cout << "Current code: " << std::hex << static_cast<int>(code) << std::endl;
        int shifted = codes[i] >> 4;
        //std::cout << "Masking result: " << std::hex << static_cast<int>(code) << std::endl;
        SDL_Rect rect;
        rect = {, y, 1, 1}; // TODO here
        y++;
    }


}
