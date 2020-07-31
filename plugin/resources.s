    # Assembly to include resources
    .section .rodata

    # shader.frag.glsl
    .global kpipes_resource_shader_frag_glsl_start
    .type   kpipes_resource_shader_frag_glsl_start, @object
    .align  8
kpipes_resource_shader_frag_glsl_start:
    .incbin "shader.frag.glsl"
kpipes_resource_shader_frag_glsl_end:
    .global kpipes_resource_shader_frag_glsl_size
    .type   kpieps_resource_shader_frag_glsl_size, @object
    .align  8
kpipes_resource_shader_frag_glsl_size:
    .quad kpipes_resource_shader_frag_glsl_end - kpipes_resource_shader_frag_glsl_start

    # shader.vert.glsl
    .global kpipes_resource_shader_vert_glsl_start
    .type   kpipes_resource_shader_vert_glsl_start, @object
    .align  8
kpipes_resource_shader_vert_glsl_start:
    .incbin "shader.vert.glsl"
kpipes_resource_shader_vert_glsl_end:
    .global kpipes_resource_shader_vert_glsl_size
    .type   kpipes_resource_shader_vert_glsl_size, @object
    .align  8
kpipes_resource_shader_vert_glsl_size:
    .quad kpipes_resource_shader_vert_glsl_end - kpipes_resource_shader_vert_glsl_start
