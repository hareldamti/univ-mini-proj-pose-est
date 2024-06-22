#include "Terrain.h"

Terrain::Terrain(float xWidth, float height): xWidth(xWidth), height(height) {}

void Terrain::loadTexture(const char* filename, int downsize) {
    const int color_depth = 255;
    vertices_vec = {};
    indices_vec = {};

    int imgWidth;
    int imgHeight;
    int numComponents;

    unsigned char* data;
    data = stbi_load(filename, &imgWidth, &imgHeight, &numComponents, 4);

     if (!data) {
        stbi_image_free(data);
        ERROR_EXIT("Failed to load terrain image: %s\n",filename);
        return;
    }

    yWidth = (xWidth * imgHeight * 1.0 / imgWidth);

    int reducedWidth = imgWidth / downsize, reducedHeight = imgHeight / downsize;

    for (int y = 0; y < reducedHeight * downsize; y += downsize) {
        for (int x = 0; x < reducedWidth * downsize; x += downsize) { 
            float x_pos = xWidth * x / imgWidth - (xWidth / 2);
            float y_pos = yWidth * y / imgHeight - (yWidth / 2);

            int pixelIndex = (y * imgWidth + x) * numComponents; 
            float averageRGB = (data[pixelIndex] + data[pixelIndex + 1] + data[pixelIndex + 2]) / 3.0; // average of RGB values

            float z_pos = height * averageRGB / color_depth;

            vertices_vec.push_back(x_pos);
            vertices_vec.push_back(y_pos);
            vertices_vec.push_back(z_pos);
            vertices_vec.push_back(1.0 * x / (imgWidth - downsize));
            vertices_vec.push_back(1.0 * y / (imgHeight - downsize)); 
        }
    }

    // calculating triangles (indices) vector
    for (int y = 0; y < reducedHeight - 1; y +=1) {
        for (int x = 0; x < reducedWidth - 1; x += 1) { 
            int topLeft = y * reducedWidth + x;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + reducedWidth;
            int bottomRight = bottomLeft + 1;

            indices_vec.push_back(topRight);
            indices_vec.push_back(bottomRight);
            indices_vec.push_back(topLeft);

            indices_vec.push_back(bottomRight);
            indices_vec.push_back(bottomLeft);
            indices_vec.push_back(topLeft);
        }
    }

    //converting vector array 

    vertices = new float[vertices_vec.size()];
    std::copy(vertices_vec.begin(), vertices_vec.end(), vertices);

    indices = new unsigned int[indices_vec.size()];
    std::copy(indices_vec.begin(), indices_vec.end(), indices);

    // creating triangles vector
    for (int i =0 ; i < indices_vec.size(); i+=3){
        Vec3 a = {vertices[indices[i]*5], vertices[indices[i]*5 + 1],vertices[indices[i]*5 + 2]};
        Vec3 b = {vertices[indices[i+1]*5], vertices[indices[i+1]*5 + 1],vertices[indices[i+1]*5 + 2]};
        Vec3 c = {vertices[indices[i+2]*5], vertices[indices[i+2]*5 + 1],vertices[indices[i+2]*5 + 2]};
        triangles.push_back({a,b,c});
    }

    // Copy image texture
    GLCall(glGenTextures(1, &texture));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture));
    GLenum format = GL_RGBA;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, format, imgWidth, imgHeight, 0, format, GL_UNSIGNED_BYTE, data));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT)); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    stbi_image_free(data);
}



Terrain::~Terrain(){
    delete[] vertices;
    delete[] indices;
}



// extract triangle by index
// Triangle Terrain::getTriangle(int i) {
//     Vec3 a = vertices_vec[indices_vec[i*3]];
//     Vec3 b = vertices_vec[indices_vec[i*3+1]];
//     Vec3 c = vertices_vec[indices_vec[i*3+2]];
//     Triangle res = {a, b, c};
//     return res;
// }



