#version 450

in vec3 i_position;               //input position
in vec3 i_normal;                 //input normal

out vec3 v_normal;                //normal with any rotations applied
out vec4 v_color;                 //output color (will be interpolated across primitive)

uniform mat4 u_projection_matrix; //perspective projection matrix
uniform mat4 u_view_matrix;       //made with glm::lookat()

uniform vec4 u_color;              //allows separate colors per piece
uniform vec3 u_offset;             //allows for placement of pieces

uniform float rotation;            //allows the user to rotate the board

void main()
{
    v_color = u_color*(i_position.y+1); //passthrough

    v_normal = i_normal;            //passthrough, CURRENTLY
    //make sure to transform normal along with the point, when we get there
    //v_normal =

    //have to add the offset, before model(rotation),view,projection
    gl_Position = u_projection_matrix*u_view_matrix*vec4( i_position, 1.0 );
}
