attribute vec3 coord;
uniform mat4 matrix;
uniform mat4 projection;
void main() {
gl_Position = projection * matrix * vec4(coord, 1.0);
}