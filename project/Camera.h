

class Camera
{
  public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    GLfloat Yaw;
    GLfloat Pitch;

    glm::mat4 View;
    glm::mat4 Projection;

    Camera(int width, int height)
    {
        Position = glm::vec3(0.0f, 2.0f, 0.0f);
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        Yaw = -90.0f;
        Pitch = 0.0f;

        Projection = glm::perspective(glm::radians(Settings::FOV), (float)width / (float)height, Settings::PerspectiveNear, Settings::PerspectiveFar);
    }

    void ComputeView()
    {
        View = glm::lookAt(Position, Position + Front, Up);
    }

    void DoMovement(glm::vec3 posOffset)
    {
        Position += posOffset;
    }

    void RotateCamera(GLfloat xoffset, GLfloat yoffset)
    {
        Yaw += xoffset;
        Pitch += yoffset;

        if (Pitch > 89.0f)
        {
            Pitch = 89.0f;
        }
        if (Pitch < -89.0f)
        {
            Pitch = -89.0f;
        }

        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));

        Front = glm::normalize(front);
    }
}
