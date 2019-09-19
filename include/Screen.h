#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <utility>
#include <vector>
#include <windows.h> // Sleep
#include <array>

#include <SDL2/SDL.h>

class Screen {
    public:
        Screen(int delay=0);
        virtual ~Screen();
        void test_display(void);
        void render_frame(std::array<uint8_t, 64 * 32>&);

    protected:
    private:
        bool init_SDL(void);
        std::pair<unsigned short, unsigned short> m_size;
        SDL_Window* p_window;
        SDL_Renderer* p_renderer;
        int upscale_value;
        int delay_display_ms;

        void draw_test_hexa_line(unsigned int, unsigned int, std::vector<uint8_t>&);

};

#endif // SCREEN_H
