#include "Ex04QuadColor.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>

struct Color {
    float R;
    float G;
    float B;
    float A;
};

void Ex04QuadColor::Start()
{
    Program = new OGLProgram("resources/shaders/quadcolor.vert", "resources/shaders/quadcolor.frag");
    
    std::vector<float> Vertices = {
        -0.5f,  0.5f, 0.f,  //top left
        -0.5f, -0.5f, 0.f,  //bottom left
         0.5f,  0.5f, 0.f,  //top right    
         0.5f, -0.5f, 0.f,  //bottom right
    };

    std::vector<uint32_t> Indexes = {
        0, 1, 2, //Left Triangle
        2, 3, 1, //Right Triangle
    };

    //1. Create VAO
    glGenVertexArrays(1, &Vao);
    glBindVertexArray(Vao);

    //2. Create VBO to load data
    glGenBuffers(1, &Vbo);
    glBindBuffer(GL_ARRAY_BUFFER, Vbo);

    int DataSize = Vertices.size() * sizeof(float);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Vertices.data(), GL_STATIC_DRAW);

    //3. Link to Vertex Shader
    GLuint Location_0 = 0;
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    //4. Crea EBO
    glGenBuffers(1, &Ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Ebo);
    int EboSize = Indexes.size() * sizeof(uint32_t);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, EboSize, Indexes.data(), GL_STATIC_DRAW);

    //4. Set Viewport
    glViewport(0, 0, 600, 400);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    GLint BaseColorLoc = glGetUniformLocation(Program->ID(), "base_color");
    Color Blue{0, 0, 1, 1};
    GLfloat* BluePtr = (GLfloat*)&Blue;
    glUniform4fv(BaseColorLoc, 1, BluePtr);
}

void Ex04QuadColor::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;

    Color TimeColor;
    TimeColor.R = sinf(ElapsedTime) * 0.5f + 0.5f;
    TimeColor.G = cosf(ElapsedTime) * 0.5f + 0.5f;
    TimeColor.B = cosf(ElapsedTime + 1.1f) * 0.5f + 0.5f;
    TimeColor.A = 1.f;

    glUniform4fv(glGetUniformLocation(Program->ID(), "base_color"), 1, (GLfloat*)&TimeColor);
}

void Ex04QuadColor::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Program;
}