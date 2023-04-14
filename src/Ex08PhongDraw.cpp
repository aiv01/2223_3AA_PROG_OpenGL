#include "Ex08PhongDraw.h"
#include "Common.h" 
#include <glad/glad.h>
#include <string>
#include <fstream>
#include <vector>
#define OBJ_PARSER_IMPLEMENTATION
#include "obj-parser.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

void Ex08PhongDraw::Start()
{
    Program = new OGLProgram("resources/shaders/phong.vert", "resources/shaders/phong.frag");

    obj_t* mesh = obj_parser_parse("resources/models/stormtrooper.obj");

    std::vector<float> Vertices;
    for(int i=0; i < mesh->face_count; ++i) 
    {
        obj_triangle_t& t = mesh->triangles[i];
        
        Vertices.push_back(t.v1.position.x);
        Vertices.push_back(t.v1.position.y);
        Vertices.push_back(t.v1.position.z);
        Vertices.push_back(t.v1.uv.x);
        Vertices.push_back(t.v1.uv.y);
        Vertices.push_back(t.v1.normal.x);
        Vertices.push_back(t.v1.normal.y);
        Vertices.push_back(t.v1.normal.z);

        Vertices.push_back(t.v2.position.x);
        Vertices.push_back(t.v2.position.y);
        Vertices.push_back(t.v2.position.z);
        Vertices.push_back(t.v2.uv.x);
        Vertices.push_back(t.v2.uv.y);
        Vertices.push_back(t.v2.normal.x);
        Vertices.push_back(t.v2.normal.y);
        Vertices.push_back(t.v2.normal.z);

        Vertices.push_back(t.v3.position.x);
        Vertices.push_back(t.v3.position.y);
        Vertices.push_back(t.v3.position.z);
        Vertices.push_back(t.v3.uv.x);
        Vertices.push_back(t.v3.uv.y);
        Vertices.push_back(t.v3.normal.x);
        Vertices.push_back(t.v3.normal.y);
        Vertices.push_back(t.v3.normal.z);
    }
    
    VerticeCount = Vertices.size() / 8;

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
    glVertexAttribPointer(Location_0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(Location_0);

    GLuint Location_1 = 1;
    glVertexAttribPointer(Location_1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(Location_1);

    GLuint Location_2 = 2;
    glVertexAttribPointer(Location_2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(Location_2);

    //4. Set Viewport
    glViewport(0, 0, 600, 400);
    glClearColor(0.5f, 0.5f, 0.5f, 1.f);
    Program->Bind();

    StormText = new OGLTexture("resources/models/stormtrooper.png");
    StormText->Bind(GL_TEXTURE0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    //glFrontFace(GL_CCW); //default
    //glCullFace(GL_BACK); //default

    //camera
    glm::vec3 Position = glm::vec3(0, 0, 8);
    glm::vec3 Direction = glm::vec3(0, 0, -1);
    glm::vec3 Up = glm::vec3(0, 1, 0);
    float FovY = 60;
    float AspectRatio = 600.f / 400.f;
    float ZNear = 0.01f;
    float ZFar = 1000.f;

    View = glm::lookAt(Position, Position + Direction, Up);
    Projection = glm::perspective(glm::radians(FovY), AspectRatio, ZNear, ZFar);


    glm::vec3 PointLightPos = glm::vec3(4, 0, 0);
    Program->SetUniform("point_light_pos", PointLightPos);
}

void Ex08PhongDraw::Update(float InDeltaTime)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    static float ElapsedTime = 0;
    ElapsedTime += InDeltaTime;
    float Angle = 20.f * ElapsedTime;

    //Model =  translate * rotate * scale;
    glm::mat4 Model = glm::mat4(1.f);
    Model = glm::translate(Model, glm::vec3(0, -4, 0));
    Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(0, 1, 0));
    Model = glm::scale(Model, glm::vec3(2.f));

    glm::mat4 Mvp = Projection * View * Model;
    Program->SetUniform("mvp", Mvp);

    glDrawArrays(GL_TRIANGLES, 0, VerticeCount);
}

void Ex08PhongDraw::Destroy()
{
    glDeleteVertexArrays(1, &Vao);
    glDeleteBuffers(1, &Vbo);
    delete Program;
    delete StormText;
}