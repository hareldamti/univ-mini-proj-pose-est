#pragma once
#include "Render.h"
#include "Terrain.h"
class Program {
    private:
        State& m_state;
        Render m_terrain_renderer, m_lines_renderer, m_points_renderer;
        Terrain m_terrain;
        // global variables
        glm::vec4 cameraPos;
        glm::mat4 cameraRot;
    public:
        Program(State& state);
        void init();
        void update();
        void draw();
};