//
//  main.cpp
//  3D Object Drawing
//
//  Created by Nazirul Hasan on 4/9/23.
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION 
#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "spotLight.h"
#include "directionalLight.h"
#include "sphere.h"
#include "cube.h"
#include "stb_image.h"
#include<math.h>
#include <iostream>
#include <corecrt_math_defines.h> 
#include "cylinder.h"

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void drawWheel(unsigned int& cubeVAO, unsigned int& cubeVAO2, unsigned int& cubeVAO3, Shader& lightingShader, glm::mat4 model, float r, float g, float b );
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

const unsigned int valueS = 0.0174532925;

// modelling transform
float rotateAngle_X = 45.0;
float rotateAngle_Y = 45.0;
float rotateAngle_Z = 45.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = -0.5;
float translate_Y = 0.0;
float translate_Z = -0.7;
float scale_X = 0.2;
float scale_Y = 0.2;
float scale_Z = 0.2;


float camera_move = 0.0f;

// camera
Camera camera(glm::vec3(6.0f, 1.0f, 4.5f));
//Camera camera(glm::vec3(6.0f, 1.0f, 4.5f));

//Camera camera(glm::vec3(-4.0f, 2.0f, 4.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float eyeX = 3.0, eyeY = 4.0, eyeZ = 3.0;
//float eyeX = 6.0, eyeY = 1.0, eyeZ = 4.5;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
//float lookAtX = 6.0, lookAtY = 1.0, lookAtZ = 4.5;
glm::vec3 V = glm::vec3(0.0f, 0.0f, 1.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);


bool fan_on = false;
float rotateAngle = 0.0;
float train_move = 0.0;

bool train_on = false;
bool train_on_reverse = false;
bool camera_on = false;




// positions of the point lights
glm::vec3 pointLightPositions[] = {
   // glm::vec3(1.50f + 10.0f,  1.50f+10.0f,  0.0f),
    glm::vec3(1.3f, 2.48f, 1.1f),
    glm::vec3(1.0f, 1.99f, 6.0f),
    glm::vec3(4.3f, 2.48f, 1.1f),
    glm::vec3(6.6f, 2.02f, 1.1f),
    glm::vec3(6.6f, 1.99f, 6.0f),

    glm::vec3(1.3f + 50.0f, 2.48f, 1.1f),
    glm::vec3(1.0f + 50.0f, 1.99f, 6.0f),
    glm::vec3(4.3f + 50.0f, 2.48f, 1.1f),
    glm::vec3(6.6f + 50.0f, 2.02f, 1.1f),
    glm::vec3(6.6f + 50.0f, 1.99f, 6.0f),
   /* glm::vec3(-3.0f, 2.02f, 1.1f),
    glm::vec3(1.5f, 1.95f, 6.1f),
    glm::vec3(3.5f, 1.95f, 6.1f),
    glm::vec3(5.5f, 1.95f, 6.1f),*/
   /* glm::vec3(1.5f + 10.0f,  -1.5f+10.0f,  0.0f),
    glm::vec3(-1.5f + 10.0f,  1.5f+10.0f,  0.0f),
    glm::vec3(-1.5f + 10.0f,  -1.5f+10.0f,  0.0f)*/
};
glm::vec3 spotLightPositions[] = {
    // glm::vec3(1.50f + 10.0f,  1.50f+10.0f,  0.0f),
     glm::vec3(-2.49f, 1.05f, -1.0f),
     glm::vec3(-2.49f, 0.0f, -1.0f)//direction

     /*glm::vec3(4.5f + 10.0f,  -1.5f + 10.0f,  0.0f),
     glm::vec3(-5.5f + 10.0f,  1.5f + 10.0f,  0.0f),
     glm::vec3(-6.5f + 10.0f,  -1.5f + 10.0f,  0.0f)*/
};
glm::vec3 directionLightPositions[] = {
     glm::vec3(4.5f,  200.0f , 3.0f),
};

PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
   
    0.5f, 0.5f, 0.5f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.5f, 0.5f, 0.5f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.5f, 0.5f, 0.5f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    3       // light number
);
PointLight pointlight4(

    pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.5f, 0.5f, 0.5f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    4       // light number
);
PointLight pointlight5(

    pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.5f, 0.5f, 0.5f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    5       // light number
);
PointLight pointlight6(

    pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    6       // light number
);
PointLight pointlight7(

    pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    7       // light number
);
PointLight pointlight8(

    pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    8       // light number
);
PointLight pointlight9(

    pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    9       // light number
);
PointLight pointlight10(

    pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    10       // light number
);
SpotLight spotlight1(

    spotLightPositions[1].x, spotLightPositions[1].y, spotLightPositions[1].z,  // direction
    spotLightPositions[0].x, spotLightPositions[0].y, spotLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
SpotLight spotlight2(

    spotLightPositions[1].x, spotLightPositions[1].y, spotLightPositions[1].z,  // direction
    spotLightPositions[0].x, spotLightPositions[0].y, spotLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
SpotLight spotlight3(

    spotLightPositions[2].x, spotLightPositions[2].y, spotLightPositions[2].z,  // direction
    spotLightPositions[0].x, spotLightPositions[0].y, spotLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);
SpotLight spotlight4(

    spotLightPositions[3].x, spotLightPositions[3].y, spotLightPositions[3].z,  // direction
    spotLightPositions[0].x, spotLightPositions[0].y, spotLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);

DirectionLight directionlight1(

    directionLightPositions[0].x, directionLightPositions[0].y, directionLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.6f, 0.6f, 0.6f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.09f,  //k_l
    0.032f, //k_q
    1       // light number
);



// light settings
bool pointLightOn = true;
bool spotLightOn = true;
bool directionLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;



glm::mat4 model_Matrix(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z,float angleX, float angleY, float angleZ)
{
    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;


    //floor_lemon_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f+angleX), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f+angleY), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f+angleZ), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
   
    return model;

}
vector<float> computeFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
{
    const float EPSILON = 0.000001f;

    vector<float> normal(3, 0.0f);     // default return value (0,0,0)
    float nx, ny, nz;

    // find 2 edge vectors: v1-v2, v1-v3
    float ex1 = x2 - x1;
    float ey1 = y2 - y1;
    float ez1 = z2 - z1;
    float ex2 = x3 - x1;
    float ey2 = y3 - y1;
    float ez2 = z3 - z1;

    // cross product: e1 x e2
    nx = ey1 * ez2 - ez1 * ey2;
    ny = ez1 * ex2 - ex1 * ez2;
    nz = ex1 * ey2 - ey1 * ex2;

    // normalize only if the length is > 0
    float length = sqrtf(nx * nx + ny * ny + nz * nz);
    if (length > EPSILON)
    {
        // normalize
        float lengthInv = 1.0f / length;
        normal[0] = nx * lengthInv;
        normal[1] = ny * lengthInv;
        normal[2] = nz * lengthInv;
    }

    return normal;
}

void platform_color(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z,float r_x,float r_y,float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.843f, 0.839f, 0.819f);
}

void color_black(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.1f, 0.1f, 0.1f);
}
void color_white(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 1.0f, 1.0f, 1.0f);
}
void rail_track(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.207f, 0.105f, 0.0784f);
}
void color_wall(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.827f, 0.729f, 0.517f);
}
void color_boat(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.16f, 0.10f, 0.06f);
}
void color_wall2(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.294f, 0.227f, 0.258f);
}
void platform_roof(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.631f, 0.270f, 0.215f);
}
void platform_roof2(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.274f, 0.450f, 0.133f);
}
void platform_roof3(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.631f, 0.270f, 0.215f);
}
void color_train_body(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.011f, 0.65f, 0.211f);
}
void color_red(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 1.0f, 0.0f, 0.0f);
}
void color_green(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, float r_x, float r_y, float r_z, unsigned int& cubeVAO, Shader& lightingShader)
{
    glm::mat4 model = model_Matrix(s_x, s_y, s_z, t_x, t_y, t_z, r_x, r_y, r_z);
    lightingShader.setMat4("model", model);
    drawCube(cubeVAO, lightingShader, model, 0.109f, 0.35f, 0.086f);
}
void rail_track(float tx, float ty, float tz, unsigned int& cubeVAO, Shader& lightingShader)
{
    for (int i = 1; i <= 300; i++)
    {
        color_white(1, 0.3, 5, 0.1 * i + tx - 4.05, 0 + ty, 4.66 + tz, 0, 0, 0, cubeVAO, lightingShader);
        i += 1;
    }
    rail_track(300, 0.5, 0.3, -4.05 + tx, 0.02 + ty, 4.75 + tz,0, 0, 0, cubeVAO, lightingShader);
    rail_track(300, 0.5, 0.3, -4.05 + tx, 0.02 + ty, 5.01 + tz, 0, 0, 0, cubeVAO, lightingShader);
}

void train_body(float tx, float ty, float tz, unsigned int& cubeVAO, Shader& lightingShader)
{
    //train body start
    color_white(21, 0.1, 7, -3.03 + tx, 0.17 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    //color_train_body(21, 11, 0.2, -3.03, 0.17, 4.65, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(5, 6, 0.2, -3.03 + tx, 0.17 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(5, 6, 0.2, -2.2 + tx, 0.17 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(4.4, 6, 0.2, -1.37 + tx, 0.17 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);

    color_train_body(3.5, 5, 0.2, -2.55 + tx, 0.17 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(3.5, 5, 0.2, -1.7 + tx, 0.17 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);

    color_red(3.5, -3, 0.2, -2.55 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(3.5, -3, 0.2, -1.7 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);

    color_red(5, 5, 0.2, -3.03 + tx, 0.77 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(5, 5, 0.2, -2.2 + tx, 0.77 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(4.4, 5, 0.2, -1.37 + tx, 0.77 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);

    /*color_train_body(3.5, -3, 0.2, -2.55 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(3.5, -3, 0.2, -1.7 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);

    color_train_body(5, 5, 0.2, -3.03 + tx, 0.77 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(5, 5, 0.2, -2.2 + tx, 0.77 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(4.4, 5, 0.2, -1.37 + tx, 0.77 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);*/



    //color_train_body(21, 11, 0.2, -3.03, 0.17, 5.35, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(5, 6, 0.2, -3.03 + tx, 0.17 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(5, 6, 0.2, -2.2 + tx, 0.17 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(4.4, 6, 0.2, -1.37 + tx, 0.17 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);

    color_train_body(3.5, 5, 0.2, -2.55 + tx, 0.17 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(3.5, 5, 0.2, -1.7 + tx, 0.17 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);

    color_red(3.5, -3, 0.2, -2.55 + tx, 1.27 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(3.5, -3, 0.2, -1.7 + tx, 1.27 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);

    color_red(5, 5, 0.2, -3.03 + tx, 0.77 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(5, 5, 0.2, -2.2 + tx, 0.77 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(4.4, 5, 0.2, -1.37 + tx, 0.77 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);

  /*  color_train_body(3.5, -3, 0.2, -2.55 + tx, 1.27 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(3.5, -3, 0.2, -1.7 + tx, 1.27 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);

    color_train_body(5, 5, 0.2, -3.03 + tx, 0.77 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(5, 5, 0.2, -2.2 + tx, 0.77 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(4.4, 5, 0.2, -1.37 + tx, 0.77 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);*/

    platform_color(21, 0.1, 7.2, -3.03 + tx, 1.265 + ty, 4.651 + tz, 0, 0, 0, cubeVAO, lightingShader);
    //train body end

    color_white(-6, 0.1, 7, -3.03 + tx, .17 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(-2.0, 6, 0.2, -2.95 + tx, .17 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(1.7, 6, 0.2, -3.63 + tx, .17 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    //color_train_body(2.0, 5,0.1, -3.43, .17, 4.65, 0, 0, 0, cubeVAO, lightingShader);

    color_red(-2.0, -5, 0.2, -2.95 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(1.7, -5, 0.2, -3.63 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(3.1, -2, 0.2, -3.46 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);

    color_train_body(-2.0, -5, 0.2, -2.95 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(1.7, -5, 0.2, -3.63 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(3.1, -2, 0.2, -3.46 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);

    platform_color(-6, 0.1, 7.2, -3.03 + tx, 1.27 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);

    color_train_body(-2.0, 6, 0.2, -2.95 + tx, .17 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(1.7, 6, 0.2, -3.63 + tx, .17 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(-2.0, -5, 0.2, -2.95 + tx, 1.27 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(1.7, -5, 0.2, -3.63 + tx, 1.27 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_red(3.1, -3, 0.2, -3.46 + tx, 1.27 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);

    /*color_train_body(-2.0, -5, 0.2, -2.95 + tx, 1.27 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(1.7, -5, 0.2, -3.63 + tx, 1.27 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(3.1, -3, 0.2, -3.46 + tx, 1.27 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);*/
   
}
void train_body_mid(float tx, float ty, float tz, unsigned int& cubeVAO, Shader& lightingShader)
{
    color_train_body(0.2, 11, 2, -3.03 + tx, .17 + ty, 4.65 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(0.2, 11, -2, -3.03 + tx, .17 + ty, 5.35 + tz, 0, 0, 0, cubeVAO, lightingShader);
    color_train_body(0.2, -2, 4, -3.03 + tx, 1.27 + ty, 4.85 + tz, 0, 0, 0, cubeVAO, lightingShader);
}


//struct Vertex {
//    glm::vec3 position;
//    glm::vec3 normal;
//};
//
//
//void generateUmbrellaCanopyVertices(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, glm::vec3 color, float rx, float ry, float rz) {
//
//    //std::vector<Vertex> vertices;
//    //std::vector<GLuint> indices;
//
//    //int numLatitudes = 16;
//    //int numLongitudes = 32;
//    //float canopyRadius = 2.0f;
//
//    //for (int i = 0; i <= numLatitudes; ++i) {
//    //    float lat0 = M_PI * (-0.5 + (float)(i - 1) / numLatitudes);
//    //    float z0 = sin(lat0) * canopyRadius;
//    //    float cosLat0 = cos(lat0);
//
//    //    float lat1 = M_PI * (-0.5 + (float)i / numLatitudes);
//    //    float z1 = sin(lat1) * canopyRadius;
//    //    float cosLat1 = cos(lat1);
//
//    //    for (int j = 0; j <= numLongitudes; ++j) {
//    //        float lng = 2 * M_PI * (float)(j - 1) / numLongitudes;
//    //        float x = cos(lng) * canopyRadius * cosLat1;
//    //        float y = sin(lng) * canopyRadius * cosLat1;
//
//    //        // Calculate vertex normal
//    //        glm::vec3 vertexNormal = glm::normalize(glm::vec3(x, y, z1));
//
//    //        Vertex vertex = { {x, y, z1}, vertexNormal };
//    //        vertices.push_back(vertex);
//
//    //        vertex = { {x, y, z0}, vertexNormal };
//    //        vertices.push_back(vertex);
//    //    }
//    //}
//
//    //for (int i = 0; i < numLatitudes; ++i) {
//    //    for (int j = 0; j < numLongitudes; ++j) {
//    //        int nextColumn = 2 * numLongitudes + 2;
//    //        int nextRow = 2 * (numLongitudes + 1);
//
//    //        indices.push_back(i * nextColumn + j * 2);
//    //        indices.push_back(i * nextColumn + j * 2 + 1);
//    //        indices.push_back((i + 1) * nextColumn + j * 2);
//
//    //        indices.push_back(i * nextColumn + j * 2 + 1);
//    //        indices.push_back((i + 1) * nextColumn + j * 2 + 1);
//    //        indices.push_back((i + 1) * nextColumn + j * 2);
//    //    }
//    //}
//
//
//    std::vector<Vertex> vertices;
//    std::vector<GLuint> indices;
//
//    int numLatitudes = 36;
//    int numLongitudes = 16;
//    float canopyRadius = 2.0f;
//
//    for (int i = 0; i < numLatitudes; ++i) {  // Adjusted loop condition
//        float lat0 = M_PI * (-0.5 + (float)i / numLatitudes);
//        float z0 = sin(lat0) * canopyRadius;
//        float cosLat0 = cos(lat0);
//
//        float lat1 = M_PI * (-0.5 + (float)(i + 1) / numLatitudes);  // Adjusted loop condition
//        float z1 = sin(lat1) * canopyRadius;
//        float cosLat1 = cos(lat1);
//
//        for (int j = 0; j <= numLongitudes; ++j) {
//            float lng = 2 * M_PI * (float)j / numLongitudes;
//            float x = cos(lng) * canopyRadius * cosLat1;
//            float y = sin(lng) * canopyRadius * cosLat1;
//
//            // Calculate vertex normal
//            glm::vec3 vertexNormal = glm::normalize(glm::vec3(x, y, z1));
//
//            Vertex vertex = { {x, y, z1}, vertexNormal };
//            vertices.push_back(vertex);
//
//            vertex = { {x, y, z0}, vertexNormal };
//            vertices.push_back(vertex);
//        }
//    }
//
//    for (int i = 0; i < numLatitudes - 1; ++i) {  // Adjusted loop condition
//        for (int j = 0; j < numLongitudes; ++j) {
//            int nextColumn = (2 * numLongitudes + 2);
//            int nextRow = (2 * (numLongitudes + 1));
//
//            indices.push_back(i * nextColumn + j * 2);
//            indices.push_back(i * nextColumn + j * 2 + 1);
//            indices.push_back((i + 1) * nextColumn + j * 2);
//
//            indices.push_back(i * nextColumn + j * 2 + 1);
//            indices.push_back((i + 1) * nextColumn + j * 2 + 1);
//            indices.push_back((i + 1) * nextColumn + j * 2);
//        }
//    }
//    
//
//
//
//
//
//    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
//
//    unsigned int VBO, VAO, EBO;
//
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
//
//    // Position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    // Normal attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)12);
//    glEnableVertexAttribArray(1);
//
//    // activate shader
//    lightingShader.use();
//
//    // Modelling Transformation
//    glm::mat4 identityMatrix = glm::mat4(1.0f);
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//
//    // Set up transformation matrices as needed...
//
//    lightingShader.setVec3("viewPos", camera.Position);
//
//    // point light 1
//    pointlight1.setUpPointLight(lightingShader);
//    // point light 2
//    pointlight2.setUpPointLight(lightingShader);
//    // point light 3
//    pointlight3.setUpPointLight(lightingShader);
//    //// point light 4
//    pointlight4.setUpPointLight(lightingShader);
//    pointlight5.setUpPointLight(lightingShader);
//    /* pointlight6.setUpPointLight(lightingShader);
//    pointlight7.setUpPointLight(lightingShader);
//    pointlight8.setUpPointLight(lightingShader);*/
//
//
//    spotlight1.setUpSpotLight(lightingShader);
//    directionlight1.setUpDirectionalLight(lightingShader);
//
//    // activate shader
//    lightingShader.use();
//
//    // pass projection matrix to shader (note that in this case it could change every frame)
//    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//    //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
//    lightingShader.setMat4("projection", projection);
//
//    // camera/view transformation
//    glm::mat4 view = camera.GetViewMatrix();
//    //glm::mat4 view = basic_camera.createViewMatrix();
//    lightingShader.setMat4("view", view);
//
//
//    // Set other uniform variables, projection, view, and other settings...
//
//   //floor_black_color_cube
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
//    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rx), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + ry), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rz), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
//    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//
//    lightingShader.setMat4("model", model);
//
//    lightingShader.setVec3("material.ambient", color);
//    lightingShader.setVec3("material.diffuse", color);
//    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
//    lightingShader.setFloat("material.shininess", 32.0f);
//
//   
//
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
//
//
//
//}

struct Vertex {
    float x, y, z;
};

void generateUmbrellaCanopyVertices(float s_x, float s_y, float s_z, float t_x, float t_y, float t_z, glm::vec3 color, float rx, float ry, float rz) {

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    int numLatitudes = 36;
    int numLongitudes = 36;
    float canopyRadius = 2.0f;

    for (int i = 0; i < numLatitudes; ++i) {  // Adjusted loop condition
        float lat0 = M_PI * (-0.5 + (float)i / numLatitudes);
        float z0 = sin(lat0) * canopyRadius;
        float cosLat0 = cos(lat0);

        float lat1 = M_PI * (-0.5 + (float)(i + 1) / numLatitudes);  // Adjusted loop condition
        float z1 = sin(lat1) * canopyRadius;
        float cosLat1 = cos(lat1);

        for (int j = 0; j <= numLongitudes; ++j) {
            float lng = 2 * M_PI * (float)j / numLongitudes;
            float x = cos(lng) * canopyRadius * cosLat1;
            float y = sin(lng) * canopyRadius * cosLat1;

            Vertex vertex = { x, y, z1 };
            vertices.push_back(vertex);
            // Add color attribute to the vertices
            vertex = { color.r, color.g, color.b };
            vertices.push_back(vertex);

            vertex = { x, y, z0 };
            vertices.push_back(vertex);
            // Add color attribute to the vertices
            vertex = { color.r, color.g, color.b };
            vertices.push_back(vertex);
        }
    }

    for (int i = 0; i < numLatitudes - 1; ++i) {  // Adjusted loop condition
        for (int j = 0; j < numLongitudes; ++j) {
            int nextColumn = (2 * numLongitudes + 2);
            int nextRow = (2 * (numLongitudes + 1));

            indices.push_back(i * nextColumn + j * 2);
            indices.push_back(i * nextColumn + j * 2 + 1);
            indices.push_back((i + 1) * nextColumn + j * 2);

            indices.push_back(i * nextColumn + j * 2 + 1);
            indices.push_back((i + 1) * nextColumn + j * 2 + 1);
            indices.push_back((i + 1) * nextColumn + j * 2);
        }
    }



    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size(), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size(), indices.data(), GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + t_x, translate_Y + t_y, translate_Z + t_z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rx), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + ry), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rz), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * s_x, scale_Y * s_y, scale_Z * s_z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLE_FAN, indices.size(), GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);


}

void leaf(float sx, float sy, float sz, float tx, float ty, float tz, float rx, float ry, float rz, unsigned int& cubeVAO, Shader& lightingShader)
{
    color_green((4 + 6) + sx, (4 + 6) + sy, (4 + 6) + sz, 14+0.2+tx, 2.9+ty, 3 - 0.5+tz, 0+rx, 0+ry, 0+rz, cubeVAO, lightingShader);
    color_green((4 + 6) + sx, (4 + 6) + sy, (4 + 6) + sz, 14 + 0.2+ tx, 3.0 + ty, 3 - 0.5 + tz, 0 + rx, 0 + ry, 30 + rz, cubeVAO, lightingShader);
    color_green((4 + 6) + sx, (4 + 6) + sy, (4 + 6) + sz, 14 + 0.2 + tx, 3.2 + ty, 3 - 0.5 + tz, 0 + rx, 0 + ry, -30 + rz, cubeVAO, lightingShader);
    color_green((4 + 6) + sx, (4 + 6) + sy, (4 + 6) + sz, 14 + 0.2 + tx, 3.2 + ty, 3 - 0.5 + tz, 30 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_green((4 + 6) + sx, (4 + 6) + sy, (4 + 6) + sz, 14 + 0.2 + tx, 3.2 + ty, 3 - 0.5 + tz, -30 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_green((4 + 6) + sx, (4 + 6) + sy, (4 + 6) + sz, 14 + 0.2 + tx, 3.2 + ty, 3 - 0.5 + tz, 0 + rx, 30 + ry, 0 + rz, cubeVAO, lightingShader);
    color_green((4 + 6) + sx, (4 + 6) + sy, (4 + 6) + sz, 14 + 0.2 + tx, 3.2 + ty, 3 - 0.5 + tz, 0 + rx, -0 + ry, 0 + rz, cubeVAO, lightingShader);
}
void tree_1(float sx, float sy, float sz, float tx, float ty, float tz, float rx, float ry, float rz, unsigned int& cubeVAO, Shader& lightingShader)
{
    Cylinder cylinder = Cylinder();

    cylinder.drawSphere(lightingShader, model_Matrix(1*sx, 0.5*sy, 1*sz, 14+tx, 0.01+ty, 3+tz, 0+rx, 0+ry, 0+rz));
    cylinder.drawSphere(lightingShader, model_Matrix(0.6 * sx, 0.5*sy, 0.6*sz, 14+tx, 1.7+ty, 3+tz, 0+rx, 0+ry, 30+rz));
    cylinder.drawSphere(lightingShader, model_Matrix(0.6 * sx, 0.5*sy, 0.6*sz, 14+tx, 1.7+ty, 3+tz, 0+rx, 0+ry, -30+rz));
    
}



//void generateWheel(float sx, float sy, float sz, float tx, float ty, float tz) {
//
//    wheelVertices.clear();
//    wheelVertices2.clear();
//    wheelVertices3.clear();
//   
//
//
//    const int numSegments = 36; // Number of segments in the wheel
//    const float radius = 1.0f;  // Radius of the wheel
//
//    // Specify color (black)
//    float r = 0.0f;
//    float g = 0.0f;
//    float b = 0.0f;
//
//    // Specify origin (center)
//    float originX = 0.0f;
//    float originY = 0.0f;
//    float originZ = 0.0f;
//
//    // Center vertex
//    wheelVertices.push_back(originX);
//    wheelVertices2.push_back(originX);
//    wheelVertices.push_back(originY);
//    wheelVertices2.push_back(originY);
//    wheelVertices.push_back(originZ);
//    wheelVertices2.push_back(0.5f);
//
//    // Color for the center vertex
//    wheelVertices.push_back(0.0f);
//    wheelVertices2.push_back(0.0f);
//    wheelVertices.push_back(0.0f);
//    wheelVertices2.push_back(0.0f);
//    wheelVertices.push_back(0.5f);
//    wheelVertices2.push_back(-0.5f);
//
//    // Generate vertices and indices for the wheel
//    for (int i = 1; i <= numSegments; i++) {
//        float angle = (2 * M_PI * i) / numSegments;
//        float x = originX + radius * cos(angle);
//        float y = originY + radius * sin(angle);
//
//        // Vertex
//        wheelVertices.push_back(x);
//        wheelVertices2.push_back(x);
//        wheelVertices.push_back(y);
//        wheelVertices2.push_back(y);
//        wheelVertices.push_back(originZ);
//        wheelVertices2.push_back(0.5f);
//
//        // Color for the vertex
//        wheelVertices.push_back(0.0f);
//        wheelVertices2.push_back(0.0f);
//        wheelVertices.push_back(0.0f);
//        wheelVertices2.push_back(0.0f);
//        wheelVertices.push_back(0.5f);
//        wheelVertices2.push_back(-0.5f);
//
//        // Indices for the triangle fan
//        wheelIndices.push_back(0);
//        wheelIndices2.push_back(0);
//        wheelIndices.push_back(i);
//        wheelIndices2.push_back(i);
//        wheelIndices.push_back((i + 1) % (numSegments + 1));
//        wheelIndices2.push_back((i + 1) % (numSegments + 1));
//    }
//    // Indices for the triangle fan
//    wheelIndices.push_back(0);
//    wheelIndices2.push_back(0);
//    wheelIndices.push_back(36);
//    wheelIndices2.push_back(36);
//    wheelIndices.push_back(1);
//    wheelIndices2.push_back(1);
//
//
//
//    for (int i = 0; i < wheelVertices.size(); i++)
//    {
//        wheelVertices3.push_back(wheelVertices[i]);
//    }
//    for (int i = 0; i < wheelVertices2.size(); i++)
//    {
//        wheelVertices3.push_back(wheelVertices2[i]);
//    }
//    for (int i = 1; i < wheelVertices2.size(); i++)
//    {
//        if (i < 36)
//        {
//
//            wheelIndices3.push_back(i);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 1);
//
//            wheelIndices3.push_back(i + 1);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 37 + 1);
//        }
//        else if (i == 36)
//        {
//            wheelIndices3.push_back(i);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 1);
//
//            wheelIndices3.push_back(i + 1);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 37 + 1);
//        }
//    }
//
//   /* cout << wheelVertices.size() << endl;
//    cout << wheelVertices2.size() << endl;
//    cout << wheelVertices3.size() << endl;
//
//    for (int i = 0,j=6,k=12; i < wheelVertices3.size(); i+=18,j+=18,k+=18)
//    {
//
//        float x1 = wheelVertices3[i];
//        float y1 = wheelVertices3[i+1];
//        float z1 = wheelVertices3[i];
//
//        float x2 = wheelVertices3[i];
//        float y2 = wheelVertices3[i ];
//        float z2 = wheelVertices3[i]; 
//
//        float x3 = wheelVertices3[i];
//        float y3 = wheelVertices3[i];
//        float z3 = wheelVertices3[i];
//
//
//        vector<float>normals = computeFaceNormal(x1, y1, z1, x2, y2, z2, x3, y3, z3);
//
//        wheelVertices4.push_back(x1);
//        wheelVertices4.push_back(y1);
//        wheelVertices4.push_back(z1);
//        wheelVertices4.push_back(normals[0]);
//        wheelVertices4.push_back(normals[1]);
//        wheelVertices4.push_back(normals[2]);
//        
//        wheelVertices4.push_back(x2);
//        wheelVertices4.push_back(y2);
//        wheelVertices4.push_back(z2);
//        wheelVertices4.push_back(normals[0]);
//        wheelVertices4.push_back(normals[1]);
//        wheelVertices4.push_back(normals[2]);
//
//        wheelVertices4.push_back(x3);
//        wheelVertices4.push_back(y3);
//        wheelVertices4.push_back(z3);
//        wheelVertices4.push_back(normals[0]);
//        wheelVertices4.push_back(normals[1]);
//        wheelVertices4.push_back(normals[2]);
//
//
//    }
//*/
//
//
//
//
//
//   Shader ourShader("vertexShader.vs", "fragmentShader.fs");
//
//
//    unsigned int VBO, VAO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, wheelVertices.size(), wheelVertices.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wheelIndices.size(), wheelIndices.data(), GL_STATIC_DRAW);
//
//    //// position attribute
//    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    //glEnableVertexAttribArray(0);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//
//
//    unsigned int VBO2, VAO2, EBO2;
//    glGenVertexArrays(1, &VAO2);
//    glGenBuffers(1, &VBO2);
//    glGenBuffers(1, &EBO2);
//
//    glBindVertexArray(VAO2);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//    glBufferData(GL_ARRAY_BUFFER, wheelVertices2.size(), wheelVertices2.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wheelIndices2.size(), wheelIndices2.data(), GL_STATIC_DRAW);
//
//
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//
//
//    unsigned int VBO3, VAO3, EBO3;
//    glGenVertexArrays(1, &VAO3);
//    glGenBuffers(1, &VBO3);
//    glGenBuffers(1, &EBO3);
//
//    glBindVertexArray(VAO3);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
//    glBufferData(GL_ARRAY_BUFFER, wheelVertices4.size(), wheelVertices4.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wheelIndices3.size(), wheelIndices3.data(), GL_STATIC_DRAW);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//
//
//    //// Modelling Transformation
//    //glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    //glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//
//    ////floor_black_color_cube
//    //translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + tx, translate_Y + ty, translate_Z + tz));
//    //rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
//    //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//    //rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
//    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * sx, scale_Y * sy, scale_Z * sz));
//    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    //ourShader.setMat4("model", model);
//
//    //glBindVertexArray(VAO);
//    //glDrawElements(GL_TRIANGLES, wheelIndices.size(), GL_UNSIGNED_INT, 0);
//    //glDeleteVertexArrays(1, &VAO);
//    //glDeleteBuffers(1, &VBO);
//    //glDeleteBuffers(1, &EBO);
//
//
//    //glBindVertexArray(VAO2);
//    //glDrawElements(GL_TRIANGLES, wheelIndices2.size(), GL_UNSIGNED_INT, 0);
//    //glDeleteVertexArrays(1, &VAO2);
//    //glDeleteBuffers(1, &VBO2);
//    //glDeleteBuffers(1, &EBO2);
//
//    //glBindVertexArray(VAO3);
//    //glDrawElements(GL_TRIANGLES, wheelIndices3.size(), GL_UNSIGNED_INT, 0);
//    //glDeleteVertexArrays(1, &VAO3);
//    //glDeleteBuffers(1, &VBO3);
//    //glDeleteBuffers(1, &EBO3);
//
//  
//
//}

void trainBody(float sx, float sy, float sz, float tx, float ty, float tz, float rx, float ry, float rz, unsigned int& cubeVAO, Shader& lightingShader)
{
    train_body(0+tx, 0+ty, 0+tz, cubeVAO, lightingShader);
    color_train_body(0.2+sx, 6+sy, -6.8+sz, -3.63+tx, .17+ty, 5.35+tz, 0+rx, 0+ry, 0+rz, cubeVAO, lightingShader);
    color_red(0.2 + sx, -2 + sy, -6.8 + sz, -3.63 + tx, 1.27 + ty, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_red(0.2 + sx, 2 + sy, -3.5 + sz, -3.63 + tx, 0.77 + ty, 4.9 + tz, 90 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);


    color_train_body(3.2 + sx, 5 + sy, 0.2 + sz, -3.46 + tx, .17 + ty, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);


    color_train_body(0.2 + sx, 11 + sy, 2 + sz, -3.03 + tx, .17 + ty, 4.65 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_train_body(0.2 + sx, 11 + sy, -2 + sz, -3.03 + tx, .17 + ty, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_train_body(0.2 + sx, -11 + sy, 4 + sz, -3.03 + tx, 1.27 + ty, 4.85 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);

    train_body(2.5 + tx, 0 + ty, 0 + tz, cubeVAO, lightingShader);
    train_body_mid(2.0 + tx, 0 + ty, 0 + tz, cubeVAO, lightingShader);
    train_body_mid(2.5 + tx, 0 + ty, 0 + tz, cubeVAO, lightingShader);





    train_body(5 + tx, 0 + ty, 0 + tz, cubeVAO, lightingShader);
    train_body_mid(4.5 + tx, 0 + ty, 0 + tz, cubeVAO, lightingShader);
    train_body_mid(5.0 + tx, 0 + ty, 0 + tz, cubeVAO, lightingShader);

    train_body(7.5 + tx, 0 + ty, 0 + tz, cubeVAO, lightingShader);
    train_body_mid(7.0 + tx, 0 + ty, 0 + tz, cubeVAO, lightingShader);
    train_body_mid(7.5 + tx, 0 + ty, 0 + tz, cubeVAO, lightingShader);



    color_white(-6.5 + sx, 0.1 + sy, 7 + sz, -3.03 + 10.08 + tx, .17 + ty, 4.65 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_train_body(-2.0 + sx, 6 + sy, 0.2 + sz, -2.95 + 10 + tx, .17 + ty, 4.65 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_train_body(1.7 + sx, 6 + sy, 0.2 + sz, -3.63 + 10 + tx, .17 + ty, 4.65 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_train_body(3.2 + sx, 5 + sy, 0.2 + sz, -3.46 + 10 + tx, .17 + ty, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    //color_train_body(2.0, 5,0.1, -3.43, .17, 4.65, 0, 0, 0, cubeVAO, lightingShader);

    color_red(-2.0 + sx, -5 + sy, 0.2 + sz, -2.95 + 10 + tx, 1.27 + ty, 4.65 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_red(1.7 + sx, -5 + sy, 0.2 + sz, -3.63 + 10 + tx, 1.27 + ty, 4.65 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_red(3.1 + sx, -2 + sy, 0.2 + sz, -3.46 + 10 + tx, 1.27 + ty, 4.65 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);

    /* color_train_body(-2.0, -5, 0.2, -2.95 + 10, 1.27, 4.65, 0, 0, 0, cubeVAO, lightingShader);
     color_train_body(1.7, -5, 0.2, -3.63 + 10, 1.27, 4.65, 0, 0, 0, cubeVAO, lightingShader);
     color_train_body(3.1, -2, 0.2, -3.46 + 10, 1.27, 4.65, 0, 0, 0, cubeVAO, lightingShader);*/

    platform_color(-6.5 + sx, 0.1 + sy, 7.2 + sz, -3.03 + 10.08 + tx, 1.27 + ty, 4.65 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);

    color_train_body(-2.0 + sx, 6 + sy, 0.2 + sz, -2.95 + 10 + tx, .17 + ty, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_train_body(1.7 + sx, 6 + sy, 0.2 + sz, -3.63 + 10 + tx, .17, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);

    color_red(-2.0 + sx, -5 + sy, 0.2 + sz, -2.95 + 10 + tx, 1.27 + ty, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_red(1.7 + sx, -5 + sy, 0.2 + sz, -3.63 + 10 + tx, 1.27 + ty, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_red(3.1 + sx, -3 + sy, 0.2 + sz, -3.46 + 10 + tx, 1.27 + ty, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);


    /*   color_train_body(-2.0, -5, 0.2, -2.95 + 10, 1.27, 5.35, 0, 0, 0, cubeVAO, lightingShader);
       color_train_body(1.7, -5, 0.2, -3.63 + 10, 1.27, 5.35, 0, 0, 0, cubeVAO, lightingShader);
       color_train_body(3.1, -3, 0.2, -3.46 + 10, 1.27, 5.35, 0, 0, 0, cubeVAO, lightingShader);*/

    train_body_mid(9.5 + tx, 0 + ty, 0, cubeVAO, lightingShader);
    color_train_body(0.2 + sx, -11 + sy, 4 + sz, -3.03 + 9.5 + tx, 1.27 + ty, 4.85 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);

    color_train_body(0.2 + sx, 6 + sy, -6.8 + sz, -3.63 + 10.66 + tx, .17 + ty, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_red(0.2 + sx, -2 + sy, -6.8 + sz, -3.63 + 10.66 + tx, 1.27 + ty, 5.35 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
    color_red(0.2 + sx, 2 + sy, -3.5 + sz, -3.63 + 10.66 + tx, 0.77 + ty, 4.9 + tz, 90 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
}

void trainWheel(float sx, float sy, float sz, float tx, float ty, float tz, float rx, float ry, float rz, unsigned int& cubeVAO, Shader& lightingShader)
{
    for (int i = 0; i < 360; i++)
    {
        color_black(.4, .4, 0.2, -3+tx, .09+ty, 4.83+tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.2, .2, 0.4, -3 + tx, .09 + ty, 4.831 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

        color_black(.2, .2, 0.4, -3 + tx, .09 + ty, 5.11 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.4, .4, 0.2, -3 + tx, .09 + ty, 5.14 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

    }
    //cylinder2.drawSphere(lightingShader, model_Matrix(0.5, 0.01, 0.5, -3, .09, 4.83, 0, 90, 90));

    color_white(0.6, 0.8, 2.6, -3.03 + tx, .09 + ty, 4.865 + tz, 0, 0, 0, cubeVAO, lightingShader);



    for (int i = 0; i < 360; i++)
    {
        color_black(.4, .4, 0.2, -1 + tx, .09 + ty, 4.83 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.2, .2, 0.4, -1 + tx, .09 + ty, 4.831 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

        color_black(.2, .2, 0.4, -1 + tx, .09 + ty, 5.11 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.4, .4, 0.2, -1 + tx, .09 + ty, 5.14 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

    }

    color_white(0.6, 0.8, 2.6, -1.03 + tx, .09 + ty, 4.865 + tz, 0, 0, 0, cubeVAO, lightingShader);


    for (int i = 0; i < 360; i++)
    {
        color_black(.4, .4, 0.2, -0.5 + tx, .09 + ty, 4.83 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.2, .2, 0.4, -0.5 + tx, .09 + ty, 4.831 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

        color_black(.2, .2, 0.4, -0.5 + tx, .09 + ty, 5.11 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.4, .4, 0.2, -0.5 + tx, .09 + ty, 5.14 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

    }
    color_white(0.6, 0.8, 2.6, -0.53 + tx, .09 + ty, 4.865 + tz, 0, 0, 0, cubeVAO, lightingShader);

    for (int i = 0; i < 360; i++)
    {
        color_black(.4, .4, 0.2, 1.5 + tx, .09 + ty, 4.83 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.2, .2, 0.4, 1.5 + tx, .09 + ty, 4.831 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

        color_black(.2, .2, 0.4, 1.5 + tx, .09 + ty, 5.11 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.4, .4, 0.2, 1.5 + tx, .09 + ty, 5.14 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

    }
    color_white(0.6, 0.8, 2.6, 1.47 + tx, .09 + ty, 4.865 + tz, 0, 0, 0, cubeVAO, lightingShader);

    for (int i = 0; i < 360; i++)
    {
        color_black(.4, .4, 0.2, 2 + tx, .09 + ty, 4.83 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.2, .2, 0.4, 2 + tx, .09 + ty, 4.831 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

        color_black(.2, .2, 0.4, 2 + tx, .09 + ty, 5.11 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.4, .4, 0.2, 2 + tx, .09 + ty, 5.14 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

    }
    color_white(0.6, 0.8, 2.6, 1.97 + tx, .09 + ty, 4.865 + tz, 0, 0, 0, cubeVAO, lightingShader);
    for (int i = 0; i < 360; i++)
    {
        color_black(.4, .4, 0.2, 4 + tx, .09 + ty, 4.83 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.2, .2, 0.4, 4 + tx, .09 + ty, 4.831 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

        color_black(.2, .2, 0.4, 4 + tx, .09 + ty, 5.11 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.4, .4, 0.2, 4 + tx, .09 + ty, 5.14 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

    }
    color_white(0.6, 0.8, 2.6, 3.97 + tx, .09 + ty, 4.865 + tz, 0, 0, 0, cubeVAO, lightingShader);

    for (int i = 0; i < 360; i++)
    {
        color_black(.4, .4, 0.2, 6 + tx, .09 + ty, 4.83 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.2, .2, 0.4, 6 + tx, .09 + ty, 4.831 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

        color_black(.2, .2, 0.4, 6 + tx, .09 + ty, 5.11 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);
        color_black(.4, .4, 0.2, 6 + tx, .09 + ty, 5.14 + tz, 0, 0, 0 + i, cubeVAO, lightingShader);

    }
    color_white(0.6, 0.8, 2.6, 5.97 + tx, .09 + ty, 4.865 + tz, 0, 0, 0, cubeVAO, lightingShader);



}


//void setUpWheelVertexData()
//{
//    const int numSegments = 36; // Number of segments in the wheel
//    const float radius = 1.0f;  // Radius of the wheel
//
//    // Specify color (black)
//    float r = 1.0f;
//    float g = 0.0f;
//    float b = 1.0f;
//
//    // Specify origin (center)
//    float originX = 0.0f;
//    float originY = 0.0f;
//    float originZ = 0.0f;
//
//    // Center vertex
//    wheelVertices.push_back(originX);
//    wheelVertices2.push_back(originX);
//    wheelVertices.push_back(originY);
//    wheelVertices2.push_back(originY);
//    wheelVertices.push_back(originZ);
//    wheelVertices2.push_back(0.5f);
//
//    // Color for the center vertex
//    wheelVertices.push_back(r);
//    wheelVertices2.push_back(r);
//    wheelVertices.push_back(g);
//    wheelVertices2.push_back(g);
//    wheelVertices.push_back(b);
//    wheelVertices2.push_back(b);
//
//    // Generate vertices and indices for the wheel
//    for (int i = 1; i <= numSegments; i++) {
//        float angle = (2 * M_PI * i) / numSegments;
//        float x = originX + radius * cos(angle);
//        float y = originY + radius * sin(angle);
//
//        // Vertex
//        wheelVertices.push_back(x);
//        wheelVertices2.push_back(x);
//        wheelVertices.push_back(y);
//        wheelVertices2.push_back(y);
//        wheelVertices.push_back(originZ);
//        wheelVertices2.push_back(0.5f);
//
//        // Color for the vertex
//        wheelVertices.push_back(r);
//        wheelVertices2.push_back(r);
//        wheelVertices.push_back(g);
//        wheelVertices2.push_back(g);
//        wheelVertices.push_back(b);
//        wheelVertices2.push_back(b);
//
//        // Indices for the triangle fan
//        wheelIndices.push_back(0);
//        wheelIndices2.push_back(0);
//        wheelIndices.push_back(i);
//        wheelIndices2.push_back(i);
//        wheelIndices.push_back((i + 1) % (numSegments + 1));
//        wheelIndices2.push_back((i + 1) % (numSegments + 1));
//    }
//    // Indices for the triangle fan
//    wheelIndices.push_back(0);
//    wheelIndices2.push_back(0);
//    wheelIndices.push_back(36);
//    wheelIndices2.push_back(36);
//    wheelIndices.push_back(1);
//    wheelIndices2.push_back(1);
//
//
//
//    for (int i = 0; i < wheelVertices.size(); i++)
//    {
//        wheelVertices3.push_back(wheelVertices[i]);
//    }
//    for (int i = 0; i < wheelVertices2.size(); i++)
//    {
//        wheelVertices3.push_back(wheelVertices2[i]);
//    }
//    for (int i = 1; i < wheelVertices2.size(); i++)
//    {
//        if (i < 36)
//        {
//
//            wheelIndices3.push_back(i);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 1);
//
//            wheelIndices3.push_back(i + 1);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 37 + 1);
//        }
//        else if (i == 36)
//        {
//            wheelIndices3.push_back(i);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 1);
//
//            wheelIndices3.push_back(i + 1);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 37 + 1);
//        }
//    }
//
//    cout << wheelIndices.size() << " " << wheelIndices2.size() << " " << wheelIndices3.size() << endl;
//
//    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
//
//    unsigned int VBO1, EBO1;
//    glGenVertexArrays(1, &VAO1);
//    glGenBuffers(1, &VBO1);
//    glGenBuffers(1, &EBO1);
//
//    glBindVertexArray(VAO1);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
//    glBufferData(GL_ARRAY_BUFFER, wheelVertices.size(), wheelVertices.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO1);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wheelIndices.size(), wheelIndices.data(), GL_STATIC_DRAW);
//
//    //// position attribute
//    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    //glEnableVertexAttribArray(0);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//
//
//    unsigned int VBO2, EBO2;
//    glGenVertexArrays(1, &VAO2);
//    glGenBuffers(1, &VBO2);
//    glGenBuffers(1, &EBO2);
//
//    glBindVertexArray(VAO2);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//    glBufferData(GL_ARRAY_BUFFER, wheelVertices2.size(), wheelVertices2.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wheelIndices2.size(), wheelIndices2.data(), GL_STATIC_DRAW);
//
//
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//
//
//    unsigned int VBO3, EBO3;
//    glGenVertexArrays(1, &VAO3);
//    glGenBuffers(1, &VBO3);
//    glGenBuffers(1, &EBO3);
//
//    glBindVertexArray(VAO3);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
//    glBufferData(GL_ARRAY_BUFFER, wheelVertices3.size(), wheelVertices3.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wheelIndices3.size(), wheelIndices3.data(), GL_STATIC_DRAW);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//
//   
//}



//std::vector<float> wheelVertices, wheelVertices2, wheelVertices3;
//std::vector<unsigned int> wheelIndices, wheelIndices2, wheelIndices3;
//
//void generateWheel2(float sx, float sy, float sz, float tx, float ty, float tz,float rx, float ry,float rz) {
//  
//    const int numSegments = 36; // Number of segments in the wheel
//    const float radius = 1.0f;  // Radius of the wheel
//
//    // Specify color (black)
//    float r = 1.0f;
//    float g = 0.0f;
//    float b = 1.0f;
//
//    // Specify origin (center)
//    float originX = 0.0f;
//    float originY = 0.0f;
//    float originZ = 0.0f;
//
//    // Center vertex
//    wheelVertices.push_back(originX);
//    wheelVertices2.push_back(originX);
//    wheelVertices.push_back(originY);
//    wheelVertices2.push_back(originY);
//    wheelVertices.push_back(originZ);
//    wheelVertices2.push_back(0.5f);
//
//    // Color for the center vertex
//    wheelVertices.push_back(r);
//    wheelVertices2.push_back(r);
//    wheelVertices.push_back(g);
//    wheelVertices2.push_back(g);
//    wheelVertices.push_back(b);
//    wheelVertices2.push_back(b);
//
//    // Generate vertices and indices for the wheel
//    for (int i = 1; i <= numSegments; i++) {
//        float angle = (2 * M_PI * i) / numSegments;
//        float x = originX + radius * cos(angle);
//        float y = originY + radius * sin(angle);
//
//        // Vertex
//        wheelVertices.push_back(x);
//        wheelVertices2.push_back(x);
//        wheelVertices.push_back(y);
//        wheelVertices2.push_back(y);
//        wheelVertices.push_back(originZ);
//        wheelVertices2.push_back(0.5f);
//
//        // Color for the vertex
//        wheelVertices.push_back(r);
//        wheelVertices2.push_back(r);
//        wheelVertices.push_back(g);
//        wheelVertices2.push_back(g);
//        wheelVertices.push_back(b);
//        wheelVertices2.push_back(b);
//
//        // Indices for the triangle fan
//        wheelIndices.push_back(0);
//        wheelIndices2.push_back(0);
//        wheelIndices.push_back(i);
//        wheelIndices2.push_back(i);
//        wheelIndices.push_back((i + 1) % (numSegments + 1));
//        wheelIndices2.push_back((i + 1) % (numSegments + 1));
//    }
//    // Indices for the triangle fan
//    wheelIndices.push_back(0);
//    wheelIndices2.push_back(0);
//    wheelIndices.push_back(36);
//    wheelIndices2.push_back(36);
//    wheelIndices.push_back(1);
//    wheelIndices2.push_back(1);
//
//
//
//    for (int i = 0; i < wheelVertices.size(); i++)
//    {
//        wheelVertices3.push_back(wheelVertices[i]);
//    }
//    for (int i = 0; i < wheelVertices2.size(); i++)
//    {
//        wheelVertices3.push_back(wheelVertices2[i]);
//    }
//    for (int i = 1; i < wheelVertices2.size(); i++)
//    {
//        if (i < 36)
//        {
//
//            wheelIndices3.push_back(i);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 1);
//
//            wheelIndices3.push_back(i + 1);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 37 + 1);
//        }
//        else if (i == 36)
//        {
//            wheelIndices3.push_back(i);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 1);
//
//            wheelIndices3.push_back(i + 1);
//            wheelIndices3.push_back(i + 37);
//            wheelIndices3.push_back(i + 37 + 1);
//        }
//    }
//
//    cout << wheelIndices.size() << " " << wheelIndices2.size() << " " << wheelIndices3.size() << endl;
//
//    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
//
//    unsigned int VBO, VAO, EBO;
//    glGenVertexArrays(1, &VAO);
//    glGenBuffers(1, &VBO);
//    glGenBuffers(1, &EBO);
//
//    glBindVertexArray(VAO);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO);
//    glBufferData(GL_ARRAY_BUFFER, wheelVertices.size(), wheelVertices.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wheelIndices.size(), wheelIndices.data(), GL_STATIC_DRAW);
//
//    //// position attribute
//    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//    //glEnableVertexAttribArray(0);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//
//
//    unsigned int VBO2, VAO2, EBO2;
//    glGenVertexArrays(1, &VAO2);
//    glGenBuffers(1, &VBO2);
//    glGenBuffers(1, &EBO2);
//
//    glBindVertexArray(VAO2);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
//    glBufferData(GL_ARRAY_BUFFER, wheelVertices2.size(), wheelVertices2.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wheelIndices2.size(), wheelIndices2.data(), GL_STATIC_DRAW);
//
//
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//
//
//    unsigned int VBO3, VAO3, EBO3;
//    glGenVertexArrays(1, &VAO3);
//    glGenBuffers(1, &VBO3);
//    glGenBuffers(1, &EBO3);
//
//    glBindVertexArray(VAO3);
//
//    glBindBuffer(GL_ARRAY_BUFFER, VBO3);
//    glBufferData(GL_ARRAY_BUFFER, wheelVertices3.size(), wheelVertices3.data(), GL_STATIC_DRAW);
//
//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO3);
//    glBufferData(GL_ELEMENT_ARRAY_BUFFER, wheelIndices3.size(), wheelIndices3.data(), GL_STATIC_DRAW);
//
//    // position attribute
//    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//    glEnableVertexAttribArray(0);
//
//    //color attribute
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//    glEnableVertexAttribArray(1);
//
//
//    // Modelling Transformation
//    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//
//    //floor_black_color_cube
//    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + tx, translate_Y + ty, translate_Z + tz));
//    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f+rx), glm::vec3(1.0f, 0.0f, 0.0f));
//    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f+ry), glm::vec3(0.0f, 1.0f, 0.0f));
//    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f+rz), glm::vec3(0.0f, 0.0f, 1.0f));
//    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * sx, scale_Y * sy, scale_Z * sz));
//    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//    ourShader.setMat4("model", model);
//
//    glBindVertexArray(VAO);
//    glDrawElements(GL_TRIANGLES, 444, GL_UNSIGNED_INT, 0);
//    glDeleteVertexArrays(1, &VAO);
//    glDeleteBuffers(1, &VBO);
//    glDeleteBuffers(1, &EBO);
//
//
//    glBindVertexArray(VAO2);
//    glDrawElements(GL_TRIANGLES, 444, GL_UNSIGNED_INT, 0);
//    glDeleteVertexArrays(1, &VAO2);
//    glDeleteBuffers(1, &VBO2);
//    glDeleteBuffers(1, &EBO2);
//
//    glBindVertexArray(VAO3);
//    glDrawElements(GL_TRIANGLES, 864, GL_UNSIGNED_INT, 0);
//    glDeleteVertexArrays(1, &VAO3);
//    glDeleteBuffers(1, &VBO3);
//    glDeleteBuffers(1, &EBO3);
//
//
//
//}
//void trainWheel2(float sx, float sy, float sz, float tx, float ty, float tz, float rx, float ry, float rz, unsigned int& cubeVAO, Shader& lightingShader)
//{
//    color_black(0.01 + sx, 0.01 + sy, 0.01 + sz, -3 + tx, 0.15 + ty, 4.8 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
//    generateWheel2(0.6 + sx, 0.6 + sy, 0.3 + sz, -3 + tx, 0.15 + ty, 4.8 + tz, 0 + rx, 0 + ry, 0 + rz);
//    generateWheel2(0.45 + sx, 0.45 + sy, 0.5 + sz, -3 + tx, 0.15 + ty, 4.8 + 0.02 + tz, 0 + rx, 0 + ry, 0 + rz);
//
//    color_black(0.01 + sx, 0.01 + sy, -0.01 + sz, -3 + tx, 0.15 + ty, 5.3 - 0.056 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
//    generateWheel2(0.6 + sx, 0.6 + sy, -0.3 + sz, -3 + tx, 0.15 + ty, 5.3 - 0.01 - 0.1 + tz, 0 + rx, 0 + ry, 0 + rz);
//    generateWheel2(0.45 + sx, 0.45 + sy, -0.5 + sz, -3 + tx, 0.15 + ty, 5.32 - 0.05 - 0.1 + tz, 0 + rx, 0 + ry, 0 + rz);
//
//    color_black(1 + sx, 1.32 + sy, 2.82 + sz, -3.05 + tx, 0.15 + ty, 4.85 + tz, 0 + rx, 0 + ry, 0 + rz, cubeVAO, lightingShader);
//}



void boat_front(float sx, float sy, float sz, float tx, float ty, float tz, float rx,float ry, float rz)
{
    float cube_vertices[] = {
          0.0f, 0.5f, 0.5f,  0.364f, 0.364f, 0.364f,//0

         -0.5f, 0.0f, 0.0f,  0.364f, 0.364f, 0.364f,//1
          -0.5f, 0.5f, 0.0f,  0.364f, 0.364f, 0.364f,//2

         -0.6f, 0.0f, 0.0f,  0.364f, 0.364f, 0.364f,//3
         -0.6f, 0.5f, 0.0f,  0.364f, 0.364f, 0.364f,//4

         0.5f, 0.0f, 0.0f,  0.364f, 0.364f, 0.364f,//5
         0.5f, 0.5f, 0.0f,  0.364f, 0.364f, 0.364f,//6

         0.6f, 0.0f, 0.0f,  0.364f, 0.364f, 0.364f,//7
         0.6f, 0.5f, 0.0f,  0.364f, 0.364f, 0.364f,//8
        
    };
    unsigned int cube_indices[] = {
      0, 1, 2,
      0, 3, 4,
      0,1,3,
      0,2,4,
      0,5,6,
      0,7,8,
      0,6,8,
      0,5,7,

      0,1,5,
      0,2,6,
     
     
     
    };
    // configure global opengl state
    // -----------------------------


    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);

    // position attribute
   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    //glEnableVertexAttribArray(0);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    //floor_black_color_cube
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X + tx, translate_Y + ty, translate_Z + tz));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rx), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f+ry), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(0.0f + rz), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X * sx, scale_Y * sy, scale_Z * sz));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    ourShader.setMat4("model", model);
   // ourShader.setVec3("color", glm::vec3(0.16f, 0.10f, 0.06f));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, 0);
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void boat(float sx, float sy, float sz, float tx, float ty, float tz, float rx, float ry, float rz, unsigned int& cubeVAO, Shader& lightingShader)
{
    color_boat(10*sx, 2*sy, 0.2*sz, 4+tx+0.76, 5+ty, 6.76+tz+.24, 0+rx, 0+ry+90, 0+rz, cubeVAO, lightingShader);
    boat_front(2*sx, 2*sy, 5 * sz, 4+tx+1, 5+ty, 7+tz, 0 , 0 , 0 );
    boat_front(2 * sx, 2 * sy, 5 * sz, 5 + tx, 5 + ty, 6 + tz, 0, -180, 0 );
    color_boat(10 * sx, 2 * sy, 0.2 * sz, 4 + tx+1.22, 5 + ty, 7.22 + tz-0.22, 0 + rx, 0 + ry+90, 0 + rz, cubeVAO, lightingShader);
    color_boat(-10 * sx, 0.2 * sy, -4.6 * sz, 4 + tx+0.76, 5 + ty, 7.22 + tz-0.22, 0 + rx, 0 + ry-90, 0 + rz, cubeVAO, lightingShader);
}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
     0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f,
     0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -0.5f,
     0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -0.5f,
     0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -0.5f,

     0.5f, 0.0f, 0.0f, 0.5f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f,
     0.5f, 0.0f, 0.5f, 0.5f, 0.0f, 0.0f,
     0.5f, 0.5f, 0.5f, 0.5f, 0.0f, 0.0f,

     0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f,
     0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.5f,
     0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f,
     0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 0.5f,

     0.0f, 0.0f, 0.5f, -0.5f, 0.0f, 0.0f,
     0.0f, 0.5f, 0.5f, -0.5f, 0.0f, 0.0f,
     0.0f, 0.5f, 0.0f, -0.5f, 0.0f, 0.0f,
     0.0f, 0.0f, 0.0f, -0.5f, 0.0f, 0.0f,

     0.5f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f,
     0.5f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
     0.0f, 0.5f, 0.0f, 0.0f, 0.5f, 0.0f,
     0.0f, 0.5f, 0.5f, 0.0f, 0.5f, 0.0f,

     0.0f, 0.0f, 0.0f, 0.0f, -0.5f, 0.0f,
     0.5f, 0.0f, 0.0f, 0.0f, -0.5f, 0.0f,
     0.5f, 0.0f, 0.5f, 0.0f, -0.5f, 0.0f,
     0.0f, 0.0f, 0.5f, 0.0f, -0.5f, 0.0f
    };

    unsigned int cube_indices[] = {
         0, 3, 2,
       2, 1, 0,

       4, 5, 7,
       7, 6, 4,

       8, 9, 10,
       10, 11, 8,

       12, 13, 14,
       14, 15, 12,

       16, 17, 18,
       18, 19, 16,

       20, 21, 22,
       22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //wheel 

   // setUpWheelVertexData();
     



    Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    string diffuseMapPath = "grass.png";
    string specularMapPath = "grass.png";

    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 20.0f, 2.0f);

    string diffuseMapPath2 = "platform_tiles.jpg";
    string specularMapPath2 = "platform_tiles.jpg";

    unsigned int diffMap2 = loadTexture(diffuseMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap2 = loadTexture(specularMapPath2.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube2 = Cube(diffMap2, specMap2, 32.0f, 0.0f, 0.0f, 15.0f, 5.0f);

    string diffuseMapPath3 = "platform_tiles.jpg";
    string specularMapPath3 = "platform_tiles.jpg";

    unsigned int diffMap3 = loadTexture(diffuseMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap3 = loadTexture(specularMapPath3.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube3 = Cube(diffMap3, specMap3, 32.0f, 0.0f, 0.0f, 20.0f, 2.0f);

    string diffuseMapPath4 = "road.jpg";
    string specularMapPath4 = "road.jpg";

    unsigned int diffMap4 = loadTexture(diffuseMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap4 = loadTexture(specularMapPath4.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube4 = Cube(diffMap4, specMap4, 32.0f, 0.0f, 0.0f, 50.0f, 2.0f);


    string diffuseMapPath5 = "wall.png";
    string specularMapPath5 = "wall.png";

    unsigned int diffMap5 = loadTexture(diffuseMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap5 = loadTexture(specularMapPath5.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube5 = Cube(diffMap5, specMap5, 32.0f, 0.0f, 0.0f, 4.0f, 4.0f);


    string diffuseMapPath6 = "wall.png";
    string specularMapPath6 = "wall.png";

    unsigned int diffMap6 = loadTexture(diffuseMapPath6.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap6 = loadTexture(specularMapPath6.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube6 = Cube(diffMap6, specMap6, 32.0f, 0.0f, 0.0f, 1.0f, 4.0f);


    string diffuseMapPath7 = "wall.png";
    string specularMapPath7 = "wall.png";

    unsigned int diffMap7 = loadTexture(diffuseMapPath7.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap7 = loadTexture(specularMapPath7.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube7 = Cube(diffMap7, specMap7, 32.0f, 0.0f, 0.0f, 1.0f, 1.5f);



    string diffuseMapPath8 = "wall.png";
    string specularMapPath8 = "wall.png";

    unsigned int diffMap8 = loadTexture(diffuseMapPath8.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap8 = loadTexture(specularMapPath8.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube8 = Cube(diffMap8, specMap8, 32.0f, 0.0f, 0.0f, 6.0f, 3.0f);

    string diffuseMapPath9 = "roof.png";
    string specularMapPath9 = "roof.png";

    unsigned int diffMap9 = loadTexture(diffuseMapPath9.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap9 = loadTexture(specularMapPath9.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube9 = Cube(diffMap9, specMap9, 32.0f, 0.0f, 0.0f, 6.0f, 3.0f);


    string diffuseMapPath10 = "bench1.png";
    string specularMapPath10 = "bench1.png";

    unsigned int diffMap10 = loadTexture(diffuseMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap10 = loadTexture(specularMapPath10.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube10 = Cube(diffMap10, specMap10, 32.0f, 0.0f, 0.0f, 6.0f, 3.0f);

    string diffuseMapPath11 = "bench2.png";
    string specularMapPath11 = "bench2.png";

    unsigned int diffMap11 = loadTexture(diffuseMapPath11.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap11 = loadTexture(specularMapPath11.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube11 = Cube(diffMap11, specMap11, 32.0f, 0.0f, 0.0f, 6.0f, 3.0f);

    string diffuseMapPath12 = "track_stone.jpg";
    string specularMapPath12 = "track_stone.jpg";

    unsigned int diffMap12 = loadTexture(diffuseMapPath12.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap12 = loadTexture(specularMapPath12.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube12 = Cube(diffMap12, specMap12, 32.0f, 0.0f, 0.0f, 50.0f, 3.0f);

    string diffuseMapPath13 = "pillar.png";
    string specularMapPath13 = "pillar.png";

    unsigned int diffMap13 = loadTexture(diffuseMapPath13.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap13 = loadTexture(specularMapPath13.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube13 = Cube(diffMap13, specMap13, 32.0f, 0.0f, 0.0f, 1.0f, 10.0f);

    string diffuseMapPath14 = "tiles.png";
    string specularMapPath14 = "tiles.png";

    unsigned int diffMap14 = loadTexture(diffuseMapPath14.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap14 = loadTexture(specularMapPath14.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube14 = Cube(diffMap14, specMap14, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    string diffuseMapPath15 = "river2.png";
    string specularMapPath15 = "river2.png";

    unsigned int diffMap15 = loadTexture(diffuseMapPath15.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap15 = loadTexture(specularMapPath15.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube15 = Cube(diffMap15, specMap15, 32.0f, 0.0f, 0.0f, 2.0f, 5.0f);

    string diffuseMapPath16 = "wood.png";
    string specularMapPath16 = "wood.png";

    unsigned int diffMap16 = loadTexture(diffuseMapPath16.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned int specMap16 = loadTexture(specularMapPath16.c_str(), GL_REPEAT, GL_REPEAT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Cube cube16 = Cube(diffMap16, specMap16, 32.0f, 0.0f, 0.0f, 2.0f, 5.0f);


    Cylinder cylinder = Cylinder();
    Cylinder cylinder2 = Cylinder(1.0f, 36, 18, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), 32.0f);

    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        //// point light 4
        pointlight4.setUpPointLight(lightingShader);
        pointlight5.setUpPointLight(lightingShader);
        pointlight6.setUpPointLight(lightingShader);
        pointlight7.setUpPointLight(lightingShader);
        pointlight8.setUpPointLight(lightingShader);
        pointlight9.setUpPointLight(lightingShader);
        pointlight10.setUpPointLight(lightingShader);
       
      
        spotlight1.setUpSpotLight(lightingShader);
        directionlight1.setUpDirectionalLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

     

        /*glm::mat4 model = model_Matrix(-31, 2, 16, 5.1, 1, 0.2);
        lightingShader.setMat4("model", model);
        drawCube(cubeVAO, lightingShader, model, 0.99207f, 0.99207f, 0.588f);*/

      //  chair_color(-31, 2, 16, 5.1, 1, 0.2,cubeVAO,lightingShader);


       /* glm::mat4 model_chair_1 = model_Matrix(-31, 2, 16, 5.1, 1, 0.2);
        lightingShader.setMat4("model", model_chair_1);
        drawCube(cubeVAO, lightingShader, model_chair_1, 0.99207f, 0.99207f, 0.588f);*/

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

       // bed(cubeVAO, lightingShader, model);

       

       // wall_color(25, 5, 1, 5, 3.5, -0.1, 0, 0, 0, cubeVAO, lightingShader);//window up wall
       
        //color_black(130, 1.5, 1, -4, 0.0, -0.1, 0, 0, 0, cubeVAO, lightingShader);
        //road
      
        /*for (int i = 0; i <= 30; i++)
        {
            color_black(10, 1, 6, -4 + i, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
            color_white(5, 0.01, 0.2, -4 + i, 0.1, 0.3, 0, 0, 0, cubeVAO, lightingShader);
        }
        for (int i = 0; i <= 30; i++)
        {
            color_black(10, 1, -6, -4 + i, 0, -0.1, 0, 0, 0, cubeVAO, lightingShader);
            color_white(5, 0.01, -0.2, -4 + i, 0.1, -0.4, 0, 0, 0, cubeVAO, lightingShader);
        }*/

       
        //platform
        //platform_color(130, 1, 40, -4, 0, 0.601, 0, 0, 0, cubeVAO, lightingShader);
        //color_black(300, 0.1, 17, -4, 0, 4.601, 0, 0, 0, cubeVAO, lightingShader);

       // rail_track(0, 0, 0.1, cubeVAO, lightingShader);
      //  rail_track(0, 0, 1, cubeVAO, lightingShader);

       // ////2nd Platform
       //// platform_color(130, 1, 15, -4, 0, 6.301, 0, 0, 0, cubeVAO, lightingShader);
       // platform_roof(90, 1, 16, 0, 2, 6.2, 0, 0, 0, cubeVAO, lightingShader);

       // platform_color(1, 19, 1, 3.5, 0.1, 6.9, 0, 0, 0, cubeVAO, lightingShader);
       // platform_color(1, 19, 1, 7.5, 0.1, 6.9, 0, 0, 0, cubeVAO, lightingShader);
       // platform_color(1, 19, 1, 0.5, 0.1, 6.9, 0, 0, 0, cubeVAO, lightingShader);


       // color_white(5, 2, 5, 3.3, 0.1, 6.7, 0, 0, 0, cubeVAO, lightingShader);
       // color_white(5, 2, 5, 7.3, 0.1, 6.7, 0, 0, 0, cubeVAO, lightingShader);
       // color_white(5, 2, 5, 0.3, 0.1, 6.7, 0, 0, 0, cubeVAO, lightingShader);

    
        //wall

     /*   color_wall(1, 20, -20, 8.8, 0.1, 3, 0, 0, 0, cubeVAO, lightingShader);
        color_wall(1, 25, -20, 6, 0.1, 3, 0, 0, 0, cubeVAO, lightingShader);*/

        ////wall+window counter 2
        //color_wall(1, 25, -6, 4, 0.1, 3, 0, 0, 0, cubeVAO, lightingShader);

        //color_wall2(1, 8, -8, 4, 0.1, 2.4, 0, 0, 0, cubeVAO, lightingShader);
        //color_wall2(1, 10, -8, 4, 1.5, 2.4, 0, 0, 0, cubeVAO, lightingShader);
        //color_wall(1, 25, -6, 4, 0.1, 1.6, 0, 0, 0, cubeVAO, lightingShader);

        //color_wall(1, 25, -6, 0, 0.1, 3, 0, 0, 0, cubeVAO, lightingShader);
        //// color_wall(1, 25, -2, 0, 0.1, 0);
        //color_wall(1, 25, -6, 0, 0.1, 1.6, 0, 0, 0, cubeVAO, lightingShader);

        ////counter 1 window
        //color_wall2(1, 8, -8, 0, 0.1, 2.4, 0, 0, 0, cubeVAO, lightingShader);
        //color_wall2(1, 10, -8, 0, 1.5, 2.4, 0, 0, 0, cubeVAO, lightingShader);
        //color_wall(1, 20, -20, -1.5, 0.1, 3, 0, 0, 0, cubeVAO, lightingShader);

        //color_wall(1, 20, -20, -4, 0.1, 3, 0, 0, 0, cubeVAO, lightingShader);

        //color_wall(40, 20, 1, -4, 0.1, 1, 0, 0, 0, cubeVAO, lightingShader);
        //color_wall(10, 25, 1, 0, 0.1, 1, 0, 0, 0, cubeVAO, lightingShader);
        //color_wall(30, 25, 1, 3, 0.1, 1, 0, 0, 0, cubeVAO, lightingShader);

        ////rest room wall
        //color_wall(28, 20, 1, 6, 0.1, 1, 0, 0, 0, cubeVAO, lightingShader);
        ////ticket counter 2
        //platform_roof3(28.1, 1, 20, 6.1, 2.05, 0.99, 0, 0, 0, cubeVAO, lightingShader);

        //color_wall2(6, 20, 1, -1.4 + 5.5, 0.1, 2.9, 0, 0, 0, cubeVAO, lightingShader);
        ////color_wall2(8, 20, 1, -1.4 + 5.5, 0.1, 2.9);
        //color_wall2(19, 5, 1, -0.7 + 4.8, 2, 2.9, 0, 0, 0, cubeVAO, lightingShader);
        //color_wall2(5, 20, 1, 0 + 5.5, 0.1, 2.9, 0, 0, 0, cubeVAO, lightingShader);

        
        // color_white(1, 30, 0.1, 0, 0.1, 0.99, 0);
         //color_white(1, 30, 0.1, 6, 0.1, 0.99, 0);

        // //fron gate top square
        //platform_roof3(61, 3.2, 20, 0, 2.5, 0.99, 0, 0, 0, cubeVAO, lightingShader);

        ////shop+ticket counter1 roof

        //platform_roof3(40, 1, 20, -4, 2.05, 0.99, 0, 0, 0, cubeVAO, lightingShader);
        ////shop
        //color_wall2(10, 10, 1, -4, 0.1, 3, 0, 0, 0, cubeVAO, lightingShader);

        ////ticket counter
        //color_wall2(6, 20, 1, -1.5, 0.1, 2.92, 0, 0, 0, cubeVAO, lightingShader);
        //color_wall2(8, 20, 1, -1.5, 0.1, 2.92, 0, 0, 0, cubeVAO, lightingShader);
        //color_wall2(7, 5, 1, -0.7, 1.6, 2.92, 0, 0, 0, cubeVAO, lightingShader);
        //color_wall2(1.5, 20, 1, -0.1, 0.1, 2.92, 0, 0, 0, cubeVAO, lightingShader);

        //road lamp

        for (int i = 0; i < 11; i+=2)
        {
            color_white(0.3, 10, 0.3, -2+i, 0, -0.07, 0, 0, 0, cubeVAO, lightingShader);
            color_white(0.3, 0.3, -5, -2+i, 0.98, -0.04, 10, 0, 0, cubeVAO, lightingShader);
        }
       
       
        //drawWheel();
       // drawCylinder();

        //for (int i = 0; i < 360; i++)
        //{
        //    color_black(.4, .4, 0.2, -3, .09, 4.83, 0, 0, 0+i, cubeVAO, lightingShader);
        //    color_black(.2, .2, 0.4, -3, .09, 4.831, 0, 0, 0+i, cubeVAO, lightingShader);

        //    color_black(.2, .2, 0.4, -3, .09, 5.11, 0, 0, 0 + i, cubeVAO, lightingShader);
        //    color_black(.4, .4, 0.2, -3, .09, 5.14, 0, 0, 0 + i, cubeVAO, lightingShader);

        //}
        ////cylinder2.drawSphere(lightingShader, model_Matrix(0.5, 0.01, 0.5, -3, .09, 4.83, 0, 90, 90));

        //color_white(0.6, 0.8, 2.6, -3.03, .09, 4.865, 0, 0, 0, cubeVAO, lightingShader);
       
        //for (int i = 0; i < 360; i++)
        //{
        //    color_black(.4, .4, 0.2, -1, .09, 4.83, 0, 0, 0 + i, cubeVAO, lightingShader);
        //    color_black(.2, .2, 0.4, -1, .09, 4.831, 0, 0, 0 + i, cubeVAO, lightingShader);

        //    color_black(.2, .2, 0.4, -1, .09, 5.11, 0, 0, 0 + i, cubeVAO, lightingShader);
        //    color_black(.4, .4, 0.2, -1, .09, 5.14, 0, 0, 0 + i, cubeVAO, lightingShader);

        //}

        //color_white(0.6, 0.8, 2.6, -1.03, .09, 4.865, 0, 0, 0, cubeVAO, lightingShader);
       
  
       /* color_train_body(0.2, -2, -6.8, -3.63, 1.27, 5.35, 0, 0, 0, cubeVAO, lightingShader);
        color_train_body(0.2, 2, -3.5, -3.63, 0.77, 4.9, 90, 0, 0, cubeVAO, lightingShader);*/

       

        // color_train_body(0.2, 11,7, -3.03, .17, 4.65, 0, 0, 0, cubeVAO, lightingShader);
      
        //  color_train_body(0.2, 11,2, -3.03, .17, 4.85, 0, 0, 0, cubeVAO, lightingShader);

       
      /*  color_train_body(0.2, -2, -6.8, -3.63 + 10.66, 1.27, 5.35, 0, 0, 0, cubeVAO, lightingShader);
        color_train_body(0.2, 2, -3.5, -3.63 + 10.66, 0.77, 4.9, 90, 0, 0, cubeVAO, lightingShader);*/
        
       /* for (int i = 0; i < 360; i++)
        {
            color_black(.4, .4, 0.2, -0.5, .09, 4.83, 0, 0, 0 + i, cubeVAO, lightingShader);
            color_black(.2, .2, 0.4, -0.5, .09, 4.831, 0, 0, 0 + i, cubeVAO, lightingShader);

            color_black(.2, .2, 0.4, -0.5, .09, 5.11, 0, 0, 0 + i, cubeVAO, lightingShader);
            color_black(.4, .4, 0.2, -0.5, .09, 5.14, 0, 0, 0 + i, cubeVAO, lightingShader);

        }
        color_white(0.6, 0.8, 2.6, -0.53, .09, 4.865, 0, 0, 0, cubeVAO, lightingShader);

        for (int i = 0; i < 360; i++)
        {
            color_black(.4, .4, 0.2, 1.5, .09, 4.83, 0, 0, 0 + i, cubeVAO, lightingShader);
            color_black(.2, .2, 0.4, 1.5, .09, 4.831, 0, 0, 0 + i, cubeVAO, lightingShader);

            color_black(.2, .2, 0.4, 1.5, .09, 5.11, 0, 0, 0 + i, cubeVAO, lightingShader);
            color_black(.4, .4, 0.2, 1.5, .09, 5.14, 0, 0, 0 + i, cubeVAO, lightingShader);

        }
        color_white(0.6, 0.8, 2.6, 1.47, .09, 4.865, 0, 0, 0, cubeVAO, lightingShader);
        
        for (int i = 0; i < 360; i++)
        {
            color_black(.4, .4, 0.2, 2, .09, 4.83, 0, 0, 0 + i, cubeVAO, lightingShader);
            color_black(.2, .2, 0.4, 2, .09, 4.831, 0, 0, 0 + i, cubeVAO, lightingShader);

            color_black(.2, .2, 0.4, 2, .09, 5.11, 0, 0, 0 + i, cubeVAO, lightingShader);
            color_black(.4, .4, 0.2, 2, .09, 5.14, 0, 0, 0 + i, cubeVAO, lightingShader);

        }
        color_white(0.6, 0.8, 2.6, 1.97, .09, 4.865, 0, 0, 0, cubeVAO, lightingShader);
        for (int i = 0; i < 360; i++)
        {
            color_black(.4, .4, 0.2, 4, .09, 4.83, 0, 0, 0 + i, cubeVAO, lightingShader);
            color_black(.2, .2, 0.4, 4, .09, 4.831, 0, 0, 0 + i, cubeVAO, lightingShader);

            color_black(.2, .2, 0.4, 4, .09, 5.11, 0, 0, 0 + i, cubeVAO, lightingShader);
            color_black(.4, .4, 0.2, 4, .09, 5.14, 0, 0, 0 + i, cubeVAO, lightingShader);

        }
        color_white(0.6, 0.8, 2.6, 3.97, .09, 4.865, 0, 0, 0, cubeVAO, lightingShader);

        for (int i = 0; i < 360; i++)
        {
            color_black(.4, .4, 0.2, 6, .09, 4.83, 0, 0, 0 + i, cubeVAO, lightingShader);
            color_black(.2, .2, 0.4, 6, .09, 4.831, 0, 0, 0 + i, cubeVAO, lightingShader);

            color_black(.2, .2, 0.4, 6, .09, 5.11, 0, 0, 0 + i, cubeVAO, lightingShader);
            color_black(.4, .4, 0.2, 6, .09, 5.14, 0, 0, 0 + i, cubeVAO, lightingShader);

        }
        color_white(0.6, 0.8, 2.6, 5.97, .09, 4.865, 0, 0, 0, cubeVAO, lightingShader);*/


        //tree 1
        cylinder.drawSphere(lightingShader, model_Matrix(1,0.5, 1, 11, 0.01, 3, 0, 0, 0));
        generateUmbrellaCanopyVertices(10, 10, 10, 11, 5.5, 3, glm::vec3(0.0f, 1.0f, 0.0f), -90, 0, 0);
        
        //tree 2
        tree_1(1, 1, 1, 2, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
        leaf(0, 0, 0, 2, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
        leaf(0, 0, 0, 0, 0, 0, 0, 0, 0, cubeVAO, lightingShader);

        ////tree 1
        //cylinder.drawSphere(lightingShader, model_Matrix(1, 0.5, 1, 11+9, 0.01, 3, 0, 0, 0));
        //generateUmbrellaCanopyVertices(10, 10, 10, 11+9, 5.5, 3, glm::vec3(0.0f, 1.0f, 0.0f), -90, 0, 0);

        ////tree 2
        //tree_1(1, 1, 1, 2+9, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
        //leaf(0, 0, 0, 2+9, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
        //leaf(0, 0, 0, 0+9, 0, 0, 0, 0, 0, cubeVAO, lightingShader);

        //tree 1
        cylinder.drawSphere(lightingShader, model_Matrix(1, 0.5, 1, 11 + 9+9+2, 0.01, 3, 0, 0, 0));
        generateUmbrellaCanopyVertices(10, 10, 10, 11 + 9+9+2, 5.5, 3, glm::vec3(0.0f, 1.0f, 0.0f), -90, 0, 0);

        //tree 2
        tree_1(1, 1, 1, 2 + 9+9 + 2, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
        leaf(0, 0, 0, 2 + 9+9 + 2, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
        leaf(0, 0, 0, 0 + 9+9 + 2, 0, 0, 0, 0, 0, cubeVAO, lightingShader);

        //tree 1
        cylinder.drawSphere(lightingShader, model_Matrix(1, 0.5, 1, 11 + 9 + 9+9+2, 0.01, 3, 0, 0, 0));
        generateUmbrellaCanopyVertices(10, 10, 10, 11 + 9 + 9+9 + 2, 5.5, 3, glm::vec3(0.0f, 1.0f, 0.0f), -90, 0, 0);

        

        if (train_on)
        {
            if (train_move < 51)
            {
                train_move += 0.5;
              // camera.ProcessKeyboard(FORWARD, deltaTime * 0.6);
                trainBody(0, 0, 0, 0 + train_move, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
                trainWheel(0, 0, 0, 0 + train_move, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
            }
            else
            {
                trainBody(0, 0, 0, 0 + train_move, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
                trainWheel(0, 0, 0, 0 + train_move, 0, 0, 0, 0, 0, cubeVAO, lightingShader);

            }
            
            
        }
        else if (train_on_reverse)
        {
            if (train_move >= 1)
            {
                train_move -= 0.5;
                trainBody(0, 0, 0, 0 + train_move, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
                trainWheel(0, 0, 0, 0 + train_move, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
            }
            else
            {
                trainBody(0, 0, 0, 0 + train_move, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
                trainWheel(0, 0, 0, 0 + train_move, 0, 0, 0, 0, 0, cubeVAO, lightingShader);

            }

        }
        else
        {
            trainBody(0, 0, 0, 0 + train_move, 0, 0, 0, 0, 0, cubeVAO, lightingShader);
            trainWheel(0, 0, 0, 0 + train_move, 0, 0, 0, 0, 0 , cubeVAO, lightingShader);
        }
        

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);


     /*   for (int i = 0; i < 2; i++)
        {
            trainWheel2(0, 0, 0, i, 5, 0, 0, 0, 0, cubeVAO, lightingShader);

            wheelVertices.clear();
            wheelVertices2.clear();
            wheelVertices3.clear();
            wheelIndices.clear();
            wheelIndices2.clear();
            wheelIndices3.clear();
           
        }*/

       // vector<unsigned int>v = generateWheel(2, 2, 1, 5, 0.1, 5);
      //  drawWheel(v[0], v[1], v[2], lightingShader, model_Matrix(5, 5, 1, 5, 5, 5, 0, 0, 0),0.5,0.5,0.5);

       
        
       // color_green(0.01, 1, 1, 5, 5, 5, 0, 0, 60, cubeVAO, lightingShader);
       
        

       /* for (int i = 0; i < 36; i++)
        {
            color_red(0.1, 5, 5, 6, 6, 6, 0, 0 + (i*18), 0, cubeVAO, lightingShader);
        }*/
       /* color_red(0.1, 5, 3, 6, 6, 6, 0, 0, 30, cubeVAO, lightingShader);
        color_red(0.1, 5, 3, 6, 6, 6.3, 0, -30, 30, cubeVAO, lightingShader);
        color_red(0.1, 5, 3, 6, 6, 6.6, -30, -30, 30, cubeVAO, lightingShader);*/
       
       // color_white(0.5, 0.8, 2.6, -0.52, .09, 4.865, 0, 0, 0, cubeVAO, lightingShader);

        /*glm::mat4 modelForSphere = glm::mat4(1.0f);
        modelForSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));*/
        sphere.drawSphere(lightingShader, model_Matrix(2,2,2,3,3,3,0,0,0));

       
       

        


        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
       
        for (int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            //if (i == 1 || i==2 || i==4 || i>=5 && i<=7)
            //{
            //    model = glm::scale(model, glm::vec3(0.5, 0.05, 0.5)); // Make it a smaller cube
            //}
            model = glm::scale(model, glm::vec3(1, 0.1, 1)); // Make it a smaller cube
            ourShader.setMat4("model", model);
            ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
            
            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
          

        }

      

        
        model = glm::mat4(1.0f);
        model = glm::translate(model, spotLightPositions[0]);
        model = glm::scale(model, glm::vec3(0.01, 0.01, 0.01)); // Make it a smaller cube
 /*       rotateXMatrix = glm::rotate(identityMatrix, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(-10.0f), glm::vec3(0.0f, 0.0f, 1.0f));
        model = (model * rotateXMatrix * rotateYMatrix * rotateZMatrix);*/
        ourShader.setMat4("model", model);
        ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

       

        //texture code start


        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        pointlight1.setUpPointLight(lightingShaderWithTexture);
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        pointlight5.setUpPointLight(lightingShaderWithTexture);
        pointlight6.setUpPointLight(lightingShaderWithTexture);
        pointlight7.setUpPointLight(lightingShaderWithTexture);
        pointlight8.setUpPointLight(lightingShaderWithTexture);
        pointlight9.setUpPointLight(lightingShaderWithTexture);
        pointlight10.setUpPointLight(lightingShaderWithTexture);

        spotlight1.setUpSpotLight(lightingShaderWithTexture);
        directionlight1.setUpDirectionalLight(lightingShaderWithTexture);

        lightingShaderWithTexture.setMat4("projection", projection);

        lightingShaderWithTexture.setMat4("view", view);

        
        //between rail track and road
       cube.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(155, 0.1, 40, 9 , 0, 0.601, 0, 0, 0));
       cube.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(100, 0.1, 15, 9, 0, 0.601+5.7, 0, 0, 0));

       cube.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(175, 0.1, 40, 9+19.5, 0, 0.601, 0, 0, 0));
       //cube.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(175, 0.1, 95, 9+19.5, 0, 0.601 + 5.7, 0, 0, 0));

       cube.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(175, 0.1, 95, 9+19.5, 0, 0.601 + 5.7, 0, 0, 0));
       color_black(175, -5, 95, 9 + 19.5, 0, 0.601 + 5.7, 0, 0, 0, cubeVAO, lightingShader);

       cube.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(-95, 0.1, -49, 9+15.5, 0, 0.601 + 5.7, 0, 90, 0));
       color_black(-95, -5, -56, 9 + 15.5, 0, 0.601 + 5.7, 0, 90, 0, cubeVAO, lightingShader);

       //under rail track floor
       cube12.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(630, 0.1, 17, -4, 0, 4.601, 0, 0, 0));

       //////////////////////////////////////////////1st station start////////////////////////////////////////////////
       
        //platform
        cube2.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(130, 1, 40, -4, 0, 0.601, 0, 0, 0));

        //platform 2nd start
        cube3.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(130, 1, 15, -4, 0, 6.301, 0, 0, 0));
       // platform_roof
        cube9.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(90, 1, 16, 0, 2, 6.2, 0, 0, 0));
         
       
        cube13.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 19, 1, 3.5, 0.1, 6.9, 0, 0, 0));
        cube13.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 19, 1, 7.5, 0.1, 6.9, 0, 0, 0));
        cube13.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 19, 1, 0.5, 0.1, 6.9, 0, 0, 0));

        cube14.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(8, 2, 5, 3.3 - .15, 0.1, 6.7, 0, 0, 0));
        cube14.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(8, 2, 5, 7.3-.15, 0.1, 6.7, 0, 0, 0));
        cube14.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(8, 2, 5, 0.3-.15, 0.1, 6.7, 0, 0, 0));

         //wall

        cube5.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 20, -20, 8.8, 0.1, 3, 0, 0, 0));
        cube5.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 25, -20, 6, 0.1, 3, 0, 0, 0));

        ////wall+window counter 2

        cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 25, -6, 4, 0.1, 3, 0, 0, 0));

        cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 8, -8, 4, 0.1, 2.4, 0, 0, 0));
        cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 10, -8, 4, 1.5, 2.4, 0, 0, 0));
        cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 25, -6, 4, 0.1, 1.6, 0, 0, 0));



        cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 25, -6, 0, 0.1, 3, 0, 0, 0));
        cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 25, -6, 0, 0.1, 1.6, 0, 0, 0));

        ////counter 1 window


        cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 8, -8, 0, 0.1, 2.4, 0, 0, 0));
        cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 10, -8, 0, 1.5, 2.4, 0, 0, 0));
        cube5.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 20, -20, -1.5, 0.1, 3, 0, 0, 0));


        cube5.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 20, -20, -4, 0.1, 3, 0, 0, 0));



        cube8.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(40, 20, 1, -4, 0.1, 1, 0, 0, 0));
        cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(10, 25, 1, 0, 0.1, 1, 0, 0, 0));
        cube8.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(30, 25, 1, 3, 0.1, 1, 0, 0, 0));

        ////rest room wall

        cube8.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(28, 20, 1, 6, 0.1, 1, 0, 0, 0));
        ////ticket counter 2
        cube9.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(28.1, 1, 20, 6.1, 2.05, 0.99, 0, 0, 0));


        cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(6, 20, 1, -1.4 + 5.5, 0.1, 2.9, 0, 0, 0));
        cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(19, 5, 1, -0.7 + 4.8, 2.1, 2.9, 0, 0, 0));
        cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(5, 20, 1, 0 + 5.5, 0.1, 2.9, 0, 0, 0));


        //platform roof 1st platform
        cube9.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(61, 3.2, 20, 0, 2.5, 0.99, 0, 0, 0));

        ////Rest room+ticket counter1 roof
         cube9.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(40, 1, 20, -4, 2.05, 0.99, 0, 0, 0));

        ////ticket counter

        cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(6, 20, 1, -1.5, 0.1, 2.92, 0, 0, 0));
        cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(8, 20, 1, -1.5, 0.1, 2.92, 0, 0, 0));
        cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(7, 5, 1, -0.7, 1.6, 2.92, 0, 0, 0));
        cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1.95, 20, 1, -0.1, 0.1, 2.92, 0, 0, 0));



        //L_bench1

        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 10, 0.5, -3.8, 0.01, 1.2, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -3.8, 0.01, 1.2, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -3.1, 0.01, 1.2, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -2.32, 0.01, 1.2, 0, 0, 0));
        cube11.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 5, 0.5, -3.8, 0.55, 1.2, 90, 0, 0));

        //L_bench2
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 10, 0.5, -3.8, 0.01, 2.0, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -3.8, 0.01, 2.0, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -3.1, 0.01, 2.0, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -2.32, 0.01, 2.0, 0, 0, 0));
        cube11.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 5, 0.5, -3.8, 0.55, 2.0, 90, 0, 0));



        //R_bench1

        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 10, 0.5, 6.5 + .2, 0.01, 1.2, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 6.5 + .2, 0.01, 1.2, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 7.2 + .2, 0.01, 1.2, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 7.98 + .2, 0.01, 1.2, 0, 0, 0));
        cube11.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 5, 0.5, 6.5 + .2, 0.55, 1.2, 90, 0, 0));

        //R_bench2
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 10, 0.5, 6.5 + .2, 0.01, 2.0, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 6.5 + .2, 0.01, 2.0, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 7.2 + .2, 0.01, 2.0, 0, 0, 0));
        cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 7.98 + .2, 0.01, 2.0, 0, 0, 0));
        cube11.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 5, 0.5, 6.5 + .2, 0.55, 2.0, 90, 0, 0));




        //road

        cube4.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(285, 1, 6, -4, 0, 0, 0, 0, 0));
        cube4.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(285, 1, -6, -4, 0, -0.1, 0, 0, 0));

        //between road line
        color_black(225, 1.5, 1, -4, 0.0, -0.1, 0, 0, 0, cubeVAO, lightingShader);
        color_black(40, 1.5, 1, -4 + 24.5, 0.0, -0.1, 0, 0, 0, cubeVAO, lightingShader);
        cube4.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(305, 1, 6, -4 + 32.5, 0, 0, 0, 0, 0));
        cube4.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(305, 1, -6, -4 + 32.5, 0, -0.1, 0, 0, 0));


        color_black(305, 1.5, 1, -4 + 32.5, 0.0, -0.1, 0, 0, 0, cubeVAO, lightingShader);

        cube4.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(-152, 1, 6, -4+23.5, -0.04, 0+0.6, 0, 90, 0));
        cube4.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(-152, 1, -6, -4+23.5, -0.04, -0.1+0.7, 0, 90, 0));


        //floor total

        color_black(285, -5, 85, -4, 0, -0.7, 0, 0, 0, cubeVAO, lightingShader);
        color_black(305, -5, 85, -4+32.5, 0, -0.7, 0, 0, 0, cubeVAO, lightingShader);

        //river
        cube15.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(40, 1, 165, -4 + 28.5, -0.5, -0.7, 0, 0, 0));

        //boat
       /* cube16.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(4, 1.1, 15, -4 + 30, -0.5, -0.7+12, 0, 0, 0));
        cube16.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(4, 0.2, 15, -4 + 30+0.03, -0.5, -0.7+12, 0, 0, 100));
        cube16.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(4, 0.2, 15, -4 + 30+0.03+0.35, -0.5, -0.7+12, 0, 0, 80));
        cube16.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(-2, 4, 0.2, -4 + 30+0.03+0.35, -0.5, -0.7+12, 0, -20, -10));*/

       // boat_front(2, 2, 5, -4 + 30 + 0.03 + 0.35, -0.5, -0.7 + 12, 0, 0, 0);
        boat(1, 1, 1, 21, -5.40, 6, 0, 0, 0, cubeVAO, lightingShader);
        boat(1, 1, 1, 22, -5.40, 4, 0, 0, 0, cubeVAO, lightingShader);
       //rail track

        for (int i = 1; i <= 630; i++)
        {
            //color_white(1, 0.3, 5, 0.1 * i  - 4.05, 0 , 4.66 , 0, 0, 0, cubeVAO, lightingShader);
            cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 0.3, 5, 0.1 * i - 4.05, 0, 4.76, 0, 0, 0));
            cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 0.3, 5, 0.1 * i - 4.05, 0, 4.66 + 1, 0, 0, 0));
            i += 1;
        }
        rail_track(630, 0.5, 0.3, -4.05 , 0.02 , 4.85 , 0, 0, 0, cubeVAO, lightingShader);
        rail_track(630, 0.5, 0.3, -4.05 , 0.02 , 5.11 , 0, 0, 0, cubeVAO, lightingShader);

        rail_track(630, 0.5, 0.3, -4.05, 0.02, 4.75+1, 0, 0, 0, cubeVAO, lightingShader);
        rail_track(630, 0.5, 0.3, -4.05, 0.02, 5.01+1, 0, 0, 0, cubeVAO, lightingShader);



        //////////////////////////////////////////////2nd station start////////////////////////////////////////////////

       
      //platform
      cube2.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(130, 1, 40, -4 + 50, 0, 0.601, 0, 0, 0));

      //platform 2nd start
      cube3.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(130, 1, 15, -4 + 50, 0, 6.301, 0, 0, 0));
     // platform_roof
      cube9.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(90, 1, 16, 0 + 50, 2, 6.2, 0, 0, 0));


      cube13.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 19, 1, 3.5 + 50, 0.1, 6.9, 0, 0, 0));
      cube13.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 19, 1, 7.5 + 50, 0.1, 6.9, 0, 0, 0));
      cube13.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 19, 1, 0.5 + 50, 0.1, 6.9, 0, 0, 0));

      cube14.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(8, 2, 5, 3.3 - .15 + 50, 0.1, 6.7, 0, 0, 0));
      cube14.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(8, 2, 5, 7.3-.15 + 50, 0.1, 6.7, 0, 0, 0));
      cube14.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(8, 2, 5, 0.3-.15 + 50, 0.1, 6.7, 0, 0, 0));

      
       //wall

      cube5.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 20, -20, 8.8 + 50, 0.1, 3, 0, 0, 0));
      cube5.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 25, -20, 6 + 50, 0.1, 3, 0, 0, 0));

      ////wall+window counter 2

      cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 25, -6, 4 + 50, 0.1, 3, 0, 0, 0));

      cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 8, -8, 4 + 50, 0.1, 2.4, 0, 0, 0));
      cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 10, -8, 4 + 50, 1.5, 2.4, 0, 0, 0));
      cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 25, -6, 4 + 50, 0.1, 1.6, 0, 0, 0));



      cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 25, -6, 0 + 50, 0.1, 3, 0, 0, 0));
      cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 25, -6, 0 + 50, 0.1, 1.6, 0, 0, 0));

      ////counter 1 window


      cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 8, -8, 0 + 50, 0.1, 2.4, 0, 0, 0));
      cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 10, -8, 0 + 50, 1.5, 2.4, 0, 0, 0));
      cube5.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 20, -20, -1.5 + 50, 0.1, 3, 0, 0, 0));


      cube5.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1, 20, -20, -4 + 50, 0.1, 3, 0, 0, 0));



      cube8.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(40, 20, 1, -4 + 50, 0.1, 1, 0, 0, 0));
      cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(10, 25, 1, 0 + 50, 0.1, 1, 0, 0, 0));
      cube8.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(30, 25, 1, 3 + 50, 0.1, 1, 0, 0, 0));

      ////rest room wall

      cube8.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(28, 20, 1, 6 + 50, 0.1, 1, 0, 0, 0));
      ////ticket counter 2
      //platform_roof3(28.1, 1, 20, 6.1, 2.05, 0.99, 0, 0, 0, cubeVAO, lightingShader);
      cube9.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(28.1, 1, 20 , 6.1 + 50, 2.05, 0.99, 0, 0, 0));


      cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(6, 20, 1, -1.4 + 5.5 + 50, 0.1, 2.9, 0, 0, 0));
      cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(19, 5, 1, -0.7 + 4.8 + 50, 2.1, 2.9, 0, 0, 0));
      cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(5, 20, 1, 0 + 5.5 + 50, 0.1, 2.9, 0, 0, 0));


      //platform_roof3(61, 3.2, 20, 0, 2.5, 0.99, 0, 0, 0, cubeVAO, lightingShader);
      cube9.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(61, 3.2, 20, 0 + 50, 2.5, 0.99, 0, 0, 0));

      ////Rest room+ticket counter1 roof

      //platform_roof3(40, 1, 20, -4, 2.05, 0.99, 0, 0, 0, cubeVAO, lightingShader);
       cube9.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(40, 1, 20, -4 + 50, 2.05, 0.99, 0, 0, 0));

      ////ticket counter

      cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(6, 20, 1, -1.5 + 50, 0.1, 2.92, 0, 0, 0));
      cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(8, 20, 1, -1.5 + 50, 0.1, 2.92, 0, 0, 0));
      cube7.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(7, 5, 1, -0.7 + 50, 1.6, 2.92, 0, 0, 0));
      cube6.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(1.95, 20, 1, -0.1 + 50, 0.1, 2.92, 0, 0, 0));



      //L_bench1

      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 10, 0.5, -3.8 + 50, 0.01, 1.2, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -3.8 + 50, 0.01, 1.2, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -3.1 + 50, 0.01, 1.2, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -2.32 + 50, 0.01, 1.2, 0, 0, 0));
      cube11.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 5, 0.5, -3.8 + 50, 0.55, 1.2, 90, 0, 0));

      //L_bench2
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 10, 0.5, -3.8 + 50, 0.01, 2.0, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -3.8 + 50, 0.01, 2.0, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -3.1 + 50, 0.01, 2.0, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, -2.32 + 50, 0.01, 2.0, 0, 0, 0));
      cube11.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 5, 0.5, -3.8 + 50, 0.55, 2.0, 90, 0, 0));



      //R_bench1

      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 10, 0.5, 6.5 + .2 + 50, 0.01, 1.2, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 6.5 + .2 + 50, 0.01, 1.2, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 7.2 + .2 + 50, 0.01, 1.2, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 7.98 + .2 + 50, 0.01, 1.2, 0, 0, 0));
      cube11.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 5, 0.5, 6.5 + .2 + 50, 0.55, 1.2, 90, 0, 0));

      //R_bench2
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 10, 0.5, 6.5 + .2 + 50, 0.01, 2.0, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 6.5 + .2 + 50, 0.01, 2.0, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 7.2 + .2 + 50, 0.01, 2.0, 0, 0, 0));
      cube10.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(0.2, 5, 5, 7.98 + .2 + 50, 0.01, 2.0, 0, 0, 0));
      cube11.drawCubeWithTexture(lightingShaderWithTexture, model_Matrix(15, 5, 0.5, 6.5 + .2 + 50, 0.55, 2.0, 90, 0, 0));

     



        //texture code end


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
//    lightingShader.setVec3("material.emission", glm::vec3(0.0f, 0.0f, 0.0));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

//void drawWheel (unsigned int& cubeVAO, unsigned int& cubeVAO2, unsigned int& cubeVAO3, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
//{
//    lightingShader.use();
//
//    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
//    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
//    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
//    //    lightingShader.setVec3("material.emission", glm::vec3(0.0f, 0.0f, 0.0));
//    lightingShader.setFloat("material.shininess", 32.0f);
//
//    lightingShader.setMat4("model", model);
//
//    glBindVertexArray(cubeVAO);
//    glDrawElements(GL_TRIANGLES, wheelIndices.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(cubeVAO2);
//    glDrawElements(GL_TRIANGLES, wheelIndices2.size(), GL_UNSIGNED_INT, 0);
//    glBindVertexArray(cubeVAO3);
//    glDrawElements(GL_TRIANGLES, wheelIndices3.size(), GL_UNSIGNED_INT, 0);
//}




// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

   /* if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if (rotateAxis_X) rotateAngle_X -= 0.1;
        else if (rotateAxis_Y) rotateAngle_Y -= 0.1;
        else rotateAngle_Z -= 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) translate_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) translate_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) translate_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) translate_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) translate_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) translate_Z -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) scale_X += 0.001;
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) scale_X -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) scale_Y += 0.001;
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) scale_Y -= 0.001;
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) scale_Z += 0.001;
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS) scale_Z -= 0.001;*/

    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
    {
        rotateAngle_X += 0.1;
        rotateAxis_X = 1.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        rotateAngle_Y += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 1.0;
        rotateAxis_Z = 0.0;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
    {
        rotateAngle_Z += 0.1;
        rotateAxis_X = 0.0;
        rotateAxis_Y = 0.0;
        rotateAxis_Z = 1.0;
    }

    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointlight5.turnOff();
            pointlight6.turnOff();
            pointlight7.turnOff();
            pointlight8.turnOff();
            pointlight9.turnOff();
            pointlight10.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointlight5.turnOn();
            pointlight6.turnOn();
            pointlight7.turnOn();
            pointlight8.turnOn();
            pointlight9.turnOn();
            pointlight10.turnOn();
 
            pointLightOn = !pointLightOn;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotLightOn)
        {
            spotlight1.turnOff();
            spotLightOn = !spotLightOn;
        }
        else
        {
            spotlight1.turnOn();
            spotLightOn = !spotLightOn;
        }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
    
        if (directionLightOn)
        {
            directionlight1.turnOff();
            directionLightOn = !directionLightOn;
        }
        else
        {
            directionlight1.turnOn();
            directionLightOn = !directionLightOn;
        }
    }
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {

        if (ambientToggle)
        {
            directionlight1.turnAmbientOff();
            pointlight1.turnAmbientOff();
            pointlight2.turnAmbientOff();
            pointlight3.turnAmbientOff();
            pointlight4.turnAmbientOff();
            pointlight5.turnAmbientOff();
            pointlight6.turnAmbientOff();
            pointlight7.turnAmbientOff();
            pointlight8.turnAmbientOff();
            pointlight9.turnAmbientOff();
            pointlight10.turnAmbientOff();
            spotlight1.turnAmbientOff();
            ambientToggle = !ambientToggle;
        }
        else
        {
            directionlight1.turnAmbientOn();
            pointlight1.turnAmbientOn();
            pointlight2.turnAmbientOn();
            pointlight3.turnAmbientOn();
            pointlight4.turnAmbientOn();
            pointlight5.turnAmbientOn();
            pointlight6.turnAmbientOn();
            pointlight7.turnAmbientOn();
            pointlight8.turnAmbientOn();
            pointlight9.turnAmbientOn();
            pointlight10.turnAmbientOn();
            spotlight1.turnAmbientOn();
            ambientToggle = !ambientToggle;
        }
    }
    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {

        if (diffuseToggle)
        {
            directionlight1.turnDiffuseOff();
            pointlight1.turnDiffuseOff();
            pointlight2.turnDiffuseOff();
            pointlight3.turnDiffuseOff();
            pointlight4.turnDiffuseOff();
            pointlight5.turnDiffuseOff();
            pointlight6.turnDiffuseOff();
            pointlight7.turnDiffuseOff();
            pointlight8.turnDiffuseOff();
            pointlight9.turnDiffuseOff();
            pointlight10.turnDiffuseOff();
            spotlight1.turnDiffuseOff();
            diffuseToggle = !diffuseToggle;
        }
        else
        {
            directionlight1.turnDiffuseOn();
            pointlight1.turnDiffuseOn();
            pointlight2.turnDiffuseOn();
            pointlight3.turnDiffuseOn();
            pointlight4.turnDiffuseOn();
            pointlight5.turnDiffuseOn();
            pointlight6.turnDiffuseOn();
            pointlight7.turnDiffuseOn();
            pointlight8.turnDiffuseOn();
            pointlight9.turnDiffuseOn();
            pointlight10.turnDiffuseOn();
            spotlight1.turnDiffuseOn();
            diffuseToggle = !diffuseToggle;
        }
    }
    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {

        if (specularToggle)
        {
            directionlight1.turnSpecularOff();
            pointlight1.turnSpecularOff();
            pointlight2.turnSpecularOff();
            pointlight3.turnSpecularOff();
            pointlight4.turnSpecularOff();
            pointlight5.turnSpecularOff();
            pointlight6.turnSpecularOff();
            pointlight7.turnSpecularOff();
            pointlight8.turnSpecularOff();
            pointlight9.turnSpecularOff();
            pointlight10.turnSpecularOff();
            spotlight1.turnSpecularOff();
            specularToggle = !specularToggle;
        }
        else
        {
            directionlight1.turnSpecularOn();
            pointlight1.turnSpecularOn();
            pointlight2.turnSpecularOn();
            pointlight3.turnSpecularOn();
            pointlight4.turnSpecularOn();
            pointlight5.turnSpecularOn();
            pointlight6.turnSpecularOn();
            pointlight7.turnSpecularOn();
            pointlight8.turnSpecularOn();
            pointlight9.turnSpecularOn();
            pointlight10.turnSpecularOn();
            spotlight1.turnSpecularOn();
            specularToggle = !specularToggle;
        }
    }
    if (key == GLFW_KEY_T && action == GLFW_PRESS)
    {
        if (!train_on)
        {
            train_on = true;
        }
        else
        {
            train_on = false;
        }

    }
    if (key == GLFW_KEY_R && action == GLFW_PRESS)
    {
        if (!train_on_reverse)
        {
            train_on_reverse = true;
        }
        else
        {
            train_on_reverse = false;
        }

    }
    if (key == GLFW_KEY_C && action == GLFW_PRESS)
    {
        if (!camera_on)
        {
            camera_on = true;
        }
        else
        {
            train_on_reverse = false;
        }

    }
    if (key == GLFW_KEY_F && action == GLFW_PRESS)
    {
        if (!fan_on)
        {
            fan_on = true;
        }
        else
        {
            fan_on = false;
        }

    }
   
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
