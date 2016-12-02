#ifndef MODEL_H
#define MODEL_H

class Model
{
public:
  Model(const GLchar *path)
  {
    std::cout << "Initalizing model" << std::endl;
    this->loadModel(path);
  }

  Model()
  {
  }

  void Draw(Shader shader)
  {
    glUniform3f(glGetUniformLocation(shader.Program, Settings::objectColorLoc), this->color[0], this->color[1], this->color[2]);

    glUniform3f(glGetUniformLocation(shader.Program, Settings::materialAmbientLoc), this->color[0], this->color[1], this->color[2]);
    glUniform3f(glGetUniformLocation(shader.Program, Settings::materialDiffuseLoc), this->color[0], this->color[1], this->color[2]);
    glUniform3f(glGetUniformLocation(shader.Program, Settings::materialSpecularLoc), 0.5f, 0.5f, 0.5f);
    glUniform1f(glGetUniformLocation(shader.Program, Settings::materialShininessLoc), 32.0f);

    //std::cout << "Drawing model" << std::endl;
    for (GLuint i = 0; i < this->meshes.size(); ++i)
    {
      this->meshes[i].Draw(shader);
    }
  }

  void SetColor(glm::vec3 color)
  {
    this->color = color;
  }

  void CreateSphere(glm::vec3 color)
  {
    int stacks = 100;
    int slices = 100;
    float radius = 1.0f;

    this->color = color;

    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    //create vertices
    for (int i = 0; i <= stacks; ++i)
    {

      float V = i / (float)stacks;
      float phi = V * glm::pi<float>();

      for (int j = 0; j <= slices; ++j)
      {

        float U = j / (float)slices;
        float theta = U * (glm::pi<float>() * 2);

        float x = cosf(theta) * sinf(phi);
        float y = cosf(phi);
        float z = sinf(theta) * sinf(phi);

        vertices.push_back({glm::vec3(x, y, z) * radius});
      }
    }

    //create indices
    for (int i = 0; i < slices * stacks + slices; ++i)
    {
      indices.push_back(i);
      indices.push_back(i + slices + 1);
      indices.push_back(i + slices);

      indices.push_back(i + slices + 1);
      indices.push_back(i);
      indices.push_back(i + 1);
    }

    Mesh m = Mesh(vertices, indices, color);
    m.CalculateNormals();
    m.setupMesh();

    this->meshes.push_back(m);
  }

private:
  std::vector<Mesh> meshes;
  std::string directory;
  glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
  bool textured;

  void loadModel(std::string path)
  {
    std::cout << "Loading model: " << path << std::endl;
    Assimp::Importer importer;

    const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals); //| aiProcess_FlipUVs

    if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
      std::cout << "ASSIMP::" << importer.GetErrorString() << std::endl;
      return;
    }

    this->directory = path.substr(0, path.find_last_of('/'));

    this->processNode(scene->mRootNode, scene);
  }

  void processNode(aiNode *node, const aiScene *scene)
  {
    std::cout << "Processing node" << std::endl;
    for (GLuint i = 0; i < node->mNumMeshes; ++i)
    {
      aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
      this->meshes.push_back(this->processMesh(mesh, scene));
    }

    for (GLuint i = 0; i < node->mNumChildren; ++i)
    {
      this->processNode(node->mChildren[i], scene);
    }
  }

  Mesh processMesh(aiMesh *mesh, const aiScene *scene)
  {
    std::cout << "Processing mesh" << std::endl;
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    std::cout << "Processing vertices" << std::endl;
    // vertices
    for (GLuint i = 0; i < mesh->mNumVertices; ++i)
    {
      Vertex vertex;

      glm::vec3 position, normal;
      glm::vec2 tex_coords;
      position.x = mesh->mVertices[i].x;
      position.y = mesh->mVertices[i].y;
      position.z = mesh->mVertices[i].z;

      vertex.Position = position;

      normal.x = mesh->mNormals[i].x;
      normal.y = mesh->mNormals[i].y;
      normal.z = mesh->mNormals[i].z;

      vertex.Normal = normal;

      if (mesh->mTextureCoords[0])
      {
        glm::vec2 texCoord;
        texCoord.x = mesh->mTextureCoords[0][i].x;
        texCoord.y = mesh->mTextureCoords[0][i].y;

        vertex.TexCoords = texCoord;
      }
      else
      {

        tex_coords.x = 0.0f;
        tex_coords.y = 0.0f;

        vertex.TexCoords = tex_coords;
      }

      vertices.push_back(vertex);
    }

    std::cout << "Processing indices" << std::endl;
    // indices
    for (GLuint i = 0; i < mesh->mNumFaces; ++i)
    {
      aiFace face = mesh->mFaces[i];
      for (GLuint j = 0; j < face.mNumIndices; ++j)
      {
        indices.push_back(face.mIndices[j]);
      }
    }

    return Mesh(vertices, indices, glm::vec3(1.0f, 1.0f, 1.0f));
  }
};

#endif // !MODEL_H
