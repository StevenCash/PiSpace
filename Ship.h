#ifndef SHIP_H
#define SHIP_H

#include <SDL2/SDL_opengles2.h> 
#include <Box2D/Box2D.h>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <vector>

#include "ShipIntf.h"

#define GL2BOX(x) (((x)*5.0f)+5.0f)
#define BOX2GL(x) (((x)-5.0f)/5.0f)

#define DEG2RAD(x) ((x) * 0.0174532925f)
#define RAD2DEG(x) ((x) * 57.2957795f)

class Ship : public ShipIntf
{
    struct TempStruct
    {
        GLfloat vertices[4];
        GLfloat colors[4];
    };

public:
    //Default constructor
    Ship(b2World& world);
    ~Ship();
    //The function that actually puts the object on the screen
    void Draw();
    void RotateCW();
    void RotateCCW();
    void Forward();

    void translateUp();
    void translateDown();
    void translateRight();
    void translateLeft();
    void stop();
private:
    static const glm::mat4 m_projMat;
    glm::vec3 m_rotateAxis;

    //only using 1 vertex buffer for this object
    GLuint m_vertexBufferHandleArray[1];    
    GLuint m_colorBufferHandleArray[1];    

    static GLuint m_shaderProgram;
    GLuint m_indexBuffer;
    GLuint m_numIndices;
    GLfloat m_angle;

    //Box2D parameters

    //reference to the world to use for Box
    b2World& m_worldRef;

    b2BodyDef m_bodyDef;
    
    //should destroy this body in the destructor
    b2Body *m_pBody;

    //ship creation stuff
    int m_index;
    static int index;

};

#endif //SHIP_H
