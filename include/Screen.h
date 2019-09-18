#ifndef SCREEN_H
#define SCREEN_H

#include <iostream>
#include <utility>
#include <vector>

#include <SDL2/SDL.h>

class Screen {
    public:
        Screen();
        virtual ~Screen();
        void test_display(void);
        void render_frame(uint8_t*);

    protected:
    private:
        bool init_SDL(void);
        std::pair<unsigned short, unsigned short> m_size;
        SDL_Window* p_window;
        SDL_Renderer* p_renderer;
        int upscale_value;

        void draw_hexa_line(unsigned int, unsigned int, uint8_t);
};

#endif // SCREEN_H
