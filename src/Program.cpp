#include "Program.h"
#include "Terrain.h"
#include "Picking.h"



Program::Program(State& state) : m_state(state), m_render(state){}

/*
float vertices[] = {
     0.5f,  0.5f, 0.0f, 1.0f, 1.0f, // top right
     0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, 1.0f, 0.0f // top left 
};

unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};  
*/

// setting up terrain
std::string file_name = "src/terrain/img_test4.png";
Terrain terrain = Terrain(file_name.c_str(), 1);

float* vertices = terrain.vertices;
unsigned int* indices = terrain.indices;

glm::vec4 pos(0.0f);
glm::vec3 rot(0.0f);
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
    m_render.setIndices(indices, terrain.indices_vec.size());
    pos.z = -.5;
}

void Program::update() {
    dir = glm::rotate(glm::mat4(1.0f), rot.y, glm::vec3(1,0,0));
    dir = glm::rotate(dir, rot.x, glm::vec3(0,1,0));

    if (m_state.input.keyState['W']) {pos += v * dir * glm::vec4(0,0,1,0);}
    if (m_state.input.keyState['S']) {pos -= v * dir * glm::vec4(0,0,1,0);}
    
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
    m_render.setVertices(vertices, terrain.vertices_vec.size()/5);
    m_render.setCamera(pos, rot, 45.0f);
    m_render.render();

    // Wireframe: glPolygonMode(GL_FRONT_AND_BACK, GL_LINE ///// GL_FILL); 
}
