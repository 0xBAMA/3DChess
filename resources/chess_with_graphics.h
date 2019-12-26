#ifndef CHESS_WITH_GRAPHICS
#define CHESS_WITH_GRAPHICS


#include <stdio.h>
#include <random>
#include <vector>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

using std::cin;
using std::cerr;

using std::cout;
using std::flush;
using std::endl;


#define GLM_FORCE_SWIZZLE
#define GLM_SWIZZLE_XYZW
#include "glm/glm.hpp" //general vector types
#include "glm/gtc/matrix_transform.hpp" // for glm::ortho
#include "glm/gtc/type_ptr.hpp" //to send matricies gpu-side
#include "glm/gtx/transform.hpp"


#define GL_GLEXT_PROTOTYPES

#include "LodePNG/lodepng.h"
// Good, simple png library

#include "perlin.h"
//perlin noise generation


#include "game.h"
//this is our chess game code


#include <SDL.h>
#include <SDL_opengl.h>





//DEBUG STUFF

void GLAPIENTRY
MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{

  bool show_high_severity         = true;
  bool show_medium_severity       = true;
  bool show_low_severity          = true;
  bool show_notification_severity = true;

  if(severity == GL_DEBUG_SEVERITY_HIGH && show_high_severity)
    fprintf( stderr, "        GL CALLBACK: %s type = 0x%x, severity = GL_DEBUG_SEVERITY_HIGH, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, message );

  if(severity == GL_DEBUG_SEVERITY_MEDIUM && show_medium_severity)
    fprintf( stderr, "        GL CALLBACK: %s type = 0x%x, severity = GL_DEBUG_SEVERITY_MEDIUM, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, message );

  if(severity == GL_DEBUG_SEVERITY_LOW && show_low_severity)
    fprintf( stderr, "        GL CALLBACK: %s type = 0x%x, severity = GL_DEBUG_SEVERITY_LOW, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, message );

  if(severity == GL_DEBUG_SEVERITY_NOTIFICATION && show_notification_severity)
    fprintf( stderr, "        GL CALLBACK: %s type = 0x%x, severity = GL_DEBUG_SEVERITY_NOTIFICATION, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, message );
}









//  ╔═╗┬ ┬┌─┐┌┬┐┌─┐┬─┐
//  ╚═╗├─┤├─┤ ││├┤ ├┬┘
//  ╚═╝┴ ┴┴ ┴─┴┘└─┘┴└─

class Shader
{
  public:
    GLuint Program;
    // Constructor generates the shader on the fly
    Shader( const GLchar *vertexPath, const GLchar *fragmentPath, bool verbose=false)
    {

        // 1. Retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensures ifstream objects can throw exceptions:
        vShaderFile.exceptions ( std::ifstream::badbit );
        fShaderFile.exceptions ( std::ifstream::badbit );
        try
        {
            // Open files
            vShaderFile.open( vertexPath );
            fShaderFile.open( fragmentPath );
            std::stringstream vShaderStream, fShaderStream;
            // Read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf( );
            fShaderStream << fShaderFile.rdbuf( );
            // close file handlers
            vShaderFile.close( );
            fShaderFile.close( );
            // Convert stream into string
            vertexCode = vShaderStream.str( );
            fragmentCode = fShaderStream.str( );
        }
        catch ( std::ifstream::failure &e )
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }


        const GLchar *vShaderCode = vertexCode.c_str( );
        const GLchar *fShaderCode = fragmentCode.c_str( );
        // 2. Compile shaders
        GLuint vertex, fragment;
        GLint success;
        GLchar infoLog[512];


        // Vertex Shader
        vertex = glCreateShader( GL_VERTEX_SHADER );

        glShaderSource( vertex, 1, &vShaderCode, NULL );

        glCompileShader( vertex );



        // Print compile errors if any
        glGetShaderiv( vertex, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( vertex, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Fragment Shader
        fragment = glCreateShader( GL_FRAGMENT_SHADER );
        glShaderSource( fragment, 1, &fShaderCode, NULL );
        glCompileShader( fragment );


        // Print compile errors if any
        glGetShaderiv( fragment, GL_COMPILE_STATUS, &success );
        if ( !success )
        {
            glGetShaderInfoLog( fragment, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // Shader Program
        this->Program = glCreateProgram( );
        glAttachShader( this->Program, vertex );
        glAttachShader( this->Program, fragment );
        glLinkProgram( this->Program );
        // Print linking errors if any
        glGetProgramiv( this->Program, GL_LINK_STATUS, &success );
        if (!success)
        {
            glGetProgramInfoLog( this->Program, 512, NULL, infoLog );
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
        // Delete the shaders as they're linked into our program now and no longer necessery
        glDeleteShader( vertex );
        glDeleteShader( fragment );

    }
    // Uses the current shader
    void Use( )
    {
        glUseProgram( this->Program );
    }
};




//  ╔═╗┌─┐┌─┐┌┐┌┌─┐┬    ╔═╗┌─┐┌┐┌┌┬┐┌─┐┬┌┐┌┌─┐┬─┐
//  ║ ║├─┘├┤ ││││ ┬│    ║  │ ││││ │ ├─┤││││├┤ ├┬┘
//  ╚═╝┴  └─┘┘└┘└─┘┴─┘  ╚═╝└─┘┘└┘ ┴ ┴ ┴┴┘└┘└─┘┴└─

class opengl_container
{
public:
  opengl_container();
  ~opengl_container();

  void update_rotation();
  void draw_board();

  void display();



private:
  game my_game;  //This is where the game is

  GLuint vao;
  GLuint vbo;

  GLuint shader_program;

  int num_pts_board;
  //indexes for pieces?

  std::vector<glm::vec3> points;    //add the 1.0 w value in the shader
  std::vector<glm::vec3> normals;   //represents surface orientation
  std::vector<glm::vec3> colors;   //represents surface color
  std::vector<glm::vec3> selection_colors; //represents surface for selection

  float rotation_of_board;

  // int white_space_start, white_space_num;
  // int black_space_start, black_space_num;
  int board_start, board_num;

  glm::vec3 white = glm::vec3(1,0.9,0.76);
  glm::vec3 black = glm::vec3(0.1,0.1,0);


  glm::vec3 offsets[8][8];  //used to place pieces more simply



  SDL_Window * window;
  SDL_GLContext context;


};



opengl_container::opengl_container()
{

  SDL_Init( SDL_INIT_VIDEO );
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
  SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
  SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLEBUFFERS, 1);
  SDL_GL_SetAttribute( SDL_GL_MULTISAMPLESAMPLES, 8);

  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
  SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

  static const int width = 1200;
  static const int height = 700;

  window = SDL_CreateWindow( "OpenGL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
  context = SDL_GL_CreateContext( window );





  //DEBUG
  glEnable              ( GL_DEBUG_OUTPUT );
  glDebugMessageCallback( MessageCallback, 0 );


  // static const int width = 1200;
  // static const int height = 700;

  Shader s("resources/shaders/phong_vs.glsl", "resources/shaders/phong_fs.glsl");
  shader_program = s.Program;


  glUseProgram( shader_program );



  glEnable( GL_DEPTH_TEST );

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


  glEnable(GL_LINE_SMOOTH);
  glEnable(GL_POLYGON_SMOOTH);







  glPointSize(16.0f);

  glClearColor( 0.5, 0.0, 0.0, 0.0 );

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );



//clear out any data
  points.clear();
  normals.clear();
  colors.clear();
  selection_colors.clear();



//Then generate points/normals

  std::random_device rd;
  std::mt19937 mt(rd());

	std::uniform_real_distribution<float> dist1(-1.0f, 1.0f); //input values
	// std::uniform_int_distribution<int> dist2(0,nodes.size()-1);

  board_start = points.size();

  glm::vec3 up = glm::vec3(0,1,0);

  for(int i = 0; i < 64; i++)
  {
    int x = i % 8;
    int y = i / 8;

    if(y % 2 == 0)
    {
      if(x % 2 == 0)
      {
        for(int i = 0; i < 6; i++) colors.push_back(black);
      }
      else
      {
        for(int i = 0; i < 6; i++) colors.push_back(white);
      }
    }
    else
    {
      if(x % 2 == 0)
      {
        for(int i = 0; i < 6; i++) colors.push_back(white);
      }
      else
      {
        for(int i = 0; i < 6; i++) colors.push_back(black);
      }
    }

//normals + selection colors
    for(int i = 0; i < 6; i++)
    {
      selection_colors.push_back(glm::vec3(x*0.125, y*0.125, 0));
      normals.push_back(up);
    }


    //  triangle 1 is ABC
    // A       B
    //  +-----+
    //  | 1 / |
    //  |  /  |
    //  | / 2 |
    //  +-----+
    // C       D
    //  triangle 2 is CBD


    //A : (n,n+1,-0.3);
    //B : (n+1,n+1,-0.3);
    //C : (n,n,-0.3);
    //D : (n+1,n,-0.3);

    glm::vec3 A = glm::vec3(-0.8+1.6*((float)(x)/8.0),-0.3,-0.8+1.6*((float)(y+1)/8.0));
    glm::vec3 B = glm::vec3(-0.8+1.6*((float)(x+1)/8.0),-0.3,-0.8+1.6*((float)(y+1)/8.0));
    glm::vec3 C = glm::vec3(-0.8+1.6*((float)(x)/8.0),-0.3,-0.8+1.6*((float)(y)/8.0));
    glm::vec3 D = glm::vec3(-0.8+1.6*((float)(x+1)/8.0),-0.3,-0.8+1.6*((float)(y)/8.0));

    offsets[x][y] = (A+B+C+D)/4.0f;

    points.push_back(A);
    points.push_back(B);
    points.push_back(C);

    points.push_back(C);
    points.push_back(B);
    points.push_back(D);
  }

  board_num = points.size() - board_start;

  // points_start = points.size();
  //
  //
  //
  // points_num = points.size() - points_start;
  //







  const GLuint num_bytes_points           = sizeof(glm::vec3) * points.size();
  cout << endl << num_bytes_points << endl;

  const GLuint num_bytes_normals          = sizeof(glm::vec3) * normals.size();
  cout << num_bytes_normals << endl;

  const GLuint num_bytes_colors           = sizeof(glm::vec3) * colors.size();
  cout << num_bytes_colors << endl;

  const GLuint num_bytes_selection_colors = sizeof(glm::vec3) * selection_colors.size();
  cout << num_bytes_selection_colors << endl;



  GLint num_bytes = num_bytes_points + num_bytes_normals + num_bytes_colors + num_bytes_selection_colors;
  cout << endl << " " << num_bytes << endl << endl;

  glBufferData(GL_ARRAY_BUFFER, num_bytes, NULL, GL_STATIC_DRAW);




  cout << "setting up points attrib" << endl << std::flush;
  glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_points, &points[0]);
  glEnableVertexAttribArray(glGetAttribLocation(shader_program, "i_position"));
  glVertexAttribPointer(glGetAttribLocation(shader_program, "i_position"), 3, GL_FLOAT, false, 0, (static_cast<const char*>(0)));

  cout << "setting up normals attrib" << endl << std::flush;
  glBufferSubData(GL_ARRAY_BUFFER, num_bytes_points, num_bytes_normals, &normals[0]);
  glEnableVertexAttribArray(glGetAttribLocation(shader_program, "i_normal"));
  glVertexAttribPointer(glGetAttribLocation(shader_program, "i_normal"), 3, GL_FLOAT, false, 0, (static_cast<const char*>(0) + (num_bytes_points)));

  cout << "setting up colors attrib" << endl << std::flush;
  glBufferSubData(GL_ARRAY_BUFFER, num_bytes_points+num_bytes_normals, num_bytes_colors, &colors[0]);
  glEnableVertexAttribArray(glGetAttribLocation(shader_program, "i_color"));
  glVertexAttribPointer(glGetAttribLocation(shader_program, "i_color"), 3, GL_FLOAT, false, 0, (static_cast<const char*>(0) + (num_bytes_points+num_bytes_normals)));

  cout << "setting up selection colors attrib" << endl << std::flush;
  glBufferSubData(GL_ARRAY_BUFFER, num_bytes_points+num_bytes_normals+num_bytes_colors, num_bytes_selection_colors, &selection_colors[0]);
  glEnableVertexAttribArray(glGetAttribLocation(shader_program, "i_sel_color"));
  glVertexAttribPointer(glGetAttribLocation(shader_program, "i_sel_color"), 3, GL_FLOAT, false, 0, (static_cast<const char*>(0) + (num_bytes_points+num_bytes_normals+num_bytes_colors)));



  // GLfloat left = -1.366f;
  // GLfloat right = 1.366f;
  // GLfloat top = -0.768f;
  // GLfloat bottom = 0.768f;
  // GLfloat zNear = 1.2f;
  // GLfloat zFar = -1.0f;
  // glm::mat4 proj = glm::ortho(left, right, top, bottom, zNear, zFar);


  //convert to perspective
  glm::mat4 proj = glm::perspective(glm::radians(65.0f), 1366.0f / 768.0f, 0.25f, 6.0f);
  glUniformMatrix4fv( glGetUniformLocation( shader_program, "u_projection_matrix" ), 1, GL_FALSE, glm::value_ptr(proj) );


  glm::mat4 view = glm::lookAt(
      glm::vec3(-1.3f, 0.7f, -1.7f),
      glm::vec3(0.0f, 0.0f, 0.0f),
      glm::vec3(0.0f, 1.0f, 0.0f)
  );
  glUniformMatrix4fv( glGetUniformLocation( shader_program, "u_view_matrix" ), 1, GL_FALSE, glm::value_ptr(view) );




  glUniform1i(glGetUniformLocation( shader_program, "mode" ), 0);




  float rotation_of_board = 0.1*SDL_GetTicks();
  glUniform1fv(glGetUniformLocation(shader_program, "rot"), 1, &rotation_of_board);



  glm::vec4 color = glm::vec4(0,0,0,1);
  glUniform4fv(glGetUniformLocation(shader_program, "u_color"), 1, glm::value_ptr(color));

}

opengl_container::~opengl_container()
{
  SDL_GL_DeleteContext( context );
  SDL_DestroyWindow( window );
  SDL_Quit();
}


void opengl_container::update_rotation()
{
  rotation_of_board = 0.1*SDL_GetTicks();
  glUniform1fv(glGetUniformLocation(shader_program, "rot"), 1, &rotation_of_board);
}


void opengl_container::draw_board()
{
  // glm::vec4 color = glm::vec4(1,0.9,0.76,1);
  // glUniform4fv(glGetUniformLocation(shader_program, "u_color"), 1, glm::value_ptr(color));

  glUniform1i(glGetUniformLocation( shader_program, "mode" ), 0);
  glDrawArrays(GL_TRIANGLES, board_start, board_num);
  SDL_GL_SwapWindow( window );
  SDL_Delay(1000);



  glUniform1i(glGetUniformLocation( shader_program, "mode" ), 4);
  glDrawArrays(GL_TRIANGLES, board_start, board_num);
  SDL_GL_SwapWindow( window );
  SDL_Delay(1000);

}


void opengl_container::display()
{
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  SDL_Event event;
  while( SDL_PollEvent( &event ) )
  {
      switch( event.type )
      {
          case SDL_KEYUP:
              if( event.key.keysym.sym == SDLK_ESCAPE )
              {
                SDL_GL_DeleteContext( context );
                SDL_DestroyWindow( window );
                SDL_Quit();
              }
                  // return 0;
              break;
          //
          // case SDL_QUIT:
          //     cout << endl << "Quiting at: " << event.key.timestamp << endl;
          //     return 0;
          //     break;
      }
  }

  draw_board();

  update_rotation();

  SDL_GL_SwapWindow( window );

  SDL_Delay( 15 );
}

#endif
