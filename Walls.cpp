#include "Walls.h"
#include <Box2D/Box2D.h>
#include "ShaderUtil.h"
#include <glm/gtc/type_ptr.hpp>
#include "TraceLogger.h"

const glm::mat4 Walls::m_projMat(glm::ortho(-10.0f,10.0f,-10.0f,10.0f));

Walls::Walls(b2World& world):
    m_worldRef(world)
{
    b2BodyDef bodyDef;

    //Top
    bodyDef.position.Set(-10.0f, 9.5f);
    m_topBody = world.CreateBody(&bodyDef);
    b2EdgeShape topEdge;
    topEdge.Set(b2Vec2(0.0, 0.0), b2Vec2(20.0,0.0));
    m_topBody->CreateFixture(&topEdge, 0.0f);
    
      
    //left
    bodyDef.position.Set(-9.5f, 10.0f);
    m_leftBody = world.CreateBody(&bodyDef);
    b2EdgeShape leftEdge;
    leftEdge.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f,-20.0f));
    m_leftBody->CreateFixture(&leftEdge, 0.0f);

    //bottom
    bodyDef.position.Set(-10.0f, -9.5f);
    m_groundBody = world.CreateBody(&bodyDef);
    b2EdgeShape groundEdge;
    groundEdge.Set(b2Vec2(0.0f, 0.0f), b2Vec2(20.0f,0.0f));
    m_groundBody->CreateFixture(&groundEdge, 0.0f);

    //right
    bodyDef.position.Set(9.5f, 10.0f);
    m_rightBody = world.CreateBody(&bodyDef);
    b2EdgeShape rightEdge;
    rightEdge.Set(b2Vec2(0.0f, 0.0f), b2Vec2(0.0f,-20.0f));
    m_rightBody->CreateFixture(&rightEdge, 0.0f);


    //Init the shader program to use
    if(!m_shaderProgram)
    {
        ShaderList_T shaderList;
        shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, "WallVertex.glsl"));
        shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, "WallFragment.glsl"));
        m_shaderProgram = createShaderProgram(shaderList);
    }
    
    GLfloat bounds[] =
        { -9.5, -9.5,
           9.5, -9.5,
           9.5,  9.5,
          -9.5,  9.5 };


    glGenBuffers ( 1 , &m_wallbuffer ) ;
    glBindBuffer ( GL_ARRAY_BUFFER , m_wallbuffer ) ;
    glBufferData ( GL_ARRAY_BUFFER , sizeof ( bounds ) , bounds , GL_STATIC_DRAW ) ;
    glBindBuffer(GL_ARRAY_BUFFER,0);


}

void Walls::Draw()
{
    TRACE;
    glUseProgram ( m_shaderProgram ) ;
    //setup the vertices!
    GLuint positionAttribute = glGetAttribLocation(m_shaderProgram,"vPosition");
    glEnableVertexAttribArray ( positionAttribute ) ;
    glBindBuffer ( GL_ARRAY_BUFFER , m_wallbuffer ) ;
    glVertexAttribPointer (
        positionAttribute , // attribute 0.
        2 , // size
        GL_FLOAT , // type
        GL_FALSE , // normalized ?
        0 , // stride
        ( void * ) 0 // array buffer offset
        ) ;
    
    GLuint locationMVP = glGetUniformLocation(m_shaderProgram,"mvpMatrix");
    glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(m_projMat));
    glDrawArrays ( GL_LINES , 0 ,4 ) ; // 4 indices starting at 0 
    glDisableVertexAttribArray ( positionAttribute ) ;

}


Walls::~Walls()
{
    m_worldRef.DestroyBody(m_topBody);
    m_worldRef.DestroyBody(m_leftBody);
    m_worldRef.DestroyBody(m_rightBody);
    m_worldRef.DestroyBody(m_groundBody);
    glDeleteBuffers(1,&m_wallbuffer);

}
