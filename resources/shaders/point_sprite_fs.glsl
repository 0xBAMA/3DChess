  #version 450

  in vec4 v_color;      //interpolated vertex color
  in vec3 v_normal;     //interpolated normal

  out vec4 o_color;     //output pixel color

  void main()
  {
      o_color = v_color;
  };
