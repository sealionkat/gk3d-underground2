#ifndef TEXTURE_H
#define TEXTURE_H

class Texture
{
  public:
    void loadFromFile(const char *fileName, bool noRepeatS, bool noRepeatT)
    {
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, noRepeatS ? GL_CLAMP_TO_BORDER : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, noRepeatT ? GL_CLAMP_TO_BORDER : GL_REPEAT);

        if (noRepeatS || noRepeatT)
        {
            GLfloat borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f}; //black and transparent
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

    void generatePerlinTexture(int perlinWidth, int perlinHeight, bool noRepeatS, bool noRepeatT)
    {
        GLubyte perlinTexture[perlinHeight][perlinWidth][4];

        for(int i = 0; i < perlinHeight; ++i) {
            for(int j = 0; j < perlinWidth; ++j) {
                float perlin = (Perlin::getPerlinValue(j, i) + 1) * 0.5 * 255;

                perlinTexture[i][j][0] = (GLubyte) perlin;
                perlinTexture[i][j][1] = (GLubyte) perlin;
                perlinTexture[i][j][2] = (GLubyte) perlin;
                perlinTexture[i][j][3] = 255;
            }
        }

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, noRepeatS ? GL_CLAMP_TO_BORDER : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, noRepeatT ? GL_CLAMP_TO_BORDER : GL_REPEAT);

        if (noRepeatS || noRepeatT)
        {
            GLfloat borderColor[] = {0.0f, 0.0f, 0.0f, 0.0f}; //black and transparent
            glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, perlinWidth, perlinHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, perlinTexture);

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
    GLuint id;
    std::string type;
};

#endif // !TEXTURE_H
