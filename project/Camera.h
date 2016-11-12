

class Camera
{
  public:
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    GLfloat yaw;
    GLfloat pitch;
    Camera()
    {
        Position = glm::vec3(0.0f, 2.0f, 0.0f);
        Front = glm::vec3(0.0f, 0.0f, -1.0f);
        Up = glm::vec3(0.0f, 1.0f, 0.0f);
        yaw = -90.0f;
        pitch = 0.0f;
    }

    void DoMovement(glm::vec3 position)
    {
        Position += position;
    }

    void RotateCamera(GLfloat xoffset, GLfloat yoffset)
    {
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

        Front = glm::normalize(front);
    }
}
