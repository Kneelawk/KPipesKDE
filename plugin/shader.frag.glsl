uniform vec3 u_light1_direction;
uniform float u_light1_strength;
uniform vec3 u_light2_direction;
uniform float u_light2_strength;
uniform float u_ambient_light;

varying vec3 v_normal;
varying vec3 v_color;

float calc_darkness(vec3 normal, vec3 direction, float strength) {
    float brightness = clamp(-dot(normal, direction) * strength, 0.0, 1.0);

    return 1.0 - brightness;
}

void main() {
    vec3 normal = normalize(v_normal);

    float darkness = 1.0 - u_ambient_light;

    darkness *= calc_darkness(normal, u_light1_direction, u_light1_strength);
    darkness *= calc_darkness(normal, u_light2_direction, u_light2_strength);

    float brightness = 1.0 - darkness;

    gl_FragColor = vec4(v_color * brightness, 1.0);
}
