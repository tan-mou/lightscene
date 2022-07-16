#include<iostream>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp> 
#include<glm/gtc/type_ptr.hpp>
#include<GL/glew.h>
#include<GLFW/glfw3.h>

#include"class/VertexBuffer.h"
#include"class/Shader.h"
#include"class/Camera.h"

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);


float fov = 45.0f;
float deltaTime = 0.0f; // 当前帧与上一帧的时间差
float lastFrame = 0.0f; // 上一帧的时间
float lastX = 320.0f;
float lastY = 240.0f;

int esc_counts = 0;
bool esc_status = false;
bool firstmouse = true;

Camera camera1(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;


    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwWindowHint(GLFW_REFRESH_RATE, 30);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSwapInterval(3);
    //Initialize glew
    glewInit();
    GLenum err = glewInit();

    if (err != GLEW_OK) {
        glfwTerminate();
        return -1;
    }

    float attribute[] = {
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
    };

    unsigned int cubevao;
    glGenVertexArrays(1, &cubevao);
    glBindVertexArray(cubevao);
    VertexBuffer vbo(attribute, 216 * sizeof(float));
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6* sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3 , GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    unsigned int lightvao;
    glGenVertexArrays(1, &lightvao);
    glBindVertexArray(lightvao);
    vbo.Bind();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);


    Shader lightshader("shader/vshader.shader", "shader/lightshader_f.shader");
    Shader shader("shader/vshader.shader", "shader/objectshader_f.shader");

    glm::mat4 view(1.0f);
    glm::mat4 projection(1.0f);
    glm::vec4 light_color(1.0f);
    glm::vec3 light_position(2.0f,1.0f, 1.0f);
    glm::vec3 object_position(0.0f, 0.0f, 0.0f);


    glEnable(GL_DEPTH_TEST);
    float k = 0.05f;
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*light_position.z += k;*/
        glm::mat4 model_1(1.0f);
        glm::mat4 model_2(1.0f);
        //light model
        model_1 = glm::translate(model_1, light_position);
        model_1 = glm::scale(model_1, glm::vec3(0.5f));


        //object model
        model_2 = glm::translate(model_2, object_position);
        model_2 = glm::rotate(model_2, glm::radians((float)glfwGetTime()*20),glm::vec3(0.0f,1.0f,0.0f));
        processInput(window);
        view = camera1.GetView();

        projection = glm::perspective(glm::radians(45.0f), 1.3f, 0.1f, 100.0f);
        //light drawcall

        glBindVertexArray(lightvao);
        lightshader.Use();
        lightshader.setMat4("u_model", model_1);
        lightshader.setMat4("u_view", view);
        lightshader.setMat4("u_projection", projection);
        lightshader.setVec4("u_light_color", light_color);
        glDrawArrays(GL_TRIANGLES, 0, 36);


        //cude drawcall
        glBindVertexArray(cubevao);

        
        shader.Use();
        shader.setMat4("u_view", view);
        shader.setMat4("u_projection", projection);
        shader.setMat4("u_model", model_2);
        shader.setVec4("u_light_color", light_color);
        shader.setVec4("u_object_color", glm::vec4(1.0f, 0.5f, 0.31f, 1.0f));
        shader.setVec3("u_light_position", light_position);
        shader.setVec3("u_camera_position", camera1.GetPos());

        glDrawArrays(GL_TRIANGLES, 0, 36);


  /*      if (light_position.z >= 2.0f)
            k=-0.05f;
        if (light_position.z <= -2.0f)
            k=0.05f;*/

        //light_color.x += k;
        /* Swap front and back buffers */
        glfwSwapBuffers(window);


        
        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}


// camera controll
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (fov >= 1.0f && fov <= 60.0f)
        fov -= yoffset;
    if (fov <= 1.0f)
        fov = 1.0f;
    if (fov >= 60.0f)
        fov = 60.0f;
}


void processInput(GLFWwindow* window)
{

    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;
    float cameraSpeed = 2.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera1.Set_Keyborad_MOVE(deltaTime, GLFW_KEY_W);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera1.Set_Keyborad_MOVE(deltaTime, GLFW_KEY_S);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera1.Set_Keyborad_MOVE(deltaTime, GLFW_KEY_A);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera1.Set_Keyborad_MOVE(deltaTime, GLFW_KEY_D);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera1.Set_Keyborad_MOVE(deltaTime, GLFW_KEY_SPACE);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        camera1.Set_Keyborad_MOVE(deltaTime, GLFW_KEY_LEFT_CONTROL);

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!esc_counts) {
            if (esc_status) {
                esc_status = false;

                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
            else {
                esc_status = true;
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            esc_counts = 1;
        }


    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE)
        esc_counts = 0;

}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {

    //intialize

    float sensitivity = 0.05f;

    float xoffset = sensitivity * (xpos - lastX);
    float yoffset = sensitivity * (lastY - ypos);

    lastX = xpos;
    lastY = ypos;
    camera1.Set_Mouse_MOVE(xoffset, yoffset);


}