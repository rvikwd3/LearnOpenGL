#version 330

layout (location = 0) in vec3 aPos;

out vec4 vertex_color;

void main(){
    vertex_color = vec4(0.5, 0.0, 0.0, 1.0);
    gl_Position = vec4(aPos, 1.0);
}
