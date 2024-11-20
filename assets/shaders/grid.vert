#version 330 core
layout (location = 0) in vec2 aPos;
uniform mat4 model;        // Model matrix
uniform mat4 view;         // View matrix
uniform mat4 projection;   // Projection matrix

void main() {
    mat4 mvp = projection * view ;
    gl_Position = mvp * vec4(aPos.x, 0.0, aPos.y, 1.0);
}
