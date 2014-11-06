#include <Windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <stdio.h>
#include "loadshaders.h"
#include "lodepng.h"
#include <iostream>
using glm::vec4;

const int NumVertices = 6;

GLuint tex_2d;
GLuint program;
void init()
{
    // Specifiy the vertices for a triangle.
	//Since it's in 2D, the last two numbers represent the texture coordinates
    vec4 vertices[] = {
        vec4(-1.0,  1.0, 0.0, 0.0),
		vec4(-1.0, -1.0, 0.0, 1.0),
		vec4( 1.0, -1.0, 1.0, 1.0),
		vec4(-1.0,  1.0, 0.0, 0.0),
		vec4( 1.0,  1.0, 1.0, 0.0),
		vec4( 1.0, -1.0, 1.0, 1.0)
    };

    // Create a vertex array object---OpenGL needs this to manage the Vertex
    // Buffer Object
    GLuint vao;

    // Generate the vertex array and then bind it to make make it active.
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Create and initialize a buffer object---that's the memory buffer that
    // will be on the card!
    GLuint vbo;

    // We only need one for this example.
    glGenBuffers(1, &vbo);

    // Bind makes it the active VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // Here we copy the vertex data into our buffer on the card.  The parameters
    // tell it the type of buffer object, the size of the data in bytes, the
    // pointer for the data itself, and a hint for how we intend to use it.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Load the shaders.  Note that this function is not offered by OpenGL
    // directly, but provided as a convenience.
    program = createShadersProgram("assets/shaders/vertshader.glsl", "assets/shaders/fragshader.glsl");

    // Make that shader program active.
    glUseProgram(program);

    // Initialize the vertex position attribute from the vertex shader.  When
    // the shader and the program are linked, a table is created for the shader
    // variables.  Here, we get the index of the vPosition variable.
    GLuint vPostionLoc = glGetAttribLocation(program, "position");

    // We want to set this with an array!
    glEnableVertexAttribArray(vPostionLoc);

    // We map it to this offset in our current buffer (VBO) So, our position
    // data is going into loc and contains 2 floats.  The parameters to this
    // function are the index for the shader variable, the number of components,
    // the type of the data, a boolean for whether or not this data is
    // normalized (0--1), stride (or byte offset between consective attributes),
    // and a pointer to the first component.  Note that BUFFER_OFFSET is a macro
    // defined in the Angel.h file.
    glVertexAttribPointer(vPostionLoc, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid*)0);

	// Load file and decode image.
	std::vector<unsigned char> image;
	unsigned width, height;
	unsigned error = lodepng::decode(image, width, height, "assets/textures/quokka.png");

	// If there's an error, display it.
	if (error != 0)
	{
		std::cout << "error " << error << ": " << lodepng_error_text(error) << std::endl;
		exit(1);
	}

	glGenTextures(1, &tex_2d);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex_2d);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); //GL_NEAREST = no smoothing
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    // Make the background white
    glClearColor(0.2, 0.2, 0.2, 1.0);
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void render()
{
    glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	int texture_location = glGetUniformLocation(program,"texUnit");
	glUniform1i(texture_location, 0);
	glBindTexture(GL_TEXTURE_2D, tex_2d);


    glDrawArrays(GL_TRIANGLES, 0, NumVertices);
    glFlush();
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    // For OS X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(512, 512, "QQ Develop", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, keyCallback);

    glewExperimental = GL_TRUE;
    glewInit();
    printf("%s\n%s\n", glGetString(GL_RENDERER), glGetString(GL_VERSION));

    init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        render();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}