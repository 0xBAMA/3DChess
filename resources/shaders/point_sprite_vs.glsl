  #version 450

  in vec2 i_position;   //input position
  in vec3 i_normal;     //input normal
  in vec4 i_color;      //input color

  out vec3 v_normal;
  out vec4 v_color;     //output color (will be interpolated across primitive)

  uniform mat4 u_projection_matrix;

  void main()
  {
      v_color = i_color;
      gl_Position = u_projection_matrix * vec4( i_position, 0.0, 1.0 );
  };
