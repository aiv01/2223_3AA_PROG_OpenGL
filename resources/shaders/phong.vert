#version 450 core
layout (location = 0) in vec3 vert_pos;
layout (location = 1) in vec2 vert_uv;
layout (location = 2) in vec3 vert_norm;

uniform mat4 mvp;

out vec2 vert_uv_out;
out vec3 vert_norm_out;

void main()
{
    gl_Position = mvp * vec4(vert_pos, 1.f);
    vert_uv_out = vert_uv;
    vert_norm_out = vert_norm;
}
