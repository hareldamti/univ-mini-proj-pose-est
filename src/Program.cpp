#include "Program.h"
#include "Pose.h"

Program::Program(State& state) :
    state(state),
    terrainRenderer(state),
    linesRenderer(state),
    pointsRenderer(state),
    terrain(20, 1){}

void Program::addCameraAnimation(Camera* camera, std::vector<Camera>&& keyframeValues, std::vector<float> keyframeTimes) {
    std::vector<glm::vec4> pos;
    std::vector<glm::mat4> rot;
    for (Camera c : keyframeValues) {
        pos.push_back(c.pos);
        rot.push_back(c.rot);
    }
    state.createAnimation(&camera->pos, pos, keyframeTimes);
    state.createAnimation(&camera->rot, rot, keyframeTimes);
}

f32 lines[25];
u32 indices[30] = {
    0, 1, 2,
    0, 1, 3,
    0, 2, 4,
    0, 3, 4,
    1, 0, 2,
    1, 0, 3,
    0, 4, 2,
    4, 3, 0,
};

void Program::wrapInit() {
    // Terrain and terrain shader
    terrain.loadTexture("height.png", "shrek.png", 6);
    terrainRenderer.createProgram("shaders/default.vert", "shaders/texture.frag");
    terrainRenderer.setIndices(terrain.indices, terrain.indices_vec.size());
    terrainRenderer.setVertices(terrain.vertices, terrain.vertices_vec.size() / 5);
    terrainRenderer.setTexture(terrain.texture);

    // Lines and points shader
    linesRenderer.createProgram("shaders/default.vert", "shaders/color.frag");    
    pointsRenderer.createProgram("shaders/default.vert", "shaders/rainbow.frag");

    init();
}

void Program::moveByInput() {
    if (state.input.keyState['W']) {obsvCamera.pos += obsvVel * obsvCamera.rot * glm::vec4(0,0,1,0);}
    if (state.input.keyState['S']) {obsvCamera.pos -= obsvVel * obsvCamera.rot * glm::vec4(0,0,1,0);}
    
    if (state.input.keyState['A']) {obsvCamera.pos += obsvVel * obsvCamera.rot * glm::vec4(1,0,0,0);}
    if (state.input.keyState['D']) {obsvCamera.pos -= obsvVel * obsvCamera.rot * glm::vec4(1,0,0,0);}

    if (state.input.keyState['C']) {obsvCamera.pos += obsvVel * obsvCamera.rot * glm::vec4(0,1,0,0);}
    if (state.input.keyState['E']) {obsvCamera.pos -= obsvVel * obsvCamera.rot * glm::vec4(0,1,0,0);}

    if (state.input.keyState[VK_LEFT]) { obsvCamera.rot = obsvCamera.rot * glm::rotate(glm::mat4(1.0f), obsvAngVel, glm::vec3(0,1,0)); }
    if (state.input.keyState[VK_RIGHT]) { obsvCamera.rot = obsvCamera.rot * glm::rotate(glm::mat4(1.0f), -obsvAngVel, glm::vec3(0,1,0)); }

    if (state.input.keyState[VK_UP]) { obsvCamera.rot = obsvCamera.rot * glm::rotate(glm::mat4(1.0f), obsvAngVel, glm::vec3(1,0,0)); }
    if (state.input.keyState[VK_DOWN]) { obsvCamera.rot = obsvCamera.rot * glm::rotate(glm::mat4(1.0f), -obsvAngVel, glm::vec3(1,0,0)); }

    if (state.input.keyState[VK_OEM_COMMA]) { obsvCamera.rot = obsvCamera.rot * glm::rotate(glm::mat4(1.0f), obsvAngVel, glm::vec3(0,0,1)); }
    if (state.input.keyState[VK_OEM_PERIOD]) { obsvCamera.rot = obsvCamera.rot * glm::rotate(glm::mat4(1.0f), -obsvAngVel, glm::vec3(0,0,1)); }
}
/*
void Program::pickByInput() {
    if (state.isMousePressed())
    {   
        cv::Point2f screen(
            ((state.input.mouseX - terrainRenderer.x) * 1.0 / terrainRenderer.width) * 2 - 1,
            (1 - (state.input.mouseY - terrainRenderer.y) * 1.0 / terrainRenderer.height) * 2 - 1
        );
        
        Intersection intersection = Pose::cast(obsvCamera, screen, terrain, terrainRenderer);
        if (intersection.hit) {
            pickingClicks.push_back(screen);
            pickingPoints.push_back(Point3f(intersection.point));
        }
    }

    if (state.isKeyPressed('C')) {
        if (pickingClicks.size() >= 6) {
            for (auto p : pickingClicks) LOG_DEBUG("(%f %f)", p.x, p.y);
            Camera computed = Pose::solvePnP(pickingPoints, pickingClicks, terrainRenderer);
            LOG_DEBUG("\nrot:\n%s\npos:\n%s\n\nrot:\n%s\npos:\n%s", format(obsvCamera.rot).c_str(), format(obsvCamera.pos).c_str(),format(computed.rot).c_str(), format(computed.pos).c_str());
            addCameraAnimation(&obsvCamera, std::vector({obsvCamera, computed}), std::vector({0.f, .5f}));
            pickingPoints.clear();
            pickingClicks.clear();
        }
    }
}
*/
void Program::placeTrackersByInput() {
    if (state.isMousePressed())
    {   
        cv::Point2f screen(
            ((state.input.mouseX - terrainRenderer.x) * 1.0 / terrainRenderer.width) * 2 - 1,
            (1 - (state.input.mouseY - terrainRenderer.y) * 1.0 / terrainRenderer.height) * 2 - 1
        );
        Intersection intersection = Pose::cast(obsvCamera, screen, terrain, terrainRenderer);
        if (intersection.hit) {
            trackerPoints.push_back(Point3f(intersection.point));
        }
    }
}

void Program::captureByInput() {
    if (state.isKeyPressed('B')) {
        std::vector<cv::Point2f> screenPoints;
        for (cv::Point3f& trackerPoint : trackerPoints) {
            screenPoints.push_back(Pose::projectToScreen(obsvCamera, trackerPoint, terrainRenderer));
        }
        Camera computed = Pose::solvePnP(trackerPoints, screenPoints, terrainRenderer);
        addCameraAnimation(&obsvCamera, std::vector({obsvCamera, computed}), std::vector({0.f, .5f}));
        computedRoute.push_back(computed);
        actualRoute.push_back(obsvCamera);
        // compute pnp location
        // add actual and computed cameras
        // present overlay
    }
}

void Program::toggleLocationByInput() {
    if (state.isKeyPressed('M')) {
        // forward
    }
    if (state.isKeyPressed('N')) {
        // backward
    }
}

void Program::switchStateByInput() {
    switch (programState) {
        case configuring: {
            if (state.isKeyPressed('P'))
            {
                programState = traversing;
            }
        }
        case traversing: {
            if (state.isKeyPressed('R') && computedRoute.size() != 0) {
                programState = reviewing;
            }
        }
        case reviewing: {
            if (state.isKeyPressed('F')) {
                programState = traversing;
                computedRoute.clear();
                actualRoute.clear();
            }
        }
    }
}

void Program::init() {
    obsvCamera.pos = glm::vec4(0.0f);
    obsvCamera.rot = glm::rotate(glm::rotate(glm::mat4(1.0), glm::pi<float>(), glm::vec3(1,0,0)), glm::pi<float>(), glm::vec3(0,0,1));
    obsvCamera.pos.z = 10;
    programState = configuring;
}

void Program::update() { 
    // Hover camera hovers
    glm::mat4 rotateHover = glm::rotate(glm::mat4(1.0), state.getMillis() / 2000.f, glm::vec3(0,0,1));
    hoverCamera.pos = rotateHover * glm::vec4(0,-20,20,1);
    hoverCamera.rot = rotateHover * glm::rotate(glm::mat4(1.0), 3.14f * 1.25f, glm::vec3(1,0,0));
    
    // States management
    switchStateByInput();
    if (programState == configuring) {
        moveByInput();
        placeTrackersByInput();
    }
    if (programState == traversing) {
        moveByInput();
        captureByInput();
    }
    if (programState == reviewing) {
        toggleLocationByInput();
    }


    /// TODO: fix
    lines[0] = obsvCamera.pos.x;
    lines[1] = obsvCamera.pos.y;
    lines[2] = obsvCamera.pos.z;
    lines[5] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(1,-1,3,0)).x;
    lines[6] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(1,-1,3,0)).y;
    lines[7] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(1,-1,3,0)).z;
    lines[10] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(1,1,3,0)).x;
    lines[11] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(1,1,3,0)).y;
    lines[12] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(1,1,3,0)).z;
    lines[15] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(-1,-1,3,0)).x;
    lines[16] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(-1,-1,3,0)).y;
    lines[17] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(-1,-1,3,0)).z;
    lines[20] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(-1,1,3,0)).x;
    lines[21] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(-1,1,3,0)).y;
    lines[22] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(-1,1,3,0)).z;
    linesRenderer.setVertices(lines, 5);
    linesRenderer.setIndices(indices, 30);
}

void Program::draw() {
    glPointSize(5);
    glLineWidth(1);
    terrainRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    terrainRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    terrainRenderer.render(GL_LINES);
    
    pointsRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    pointsRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    pointsRenderer.renderPoints(trackerPoints);
    glLineWidth(3);
    linesRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    linesRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    linesRenderer.render(GL_LINES);

    // Picking view rendered last
    glPointSize(15);
    terrainRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
    terrainRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
    terrainRenderer.render();
    
    pointsRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
    pointsRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
    pointsRenderer.renderPoints(trackerPoints);
}


