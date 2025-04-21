#ifndef TRANSFORM_TRANSLATION_H
#define TRANSFORM_TRANSLATION_H
#ifndef CAL_MATRIX_H
#include "cal/matrix.h"
#endif
#ifndef GRAPH_INIT_H_INCLUED
#include<graph/init.h>
#endif

void Translation(GLFWwindow* window) {
    if (mode != -1) return;

    if (glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS) {
        if (ChooseIdx < 0 || ChooseIdx >= graphics.size()) return;
        for (int i = 0; i < graphics[ChooseIdx].points.size(); i++) {
            graphics[ChooseIdx].points[i].x -= 1;
        }
        printf("x-1\n");
    }
    if (glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS) {
        if (ChooseIdx < 0 || ChooseIdx >= graphics.size()) return;
        for (int i = 0; i < graphics[ChooseIdx].points.size(); i++) {
            graphics[ChooseIdx].points[i].x += 1;
        }
        printf("x+1\n");
    }
    if (glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS) {
        if (ChooseIdx < 0 || ChooseIdx >= graphics.size()) return;
        for (int i = 0; i < graphics[ChooseIdx].points.size(); i++) {
            graphics[ChooseIdx].points[i].y += 1;
        }
        printf("y+1\n");
    }
    if (glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS) {
        if (ChooseIdx < 0 || ChooseIdx >= graphics.size()) return;
        for (int i = 0; i < graphics[ChooseIdx].points.size(); i++) {
            graphics[ChooseIdx].points[i].y -= 1;
        }
        printf("y-1\n");
    }
}

#endif