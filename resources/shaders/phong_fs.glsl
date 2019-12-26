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
  vec3 light = (rotation(vec3(0,1,0),-0.001*light_rotation)*vec4(0.3, 0.4+0.1*sin(0.01*light_rotation), 0.25, 1)).xyz;

  o_color = v_color;



  if((mode == 4||mode == 2||mode == 3) && v_color.b > 0.5)
  {
    discard;
  }

  if(mode == 0)
  {
    //phong lighting model

    //l is norm(vpos - light_position)
    //v is norm(vpos - eye_position)
    //r comes from l and normal, using reflect()
    //n is the normal that's been passed in

    vec3 l = normalize(v_position - light);
    vec3 v = normalize(v_position - eye);
    vec3 n = normalize(v_normal);
    vec3 r = normalize(reflect(l, n));

    //ambient
    float a = -0.4;
    //diffuse
    float d = (1/(pow(0.25*distance(v_position,light),2))) * 0.7 * max(dot(n, l),-0.4);
    //specular
    float s = (1/(pow(0.25*distance(v_position,light),2))) * 1.6 * pow(max(dot(r,v),0),10);

    o_color += a * vec4(0.4,0.4,0.4,1);
    o_color += d * vec4(0.56, 0.3,0.1, 1);

    if(!gl_FrontFacing)
    {
      if(dot(n,l) > 0)
        o_color += s * vec4(1,0.3,0,1);

        // o_color = vec4(1,0,0,1);

    }
  }


  o_color.a = 1;

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
