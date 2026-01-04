#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include  "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"

GLfloat point[] = {
     0.0f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
    -0.5,  -0.5f, 0.0f
};
GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f
};


int g_windowSizeX = 640;
int g_windowSizeY = 480;

void glfwWindowSizeCallback(GLFWwindow* pWindow, int width, int height) {
    g_windowSizeX = width;
    g_windowSizeY = height;
    glViewport(0, 0, g_windowSizeX, g_windowSizeY);
}

void glfwKeyCallback(GLFWwindow* pWindow, int key, int scanncode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
}

int main(int argc, char** argv) {

    //GLFWwindow* pWindow;

    /* Initialize the library */
    if (!glfwInit()) {
        std::cout << "glfwInit failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* pWindow = glfwCreateWindow(g_windowSizeX, g_windowSizeY, "Battle City", nullptr, nullptr);
    if (!pWindow) {
        std::cout << "glfwCreateWindow failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(pWindow, glfwWindowSizeCallback);
    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    /* Make the window's context current */
    glfwMakeContextCurrent(pWindow);

	if(!gladLoadGL()) {
        std::cout << "Can't load GLAD" << std::endl;
	}

    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	//std::cout << "OpenGL " << GLVersion.major << "." <<GLVersion.minor << std::endl;
	glClearColor(1, 1, 0, 1);

    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram) {
            std::cerr << "Can't create shader program: " << "DefaultShader" << std::endl;
            return -1;
        }

        // данные указанные на 6-33 строках сохраняются в памяти видеокарты
        GLuint points_vbo = 0; // буфер позиций
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo); // делаем текущим Buffer
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW); // перемещение из оперативной памяти в память видеокарты в статичном виде

        GLuint colors_vbo = 0; // буфер цвета
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo); // делаем текущим Buffer на 102 строке
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        // указыввем что делать с данными выше, где делались и сохранялись в видеокарту буферы
        GLuint vao = 0; // Vertex attribute option
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        // связываение vbo из позиции куда попадут в шейдер
        glEnableVertexAttribArray(0); //  включение location для вертекс 
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1); //  включение location для цвета
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(pWindow)) {
            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            // отрисовка
            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLES, 0, 3);// сама команда отрисовки(отрисовывает текущий!)


            /* Swap front and back buffers */
            glfwSwapBuffers(pWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}