#ifndef LOADSHADERS_H
#define LOADSHADERS_H

#include <GL/glew.h>
#include <stdio.h>
#include <stdlib.h>

// Create a NULL-terminated string by reading the provided file
char* readFile(const char* filename)
{
    FILE* fp = fopen(filename, "r");

    if (fp == NULL)
        return NULL;

    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char* buf = new char[size + 1];
    fread(buf, 1, size, fp);
    buf[size] = '\0';

    fclose(fp);
    return buf;
}

// Create a GLSL program object from vertex and fragment shader files
GLuint createShadersProgram(const char* vsFile, const char* fsFile)
{
    struct Shader
    {
        const char* filename;
        GLenum type;
        GLchar* source;
    };

    struct Shader shaders[] = {
        { vsFile, GL_VERTEX_SHADER, NULL },
        { fsFile, GL_FRAGMENT_SHADER, NULL }
    };

    GLuint program = glCreateProgram();

    for (int i = 0; i < 2; ++i)
    {
        Shader& s = shaders[i];
        s.source = readFile(s.filename);

        if (shaders[i].source == NULL)
        {
            fprintf(stderr, "Failed to read %s\n", s.filename);
            exit(EXIT_FAILURE);
        }

        GLuint shader = glCreateShader(s.type);
        glShaderSource(shader, 1, (const GLchar**)&s.source, NULL);
        glCompileShader(shader);
        GLint  compiled;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

        if (!compiled)
        {
            GLint  logSize;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
            char* logMsg = new char[logSize];
            glGetShaderInfoLog(shader, logSize, NULL, logMsg);
            fprintf(stderr, "%s failed to compile:\n%s\n", s.filename, logMsg);
            delete[] logMsg;
            exit(EXIT_FAILURE);
        }

        delete[] s.source;
        glAttachShader(program, shader);
    }

    // link and error check
    glLinkProgram(program);
    GLint  linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);

    if (!linked)
    {
        GLint  logSize;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
        char* logMsg = new char[logSize];
        glGetProgramInfoLog(program, logSize, NULL, logMsg);
        fprintf(stderr, "Shader program failed to link:\n%s\n", logMsg);
        delete[] logMsg;
        exit(EXIT_FAILURE);
    }

    // use program object
    glUseProgram(program);
    return program;
}

#endif // LOADSHADERS_H