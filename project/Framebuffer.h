#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class Framebuffer
{
  public:
    Framebuffer(GLuint samplesNo, GLuint screenWidth, GLuint screenHeight)
    {
        samples = samplesNo;
        width = screenWidth;
        height = screenHeight;
    }

    Framebuffer(GLboolean depth, GLboolean stencil, GLuint screenWidth, GLuint screenHeight)
    {
        width = screenWidth;
        height = screenHeight;
        depth = depth;
        stencil = stencil;
    }

    void InitMultisampled()
    {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        textureColorBufferMultiSampled = generateMultiSampleTexture();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Init()
    {
        glGenFramebuffers(1, &ifbo);
        glBindFramebuffer(GL_FRAMEBUFFER, ifbo);

        GLuint textureColorBuffer = generateAttachmentTexture();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorBuffer, 0);

        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void BindBuffer()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void UseBuffer()
    {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    }

    void UseBlurTexture(Shader shader) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureColorBuffer);
        glUniform1i(glGetUniformLocation(shader.Program, "nazwa"), 0);
    }

    void UseBlurBuffer() {
        glBindFramebuffer(GL_READ_FRAMEBUFFER, fbo);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, ifbo);
        glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_LINEAR);
    }

  private:
    GLuint fbo;
    GLuint ifbo;
    GLuint rbo;

    GLuint width;
    GLuint height;
    GLuint samples;

    GLboolean depth = false;
    GLboolean stencil = false;

    GLuint textureColorBufferMultiSampled;
    GLuint textureColorBuffer;

    GLuint generateMultiSampleTexture()
    {
        GLuint texture;
        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

        return texture;
    };

    GLuint generateAttachmentTexture()
    {
        GLenum attachmentType;
        if (!depth && !stencil)
        {
            attachmentType = GL_RGB;
        }
        else if (depth && !stencil)
        {
            attachmentType = GL_DEPTH_COMPONENT;
        }
        else if (!depth && stencil)
        {
            attachmentType = GL_STENCIL_INDEX;
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        if (!depth && !stencil)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, attachmentType, width, height, 0, attachmentType, GL_UNSIGNED_BYTE, NULL);
        }
        else
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        return textureID;
    }
};

#endif // !FRAMEBUFFER_H
