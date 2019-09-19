#include "Screen.h"

Screen::Screen(int delay) {
    m_size = std::pair<unsigned short, unsigned short>(64, 32);
    p_window = nullptr;
    p_renderer = nullptr;
    upscale_value = 10;
    delay_display_ms = delay;
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
                                                  m_size.first * upscale_value,
                                                  m_size.second * upscale_value,
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

void Screen::render_frame(std::array<uint8_t, 64 * 32>& to_render) {

    SDL_Rect rect;

    uint8_t test_value = 0x80;
    int result = test_value & 0x80;
    if (result) {
        std::cout << "It worked" << std::endl;
    }

    if (test_value & 0x80) {
        std::cout << "It worked too" << std::endl;
    }

    test_value = to_render[0];
    result = test_value & 0x80;


    for (int i = 0; i < m_size.second; i++) {
        for (int j = 0; j < m_size.first; j++) {
            //std::cout << std::hex << static_cast<int>(to_render[i*j + j]) << std::endl;

            int x = j * upscale_value;
            int y = i * upscale_value;

            if (to_render[i*m_size.first + j] != 0x00) {
                std::cout << i << " " << j << " " << i*j + i << std::endl;
                std::cout << std::hex << static_cast<int>(to_render[i*m_size.first + j]) << std::dec << " " << static_cast<int>(to_render[i*m_size.first + j]) << std::endl;
                std::cout << "Yo" << std::endl;
            }

            // Bug here: never works (even when byte & 0x80 == 0x80), and can't access memory after
            if (to_render[i*m_size.first + j] & 0x80) {
               rect = {x * upscale_value, y * upscale_value, this->upscale_value, this->upscale_value};
               SDL_RenderFillRect(this->p_renderer, &rect);
               std::cout << x * upscale_value << " " << y * upscale_value << std::endl;
            }

            if (to_render[i*m_size.first + j] & 0x40) {
               rect = {x + this->upscale_value, y * upscale_value, this->upscale_value, this->upscale_value};
               SDL_RenderFillRect(this->p_renderer, &rect);
            }

            if (to_render[i*m_size.first + j] & 0x20) {
               rect = {x + 2 * this->upscale_value, y * upscale_value, this->upscale_value, this->upscale_value};
               SDL_RenderFillRect(this->p_renderer, &rect);
            }

            if (to_render[i*m_size.first + j] & 0x10) {
               rect = {x + 3 * this->upscale_value, y * upscale_value, this->upscale_value, this->upscale_value};
               SDL_RenderFillRect(this->p_renderer, &rect);
            }

            if (to_render[i*m_size.first + j] & 0x08) {
               rect = {x + 4 * this->upscale_value, y * upscale_value, this->upscale_value, this->upscale_value};
               SDL_RenderFillRect(this->p_renderer, &rect);
            }

            if (to_render[i*m_size.first + j] & 0x04) {
               rect = {x + 5 * this->upscale_value, y * upscale_value, this->upscale_value, this->upscale_value};
               SDL_RenderFillRect(this->p_renderer, &rect);
            }

            if (to_render[i*m_size.first + j] & 0x02) {
               rect = {x + 6 * this->upscale_value, y * upscale_value, this->upscale_value, this->upscale_value};
               SDL_RenderFillRect(this->p_renderer, &rect);
            }

            if (to_render[i*m_size.first + j] & 0x01) {
               rect = {x + 7 * this->upscale_value, y * upscale_value, this->upscale_value, this->upscale_value};
               SDL_RenderFillRect(this->p_renderer, &rect);
            }

            if (delay_display_ms > 0) {
                SDL_RenderPresent(this->p_renderer);
                Sleep(delay_display_ms);
            }

        }
    }

    SDL_RenderPresent(this->p_renderer);
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
    unsigned int x = 0;
    std::vector<uint8_t> line_to_draw;

    SDL_SetRenderDrawColor(this->p_renderer,255, 255, 255, 255);

    for (int i=0; i<80; i += 5) {
        /*std::cout << "i: " << i << std::endl;
        std::cout << "x: " << x << std::endl;
        std::cout << "y: " << y << std::endl;*/
        line_to_draw.clear();

        for (int j=i; j<i+5; j++)
            line_to_draw.push_back(array[j]);

        draw_test_hexa_line(x*this->upscale_value, y*this->upscale_value, line_to_draw);

        x += 5;
        if (x%20  == 0) {
            x = 0;
            y += 6;
        }

    }

    SDL_RenderPresent(this->p_renderer);
    std::cout << "Screen is OK" << std::endl;
}

void Screen::draw_test_hexa_line(unsigned int x, unsigned int y, std::vector<uint8_t>& codes) {

    SDL_Rect rect;
    // For every lines (vertical)
    for (unsigned int i = 0; i < codes.size(); i++) {
        //std::cout << "Current code: " << std::hex << static_cast<int>(codes[i]) << std::endl;
        int shifted = codes[i] >> 4;
        //std::cout << "Masking result: " << std::hex << static_cast<int>(shifted) << std::endl;

        // For every pixel in the line
        if (shifted & 0x8) {
           rect = {x, y, this->upscale_value, this->upscale_value};
           SDL_RenderFillRect(this->p_renderer, &rect);
        }

        if (shifted & 0x4) {
           rect = {x + this->upscale_value, y, this->upscale_value, this->upscale_value};
           SDL_RenderFillRect(this->p_renderer, &rect);
        }

        if (shifted & 0x2) {
           rect = {x + 2 * this->upscale_value, y, this->upscale_value, this->upscale_value};
           SDL_RenderFillRect(this->p_renderer, &rect);
        }

        if (shifted & 0x1) {
           rect = {x + 3 * this->upscale_value, y, this->upscale_value, this->upscale_value};
           SDL_RenderFillRect(this->p_renderer, &rect);
        }

        y += this->upscale_value;

        // If there's no delay, no need to make multiple calls to the rendering function
        // nor the Sleep function
        if (delay_display_ms > 0) {
            SDL_RenderPresent(this->p_renderer);
            Sleep(delay_display_ms);
        }

    }
}
