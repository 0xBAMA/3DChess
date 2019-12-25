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
  game my_game;  //This is where the game is

  GLuint vao;
  GLuint vbo;

  GLuint shader_program;

  int num_pts_board;
  //indexes for pieces?

  std::vector<glm::vec3> points;    //add the 1.0 w value in the shader
  // std::vector<glm::vec2> texcoords; //texture coordinates
  std::vector<glm::vec3> normals;   //represents surface orientation
  // std::vector<glm::vec4> colors;    //support alpha

};


opengl_container::opengl_container()
{

  // static const int width = 1200;
  // static const int height = 700;

  shader_program = Shader("resources/shaders/point_sprite_vs.glsl", "resources/shaders/point_sprite_fs.glsl").Program;

  glUseProgram( shader_program );

  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.5, 0.0, 0.0, 0.0 );
  // glViewport( 0, 0, width, height );

  glGenVertexArrays( 1, &vao );
  glBindVertexArray( vao );

  glGenBuffers( 1, &vbo );
  glBindBuffer( GL_ARRAY_BUFFER, vbo );



//Generate points

  // std::random_device rd;
  // std::mt19937 mt(rd());
  //
	// std::uniform_real_distribution<float> dist1(0.3f,9.8f); //input values
	// std::uniform_int_distribution<int> dist2(0,nodes.size()-1);  //nodes







  GLfloat left = -1.366f;
  GLfloat right = 1.366f;
  GLfloat top = -0.768f;
  GLfloat bottom = 0.768f;
  GLfloat zNear = 1.2f;
  GLfloat zFar = -1.0f;
  glm::mat4 proj = glm::ortho(left, right, top, bottom, zNear, zFar);
  glUniformMatrix4fv( glGetUniformLocation( shader_program, "u_projection_matrix" ), 1, GL_FALSE, glm::value_ptr(proj) );



  glBindVertexArray( vao );


  // my_game.dump();

}


#endif
