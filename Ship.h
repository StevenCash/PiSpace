#ifndef SHIP_H
#define SHIP_H

#include "GLHeader.h"

#include <Box2D/Box2D.h>
#include <vector>
#include <deque>
#include "Bullet.h"
#include "Vortex.h"
#include "ShipIntf.h"
#include "DestroyableIntf.h"
#include "Explosion.h"

//changed by SC
#define RAD2DEG(x) ((x) * 57.2957795f)
//#define RAD2DEG(x) ((x))

class Ship : public ShipIntf, public DestroyableIntf
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

    //called every frame before drawing to do whatever was commanded
    //in ProcessInput()
    void DoCommands();
    
    //turn a bit mapped field into a set of commands to be handled
    //during DoCommands
    void ProcessInput(int commands);
    void AddBullet(Bullet* pBullet);

    //override of DestroyableIntf function
    virtual void DestroyObject();


private:
    static const glm::mat4 m_projMat;
    glm::vec3 m_rotateAxis;

    //only using 1 vertex buffer for this object
    GLuint m_vertexBufferHandleArray[1];    
    GLuint m_colorBufferHandleArray[1];    

    static GLuint m_shaderProgram;
    static uint32 m_soundIndex;
    static bool m_bSoundLoaded;

    GLuint m_indexBuffer;
    GLuint m_numIndices;
    GLfloat m_angle;

    //Box2D parameters

    //reference to the world to use for Box
    b2World& m_worldRef;
    
    //should destroy this body in the destructor
    b2Body *m_pBody;

    //ship creation stuff
    int m_index;
    int m_hitCount;
    static int index;


    //Flags to indicate button states
    bool m_bForceCCW; //left
    bool m_bForceCW; //right
    bool m_bForceForward; // Forward thrust

    Bullet m_bullet1;
    Bullet m_bullet2;
    Bullet m_bullet3;
    
    std::deque<Bullet*> m_bullets;

    float m_startX;
    float m_startY;

    Vortex m_vortex;

    Explosion m_explosion;
    //Container to hold the interface to pass to
    //Box2D as UserData, since it's stores it as a void
    //and will lose most of its vtable
    DestroyableIntfContainer m_destroyableContainer;

    void respawn();
};

#endif //SHIP_H
