#ifndef INCLUDES_H
#define INCLUDES_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// SOIL
#include <SOIL/SOIL.h>

#include "Settings.h"
#include "Shader.h"
#include "Vertex.h"
#include "Mesh.h"
#include "Model.h"
#include "Camera.h"

#endif // !INCLUDES_H