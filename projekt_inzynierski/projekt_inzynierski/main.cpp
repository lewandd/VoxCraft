#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "constans.h"
#include "camera.h"
#include "shader_s.h"
#include "octree.h"
#include "perlin.h"
#include "generate.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
void selectBlock();
void updateVisibleChunks();

//CHUNK* generate_chunk(int x, int y);

// settings
unsigned int SCR_WIDTH = 800;
unsigned int SCR_HEIGHT = 800;

// camera
//Camera camera(glm::vec3(-3.0f, 28.0f, 3.0f));
Camera camera(glm::vec3(8192.0f, 30.0f, 8192.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// selected
int selected_chunk_x = -1;
int selected_chunk_y = -1;
int selected_octree = -1;
int selected_x = -1;
int selected_y = -1;
int selected_z = -1;
bool selected = false;
int selectedStart = 0;

int side = 0;
int choosedType = 0;
float changeScroll = 0.0f;

// visible chunks
const int renderChunkDistance = 7;
const int numNearChunks = 2 * renderChunkDistance + 1;
CHUNK*** nearChunks;
vector <CHUNK*> visibleChunks;
vector <CHUNK*> unvisibleChunks;

bool compareChunks(CHUNK* ch1, CHUNK* ch2) {
    float cdx1 = camera.position.x - (ch1->x * 16.0 + 8.0);
    float cdy1 = camera.position.z - (ch1->y * 16.0 + 8.0);

    float cdx2 = camera.position.x - (ch2->x * 16.0 + 8.0);
    float cdy2 = camera.position.z - (ch2->y * 16.0 + 8.0);
    
    return (cdx1*cdx1 + cdy1 * cdy1 < cdx2 * cdx2 + cdy2 * cdy2);
}

int INDEX[5][16][16][16];
int POSITION[5][16][16][16][3];

int main() {

    // initialize glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // create glfw window
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    glfwMakeContextCurrent(window);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // load GLAD
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // create shader program
    Shader blockShader("block.vs", "block.fs");
    Shader targetShader("target.vs", "target.fs");
    Shader interfaceShader("interface.vs", "interface.fs");
    Shader selectShader("select.vs", "select.fs");

    // prepare data

    for (int i = 0; i < NOISE_MAP_COUNT; ++i) {
        for (int j = 0; j < NOISE_MAP_COUNT; ++j) {
            noise[i][j] = NULL;
        }
    }

    for (int lvl = 0; lvl < 5; ++lvl) {
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                for (int z = 0; z < 16; ++z) {
                    int offset = 1 << (MAX_LEVEL - lvl - 1);
                    INDEX[lvl][x][y][z] = ((x & offset) << 2 | ((y & offset) << 1) | ((z & offset))) >> (MAX_LEVEL - lvl - 1);
                }
            }
        }
    }

    for (int lvl = 0; lvl < 5; ++lvl) {
        for (int x = 0; x < 16; ++x) {
            for (int y = 0; y < 16; ++y) {
                for (int z = 0; z < 16; ++z) {
                    POSITION[lvl][x][y][z][0] = (x >> (MAX_LEVEL - lvl)) << (MAX_LEVEL - lvl);
                    POSITION[lvl][x][y][z][1] = (y >> (MAX_LEVEL - lvl)) << (MAX_LEVEL - lvl);
                    POSITION[lvl][x][y][z][2] = (z >> (MAX_LEVEL - lvl)) << (MAX_LEVEL - lvl);
                }
            }
        }
    }

    // targetVAO

    unsigned int targetVBO, targetVAO;
    glGenVertexArrays(1, &targetVAO);
    glGenBuffers(1, &targetVBO);
    glBindVertexArray(targetVAO);

    glBindBuffer(GL_ARRAY_BUFFER, targetVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(target_vertices), target_vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // select VAO

    unsigned int selectVBO, selectVAO;
    glGenVertexArrays(1, &selectVAO);
    glGenBuffers(1, &selectVBO);
    glBindVertexArray(selectVAO);

    glBindBuffer(GL_ARRAY_BUFFER, selectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(select_vertices), select_vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // textures load

    glGenTextures(1, &texture); 
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    unsigned char** data = new unsigned char*[NUM_BLOCK_TEXTURES *3];
    stbi_set_flip_vertically_on_load(true);
    for (int i = 0; i < NUM_BLOCK_TEXTURES; i ++) {
        string src = "textures/" + to_string(i) + "s.png";
        data[3*i] = stbi_load(src.c_str(), &width, &height, &nrChannels, 0);
        src = "textures/" + to_string(i) + "d.png";
        data[3*i + 1] = stbi_load(src.c_str(), &width, &height, &nrChannels, 0);
        src = "textures/" + to_string(i) + "u.png";
        data[3*i + 2] = stbi_load(src.c_str(), &width, &height, &nrChannels, 0);
    }
    
    glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA8, width, height, NUM_BLOCK_TEXTURES * 3, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
    
    for (int i = 0; i < NUM_BLOCK_TEXTURES *3; i++) {
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
    }

    glGenerateMipmap(GL_TEXTURE_2D_ARRAY);
    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

    // connect texture to sampler in fragmentshader
    int texLoc = glGetUniformLocation(blockShader.ID, "tex");
    glUniform1i(texLoc, 0);

    int layerLoc= glGetUniformLocation(blockShader.ID, "layer");
    glUniform1i(layerLoc, 0);

    glActiveTexture(GL_TEXTURE0);

    for (int i = 0; i < NUM_BLOCK_TEXTURES * 3; i++) {
        stbi_image_free(data[i]);
    }

    // interface VAO
    // -------------

    unsigned int interfaceVBO, interfaceVAO;
    glGenVertexArrays(1, &interfaceVAO);
    glGenBuffers(1, &interfaceVBO);
    glBindVertexArray(interfaceVAO);

    glBindBuffer(GL_ARRAY_BUFFER, interfaceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(interface_vertices), interface_vertices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // texture atribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

    // connect texture to sampler in fragmentshader
    int interfaceTexLoc = glGetUniformLocation(interfaceShader.ID, "tex");
    glUniform1i(interfaceTexLoc, 0);

    int interfaceLayerLoc = glGetUniformLocation(interfaceShader.ID, "layer");
    glUniform1i(interfaceLayerLoc, 0);

    glActiveTexture(GL_TEXTURE0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // set uniforms
    // -----------

    int projLoc = glGetUniformLocation(blockShader.ID, "projection");
    int viewLoc = glGetUniformLocation(blockShader.ID, "view");

    int targetColorLoc = glGetUniformLocation(targetShader.ID, "aColor");
    int targetSizeLoc = glGetUniformLocation(targetShader.ID, "aSize");

    int interfaceTranslateLoc = glGetUniformLocation(interfaceShader.ID, "translate");
    int interfaceSelectedLoc = glGetUniformLocation(interfaceShader.ID, "selected");

    int selectProjLoc = glGetUniformLocation(blockShader.ID, "projection");
    int selectViewLoc = glGetUniformLocation(blockShader.ID, "view");
    int selectTransformLoc = glGetUniformLocation(selectShader.ID, "transform");

    int rot0Loc = glGetUniformLocation(blockShader.ID, "rot[0]");
    int rot1Loc = glGetUniformLocation(blockShader.ID, "rot[1]");
    int rot2Loc = glGetUniformLocation(blockShader.ID, "rot[2]");
    int rot3Loc = glGetUniformLocation(blockShader.ID, "rot[3]");
    int rot4Loc = glGetUniformLocation(blockShader.ID, "rot[4]");
    int rot5Loc = glGetUniformLocation(blockShader.ID, "rot[5]");

    glm::mat4 rot0 = glm::mat4(1.0f);
    glm::mat4 rot1 = glm::mat4(1.0f);
    glm::mat4 rot2 = glm::mat4(1.0f);
    glm::mat4 rot3 = glm::mat4(1.0f);
    glm::mat4 rot4 = glm::mat4(1.0f);
    glm::mat4 rot5 = glm::mat4(1.0f);

    rot0 = glm::translate(rot0, glm::vec3(-0.0f, 0.0f, 1.0f));
    rot0 = glm::rotate(rot0, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
    rot1 = glm::translate(rot1, glm::vec3(1.0f, 0.0f, 0.0f));
    rot1 = glm::rotate(rot1, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
    rot2 = glm::translate(rot2, glm::vec3(0.0f, 0.0f, 1.0f));
    rot2 = glm::rotate(rot2, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
    rot3 = glm::translate(rot3, glm::vec3(0.0f, 1.0f, 0.0f));
    rot3 = glm::rotate(rot3, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
    rot4 = glm::translate(rot4, glm::vec3(0.0f, 0.0f, 0.0f));
    rot5 = glm::translate(rot5, glm::vec3(1.0f, 0.0f, 1.0f));
    rot5 = glm::rotate(rot5, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));

    // set chunks
    nearChunks = new CHUNK ** [numNearChunks];
    for (int i = 0; i < numNearChunks; ++i) {
        nearChunks[i] = new CHUNK * [numNearChunks];
        for (int j = 0; j < numNearChunks; ++j) {
            nearChunks[i][j] = new CHUNK;
        }
    }

    for (int i = 0; i < numNearChunks; ++i) {
        for (int j = 0; j < numNearChunks; ++j) {
            unvisibleChunks.push_back(nearChunks[i][j]);
        }
    }

    // fps
    float fpsLast = 0.0f;
    float fpsFrames = 0.0f;

    // render loop
    // -----------
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    while (!glfwWindowShouldClose(window)) {

        // count fps
        ++fpsFrames;
        if ((float)glfwGetTime() - fpsLast > 0.5f) {
            printf("FPS: %f\n", fpsFrames/((float)glfwGetTime() - fpsLast));
            fpsLast = (float)glfwGetTime();
            fpsFrames = 0.0f;
        }

        // get delta time
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.55f, 0.75f, 0.85f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // render blocks
        // -------------

        blockShader.use();

        glUniformMatrix4fv(rot0Loc, 1, GL_FALSE, glm::value_ptr(rot0));
        glUniformMatrix4fv(rot1Loc, 1, GL_FALSE, glm::value_ptr(rot1));
        glUniformMatrix4fv(rot2Loc, 1, GL_FALSE, glm::value_ptr(rot2));
        glUniformMatrix4fv(rot3Loc, 1, GL_FALSE, glm::value_ptr(rot3));
        glUniformMatrix4fv(rot4Loc, 1, GL_FALSE, glm::value_ptr(rot4));
        glUniformMatrix4fv(rot5Loc, 1, GL_FALSE, glm::value_ptr(rot5));

        // set matrices
        glm::mat4 proj = glm::perspective(glm::radians(70.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);        
        glm::mat4 view = camera.getViewMatrix();
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

        updateVisibleChunks();

        int sum = 0;
        int sum_chunks = 0;

        for (int i = 0; i < (int)visibleChunks.size(); ++i) {
            CHUNK* ch = visibleChunks[i];

            glBindVertexArray(ch->VAO);
            glDrawArraysInstanced(GL_TRIANGLES, 0, 6, ch->data_size/6);

            sum_chunks++;
            sum += ch->data_size;
        }
        //cout << sum << " " << sum_chunks << endl;
        
        selectBlock();

        // render select
        // -------------

        if (selected) {
            selectShader.use();
            glm::mat4 trans = glm::mat4(1.0f);
            trans = glm::translate(trans, glm::vec3(16*selected_chunk_x + selected_x, 16 * selected_octree + selected_y, 16 * selected_chunk_y + selected_z));

            switch (side) {
                case 1: // right
                    trans = glm::translate(trans, glm::vec3(-0.01f, 0.0f, 1.0f));
                    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
                    break;
                case 2: // left
                    trans = glm::translate(trans, glm::vec3(1.01f, 0.0f, 0.0f));
                    trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(0.0, 1.0, 0.0));
                    break;
                case 3: // bottom
                    trans = glm::translate(trans, glm::vec3(0.0f, -0.01f, 1.0f));
                    trans = glm::rotate(trans, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
                    break;
                case 4: // up
                    trans = glm::translate(trans, glm::vec3(0.0f, 1.01f, 0.0f));
                    trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
                    break;
                case 5: // front
                    trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -0.01f));
                    break;
                case 6: // back
                    trans = glm::translate(trans, glm::vec3(1.0f, 0.0f, 1.01f));
                    trans = glm::rotate(trans, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0));
                    
                    break;
            }

            glUniformMatrix4fv(selectProjLoc, 1, GL_FALSE, glm::value_ptr(proj));
            glUniformMatrix4fv(selectViewLoc, 1, GL_FALSE, glm::value_ptr(view));
            glUniformMatrix4fv(selectTransformLoc, 1, GL_FALSE, glm::value_ptr(trans));

            glBindVertexArray(selectVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // render target
        // -------------

        targetShader.use();

        glUniform3f(targetColorLoc, TARGET_COLOR_R, TARGET_COLOR_G, TARGET_COLOR_B);
        glUniform1f(targetSizeLoc, TARGET_SIZE);
        glLineWidth(TARGET_THICKNESS);

        glBindVertexArray(targetVAO);

        glDrawArrays(GL_LINES, 0, 4);

        // render interface
        // ----------------

        interfaceShader.use();
        if (choosedType > selectedStart + 7) {
            selectedStart++;
        }
        else if (choosedType < selectedStart) {
            selectedStart--;
        }

        for (int i = 0; i < min(8, NUM_BLOCK_TEXTURES); ++i) {
            glUniform3f(interfaceTranslateLoc, -0.45 + (float)i*0.11, -0.99, -1.0);
            glUniform1i(interfaceLayerLoc, 3*(i + selectedStart));

            if (choosedType == i + selectedStart)
                glUniform1f(interfaceSelectedLoc, 1.0f);
            else 
                glUniform1f(interfaceSelectedLoc, 0.4f);

            glBindVertexArray(interfaceVAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void updateVisibleChunks() {
    int chunk_x = camera.position.x / 16;
    int chunk_y = camera.position.z / 16;

    // clear near table
    for (int i = 0; i < numNearChunks; ++i)
        for (int j = 0; j < numNearChunks; ++j) 
            nearChunks[i][j] = NULL;

    // update visible vector (remove)
    for (int i = 0; i < (int)visibleChunks.size(); ++i) {
        float cdx = camera.position.x - (visibleChunks[i]->x * 16.0 + 8.0);
        float cdy = camera.position.z - (visibleChunks[i]->y * 16.0 + 8.0);
        if (cdx * cdx + cdy * cdy > 12000.0) {
            unvisibleChunks.push_back(visibleChunks[i]);
            visibleChunks.erase(visibleChunks.begin() + i);
            i--;
        }
    }

    // update near table - add already visible
    for (int i = 0; i < (int)visibleChunks.size(); ++i)
        nearChunks[visibleChunks[i]->x - chunk_x + renderChunkDistance][visibleChunks[i]->y - chunk_y + renderChunkDistance] = visibleChunks[i];

    // update near table - add new visible
    for (int i = -renderChunkDistance; i <= renderChunkDistance; ++i) {
        for (int j = -renderChunkDistance; j <= renderChunkDistance; ++j) {

            // is free space
            if (nearChunks[i + renderChunkDistance][j + renderChunkDistance] == NULL) {
                // is pos available
                if ((chunk_x + i) >= 0 && (chunk_x + i) < CHUNKS_COUNT && (chunk_y + j) >= 0 && (chunk_y + j) < CHUNKS_COUNT) {
                    float cdx = camera.position.x - ((chunk_x + i) * 16.0 + 8.0);
                    float cdy = camera.position.z - ((chunk_y + j) * 16.0 + 8.0);

                    // near (should be added)
                    if (cdx * cdx + cdy * cdy < 12000.0) {

                        if (!unvisibleChunks.empty()) {
                            // update some unvisible CHUNK and add to visible

                            unvisibleChunks[0]->setNew(chunk_x + i, chunk_y + j);

                            visibleChunks.push_back(unvisibleChunks[0]);
                            nearChunks[i + renderChunkDistance][j + renderChunkDistance] = unvisibleChunks[0];
                            unvisibleChunks.erase(unvisibleChunks.begin());
                        }
                    }
                }
            }
        }
    }

    for (int i = 0; i < numNearChunks; ++i) {
        for (int j = 0; j < numNearChunks; ++j) {
            if (nearChunks[i][j] != NULL) {
                nearChunks[i][j]->n[0] = NULL;
                nearChunks[i][j]->n[1] = NULL;
                nearChunks[i][j]->n[2] = NULL;
                nearChunks[i][j]->n[3] = NULL;

                if (i + 1 < numNearChunks)
                    nearChunks[i][j]->n[0] = nearChunks[i + 1][j];
                if (i - 1 >= 0)
                    nearChunks[i][j]->n[1] = nearChunks[i - 1][j];
                if (j + 1 < numNearChunks)
                    nearChunks[i][j]->n[2] = nearChunks[i][j + 1];
                if (j - 1 >= 0)
                    nearChunks[i][j]->n[3] = nearChunks[i][j - 1];
            }
        }
    }

    // sort
    sort(visibleChunks.begin(), visibleChunks.end(), compareChunks);
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        choosedType = 0;

    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        choosedType = 1;

    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)
        choosedType = 2;

    if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)
        choosedType = 3;

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)
        choosedType = 4;

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
    SCR_WIDTH = width;
    SCR_HEIGHT = height;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        if (selected) {
            CHUNK* ch = NULL;
            
            for (int i = 0; i < (int)visibleChunks.size(); ++i) {
                if (visibleChunks[i]->x == selected_chunk_x && visibleChunks[i]->y == selected_chunk_y) {
                    ch = visibleChunks[i];
                    break;
                }
            }
            if (ch != NULL) {
                int type = ch->o[selected_octree]->remove(selected_x, selected_y, selected_z);
                changes[ch->x][ch->y].action.push_back(Action(0, selected_octree, selected_x, selected_y, selected_z, type));
                ch->update();
            }
        }
    }

    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        if (selected) {
            int final_selected_chunk_x = selected_chunk_x;
            int final_selected_chunk_y = selected_chunk_y;
            int final_selected_octree = selected_octree;
            int final_selected_x = selected_x;
            int final_selected_y = selected_y;
            int final_selected_z = selected_z;

            // set final coords
            switch (side) {
                case 1: 
                    if (selected_x == 0)
                        final_selected_chunk_x--;
                    final_selected_x--;
                    break;
                case 2:
                    if (selected_x == 15)
                        final_selected_chunk_x++;
                    final_selected_x++;
                    break;
                case 3: 
                    if (selected_y == 0)
                        final_selected_octree--;
                    final_selected_y--;
                    break;
                case 4:
                    if (selected_y == 15)
                        final_selected_octree++;
                    final_selected_y++;
                    break;
                case 5:
                    if (selected_z == 0)
                        final_selected_chunk_y--;
                    final_selected_z--;
                    break;
                case 6:
                    if (selected_z == 15)
                        final_selected_chunk_y++;
                    final_selected_z++;
                    break;
            }
            
            final_selected_x = (final_selected_x + 16) % 16;
            final_selected_y = (final_selected_y + 16) % 16;
            final_selected_z = (final_selected_z + 16) % 16;

            // check if correct position coords
            if (final_selected_chunk_x >= 0 && final_selected_chunk_x < CHUNKS_COUNT && 
                final_selected_chunk_y >= 0 && final_selected_chunk_y < CHUNKS_COUNT && 
                final_selected_octree >= 0 && final_selected_octree < 8) {

                if (selected) {
                    CHUNK* ch = NULL;

                    for (int i = 0; i < (int)visibleChunks.size(); ++i) {
                        if (visibleChunks[i]->x == final_selected_chunk_x && visibleChunks[i]->y == final_selected_chunk_y) {
                            ch = visibleChunks[i];
                            break;
                        }
                    }

                    if (ch->o[final_selected_octree] == NULL) {
                        ch->o[final_selected_octree] = new Octree(ch, ch->x, ch->y, final_selected_octree);
                        ch->o[final_selected_octree]->chunk = ch;
                    }

                    if (ch != NULL) {
                        ch->o[final_selected_octree]->add(final_selected_x, final_selected_y, final_selected_z, choosedType + 1);
                        changes[ch->x][ch->y].action.push_back(Action(1, final_selected_octree, final_selected_x, final_selected_y, final_selected_z, choosedType + 1));
                        ch->update();
                    }
                }

            }
        }
    }
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    changeScroll -= yoffset;
    if (changeScroll >= 1.0f) {
        choosedType = min(choosedType + 1, NUM_BLOCK_TEXTURES-1);
        changeScroll = 0.0f;
    }
    else if (changeScroll <= -1.0f) {
        choosedType = max(choosedType - 1, 0);
        changeScroll = 0.0f;
    }
    
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = (float) xpos;
        lastY = (float) ypos;
        firstMouse = false;
    }

    float xoffset = (float) (xpos - lastX);
    float yoffset = (float) (lastY - ypos); // reversed since y-coordinates go from bottom to top

    lastX = (float) xpos;
    lastY = (float) ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

void selectBlock() {
    selected_chunk_x = -1;
    selected_chunk_y = -1;
    selected_octree = -1;
    selected_x = -1;
    selected_y = -1;
    selected_z = -1;
    selected = false;

    float depth;
    glReadPixels(SCR_WIDTH/2, SCR_HEIGHT/2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
    float distance = 1.0f / (depth * (1.0f / 100.0f - 1.0f / 0.1f) + 1.0f / 0.1f);

    glm::vec3 lookAt = camera.position + camera.Front * distance;
    //printf("exactly (%8.5f, %8.5f, %8.5f) ", lookAt.x, lookAt.y, lookAt.z);

    if (distance < 99.9f) {
        glm::vec3 dl = glm::vec3(round(lookAt.x) - lookAt.x, round(lookAt.y) - lookAt.y, round(lookAt.z) - lookAt.z);

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

        lookAt.x = floor(lookAt.x);
        lookAt.y = floor(lookAt.y);
        lookAt.z = floor(lookAt.z);

        selected_x = (int)lookAt.x % 16;
        selected_y = (int)lookAt.y % 16;
        selected_z = (int)lookAt.z % 16;

        selected_chunk_x = (int)lookAt.x / 16;
        selected_chunk_y = (int)lookAt.z / 16;
        selected_octree = (int)lookAt.y / 16;

        float line_t = 0.007f;
        if (!((abs(dl.x) < line_t && abs(dl.y) < line_t) || (abs(dl.x) < line_t && abs(dl.z) < line_t) || (abs(dl.y) < line_t && abs(dl.z) < line_t))) {

            if (selected_chunk_x < CHUNKS_COUNT && selected_chunk_y < CHUNKS_COUNT && selected_chunk_x >= 0 && selected_chunk_y >= 0) {
                selected = true;
            }
        }

        //printf("%5.1f noticed (%2d, %2d, %2d) \n", z, selected_x, selected_y, selected_z);
    }
}