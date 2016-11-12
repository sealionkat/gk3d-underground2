

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

    void DoMovement(GLfloat cameraSpeed)
    {
    }

    void RotateCamera()
    {
    }
}
