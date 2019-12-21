/*
    Minimal SDL2 + OpenGL3 example.
    Author: https://github.com/koute
    This file is in the public domain; you can do whatever you want with it.
    In case the concept of public domain doesn't exist in your jurisdiction
    you can also use this code under the terms of Creative Commons CC0 license,
    either version 1.0 or (at your option) any later version; for details see:
        http://creativecommons.org/publicdomain/zero/1.0/
    This software is distributed without any warranty whatsoever.
    Compile and run with: gcc opengl3_hello.c `sdl2-config --libs --cflags` -lGL -Wall && ./a.out
*/


#include <random>
#include <vector>
#include <stdio.h>
#include <iostream>

using std::cin;
using std::cerr;

using std::cout;
using std::flush;
using std::endl;


#define GLM_FORCE_SWIZZLE
#define GLM_SWIZZLE_XYZW
#include "resources/glm/glm.hpp" //general vector types
#include "resources/glm/gtc/matrix_transform.hpp" // for glm::ortho
#include "resources/glm/gtc/type_ptr.hpp" //to send matricies gpu-side
#include "resources/glm/gtx/transform.hpp"


#define GL_GLEXT_PROTOTYPES

#include "resources/LodePNG/lodepng.h"
// Good, simple png library

#include "resources/perlin.h"
//perlin noise generation


#include "chess_board.h"

// #include "resources/shaders/Shader.h"  //including this seg faults - I'm going to have to rewrite it


#include <SDL.h>
#include <SDL_opengl.h>

#include <stdio.h>


static inline void mat4x4_ortho( glm::mat4 &out, float left, float right, float bottom, float top, float znear, float zfar )
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


static const char * vertex_shader =
    "#version 450\n"
    "in vec2 i_position;\n"
    "in vec4 i_color;\n"
    "out vec4 v_color;\n"
    "uniform mat4 u_projection_matrix;\n"
    "void main() {\n"
    "    v_color = i_color;\n"
    "    gl_Position = u_projection_matrix * vec4( i_position, 0.0, 1.0 );\n"
    "}\n";


static const char * fragment_shader =
    "#version 450\n"
    "in vec4 v_color;\n"
    "out vec4 o_color;\n"
    "void main() {\n"
    "    o_color = v_color;\n"
    "}\n";


typedef enum t_attrib_id
{
    attrib_position,
    attrib_color

} t_attrib_id;


int main( int argc, char * argv[] )
{
    SDL_Init( SDL_INIT_VIDEO );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 5 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

    static const int width = 1200;
    static const int height = 700;

    SDL_Window * window = SDL_CreateWindow( "OpenGL Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    SDL_GLContext context = SDL_GL_CreateContext( window );



    //-----BEGIN SHADER COMPILATION SHIT-----

    GLuint vs, fs, program;

    vs = glCreateShader( GL_VERTEX_SHADER );
    fs = glCreateShader( GL_FRAGMENT_SHADER );

    int length = strlen( vertex_shader );
    glShaderSource( vs, 1, ( const GLchar ** )&vertex_shader, &length );
    glCompileShader( vs );

    GLint status;
    glGetShaderiv( vs, GL_COMPILE_STATUS, &status );
    if( status == GL_FALSE )
    {
        fprintf( stderr, "vertex shader compilation failed\n" );
        return 1;
    }

    length = strlen( fragment_shader );
    glShaderSource( fs, 1, ( const GLchar ** )&fragment_shader, &length );
    glCompileShader( fs );

    glGetShaderiv( fs, GL_COMPILE_STATUS, &status );
    if( status == GL_FALSE )
    {
        fprintf( stderr, "fragment shader compilation failed\n" );
        return 1;
    }

    program = glCreateProgram();
    glAttachShader( program, vs );
    glAttachShader( program, fs );

    glBindAttribLocation( program, attrib_position, "i_position" );
    glBindAttribLocation( program, attrib_color, "i_color" );
    glLinkProgram( program );

    //-----END SHADER COMPILATION SHIT-----

    glUseProgram( program );

    glDisable( GL_DEPTH_TEST );
    glClearColor( 0.5, 0.0, 0.0, 0.0 );
    glViewport( 0, 0, width, height );

    GLuint vao, vbo;

    glGenVertexArrays( 1, &vao );
    glGenBuffers( 1, &vbo );
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    glEnableVertexAttribArray( attrib_position );
    glEnableVertexAttribArray( attrib_color );

    glVertexAttribPointer( attrib_color, 4, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, 0 );
    glVertexAttribPointer( attrib_position, 2, GL_FLOAT, GL_FALSE, sizeof( float ) * 6, ( void * )(4 * sizeof(float)) );

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

    glm::mat4 projection_matrix;
    mat4x4_ortho( projection_matrix, 0.0f, (float)width, (float)height, 0.0f, 0.0f, 100.0f );
    glUniformMatrix4fv( glGetUniformLocation( program, "u_projection_matrix" ), 1, GL_FALSE, glm::value_ptr(projection_matrix) );


    //can we do images on top of OpenGL?  - looking like not really (12/18)

    // SDL_Renderer * ren  = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    // if (ren == NULL){ cerr << "SDL_CreateRenderer Error" << SDL_GetError() << endl; return EXIT_FAILURE; }
    //
    // SDL_Surface * bmp = SDL_LoadBMP("../2d/grumpy-cat.bmp");
    // if (bmp == NULL){ cerr << "SDL_LoadBMP Error: " << SDL_GetError() << endl; return EXIT_FAILURE; }
    //
    // SDL_Texture * tex = SDL_CreateTextureFromSurface(ren, bmp);
    // if (tex == NULL){ cerr << "SDL_CreateTextureFromSurface Error: " << SDL_GetError() << endl; return EXIT_FAILURE; }
    //
    //
    // SDL_FreeSurface(bmp);
    //
    // SDL_Rect SrcRect; //where are we taking pixels from?
    // SDL_Rect DestRect;  //the pixels we took from SrcRect?
    //
    // DestRect.x = SrcRect.x = 0;
    // DestRect.y = SrcRect.y = 0;
    // DestRect.w = SrcRect.w = 620;
    // DestRect.h = SrcRect.h = 387;

    glBindVertexArray( vao );


    for( ;; ) //same as while(true){}
    {
        glClear( GL_COLOR_BUFFER_BIT );

        SDL_Event event;
        while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                case SDL_KEYUP:
                    if( event.key.keysym.sym == SDLK_ESCAPE )
                        return 0;
                    break;

                case SDL_QUIT: 
                    cout << endl << "Quiting at: " << event.key.timestamp << endl;
                    return 0;
                    break;
            }
        }

        // SDL_RenderCopy(ren, tex, &SrcRect, &DestRect);
        // SDL_RenderFlush(ren);
        // SDL_RenderPresent(ren);

        glDrawArrays( GL_TRIANGLES, 0, 3 );
        glBindVertexArray( vao );

        SDL_GL_SwapWindow( window );

        SDL_Delay( 200 );

        glDrawArrays( GL_TRIANGLES, 3, 3 );
        SDL_GL_SwapWindow( window );

        SDL_Delay( 100 );
    }

    SDL_GL_DeleteContext( context );
    SDL_DestroyWindow( window );
    SDL_Quit();

    return 0;
}
