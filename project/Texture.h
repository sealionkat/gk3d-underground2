#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
  public:
    void loadFromFile(const char *fileName, bool noRepeat)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, noRepeat ? GL_CLAMP_TO_BORDER : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, noRepeat ? GL_CLAMP_TO_BORDER : GL_REPEAT);

        if (noRepeat)
        {
            GLfloat borderColor[] = {1.0f, 0.0f, 0.0f, 0.0f}; //black and transparent
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        unsigned char *image = SOIL_load_image(fileName, &width, &height, 0, SOIL_LOAD_RGBA);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);

        SOIL_free_image_data(image);

        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void use(Shader shader, int num)
    {
        if (num >= 0 && num < Settings::TexturesCount)
        {
            glActiveTexture(GL_TEXTURE0 + num);
            glBindTexture(GL_TEXTURE_2D, texture);
            std::string name = std::string(Settings::texturesLoc) + std::string("[") + std::to_string(num) + std::string("]");
            glUniform1i(glGetUniformLocation(shader.Program, name.c_str()), num);
        }
    }

  private:
    GLuint texture;
    int width;
    int height;
};

#endif // !TEXTURE_H
