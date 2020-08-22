uniform mat4 u_vpf_matrix;

attribute vec3 s_color;
attribute mat4 s_model;

attribute vec3 a_position;
attribute vec3 a_normal;

varying vec3 v_normal;
varying vec3 v_color;

void main() {
    gl_Position = u_vpf_matrix * s_model * vec4(a_position, 1);
    v_normal = normalize((s_model * vec4(a_normal, 0.0)).xyz);
    v_color = s_color;
}
