  #version 450

  in vec4 v_color;      //interpolated vertex color
  out vec4 o_color;     //output pixel color

  void main() {
      o_color = v_color;
  };
