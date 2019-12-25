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

  void update_rotation();




private:
  game my_game;  //This is where the game is

  GLuint vao;
  GLuint vbo;

  GLuint shader_program;

  int num_pts_board;
  //indexes for pieces?

  std::vector<glm::vec3> points;    //add the 1.0 w value in the shader
  std::vector<glm::vec3> normals;   //represents surface orientation

  float rotation_of_board;

};


opengl_container::opengl_container()
{

  //DEBUG
  glEnable              ( GL_DEBUG_OUTPUT );
  glDebugMessageCallback( MessageCallback, 0 );


  // static const int width = 1200;
  // static const int height = 700;

  Shader s("resources/shaders/phong_vs.glsl", "resources/shaders/phong_fs.glsl");
  shader_program = s.Program;

  cout << endl << endl << glGetAttribLocation(shader_program, "i_position");
  cout << endl << glGetAttribLocation(shader_program, "i_normal") << endl << endl;

  cout << endl << glGetUniformLocation( shader_program, "u_projection_matrix" ) << endl << endl;


  glUseProgram( shader_program );

  cout << endl << endl << glGetAttribLocation(shader_program, "i_position");
  cout << endl << glGetAttribLocation(shader_program, "i_normal") << endl << endl;

  cout << endl << glGetUniformLocation( shader_program, "u_projection_matrix" ) << endl << endl;


  glEnable( GL_DEPTH_TEST );
  glPointSize(16.0f);

  glClearColor( 0.5, 0.0, 0.0, 0.0 );
  // glViewport( -1, -1, 2, 2 );

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );



//clear out any data
  points.clear();
  normals.clear();



//Then generate points/normals

  std::random_device rd;
  std::mt19937 mt(rd());

	std::uniform_real_distribution<float> dist1(-1.0f, 1.0f); //input values
	// std::uniform_int_distribution<int> dist2(0,nodes.size()-1);  //nodes

  for(int i = 0; i < 1000; i++)
  {
    points.push_back(glm::vec3(dist1(mt),dist1(mt),dist1(mt)));
    normals.push_back(glm::normalize(glm::vec3(dist1(mt),dist1(mt),dist1(mt))));
  }



  const GLuint num_bytes_points = sizeof(glm::vec3) * points.size();
  const GLuint num_bytes_normals = sizeof(glm::vec3) * normals.size();

  GLint num_bytes = num_bytes_points + num_bytes_normals;

  glBufferData(GL_ARRAY_BUFFER, num_bytes, NULL, GL_STATIC_DRAW);

  glBufferSubData(GL_ARRAY_BUFFER, 0, num_bytes_points, &points[0]);
  glBufferSubData(GL_ARRAY_BUFFER, num_bytes_points, num_bytes_normals, &normals[0]);


  glEnableVertexAttribArray(glGetAttribLocation(shader_program, "i_position"));
  glEnableVertexAttribArray(glGetAttribLocation(shader_program, "i_normal"));

  cout << "setting up points attrib" << endl << std::flush;
  glVertexAttribPointer(glGetAttribLocation(shader_program, "i_position"), 3, GL_FLOAT, false, 0, (static_cast<const char*>(0)));

  cout << "setting up normals attrib" << endl << std::flush;
  glVertexAttribPointer(glGetAttribLocation(shader_program, "i_normal"), 3, GL_FLOAT, false, 0, (static_cast<const char*>(0) + (num_bytes_points)));




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


  float rotation_of_board = 0.1*SDL_GetTicks();
  glUniform1fv(glGetUniformLocation(shader_program, "rot"), 1, &rotation_of_board);

  glm::vec4 color = glm::vec4(1,0.4,0,1);
  glUniform4fv(glGetUniformLocation(shader_program, "u_color"), 1, glm::value_ptr(color));

}

void opengl_container::update_rotation()
{
  rotation_of_board = 0.1*SDL_GetTicks();
  glUniform1fv(glGetUniformLocation(shader_program, "rot"), 1, &rotation_of_board);
}


#endif
