#include "Terrain.h"

Terrain::Terrain(const char* filename, int downsize, float xWidth, float height): xWidth(xWidth), height(height)
{
    color_depth = 255;
    vertices_vec = {}; // maybe delete
    indices_vec = {};  // maybe delete

    int width_res;
    int height_res;
    int numComponents;

    unsigned char* data;
    data = stbi_load(filename, &width_res, &height_res, &numComponents, 4);

     if (!data) {
        std::cerr << "Failed to load terrain image: " << filename << std::endl;
        return;
    }

    yWidth = (xWidth * width_res / height_res);

    // calculating vertices_vec vector
    for (int y = 0; y < height_res; y += downsize){
        for (int x = 0; x < width_res; x += downsize){ 
            float x_pos = xWidth * x / width_res - (xWidth / 2);
            float y_pos = yWidth * y / width_res - (yWidth / 2);

            int pixelIndex = (y * width_res + x) * 4; 
            float averageRGB = (data[pixelIndex] + data[pixelIndex + 1] + data[pixelIndex + 2]) / 3.0; // average of RGB values

            float z_pos = height * averageRGB / color_depth;

            vertices_vec.push_back(x_pos);
            vertices_vec.push_back(y_pos);
            vertices_vec.push_back(z_pos);
            vertices_vec.push_back(1.0 * x / (width_res-1));
            vertices_vec.push_back(1.0 * y / (height_res-1)); 

        }
    }

    // calculating triangles (indices) vector
    for (int y = 0; y < (height_res / downsize - 1); y++) {
        for (int x = 0; x < (width_res / downsize - 1); x++){
            int topLeft = y * (width_res / downsize) + x;
            int topRight = topLeft + 1;
            int bottomLeft = topLeft + (width_res / downsize);
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

        //Triangle triangle = 
        triangles.push_back({a,b,c});
    }

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



