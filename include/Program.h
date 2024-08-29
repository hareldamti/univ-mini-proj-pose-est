#pragma once
#include "Render.h"
#include "Terrain.h"
#include "Mesh.h"

enum ProgramState { configuring, traversing, picking, reviewing };

class Program {
    private:
        State& state;
        Render terrainRenderer, linesRenderer, pointsRenderer;
        Terrain terrain;
        Camera obsvCamera, compCamera, hoverCamera;
        f32 obsvVel = 0.1f, obsvAngVel = 0.015f;
        f32 hoverDistance;

        void init();
        void moveByInput();
        void pickByInput();
        void switchStateByInput();
        void placeTrackersByInput();
        void captureByInput();
        void toggleLocationByInput();

        void addCameraAnimation(Camera* camera, std::vector<Camera>&& keyframeValues, std::vector<float> keyframeTimes);
        
        // state variables
        std::vector<cv::Point3f> trackerPoints;
        std::vector<Camera> actualRoute;
        std::vector<Camera> computedRoute;
        int routeIndex;

    public:
        Program(State& state);
        ProgramState programState;
        void wrapInit();
        void update();
        void draw();
};