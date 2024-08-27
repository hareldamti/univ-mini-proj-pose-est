#pragma once
#include "Render.h"
#include "Terrain.h"

enum ProgramState { configuring, traversing, picking, reviewing };

class Program {
    private:
        State& state;
        Render terrainRenderer, linesRenderer, pointsRenderer;
        Terrain terrain;
        Camera obsvCamera, hoverCamera;
        float obsvVel = 0.1f, obsvAngVel = 0.015f;
        ProgramState programState;

        void init();
        void moveByInput();
        void pickByInput();
        void switchStateByInput();
        void placeTrackersByInput();
        void captureByInput();
        void toggleLocationByInput();
        
        void addCameraAnimation(Camera* camera, std::vector<Camera>&& keyframeValues, std::vector<float> keyframeTimes);
        // state variables
        std::vector<cv::Point2f> pickingClicks;
        std::vector<cv::Point3f> pickingPoints;
        

    public:
        Program(State& state);
        void wrapInit();
        void update();
        void draw();
};