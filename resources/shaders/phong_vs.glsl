#version 450

in vec3 i_position;   //input position
in vec3 i_normal;     //input normal

out vec3 v_normal;    //normal with any rotations applied
out vec4 v_color;     //output color (will be interpolated across primitive)

uniform mat4 u_projection_matrix;
uniform mat4 u_view_matrix;

void main()
{
    v_color = vec4(0,0,0,1);
    gl_Position = vec4( i_position, 1.0 );
    //gl_Position = vec4(0,0,0,1);
}
