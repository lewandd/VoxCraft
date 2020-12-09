#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "camera.h"
#include "shader_s.h"
#include "data_sets.h"
#include "octree.h"
#include <iostream>
#include <vector>
#include <cmath>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// octree
Octree o;

// wybrana ściana bloku
int side = 0;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile shader program
    // ------------------------------------
    Shader shader("shader.vs", "shader.fs");

    // set up vertex data
    // ------------------
    float vertices[] = {
    0.0f,   0.0f,   0.0f,   0.0f,   0.0f,
    1.0f,   0.0f,   0.0f,   1.0f,   0.0f,
    1.0f,   1.0f,   0.0f,   1.0f,   1.0f,
    1.0f,   1.0f,   0.0f,   1.0f,   1.0f,
    0.0f,   1.0f,   0.0f,   0.0f,   1.0f,
    0.0f,   0.0f,   0.0f,   0.0f,   0.0f,

    0.0f,   0.0f,   1.0f,   0.0f,   0.0f,
    1.0f,   0.0f,   1.0f,   1.0f,   0.0f,
    1.0f,   1.0f,   1.0f,   1.0f,   1.0f,
    1.0f,   1.0f,   1.0f,   1.0f,   1.0f,
    0.0f,   1.0f,   1.0f,   0.0f,   1.0f,
    0.0f,   0.0f,   1.0f,   0.0f,   0.0f,

    0.0f,   1.0f,   1.0f,   1.0f,   0.0f,
    0.0f,   1.0f,   0.0f,   1.0f,   1.0f,
    0.0f,   0.0f,   0.0f,   0.0f,   1.0f,
    0.0f,   0.0f,   0.0f,   0.0f,   1.0f,
    0.0f,   0.0f,   1.0f,   0.0f,   0.0f,
    0.0f,   1.0f,   1.0f,   1.0f,   0.0f,

    1.0f,   1.0f,   1.0f,   1.0f,   0.0f,
    1.0f,   1.0f,   0.0f,   1.0f,   1.0f,
    1.0f,   0.0f,   0.0f,   0.0f,   1.0f,
    1.0f,   0.0f,   0.0f,   0.0f,   1.0f,
    1.0f,   0.0f,   1.0f,   0.0f,   0.0f,
    1.0f,   1.0f,   1.0f,   1.0f,   0.0f,

    0.0f,   0.0f,   0.0f,   0.0f,   1.0f,
    1.0f,   0.0f,   0.0f,   1.0f,   1.0f,
    1.0f,   0.0f,   1.0f,   1.0f,   0.0f,
    1.0f,   0.0f,   1.0f,   1.0f,   0.0f,
    0.0f,   0.0f,   1.0f,   0.0f,   0.0f,
    0.0f,   0.0f,   0.0f,   0.0f,   1.0f,

    0.0f,   1.0f,   0.0f,   0.0f,   1.0f,
    1.0f,   1.0f,   0.0f,   1.0f,   1.0f,
    1.0f,   1.0f,   1.0f,   1.0f,   0.0f,
    1.0f,   1.0f,   1.0f,   1.0f,   0.0f,
    0.0f,   1.0f,   1.0f,   0.0f,   0.0f,
    0.0f,   1.0f,   0.0f,   0.0f,   1.0f,
    };

    set_data();

    for (int scale = 0; scale < 9; ++scale) {
        for (int i = 0; i < c[scale].size(); ++i) {
            // dodanie bloku
            for (int j = 0; j < (1 << scale); ++j)
                for (int k = 0; k < (1 << scale); ++k)
                    for (int l = 0; l < (1 << scale); ++l) {
                        o.add((int)c[scale][i][0] + j, (int)c[scale][i][1] + k, (int)c[scale][i][2] + l, 1);
                    }
        }
    }

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind VAO
    glBindVertexArray(VAO);

    // bind VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("textures/white.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    // unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // unbind VAO
    glBindVertexArray(0);

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    int projLoc = glGetUniformLocation(shader.ID, "projection");
    int viewLoc = glGetUniformLocation(shader.ID, "view");
    int scaleLoc = glGetUniformLocation(shader.ID, "scale");
    int selectedLoc = glGetUniformLocation(shader.ID, "selected");

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)width / (float)height, 0.1f, 100.0f);        
        glm::mat4 view = camera.GetViewMatrix();

        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        
        for (int i = 0; i < (int)o.fullBlocks.size(); ++i) {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(o.fullBlocks[i]->x, o.fullBlocks[i]->y, o.fullBlocks[i]->z));
            float scale = o.fullBlocks[i]->intoTRI_ARGS().scale;
            model = glm::scale(model, glm::vec3(o.fullBlocks[i]->intoTRI_ARGS().scale));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1f(scaleLoc, scale);
            glUniform1f(selectedLoc, 1.0f);

            if (o.fullBlocks[i]->isSelected())
                glUniform1f(selectedLoc, 0.9f);

            // draw triangle
            shader.use();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        
        o.unsetSelected();

        float dep;
        glReadPixels(400, 400, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &dep);
        float z = 1.0f / (dep * (1.0f / 100.0f - 1.0f / 0.1f) + 1.0f / 0.1f);

        glm::vec3 lookAt = camera.Position + camera.Front * z;
        //printf("exactly (%8.5f, %8.5f, %8.5f) ", lookAt.x, lookAt.y, lookAt.z);
        glLineWidth(3.f);

        if (z < 99.9f) {
            glm::vec3 dl = glm::vec3(round(lookAt.x)-lookAt.x, round(lookAt.y)-lookAt.y, round(lookAt.z)-lookAt.z);

            if ((abs(dl.x) < abs(dl.y)) && (abs(dl.x) < abs(dl.z))) {
                if (camera.Front.x > 0.0f) {
                    side = 1;
                    lookAt.x += 0.01f;
                }
                else {
                    side = 2;
                    lookAt.x -= 0.01f;
                }
            }
            else if (abs(dl.y) < abs(dl.z)) {
                if (camera.Front.y > 0.0f) {
                    side = 3;
                    lookAt.y += 0.01f;
                }
                else {
                    side = 4;
                    lookAt.y -= 0.01f;
                }
            }
            else {
                if (camera.Front.z > 0.0f) {
                    side = 5;
                    lookAt.z += 0.01f;
                }
                else {
                    side = 6;
                    lookAt.z -= 0.01f;
                }
            }

            //printf("changed (%8.5f, %8.5f, %8.5f) ", lookAt.x, lookAt.y, lookAt.z);

            float line_t = 0.007f;
            if (!((abs(dl.x) < line_t && abs(dl.y) < line_t) || (abs(dl.x) < line_t && abs(dl.z) < line_t) || (abs(dl.y) < line_t && abs(dl.z) < line_t)))
                o.setSelected(floor(lookAt.x), floor(lookAt.y), floor(lookAt.z));
        }

        //printf("%5.1f noticed (%2d, %2d, %2d) \n", z, o.selected_x, o.selected_y, o.selected_z);

        // tymczasowy wskaźnik środka
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(glm::scale(glm::mat4(1.0f), glm::vec3(0.01f, 0.01f, 0.01f))));

        glDrawArrays(GL_LINES, 0, 36);      

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // de-allocate
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (o.existSelected())
            o.remove(o.selected_x, o.selected_y, o.selected_z);
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (o.existSelected()) {
            if (side == 1) {
                o.add(o.selected_x - 1, o.selected_y, o.selected_z, 1);
            }
            if (side == 2)
                o.add(o.selected_x + 1, o.selected_y, o.selected_z, 1);
            if (side == 3){
                o.add(o.selected_x, o.selected_y-1, o.selected_z, 1);
             }
            if (side == 4)
                o.add(o.selected_x, o.selected_y+1, o.selected_z, 1);
            if (side == 5)
                o.add(o.selected_x, o.selected_y, o.selected_z-1, 1);
            if (side == 6)
                o.add(o.selected_x, o.selected_y, o.selected_z+1, 1);
        }
    }
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}