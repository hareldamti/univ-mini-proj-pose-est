#include "Program.h"
#include "Pose.h"
#include "Mesh.h"

Mesh cameraMesh(
    std::vector<u32>({
        0, 1, 2,
        0, 1, 3,
        0, 2, 4,
        0, 3, 4,
        1, 0, 2,
        1, 0, 3,
        0, 4, 2,
        4, 3, 0
    }),
    std::vector<f32>({
        0,   0,  0,
        1,  -1,  3,
        1,   1,  3,
        -1, -1,  3,
        -1,  1,  3
    })
);

Mesh towerMesh(
    std::vector<u32>({
        0, 1, 2,
        0, 1, 3,
        0, 2, 4,
        0, 3, 4,
        1, 0, 2,
        1, 0, 3,
        0, 4, 2,
        4, 3, 0,
        1, 2, 6,
        1, 6, 2,
        1, 5, 6, 
        1, 6, 5,
        1, 3, 7,
        1, 7, 3,
        1, 5, 7,
        1, 7, 5,
        3, 4, 7,
        3, 7, 4,
        2, 4, 6,
        2, 6, 4,
        2, 4, 8,
        2, 8, 4,
        5, 6, 9,
        5, 7, 9, 
        5, 9, 10,
        6, 8, 10,
        6, 12, 10,
        8, 12, 11, 
        8, 7, 12, 
        8, 12, 7, 
        7, 8, 11, 
        7, 11, 8, 
        7, 11, 5, 
        7, 5, 11,
        9, 10, 11,
        11, 12, 10

    }),
    std::vector<f32>({
        0,   0,  0,     //0
        1,  -1,  6,     //1
        1,   1,  6,     //2
        -1, -1,  6,     //3
        -1,  1,  6,     //4
        2,  -2,  12,    //5
        2,   2,  12,    //6
        -2, -2,  12,    //7
        -2,  2,  12,    //8
        3,  -3,  18,    //9
        3,   3,  18,    //10
        -3, -3,  18,    //11
        -3,  3,  18,    //12
    })
);

Program::Program(State& state) :
    state(state),
    terrainRenderer(state),
    linesRenderer(state),
    pointsRenderer(state),
    terrain(state.params.get("terrain-width"), state.params.get("terrain-height")){}

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

void Program::wrapInit() {
    // Terrain and terrain shader
    terrain.loadTexture("height.png", "arial.jpg", state.params.get("downsize"));
    if (terrain.vertices_vec.size() > VERTEX_BUFFER_SIZE) ERROR_EXIT("Vertex buffer size limit (%d) exceeded - %d", VERTEX_BUFFER_SIZE, terrain.vertices_vec.size());
    terrainRenderer.createProgram("shaders/default.vert", "shaders/texture.frag");
    terrainRenderer.setIndices(&terrain.indices_vec[0], terrain.indices_vec.size());
    terrainRenderer.setVertices(&terrain.vertices_vec[0], terrain.vertices_vec.size() / 5);
    terrainRenderer.setTexture(terrain.texture);

    // Lines and points shader
    linesRenderer.createProgram("shaders/default.vert", "shaders/color.frag");    
    pointsRenderer.createProgram("shaders/default.vert", "shaders/rainbow.frag");

    init();
}

void Program::moveByInput() {
    if (state.input.keyState['O']) {hoverDistance -= obsvVel;}
    if (state.input.keyState['L']) {hoverDistance += obsvVel;}

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
            state.createAnimation(&trackerPoints.at(trackerPoints.size()-1).z, std::vector({intersection.point.z, intersection.point.z + 2}), std::vector({0.f,1.f}));
        }
    }
}

void Program::captureByInput() {
    if (state.isKeyPressed('B')) {
        std::vector<cv::Point2f> screenPoints;
        std::vector<cv::Point3f> actualPoints;
        for (cv::Point3f& trackerPoint : trackerPoints) {

            cv::Point2f screen = Pose::projectToScreen(obsvCamera, trackerPoint, terrainRenderer);
            Intersection intersection = Pose::cast(obsvCamera, screen, terrain, terrainRenderer);
            if (intersection.hit && glm::length(Vec4(trackerPoint) - obsvCamera.pos) - intersection.distance < 0.1f ) {
                screen.x = (std::floor(screen.x * state.params.get("cam-res")) + 0.5f) / state.params.get("cam-res");
                screen.y = (std::floor(screen.y * state.params.get("cam-res")) + 0.5f) / state.params.get("cam-res");
                actualPoints.push_back(trackerPoint);
                screenPoints.push_back(screen);
            }
        }
        if (screenPoints.size() < 6) {
            return;
        }
        Camera computed = Pose::solvePnP(trackerPoints, screenPoints, terrainRenderer);
        computedRoute.push_back(computed);
        actualRoute.push_back(obsvCamera);
        // compute pnp location
        // add actual and computed cameras
        // present overlay
    }
}

void Program::toggleLocationByInput() {
    if (state.isKeyPressed('M')) {
        routeIndex = (++routeIndex + actualRoute.size()) % actualRoute.size();
        addCameraAnimation(&obsvCamera, std::vector({obsvCamera, actualRoute.at(routeIndex)}), std::vector({0.f, state.params.get("camera-animation-duration")}));
        addCameraAnimation(&compCamera, std::vector({compCamera, computedRoute.at(routeIndex)}), std::vector({0.f, state.params.get("camera-animation-duration")}));
    
    }
    if (state.isKeyPressed('N')) {
        routeIndex = (--routeIndex + actualRoute.size()) % actualRoute.size();
        addCameraAnimation(&compCamera, std::vector({compCamera, computedRoute.at(routeIndex)}), std::vector({0.f, state.params.get("camera-animation-duration")}));
        addCameraAnimation(&obsvCamera, std::vector({obsvCamera, actualRoute.at(routeIndex)}), std::vector({0.f, state.params.get("camera-animation-duration")}));
    }
}

void Program::switchStateByInput() {
    switch (programState) {
        case configuring: {
            if (state.isKeyPressed('P') && trackerPoints.size() >= 6)
            {
                programState = traversing;
            }
        }
        case traversing: {
            if (state.isKeyPressed('R') && computedRoute.size() != 0) {
                programState = reviewing;
                routeIndex = 0;
                compCamera = obsvCamera;
                addCameraAnimation(&compCamera, std::vector({compCamera, computedRoute.at(routeIndex)}), std::vector({0.f, state.params.get("camera-animation-duration")}));
                addCameraAnimation(&obsvCamera, std::vector({obsvCamera, actualRoute.at(routeIndex)}), std::vector({0.f, state.params.get("camera-animation-duration")}));
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
    hoverDistance = state.params.get("hover-distance");
}

void Program::update() { 
    // Hover camera hovers
    glm::mat4 rotateHover = glm::rotate(glm::mat4(1.0), state.getMillis() * 3.14f / 500 / state.params.get("hover-rotation-duration"), glm::vec3(0,0,1));
    hoverCamera.pos = rotateHover * glm::vec4(0,-hoverDistance,hoverDistance,1);
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

    Mesh elements;
    for (int i = 0; i < actualRoute.size(); i++) {
        elements.add(cameraMesh, actualRoute[i].pos, actualRoute[i].rot, 0.3, 0);
        elements.add(cameraMesh, computedRoute[i].pos, computedRoute[i].rot, 0.7, 0);
    }
    if (actualRoute.size() > 1) {
        Mesh actualRouteMesh = Mesh::fromRoute(actualRoute), computedRouteMesh = Mesh::fromRoute(computedRoute);
        elements.add(actualRouteMesh, 0.3, 0);
        elements.add(computedRouteMesh, 0.7, 0); 
    }
    elements.add(cameraMesh, obsvCamera.pos, obsvCamera.rot, 0, 0);
    if (programState == reviewing) elements.add(cameraMesh, compCamera.pos, compCamera.rot, 1, 0);

    for (cv::Point3f& tracker : trackerPoints) {
        glm::vec4 point = Vec4(tracker);
        glm::mat4 resize(-0.12);
        elements.add(towerMesh, point, resize, 0, 0);
    }

    linesRenderer.setVertices(&elements.vertices[0], elements.vertices.size() / 5);
    linesRenderer.setIndices(&elements.indices[0], elements.indices.size());
}

void Program::draw() {
    GLCall(glEnable(GL_DEPTH_TEST));

    // Hover camera terrain
    glLineWidth(1);
    terrainRenderer.setUniform("tint", glm::vec4(1.0f));
    terrainRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    terrainRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    terrainRenderer.render(programState == configuring ? GL_LINES : GL_TRIANGLES);
    
    // Hover camera trackers
    glPointSize(5);
    pointsRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    pointsRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    pointsRenderer.renderPoints(trackerPoints);

    // Hover camera obsv
    glLineWidth(3);
    linesRenderer.setUniform("color", glm::vec4(1.f));
    linesRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    linesRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    linesRenderer.render(GL_LINES);

    if (programState == configuring) {
        // Obsv camera towers
        glLineWidth(3);
        linesRenderer.setUniform("color", glm::vec4(1.f));
        linesRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
        linesRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
        linesRenderer.render(GL_LINES);
    }
    // Obsv camera terrain
    terrainRenderer.setUniform("tint", glm::vec4(1.f));
    terrainRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
    terrainRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
    terrainRenderer.render(programState == configuring ? GL_LINES : GL_TRIANGLES);
    

    // Obsv camera trackers
    glPointSize(15);
    pointsRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
    pointsRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
    pointsRenderer.renderPoints(trackerPoints);

    if (programState == reviewing) {
        // Computed camera terrain
        GLCall(glDisable(GL_DEPTH_TEST));
        terrainRenderer.setUniform("tint", glm::vec4(1,0.5,0,0.6));
        terrainRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
        terrainRenderer.setCamera(compCamera.pos, compCamera.rot);
        terrainRenderer.render();
    }
    
    
    // // Computed camera trackers
    // glPointSize(15);
    // pointsRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
    // pointsRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
    // pointsRenderer.renderPoints(trackerPoints);
    
}


