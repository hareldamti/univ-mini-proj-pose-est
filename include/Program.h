#pragma once
#include "State.h"

class Program {
    private:
        State m_state;
        // global variables

    public:
        Program(State state);
        void init();
        void update();
        void draw();
};