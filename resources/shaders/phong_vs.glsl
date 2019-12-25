#version 450

in vec3 i_position;               //input position
in vec3 i_normal;                 //input normal

out vec3 v_normal;                //normal with any rotations applied
out vec4 v_color;                 //output color (will be interpolated across primitive)

uniform mat4 u_projection_matrix; //perspective projection matrix
uniform mat4 u_view_matrix;       //made with glm::lookat()

uniform vec4 u_color;              //allows separate colors per piece
uniform vec3 u_offset;             //allows for placement of pieces

uniform float rot;            //allows the user to rotate the board

mat4 rotation(vec3 axis, float angle)
{//thanks to Neil Mendoza via http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    v_color = u_color*(i_position.y+1); //passthrough

    v_normal = i_normal;            //passthrough, CURRENTLY
    //make sure to transform normal along with the point, when we get there
    //v_normal =

    //have to add the offset, before model(rotation),view,projection
    gl_Position = u_projection_matrix*u_view_matrix*rotation(vec3(0,1,0),0.01*rot)*vec4( i_position, 1.0 );
}
