#include "Program.h"
#include "Pose.h"

Program::Program(State& state) :
    state(state),
    terrainRenderer(state),
    linesRenderer(state),
    pointsRenderer(state),
    terrain(10, 1){}

void Program::wrapInit() {
    // Terrain and terrain shader
    terrain.loadTexture("IMG-0458.png", "IMG-0458.png", 100);
    terrainRenderer.createProgram("shaders/default.vert", "shaders/texture.frag");
    terrainRenderer.setIndices(terrain.indices, terrain.indices_vec.size());
    terrainRenderer.setVertices(terrain.vertices, terrain.vertices_vec.size() / 5);
    terrainRenderer.setTexture(terrain.texture);

    // Lines and points shader
    linesRenderer.createProgram("shaders/default.vert", "shaders/rainbow.frag");
    pointsRenderer.createProgram("shaders/default.vert", "shaders/rainbow.frag");

    glPointSize(10);

    init();
}
void Program::moveByInput() {
    if (state.input.keyState['S']) {obsvCamera.pos += obsvVel * obsvCamera.rot * glm::vec4(0,0,1,0);}
    if (state.input.keyState['W']) {obsvCamera.pos -= obsvVel * obsvCamera.rot * glm::vec4(0,0,1,0);}
    
    if (state.input.keyState['D']) {obsvCamera.pos += obsvVel * obsvCamera.rot * glm::vec4(1,0,0,0);}
    if (state.input.keyState['A']) {obsvCamera.pos -= obsvVel * obsvCamera.rot * glm::vec4(1,0,0,0);}

    if (state.input.keyState['E']) {obsvCamera.pos += obsvVel * obsvCamera.rot * glm::vec4(0,1,0,0);}
    if (state.input.keyState['C']) {obsvCamera.pos -= obsvVel * obsvCamera.rot * glm::vec4(0,1,0,0);}

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
            (state.input.mouseY - terrainRenderer.y) * 1.0 / terrainRenderer.height
        );
        
        Intersection intersection = Pose::cast(obsvCamera, screen, terrain, terrainRenderer);
        if (intersection.hit) {
            pickingClicks.push_back(screen);
            pickingPoints.push_back(Point3f(intersection.point));
        }
        Pose::printIntersection(intersection);
    }

    if (state.isKeyPressed('C')) {
        Camera computed = Pose::solvePnP(pickingPoints, pickingClicks, terrainRenderer);
        obsvCamera = computed;
        pickingPoints.clear();
        pickingClicks.clear();
    }
}

void Program::switchStateByInput() {
    switch (programState) {
        case traversing: {
            if (state.isKeyPressed('P')) 
                programState = picking;
        }
        case picking: {
            if (state.isKeyPressed('P')) 
                programState = traversing;
        }
    }
}

void Program::init() {
    obsvCamera.pos = glm::vec4(0.0f);
    obsvCamera.rot = glm::rotate(glm::mat4(1.0), 0.f, glm::vec3(0,1,0));
    obsvCamera.pos.z = 10;
    programState = traversing;
}

void Program::update() {
    /// TODO: rotate hover camera
    glm::mat4 rotateHover = glm::rotate(glm::mat4(1.0), state.getMillis() / 2000.f, glm::vec3(0,0,1));
    hoverCamera.pos = rotateHover * glm::vec4(0,-10,10,1);
    hoverCamera.rot = rotateHover * glm::rotate(glm::mat4(1.0), glm::pi<float>() / 4.f, glm::vec3(1,0,0));

    if (state.isKeyPressed('C')) {LOG_DEBUG("%d",state.getMillis());}
    switchStateByInput();
    if (programState == traversing) moveByInput();
    if (programState == picking) pickByInput();
}

void Program::draw() {
    terrainRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    terrainRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    terrainRenderer.render(GL_LINES);

    pointsRenderer.viewport(0, 0, state.window.width/2., state.window.height);
    pointsRenderer.setCamera(hoverCamera.pos, hoverCamera.rot);
    pointsRenderer.renderPoints(pickingPoints);

    // Picking view rendered last
    terrainRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
    terrainRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
    terrainRenderer.render();
    
    pointsRenderer.viewport(state.window.width/2., 0, state.window.width/2., state.window.height);
    pointsRenderer.setCamera(obsvCamera.pos, obsvCamera.rot);
    pointsRenderer.renderPoints(pickingPoints);
}
// GLCall(glDisable(GL_DEPTH_TEST));
// GLCall(glEnable(GL_BLEND));
// GLCall(glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA));
// m_tinted.setVertices(vertices, 8);
// m_tinted.viewport(400, 0, 400, 600);
// m_tinted.setCamera(obsvCamera.pos, rot, 45.0f);
// m_tinted.setUniform("tintColor", glm::vec4(0.f,0.f,1.f,1.f));
// m_tinted.render();
// m_tinted.setUniform("tintColor", glm::vec4(1.f,0.f,0.f,1.f));
// m_tinted.render();
// m_tinted.setUniform("tintColor", glm::vec4(0.f,1.f,0.f,1.f));
// m_tinted.render();
// GLCall(glEnable(GL_DEPTH_TEST));
// GLCall(glDisable(GL_BLEND));
// GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); ///// GL_FILL); 