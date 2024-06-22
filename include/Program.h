#pragma once
#include "Render.h"

class Program {
    private:
        State& m_state;
        Render m_render, m_tinted;
        // global variables

    public:
        Program(State& state);
        void init();
        void update();
        void draw();
};