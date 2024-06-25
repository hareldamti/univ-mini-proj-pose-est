#include "Program.h"
#include "Pose.h"

Program::Program(State& state) :
    state(state),
    terrainRenderer(state),
    linesRenderer(state),
    pointsRenderer(state),
    terrain(10, 1){}

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

f32 lines[45] = {
    0, 0, 0, 0, 0
    -1000, 0, 0, 1, 0,
    1000, 0, 0, 1, 1,
    0, -1000, 0, 0, -1,
    0, 1000, 0, 1, 1, 
    0, 0, -1000, 0, 1,
    0, 0, 1000, 1, 1,
    0,0,0,0,0,
    0,0,0,0,0
};
u32 indices[4] = {
    0,1,2,0
};
int t = 0;

void Program::wrapInit() {
    // Terrain and terrain shader
    terrain.loadTexture("IMG-0458.png", "IMG-0458.png", 100);
    terrainRenderer.createProgram("shaders/default.vert", "shaders/texture.frag");
    terrainRenderer.setIndices(terrain.indices, terrain.indices_vec.size());
    terrainRenderer.setVertices(terrain.vertices, terrain.vertices_vec.size() / 5);
    terrainRenderer.setTexture(terrain.texture);

    // Lines and points shader
    linesRenderer.createProgram("shaders/default.vert", "shaders/color.frag");    
    pointsRenderer.createProgram("shaders/default.vert", "shaders/rainbow.frag");

    glPointSize(10);

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


void Program::pickByInput() {
    if (state.isMousePressed())
    {   
        cv::Point2f screen(
            (state.input.mouseX - terrainRenderer.x) * 1.0 / terrainRenderer.width,
            1 - (state.input.mouseY - terrainRenderer.y) * 1.0 / terrainRenderer.height
        );
        
        Intersection intersection = Pose::cast(obsvCamera, screen, terrain, terrainRenderer);
        if (intersection.hit) {
            pickingClicks.push_back(screen);
            pickingPoints.push_back(Point3f(intersection.point));
        }
    }

    if (state.isKeyPressed('C')) {
        if (pickingClicks.size() >= 6) {
            Camera computed = Pose::solvePnP(pickingPoints, pickingClicks, terrainRenderer);
            LOG_DEBUG("\nrot:\n%s\npos:\n%s\n\nrot:\n%s\npos:\n%s", format(obsvCamera.rot).c_str(), format(obsvCamera.pos).c_str(),format(computed.rot).c_str(), format(computed.pos).c_str());
            addCameraAnimation(&obsvCamera, std::vector({obsvCamera, computed}), std::vector({0.f, 2.f}));
            pickingPoints.clear();
            pickingClicks.clear();
            t = 20000;
        }
    }
}

void Program::switchStateByInput() {
    switch (programState) {
        case traversing: {
            if (state.isKeyPressed('P')) 
                programState = picking;
                pickingPoints.clear();
                pickingClicks.clear();
                t = 0;
        }
        case picking: {
            if (state.isKeyPressed('P')) 
                programState = traversing;
        }
    }
}

void Program::init() {
    obsvCamera.pos = glm::vec4(0.0f);
    obsvCamera.rot = glm::rotate(glm::rotate(glm::mat4(1.0), glm::pi<float>(), glm::vec3(1,0,0)), glm::pi<float>(), glm::vec3(0,0,1));

    obsvCamera.pos.z = 10;
    programState = traversing;
}

void Program::update() { 
    if (t > 0 && t%10 == 0) {auto g = glm::vec3(obsvCamera.pos);pickingPoints.push_back(Point3f(g));}
    t--;
    glm::mat4 rotateHover = glm::rotate(glm::mat4(1.0), state.getMillis() / 2000.f, glm::vec3(0,0,1));
    hoverCamera.pos = rotateHover * glm::vec4(0,0,20,1);
    hoverCamera.rot = rotateHover * glm::rotate(glm::mat4(1.0), 3.14f, glm::vec3(1,0,0));
    
    switchStateByInput();
    if (programState == traversing) moveByInput();
    if (programState == picking) pickByInput();

    lines[0] = obsvCamera.pos.x;
    lines[1] = obsvCamera.pos.y;
    lines[2] = obsvCamera.pos.z;
    lines[5] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(0,-1,5,0)).x;
    lines[6] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(0,-1,5,0)).y;
    lines[7] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(0,-1,5,0)).z;
    lines[10] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(0,1,5,0)).x;
    lines[11] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(0,1,5,0)).y;
    lines[12] = (obsvCamera.pos + obsvCamera.rot * glm::vec4(0,1,5,0)).z;
    linesRenderer.setVertices(lines, 3);
    linesRenderer.setIndices(indices, 4);
}

void Program::draw() {
    terrainRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    terrainRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    terrainRenderer.render(GL_LINES);

    pointsRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    pointsRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    pointsRenderer.renderPoints(pickingPoints);

    linesRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    linesRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    linesRenderer.render(GL_LINES);

    // Picking view rendered last
    terrainRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
    terrainRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
    terrainRenderer.render();
    
    pointsRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
    pointsRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
    pointsRenderer.renderPoints(pickingPoints);

    linesRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
    linesRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
    linesRenderer.render(GL_LINES);
}


