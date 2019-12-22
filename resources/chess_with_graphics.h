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




private:
  void mat4x4_ortho( glm::mat4 &out, float left, float right, float bottom, float top, float znear, float zfar );
  game my_game;
};


opengl_container::opengl_container()
{

  static const int width = 1200;
  static const int height = 700;

  GLuint program = Shader("resources/shaders/vs.glsl", "resources/shaders/fs.glsl").Program;

  glUseProgram( program );

  glDisable( GL_DEPTH_TEST );
  glClearColor( 0.5, 0.0, 0.0, 0.0 );
  glViewport( 0, 0, width, height );

  GLuint vao, vbo;

  glGenVertexArrays( 1, &vao );
  glGenBuffers( 1, &vbo );
  glBindVertexArray( vao );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );

  glEnableVertexAttribArray( glGetAttribLocation(program, "i_position"));
  glEnableVertexAttribArray( glGetAttribLocation(program, "i_color"));

  glVertexAttribPointer( glGetAttribLocation(program, "i_position"), 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, 0 );
  glVertexAttribPointer( glGetAttribLocation(program, "i_color"), 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, ( void * )(4 * sizeof(float)) );

  const GLfloat g_vertex_buffer_data[] = {
  /*  R, G, B, A, X, Y  */
      1, 0, 0, 1, 0, 0,
      0, 1, 0, 1, width, 0,
      0, 0, 1, 1, width, height,

      1, 0, 0, 1, 0, 0,
      0, 0, 1, 1, width, height,
      1, 1, 1, 1, 0, height
  };

  glBufferData( GL_ARRAY_BUFFER, sizeof( g_vertex_buffer_data ), g_vertex_buffer_data, GL_STATIC_DRAW );

  // glm::mat4 projection_matrix;
  // mat4x4_ortho( projection_matrix, 0.0f, (float)width, (float)height, 0.0f, 0.0f, 100.0f );
  // glUniformMatrix4fv( glGetUniformLocation( program, "u_projection_matrix" ), 1, GL_FALSE, glm::value_ptr(projection_matrix) );


  GLfloat left = -1.366f;
  GLfloat right = 1.366f;
  GLfloat top = -0.768f;
  GLfloat bottom = 0.768f;
  GLfloat zNear = 1.2f;
  GLfloat zFar = -1.0f;
  glm::mat4 proj = glm::ortho(left, right, top, bottom, zNear, zFar);
  glUniformMatrix4fv( glGetUniformLocation( program, "u_projection_matrix" ), 1, GL_FALSE, glm::value_ptr(proj) );



  glBindVertexArray( vao );


  // my_game.dump();

}






void opengl_container::mat4x4_ortho( glm::mat4 &out, float left, float right, float bottom, float top, float znear, float zfar )
{

    out[0][0] = 2.0f / (right - left);
    out[0][1] = 0.0f;
    out[0][2] = 0.0f;
    out[0][3] = 0.0f;

    out[1][1] = 2.0f / (top - bottom);
    out[1][0] = 0.0f;
    out[1][2] = 0.0f;
    out[1][3] = 0.0f;

    out[2][2] = -2.0f / (zfar - znear);
    out[2][0] = 0.0f;
    out[2][1] = 0.0f;
    out[2][3] = 0.0f;

    out[3][0] = -(right + left) / (right - left);
    out[3][1] = -(top + bottom) / (top - bottom);
    out[3][2] = -(zfar + znear) / (zfar - znear);
    out[3][3] = 1.0f;

}








#endif
