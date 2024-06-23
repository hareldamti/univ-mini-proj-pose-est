#pragma once
#include "Render.h"
#include "Terrain.h"

enum ProgramState { traversing, picking, reviewing };

class Program {
    private:
        State& state;
        Render terrainRenderer, linesRenderer, pointsRenderer;
        Terrain terrain;
        Camera obsvCamera, hoverCamera;
        float obsvVel = 0.05f, obsvAngVel = 0.01f;
        ProgramState programState;

        void init();
        void moveByInput();
        void pickByInput();
        void switchStateByInput();

        // state variables
        std::vector<cv::Point2f> pickingClicks;
        std::vector<cv::Point3f> pickingPoints;
        

    public:
        Program(State& state);
        void wrapInit();
        void update();
        void draw();
};