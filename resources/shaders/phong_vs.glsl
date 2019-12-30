#version 450

in vec3 i_position;               //input position
in vec3 i_normal;                 //input normal
in vec3 i_color;                  //input color
in vec3 i_sel_color;              //input selection color

out vec3 v_position;              //position with any rotations applied
out vec3 v_normal;                //normal with any rotations applied
out vec4 v_color;                 //output color (will be interpolated across primitive)

uniform mat4 u_projection_matrix; //perspective projection matrix
uniform mat4 u_view_matrix;       //made with glm::lookat()

uniform vec4 u_color;              //allows separate colors per piece
uniform vec4 u_sel_color;          //allows separate selection colors per piece
uniform vec3 u_offset;             //allows for placement of pieces

//0 is board state
//1 is black piece
//2 is white piece
//3 is highlight square
//4 is selection mode
// ... ?
uniform int mode;

uniform float rot;            //allows the user to rotate the board
uniform float vrot;           // "     " but vertically

mat4 rotation(vec3 a, float angle)
{//thanks to Neil Mendoza via http://www.neilmendoza.com/glsl-rotation-about-an-arbitrary-axis/
    a = normalize(a); //a is the axis
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * a.x * a.x + c,         oc * a.x * a.y - a.z * s,  oc * a.z * a.x + a.y * s,  0.0,
                oc * a.x * a.y + a.z * s,   oc * a.y * a.y + c,        oc * a.y * a.z - a.x * s,  0.0,
                oc * a.z * a.x - a.y * s,   oc * a.y * a.z + a.x * s,  oc * a.z * a.z + c,        0.0,
                0.0,                        0.0,                       0.0,                       1.0);
}

void main()
{
  switch(mode)
  {
    case 0://standard display of board
      v_color = vec4(i_color,1.0);
      break;

    case 1://displaying white pieces
      v_color = vec4(1,0.9,0.76,1);
      break;

    case 2://displaying black pieces
      v_color = vec4(0.25,0.25,0,1);
      break;



    case 4://selection mode
      v_color = vec4(i_sel_color,1);
      break;

    case 5://piece selection mode
      v_color = u_color;
      break;
  }

    mat4 comp = rotation(vec3(1,0,0),0.001*vrot) * rotation(vec3(0,1,0),0.001*rot);

    v_normal = normalize((u_projection_matrix*u_view_matrix*comp*vec4(i_normal,1.0)).xyz);

    //have to add the offset, before model(rotation),view,projection
    gl_Position = u_projection_matrix*u_view_matrix*comp*vec4( i_position+u_offset, 1.0 );
    v_position = gl_Position.xyz;
}
