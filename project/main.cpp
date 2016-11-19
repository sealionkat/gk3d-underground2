#include "Includes.h"

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void do_movement();

const GLuint WIDTH = Settings::ScreenWidth, HEIGHT = Settings::ScreenHeight;

// Camera
glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 0.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;

Camera* cam = new Camera(WIDTH, HEIGHT);
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int main()
{
    std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, Settings::OpenGLMajorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, Settings::OpenGLMinorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, Settings::WindowTitle, nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    /* Shaders initialization */
    std::cout << "Preparing shaders..." << std::endl;

    const GLchar *vertexPath = "shaders/vertexShader.glsl";
    const GLchar *fragmentPath = "shaders/fragmentShader.glsl";

    Shader shaderMtn(vertexPath, fragmentPath);

    /* /Shaders initialization */

    /* Objects setup */

    std::cout << "Creating objects..." << std::endl;

    std::cout << "Creating platform" << std::endl;
    GLfloat vertices[] = {
        -3.0f, -1.0f, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //front
        3.0f, 1.0f, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        3.0f, -1.0f, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -3.0f, -1.0f, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        3.0f, 1.0f, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -3.0f, 1.0f, 15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        3.0f, 1.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //right
        3.0f, 1.0f, -15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        3.0f, -1.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        3.0f, -1.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        3.0f, 1.0f, -15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        3.0f, -1.0f, -15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -3.0f, -1.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,//down
        -3.0f, -1.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        3.0f, -1.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

        -3.0f, -1.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        3.0f, -1.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        3.0f, -1.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

        -3.0f, -1.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,//left
        -3.0f, 1.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -3.0f, 1.0f, -15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -3.0f, -1.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -3.0f, 1.0f, -15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -3.0f, -1.0f, -15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -3.0f, -1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //back
        -3.0f, 1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        3.0f, 1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        3.0f, 1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        3.0f, -1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -3.0f, -1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -3.0f, 1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,//up
        -3.0f, 1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        3.0f, 1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        3.0f, 1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        3.0f, 1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -3.0f, 1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};
    
    std::cout << "Creating walls, floor and ceiling" << std::endl;
    GLfloat verticesBig[] = {//position, normal, texture
        -8.0f, -1.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //front
        8.0f, 7.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        8.0f, -1.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
 
        -8.0f, -1.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        8.0f, 7.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -8.0f, 7.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        8.0f, 7.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,//right
        8.0f, 7.0f, -15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        8.0f, -1.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        8.0f, -1.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        8.0f, 7.0f, -15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        8.0f, -1.0f, -15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, -1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,//down
        -8.0f, -1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        8.0f, -1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, -1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        8.0f, -1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        8.0f, -1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, -1.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,//left
        -8.0f, 7.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -8.0f, 7.0f, -15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, -1.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -8.0f, 7.0f, -15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -8.0f, -1.0f, -15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        -8.0f, -1.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,//back
        -8.0f, 7.0f, -15.0f, 0.0f, 0.0f, 1.0f,0.0f, 0.0f,
        8.0f, 7.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        8.0f, 7.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        8.0f, -1.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -8.0f, -1.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

        -8.0f, 7.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,//up
        -8.0f, 7.0f, -15.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        8.0f, 7.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

        8.0f, 7.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        8.0f, 7.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -8.0f, 7.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f

                             
    };

    // VBO, VAO
    GLuint VBO, VAO, VBOBig, VAOBig;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAOBig);
    glGenBuffers(1, &VBOBig);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); //location 0 - position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat))); //location 1 - normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(2 * sizeof(GLfloat))); // location 2 - texture coords
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    glBindVertexArray(VAOBig);

    glBindBuffer(GL_ARRAY_BUFFER, VBOBig);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesBig), verticesBig, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)0); //location 0 - position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat))); //location 1 - normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat))); // location 2 - texture coords
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    std::cout << "Creating bench" << std::endl;

    Model* bench = new Model("models/bench/bench_v01.obj");
    bench->SetColor(glm::vec3(0.0f, 0.4f, 0.15f));

    Model* slenderman = new Model("models/slenderman/slenderman.obj");
    slenderman->SetColor(glm::vec3(0.3f, 0.3f, 0.3f));

    Model* flashlight = new Model("models/flashlight/flashlight.obj");
    flashlight->SetColor(glm::vec3(0.0f, 1.0f, 1.0f));


    /* /Objects setup */

    /* Textures setup */
    Texture* brickTex = new Texture();
    brickTex->loadFromFile("textures/brickwall.jpg", true);

    /* /Textures setup */


    // Game loop
    std::cout << "Starting main loop!" << std::endl;
    while (!glfwWindowShouldClose(window))
    {
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();
        do_movement();

        //std::cout << "Preparing window" << std::endl;
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderMtn.Use();

        // matrices
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;
        //std::cout << "Prepating matrices" << std::endl;
        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);


        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

        // setting camera position
        glUniform3f(glGetUniformLocation(shaderMtn.Program, Settings::viewPosLoc), cameraPos.x, cameraPos.y, cameraPos.z);

        // Point lights
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "pointLights[0].position"), 0.0f, 6.5f, 12.0f);
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "pointLights[0].color"), ((glm::sin((GLfloat)glfwGetTime()) + 1) / 2), 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "pointLights[0].linear"), 0.1f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "pointLights[0].quadratic"), 0.1f);
        // Point light 2
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "pointLights[1].position"), 0.0f, 6.5f, -12.0f);
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "pointLights[1].color"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "pointLights[1].linear"), 0.1f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "pointLights[1].quadratic"), 0.1f);

        // Spotlight
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "spotLights[0].position"), cameraPos.x, cameraPos.y, cameraPos.z);
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "spotLights[0].direction"), cameraFront.x, cameraFront.y, cameraFront.z);
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "spotLights[0].color"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "spotLights[0].cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "spotLights[0].outerCutOff"), glm::cos(glm::radians(17.5f)));
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "spotLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "spotLights[0].linear"), 0.5f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "spotLights[0].quadratic"), 0.05f);

        glUniform3f(glGetUniformLocation(shaderMtn.Program, "spotLights[1].position"), 3.75f, -0.6f, 13.9f);
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "spotLights[1].direction"), 0.0f, 0.0f, -1.0f);
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "spotLights[1].color"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "spotLights[1].cutOff"), glm::cos(glm::radians(12.5f)));
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "spotLights[1].outerCutOff"), glm::cos(glm::radians(17.5f)));
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "spotLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "spotLights[1].linear"), 0.5f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "spotLights[1].quadratic"), 0.05f);


        glUniform3f(glGetUniformLocation(shaderMtn.Program, Settings::objectColorLoc), 0.75f, 0.75f, 0.75f);
        

        glUniform3f(glGetUniformLocation(shaderMtn.Program, Settings::materialAmbientLoc), 0.75f, 0.75f, 0.75f);
        glUniform3f(glGetUniformLocation(shaderMtn.Program, Settings::materialDiffuseLoc), 0.75f, 0.75f, 0.75f);
        glUniform3f(glGetUniformLocation(shaderMtn.Program, Settings::materialSpecularLoc), 0.5f, 0.5f, 0.5f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, Settings::materialShininessLoc), 32.0f);

        brickTex->use(shaderMtn, 0);
        glUniform1i(glGetUniformLocation(shaderMtn.Program, Settings::numTexturesLoc), 1);
        

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); //36 for whole cube
        glBindVertexArray(0);

       // glBindTexture(GL_TEXTURE_2D, 0);

        
        glUniform1i(glGetUniformLocation(shaderMtn.Program, Settings::numTexturesLoc), 0);

        glBindVertexArray(VAOBig);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // drawing loaded models
        glm::mat4 scaledBench;
        glm::mat4 translatedBench;
        glm::mat4 benchModel;
        
        scaledBench = glm::scale(scaledBench, glm::vec3(0.007f, 0.007f, 0.007f));
        translatedBench = glm::translate(translatedBench, glm::vec3(0.25f, 1.0f, 5.0f));
        benchModel = translatedBench * scaledBench;
        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(benchModel));

        bench->Draw(shaderMtn);

        glm::mat4 translatedBench2;
        glm::mat4 rotatedBench2;
        glm::mat4 benchModel2;

        translatedBench2 = glm::translate(translatedBench2, glm::vec3(-0.25f, 1.0f, 5.0f));
        rotatedBench2 = glm::rotate(rotatedBench2, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        benchModel2 = translatedBench2 * rotatedBench2 * scaledBench;

        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(benchModel2));

        bench->Draw(shaderMtn);

        glm::mat4 slendermanModel;
        glm::mat4 rotatedSlenderman;
        glm::mat4 translatedSlenderman;
        glm::mat4 scaledSlenderman;

        scaledSlenderman = glm::scale(scaledSlenderman, glm::vec3(0.2f, 0.2f, 0.2f));
        translatedSlenderman = glm::translate(translatedSlenderman, glm::vec3(4.0f, -1.0f, 14.0f));
        rotatedSlenderman = glm::rotate(rotatedSlenderman, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        slendermanModel = translatedSlenderman * rotatedSlenderman * scaledSlenderman;
        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(slendermanModel));
        
        slenderman->Draw(shaderMtn);

        glm::mat4 flashlightModel;
        glm::mat4 translatedFirstFlashlight;
        glm::mat4 scaledFlashlight;
        glm::mat4 rotatedFlashlight;
        glm::mat4 translatedFlashlight;

        translatedFirstFlashlight = glm::translate(translatedFirstFlashlight, glm::vec3(0.0f, 0.0f, -58.0f));
        rotatedFlashlight = glm::rotate(rotatedFlashlight, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        scaledFlashlight = glm::scale(scaledFlashlight, glm::vec3(0.02f, 0.02f, 0.02f));
        translatedFlashlight = glm::translate(translatedFlashlight, glm::vec3(3.75f, -0.6f, 14.0f));

        flashlightModel = translatedFlashlight * rotatedFlashlight * scaledFlashlight * translatedFirstFlashlight;

        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(flashlightModel));
        
        flashlight->Draw(shaderMtn);

        glfwSwapBuffers(window);
    }

    std::cout << "Terminating application..." << std::endl;
    glDeleteVertexArrays(1, &VAO);
    glDeleteVertexArrays(1, &VAOBig);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &VBOBig);

    glfwTerminate();
    return 0;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    std::cout << "Key: " << key << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }
}

void do_movement()
{
    GLfloat cameraSpeed = Settings::CameraSpeed * deltaTime;
    if (keys[Settings::UpKey])
    {
        cameraPos += cameraSpeed * cameraUp;
    }
    if (keys[Settings::DownKey])
    {
        cameraPos -= cameraSpeed * cameraUp;
    }
    if (keys[Settings::ForwardKey])
    {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (keys[Settings::BackwardKey])
    {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (keys[Settings::LeftKey])
    {
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
    if (keys[Settings::RightKey])
    {
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    }
}

bool firstMouse = true;
void mouse_callback(GLFWwindow *window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    GLfloat xoffset = xpos - lastX;
    GLfloat yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    GLfloat sensitivity = Settings::MouseSensitivity;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
    {
        pitch = 89.0f;
    }
    if (pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(front);
}
