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

Camera *cam = new Camera(WIDTH, HEIGHT);
bool keys[1024];

// Deltatime
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

int CurrentPlatformTexNo = 0;
bool fogOn = false;
int fogIntensity = Settings::FogIntensityDefault;

Framebuffer *msaafbo = new Framebuffer(8, WIDTH, HEIGHT);
bool antyaliasingOn = false;

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

    glEnable(GL_MULTISAMPLE); //antyaliasing

    glEnable(GL_DEPTH_TEST);

    std::cout << "Using " << glGetString(GL_VENDOR) << ", OpenGL " << glGetString(GL_VERSION) << ", Renderer " << glGetString(GL_RENDERER) << std::endl;

    /* Shaders initialization */
    std::cout << "Preparing shaders..." << std::endl;

    const GLchar *vertexPath = "shaders/vertexShader.glsl";
    const GLchar *fragmentPath = "shaders/fragmentShader.glsl";
    const GLchar *vertexEnvPath = "shaders/vertexEnvShader.glsl";
    const GLchar *fragmentEnvPath = "shaders/fragmentEnvShader.glsl";

    Shader shaderMtn(vertexPath, fragmentPath);
    Shader shaderEnv(vertexEnvPath, fragmentEnvPath);

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

        3.0f, 1.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, //right
        3.0f, 1.0f, -15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        3.0f, -1.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

        3.0f, -1.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        3.0f, 1.0f, -15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        3.0f, -1.0f, -15.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -3.0f, -1.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //down
        -3.0f, -1.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        3.0f, -1.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

        -3.0f, -1.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        3.0f, -1.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        3.0f, -1.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

        -3.0f, -1.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //left
        -3.0f, 1.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -3.0f, 1.0f, -15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,

        -3.0f, -1.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -3.0f, 1.0f, -15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -3.0f, -1.0f, -15.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

        -3.0f, -1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //back
        -3.0f, 1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        3.0f, 1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        3.0f, 1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        3.0f, -1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -3.0f, -1.0f, -15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        -3.0f, 1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //up
        -3.0f, 1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        3.0f, 1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,

        3.0f, 1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        3.0f, 1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -3.0f, 1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f};

    std::cout << "Creating walls, floor and ceiling" << std::endl;
    GLfloat verticesBig[] = {//position, normal, texture

                             8.0f, 7.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //right
                             8.0f, 7.0f, -15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                             8.0f, -1.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                             8.0f, -1.0f, 15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                             8.0f, 7.0f, -15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                             8.0f, -1.0f, -15.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                             -8.0f, -1.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, //left
                             -8.0f, 7.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                             -8.0f, 7.0f, -15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                             -8.0f, -1.0f, 15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                             -8.0f, 7.0f, -15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
                             -8.0f, -1.0f, -15.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,

                             -8.0f, 7.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, //up
                             -8.0f, 7.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                             8.0f, 7.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

                             8.0f, 7.0f, -15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                             8.0f, 7.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                             -8.0f, 7.0f, 15.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,

                             -8.0f, -1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, //down
                             -8.0f, -1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                             8.0f, -1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

                             -8.0f, -1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                             8.0f, -1.0f, -15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
                             8.0f, -1.0f, 15.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,

                             -8.0f, -1.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, //back
                             -8.0f, 7.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                             8.0f, 7.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                             8.0f, 7.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                             8.0f, -1.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
                             -8.0f, -1.0f, -15.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                             -8.0f, -1.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, //front
                             8.0f, 7.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                             8.0f, -1.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

                             -8.0f, -1.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                             8.0f, 7.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
                             -8.0f, 7.0f, 15.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f

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
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat))); // location 2 - texture coords
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
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid *)(6 * sizeof(GLfloat))); // location 2 - texture coords
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    std::cout << "Creating bench..." << std::endl;

    Model *bench = new Model("models/bench/bench_v01.obj");
    bench->SetColor(glm::vec3(0.75f, 0.75f, 0.0f));

    std::cout << "Creating slenderman..." << std::endl;
    Model *slenderman = new Model("models/slenderman/slenderman.obj");
    slenderman->SetColor(glm::vec3(0.3f, 0.3f, 0.3f));

    std::cout << "Creating flashlight..." << std::endl;
    Model *flashlight = new Model("models/flashlight/flashlight.obj");
    flashlight->SetColor(glm::vec3(0.0f, 1.0f, 1.0f));

    std::cout << "Creating sphere..." << std::endl;
    Model *sphere = new Model();
    sphere->CreateSphere(glm::vec3(1.0f, 0.0f, 0.0f));

    /* /Objects setup */

    /* Textures setup */

    std::cout << "Loading textures..." << std::endl;

    Texture *concrete0 = new Texture();
    concrete0->loadFromFile("textures/concrete0.jpg", false, false);

    Texture *concrete1 = new Texture();
    concrete1->loadFromFile("textures/concrete1.jpg", false, false);

    Texture *benchTex = new Texture();
    benchTex->loadFromFile("textures/bench_wood_color.jpg", false, false);

    Texture *platformStripes = new Texture();
    platformStripes->loadFromFile("textures/platformstripes.png", true, false);

    std::cout << "Generating texture with Perlin noise" << std::endl;
    Texture *platformPerlin = new Texture();
    platformPerlin->generatePerlinTexture(1024, 1024, false, false);

    std::cout << "Loading cubemap..." << std::endl;

    std::vector<const GLchar *> faces;
    faces.push_back("textures/grimmnight0.bmp"); //right
    faces.push_back("textures/grimmnight1.bmp"); //left
    faces.push_back("textures/grimmnight2.bmp"); //top
    faces.push_back("textures/grimmnight3.bmp"); //bottom
    faces.push_back("textures/grimmnight4.bmp"); //back
    faces.push_back("textures/grimmnight5.bmp"); //front
    Texture *cubemap = new Texture();
    cubemap->loadCubemap(faces);

    /* /Textures setup */

    /* Antyaliasing setup */
    msaafbo->Init();
    /* /Antyaliasing setup */

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

        glBindBuffer(GL_FRAMEBUFFER, 0);
        if (antyaliasingOn)
        {
            msaafbo->BindBuffer();
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shaderMtn.Use();

        // matrices
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);

        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

        // setting camera position
        glUniform3f(glGetUniformLocation(shaderMtn.Program, Settings::viewPosLoc), cameraPos.x, cameraPos.y, cameraPos.z);

        // fog
        glUniform1i(glGetUniformLocation(shaderMtn.Program, Settings::fogOnLoc), fogOn);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, Settings::fogIntensityLoc), GLfloat(fogIntensity) / 2000.0);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, Settings::fogBrightnessLoc), Settings::FogBrightness);

        // Point lights
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "pointLights[0].position"), 0.0f, 6.25f, 12.0f);
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "pointLights[0].color"), ((glm::sin((GLfloat)glfwGetTime()) + 1) / 2), 0.0f, 0.0f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "pointLights[0].linear"), 0.1f);
        glUniform1f(glGetUniformLocation(shaderMtn.Program, "pointLights[0].quadratic"), 0.1f);
        // Point light 2
        glUniform3f(glGetUniformLocation(shaderMtn.Program, "pointLights[1].position"), 0.0f, 6.25f, -12.0f);
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

        if (CurrentPlatformTexNo == 0)
        {
            concrete0->use(shaderMtn, 0);
        }
        else if (CurrentPlatformTexNo == 1)
        {
            concrete1->use(shaderMtn, 0);
        }
        else
        {
            platformPerlin->use(shaderMtn, 0);
        }
        platformStripes->use(shaderMtn, 1);
        glUniform1i(glGetUniformLocation(shaderMtn.Program, Settings::numTexturesLoc), 2);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36); //36 for whole cube
        glBindVertexArray(0);

        glBindTexture(GL_TEXTURE_2D, 0);

        glUniform1i(glGetUniformLocation(shaderMtn.Program, Settings::numTexturesLoc), 0);

//todo:::: big rectangles
        // glBindVertexArray(VAOBig);
        // glDrawArrays(GL_TRIANGLES, 0, 36);
        // glBindVertexArray(0);

        // drawing loaded models
        glm::mat4 scaledBench;
        glm::mat4 translatedBench;
        glm::mat4 benchModel;

        scaledBench = glm::scale(scaledBench, glm::vec3(0.007f, 0.007f, 0.007f));
        translatedBench = glm::translate(translatedBench, glm::vec3(0.25f, 1.0f, 5.0f));
        benchModel = translatedBench * scaledBench;
        glUniformMatrix4fv(glGetUniformLocation(shaderMtn.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(benchModel));

        benchTex->use(shaderMtn, 0);
        glUniform1i(glGetUniformLocation(shaderMtn.Program, Settings::numTexturesLoc), 1);
        bench->Draw(shaderMtn);
        glUniform1i(glGetUniformLocation(shaderMtn.Program, Settings::numTexturesLoc), 0);

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

        shaderEnv.Use();

        glUniformMatrix4fv(glGetUniformLocation(shaderEnv.Program, Settings::viewMatrixLoc), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderEnv.Program, Settings::projectionMatrixLoc), 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 sphereModel;
        glm::mat4 translatedSphere;

        translatedSphere = glm::translate(translatedSphere, glm::vec3(0.0f, 4.0f, -3.0f));

        sphereModel = translatedSphere;

        glUniformMatrix4fv(glGetUniformLocation(shaderEnv.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(sphereModel));

        cubemap->useCubemap(shaderEnv);

        sphere->Draw(shaderEnv);

        glm::mat4 wallsModel;

        wallsModel = glm::rotate(wallsModel, glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(shaderEnv.Program, Settings::modelMatrixLoc), 1, GL_FALSE, glm::value_ptr(wallsModel));

        glBindVertexArray(VAOBig);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        if (antyaliasingOn)
        {
            msaafbo->UseBuffer();
        }

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
    // std::cout << "Key: " << key << std::endl;
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
    if (keys[Settings::SwitchPlatformTexture])
    {
        CurrentPlatformTexNo = (CurrentPlatformTexNo + 1) % 3;
        keys[Settings::SwitchPlatformTexture] = false;
        std::cout << "Switching platform texture. Texture no: " << CurrentPlatformTexNo << std::endl;
    }
    if (keys[Settings::SwitchFog])
    {
        fogOn = !fogOn;
        keys[Settings::SwitchFog] = false;
        std::cout << "Switching fog" << std::endl;
    }
    if (keys[Settings::UpFogIntensity])
    {
        fogIntensity += Settings::FogIntensityStep;
        if (fogIntensity > Settings::FogIntensityMax)
        {
            fogIntensity = Settings::FogIntensityMax;
        }
        //keys[Settings::UpFogIntensity] = false;
    }
    if (keys[Settings::DownFogIntensity])
    {
        fogIntensity -= Settings::FogIntensityStep;
        if (fogIntensity < Settings::FogIntensityMin)
        {
            fogIntensity = Settings::FogIntensityMin;
        }
        //keys[Settings::DownFogIntensity] = false;
    }
    if (keys[Settings::SwitchAntyaliasing])
    {
        antyaliasingOn = !antyaliasingOn;
        keys[Settings::SwitchAntyaliasing] = false;
        std::cout << "Switching antialiasing" << std::endl;
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
