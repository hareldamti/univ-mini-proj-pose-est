#include "Program.h"
#include "Terrain.h"
#include "Picking.h"



Program::Program(State& state) : m_state(state), m_render(state), m_tinted(state){}

/*
float vertices[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top left 
    0.0f,  0.0f, 0.5f, 1.0f, 1.0f, // top right
     0.5f, -0.0f, -0.5f, 0.0f, 1.0f, // bottom right
    -0.5f, -0.0f, -0.5f, 0.0f, 0.0f, // bottom left
    -0.0f,  0.0f, 0.5f, 1.0f, 0.0f // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3,    // second triangle
    4, 5, 7,   // first triangle
    5, 6, 7    // second triangle
    
};  
*/

// setting up terrain
Terrain terrain = Terrain("a.png", 2,5, 4.f);

glm::vec4 pos(0.0f);
glm::vec3 rot(0.f, glm::pi<float>(),0.f);
glm::mat4 dir;
float v = 0.05f, vr = 0.01f;

void Program::init() {

    /*
    // Printing indices and vertices (delete) -------
    for (int i=0; i<terrain.indices_vec.size(); i+=3){
    std::cout << "triangle: " << (i/3 ) << " ------ "  << std::endl; 
    std::cout << indices[i]<< ", " << indices[i+1]<< ", " << indices[i+2] << std::endl;
    }

    for (int i=0; i<terrain.vertices_vec.size(); i+=5){
    std::cout << "vertecs: " << (i/5 ) << " ------ "  << std::endl; 
    std::cout << vertices[i]<< ", " << vertices[i+1]<< ", " << vertices[i+2]<< ", " << vertices[i+3]<< ", " << vertices[i+4] << std::endl;
    }
    ///                                       --------
    */

    m_render.createProgram("shaders/default.vert", "shaders/default.frag");
    m_render.setIndices(terrain.indices, terrain.indices_vec.size());
    //m_tinted.createProgram("shaders/default.vert", "shaders/tinted.frag");
    //m_tinted.setIndices(indices, 12);
    pos.z = -10;
    //glEnable(GL_BLEND);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

void Program::update() {
    dir = glm::rotate(glm::mat4(1.0f), rot.x, glm::vec3(0,1,0));
    dir = glm::rotate(dir, rot.y, glm::vec3(1,0,0));


    if (m_state.input.keyState['S']) {pos += v * dir * glm::vec4(0,0,1,0);}
    if (m_state.input.keyState['W']) {pos -= v * dir * glm::vec4(0,0,1,0);}
    
    if (m_state.input.keyState['D']) {pos += v * dir * glm::vec4(1,0,0,0);}
    if (m_state.input.keyState['A']) {pos -= v * dir * glm::vec4(1,0,0,0);}

    if (m_state.input.keyState['E']) {pos += v * dir * glm::vec4(0,1,0,0);}
    if (m_state.input.keyState['C']) {pos -= v * dir * glm::vec4(0,1,0,0);}

    if (m_state.input.keyState[VK_LEFT]) {rot.x += vr;}
    if (m_state.input.keyState[VK_RIGHT]) {rot.x -= vr;}

    if (m_state.input.keyState[VK_UP]) {rot.y +=  vr;}
    if (m_state.input.keyState[VK_DOWN]) {rot.y -= vr;}

    if (m_state.input.keyState[VK_OEM_COMMA]) {rot.z +=  vr;}
    if (m_state.input.keyState[VK_OEM_PERIOD]) {rot.z -= vr;}

    //picking
    if (m_state.input.keyState['P'])
    {   
        Vec3 camera_pos = {pos.x, pos.y, pos.z}; // lets unite Vec3 of "GraphicUtils.h" with "glm::vec" later
        Intersection intersection = Picking::pickAboo(camera_pos, m_state.input.mouseX, m_state.input.mouseY, m_state.window.width, m_state.window.height, terrain);
        Picking::printIntersection(intersection);
    }

}

void Program::draw() {
    m_render.setVertices(terrain.vertices, terrain.vertices_vec.size()/5);
    m_render.viewport(0, 0, m_state.window.width, m_state.window.height);
    m_render.setCamera(pos, rot, 45.0f);
    m_render.render();
    
    // GLCall(glDisable(GL_DEPTH_TEST));
    // GLCall(glEnable(GL_BLEND));
    // GLCall(glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA));
    // m_tinted.setVertices(vertices, 8);
    // m_tinted.viewport(400, 0, 400, 600);
    // m_tinted.setCamera(pos, rot, 45.0f);
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
}
