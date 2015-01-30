#ifndef WALLS_H
#define WALLS_H

#include <SDL2/SDL_opengles2.h> 
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective

class b2World;
class b2Body;

class Walls
{
public:
    explicit Walls(b2World& world);
    void Draw();
    ~Walls();
private:
    static const glm::mat4 m_projMat;

    b2World& m_worldRef;

    b2Body* m_topBody;
    b2Body* m_leftBody;
    b2Body* m_groundBody;
    b2Body* m_rightBody;

    GLuint m_wallbuffer;
    GLuint m_shaderProgram;
};

#endif
