uniform mat4 u_vpf_matrix;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec3 a_color;

varying vec3 v_normal;
varying vec3 v_color;

void main() {
    gl_Position = u_vpf_matrix * vec4(a_position, 1);
    v_normal = a_normal;
    v_color = a_color;
}
