
#include <GL/glew.h>
#include <GLFW/glfw3.h>

int glfwInit(void);

GLFWwindow* glfwCreateWindow(int width, int height, const char* title, GLFWmonitor* monitor,
	GLFWwindow* share);

void glfwTerminate(void);

