#ifndef MESH_H
#define MESH_H

// struct Vertex
// {
//     glm::vec3 Position;
//     glm::vec3 Normal;
// };

class Mesh
{
  public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    glm::vec3 color;

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, glm::vec3 color)
    {
        std::cout << "Initializing color mesh" << std::endl;
        this->vertices = vertices;
        this->indices = indices;
        this->color = color;

        this->setupMesh();
    }

    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures)
    {
        std::cout << "Initializing texture mesh" << std::endl;

        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;

        this->setupMesh();
    }

    void Draw(Shader shader)
    {
        //std::cout << "Drawing mesh" << std::endl;
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

  private:
    GLuint VBO, VAO, EBO;

    void setupMesh()
    {
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO); //enable VAO

        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

        glEnableVertexAttribArray(0); // position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Position));

        glEnableVertexAttribArray(1); // normal
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, Normal));

        glEnableVertexAttribArray(2); // texture coords
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid *)offsetof(Vertex, TexCoords));

        glBindVertexArray(0); // disable VAO
    }
};

#endif // !MESH_H