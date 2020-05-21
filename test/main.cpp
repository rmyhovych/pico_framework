//
// Created by rmyho on 5/19/2020.
//

#include <cstdio>

#include "instance.h"
#include "window/glfw_window_manager.h"

int main()
{
	printf("Hello, World!");

	GLFWWindowManager windowManager(800, 600);

	Instance instance(&windowManager);
	instance.createSurface(VK_FORMAT_R8G8B8A8_UNORM);

	return 0;
}