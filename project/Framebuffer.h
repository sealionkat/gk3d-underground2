class Framebuffer
{
  public:
    Framebuffer(GLuint samplesNo, GLuint screenWidth, GLuint screenHeight)
    {
        samples = samplesNo;
        width = screenWidth;
        height = screenHeight;
    }
    void Init()
    {
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        // Create a multisampled color attachment texture
        textureColorBufferMultiSampled = generateMultiSampleTexture();
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, textureColorBufferMultiSampled, 0);
        // Create a renderbuffer object for depth and stencil attachments
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

  private:
    GLuint fbo;
    GLuint rbo;

    GLuint width;
    GLuint height;
    GLuint samples;

    GLuint textureColorBufferMultiSampled;

    GLuint generateMultiSampleTexture()
    {
        GLuint texture;
        glGenTextures(1, &texture);

        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, texture);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB, width, height, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);

        return texture;
    };
};
