#version 330 core
layout (location = 0) in vec3 aPos; // Position attribute
layout (location = 1) in vec3 aNormal; // normal attribute
// view matrix
// Uniforms
uniform mat4 model;        // Model matrix
uniform mat4 view;         // View matrix
uniform mat4 projection;   // Projection matrix

vec3 color = vec3(1.0, 0.0, 0.0); // Default color

out vec3 vertexColor; // Pass color to fragment shader
out vec3 fragNormal;       // Transformed normal for lighting (optional)

// Output to fragment shader
// out vec3 fragNormal;       // Transformed normal for lighting (optional)

void main() {
    // gl_Position = vec4(aPos, 1.0); // Set the vertex position
    

     // Apply transformations: Model -> View -> Projection
    mat4 modelViewProjection = projection * view;// * model;
    gl_Position = modelViewProjection * vec4(aPos, 1.0);

    vertexColor = color; // Pass color to fragment shader 
    fragNormal = aNormal;


    // Pass transformed normal to fragment shader (if needed)
    // fragNormal = mat3(transpose(inverse(model))) * normal;
}