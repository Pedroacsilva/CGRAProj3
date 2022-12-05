#ifndef DEECSHADER
#define DEECSHADER
#define GLEW_STATIC
#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/gl.h>


class DEECShader{
    public:

    GLuint  vertexshader;
    GLuint  fragmentshader;
    GLuint   shaderprogram;
    GLchar *   vertexsource;
    GLchar *   fragmentsource;
    DEECShader(){}
    ~DEECShader(){cleanup();}
    char * filetobuf(const char * file);
    GLuint  loadShaders(const char * vertex_file_path,const char * fragment_file_path);
    GLuint linkShaderProgram();
    void cleanup();
    inline void startUsing(){  glUseProgram(shaderprogram);}
    inline void stopUsing(){ glUseProgram(0);}
};

#endif
