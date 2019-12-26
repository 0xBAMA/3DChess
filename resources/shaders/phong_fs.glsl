#version 450

in vec3 v_position;   //interpolated position
in vec3 v_normal;     //interpolated normal
in vec4 v_color;      //interpolated vertex color

out vec4 o_color;     //output pixel color

uniform int mode;


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


vec3 eye = vec3(-1.3, 1.0, -1.7);

uniform float light_rotation;

void main()
{
  vec3 light = (rotation(vec3(0,1,0),0.001*light_rotation)*vec4(1.8, 1.6, -1.9, 1)).xyz;

  o_color = v_color;

  // if(gl_FrontFacing)
  // {
  //   o_color = vec4(1,0,0,1);
  // }

  if(mode == 4 && v_color.b > 0.5)
  {
    discard;
  }

  if(mode == 0)
  { //board


  }

}











//    if(gl_FrontFacing)
//    {
//      int fcxmod = int(gl_FragCoord.x) % 2;
//      int fcymod = int(gl_FragCoord.y) % 2;
//
//      if((fcymod != 0) || (fcxmod != 0))
//      {
//        discard;
//      }
//    }
