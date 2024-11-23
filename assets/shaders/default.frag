#version 330 core
in vec3 vertexColor; // Receive color from vertex shader
in vec3 fragNormal; // Receive normal from vertex shader

out vec4 FragColor; // Output fragment color
vec3 lightDirection = normalize(vec3(1.0, 1.0, 1.0)); // Light direction
vec3 lightColor = vec3(1.0, 1.0, 1.0); // Light color
float ambientStrength = 0.3; // Ambient light strength

void main() {
    // Calculate light intensity
    float intensity = max(dot(normalize(fragNormal), lightDirection), 0.0);
    // Calculate final color
    vec3 color = (ambientStrength + (1.0 - ambientStrength) * intensity) * vertexColor * lightColor;
    FragColor = vec4(color, 1.0);

}