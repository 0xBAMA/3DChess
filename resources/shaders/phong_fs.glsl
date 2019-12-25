#version 450

in vec4 v_color;      //interpolated vertex color
in vec3 v_normal;     //interpolated normal

out vec4 o_color;     //output pixel color

void main()
{
    o_color = v_color;

    if(gl_FrontFacing)
    {
      int fcxmod = int(gl_FragCoord.x) % 2;
      int fcymod = int(gl_FragCoord.y) % 2;

      if((fcymod != 0) || (fcxmod != 0))
      {
        discard;
      }
    }
    else
    {
      //int fcxmod = int(gl_FragCoord.x) % 2;
      //int fcymod = int(gl_FragCoord.y) % 2;

      //if((fcymod == 0) || (fcxmod == 0))
      //{
      //  discard;
      //}
    }
}
