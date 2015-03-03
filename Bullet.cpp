#include <SDL2/SDL.h>
#include "Bullet.h"
#include "ShaderUtil.h"

#include <glm/gtc/type_ptr.hpp>
#include <math.h>
#include <iostream>

#include "TraceLogger.h"

const glm::mat4 Bullet::m_projMat(glm::ortho(-10.0f,10.0f,-10.0f,10.0f));
GLuint Bullet::m_shaderProgram = 0;
GLuint Bullet::m_vertexbuffer = 0;
GLuint Bullet::m_colorbuffer = 0;
GLuint Bullet::m_numVertices = 0;

Bullet::Bullet(b2World& world, ShipIntf *pShip, uint32 index):
    m_pBody(0),
    m_world(world),
    m_radius(0.1f),
    m_pShip(pShip),
    m_timestamp(0)
{
    m_destroyableContainer.pDestroyable = this;

    b2BodyDef bulletBodyDef;
    bulletBodyDef.bullet = true; //does it bounce?
    bulletBodyDef.active = false; //is it an active physics object
    bulletBodyDef.type = b2_dynamicBody;
    bulletBodyDef.userData = &m_destroyableContainer;
    m_pBody = m_world.CreateBody(&bulletBodyDef);

    b2CircleShape bulletShape;
    bulletShape.m_radius = m_radius;

    b2FixtureDef bulletFixture;
    bulletFixture.restitution = 0.3f;
    bulletFixture.density = 10.0f;
    bulletFixture.shape = &bulletShape;

    m_pBody->CreateFixture(&bulletFixture);

//Base the color of this ship on the index
    GLfloat blue = 0.0f;
    GLfloat red = 0.0f;
    GLfloat green = 0.0f;

    switch(index)
    {
        case 0:
        {
            blue = 1.0f;
            green = 0.0f;
            red = 0.7f;
            break;
        }
        case 1:
        {
            red = 1.0f;
            break;
        }
        case 2:
        {
            green = 1.0f;
            break;
        }
        case 3:
        {
            blue = 1.0f;
            break;
        }
        default:
        {
            std::cerr << "Too many bullets." << std::endl;
            break;
            exit(1);
        }
    }

    //Init the shader program to use
    if(!m_shaderProgram)
    {
        ShaderList_T shaderList;
        shaderList.push_back(createShaderFromFile(GL_VERTEX_SHADER, "BulletVertex.glsl"));
        shaderList.push_back(createShaderFromFile(GL_FRAGMENT_SHADER, "BulletFragment.glsl"));
        m_shaderProgram = createShaderProgram(shaderList);

//Setup Position ATTRIBUTES
        GLuint numOutsideVertices=16;
        m_numVertices=numOutsideVertices+2;
        GLuint numValues=m_numVertices*3;
        GLfloat centerx = 0.0f;
        GLfloat centery = 0.0f;
        int i=0;
        GLfloat f[numValues];
        f[0] = centerx;
        f[1] = centery;
        f[2] = 0.0f;

        float increment = (3.14159265 *2)/numOutsideVertices;
        for(float angle=(3.14159265 * 2.0); angle > 0.0f; angle-=increment)
        {
            i+=3;
            f[i]=centerx+sin(angle)*m_radius;
            f[i+1]=centery+cos(angle)*m_radius;
            f[i+2]=0.0f;
        }
        f[numValues-3] = f[3];
        f[numValues-2] = f[4];
        f[numValues-1] = f[5];

  
        glGenBuffers ( 1 , &m_vertexbuffer ) ;
        glBindBuffer ( GL_ARRAY_BUFFER , m_vertexbuffer ) ;
        glBufferData ( GL_ARRAY_BUFFER , sizeof ( f ) , f , GL_STATIC_DRAW ) ;
        glBindBuffer(GL_ARRAY_BUFFER,0);


        //Setup COLOR attributes
        GLfloat colorArray[numValues];

        colorArray[0]=1.0f;
        colorArray[1]=1.0f;
        colorArray[2]=1.0f;
        for(GLuint x=3; x < numValues; x+=3)
        {
            colorArray[x]= red;
            colorArray[x+1]= green;
            colorArray[x+2]= blue;
        }
  
        glGenBuffers(1,&m_colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, m_colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorArray), colorArray, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER,0);
    }

    m_pBody->SetActive(false);

}


Bullet::~Bullet()
{
    m_world.DestroyBody(m_pBody);

    glDeleteBuffers(1,&m_vertexbuffer);
    glDeleteBuffers(1,&m_colorbuffer);

}


void Bullet::Fire(const b2Transform& xf)
{
    //Only do this if the bullet is not active.
    if(! m_pBody->IsActive())
    {
        m_pBody->SetActive(true);
        //get a direction and a position to start from
        //This should probably be based on the transform
        //from the ship
        b2Vec2 pos  = b2Mul(xf,b2Vec2(0.0f, 0.6f));
        m_pBody->SetTransform(pos,0.0f);
        b2Transform bXf=xf;
        bXf.p.SetZero();
        b2Vec2 velocity = b2Mul(bXf,b2Vec2(0.0f, 50.0f));
        m_pBody->SetLinearVelocity(velocity);
        
        //Get/Set the starting timestamp
        m_timestamp = SDL_GetTicks();
    }
}



void Bullet::Draw()
{
    if(m_pBody->IsActive())
    {  
        uint32 aliveTime=SDL_GetTicks() - m_timestamp;
        if(aliveTime > 9999)
        {
            m_pBody->SetLinearVelocity(b2Vec2(0.0f,0.0f));
            m_pBody->SetAngularVelocity(0.0f);
            m_pBody->SetActive(false);
            m_pShip->AddBullet(this);
        }
        else
        {
            GLfloat fade=1.0f - (float(aliveTime)/10000.0f);
            // Use our shader
            glUseProgram ( m_shaderProgram ) ;

            //setup the vertices!
            GLuint positionAttribute = glGetAttribLocation(m_shaderProgram,"vPosition");
            glEnableVertexAttribArray ( positionAttribute ) ;
            glBindBuffer ( GL_ARRAY_BUFFER , m_vertexbuffer ) ;
            glVertexAttribPointer (
                positionAttribute , // attribute 0.
                3 , // size
                GL_FLOAT , // type
                GL_FALSE , // normalized ?
                0 , // stride
                ( void * ) 0 // array buffer offset
                ) ;

            //Setup the color(s)!
            glBindBuffer ( GL_ARRAY_BUFFER , m_colorbuffer ) ;
            GLuint colorAttribute = glGetAttribLocation(m_shaderProgram, "vColor");
            glEnableVertexAttribArray(colorAttribute);
            glVertexAttribPointer (
                colorAttribute , // attribute 1. 
                3 , // size 
                GL_FLOAT , // type
                GL_FALSE , // normalized ?
                0 , // stride
                ( void * ) 0 // array buffer offset
                ) ;

            glm::mat4 modelMatrix(1.0f);    
            GLuint locationMVP = glGetUniformLocation(m_shaderProgram,"mvpMatrix");
            GLuint locationFade = glGetUniformLocation(m_shaderProgram,"uFade");
            glUniform1f(locationFade,fade);

            b2Vec2 position = m_pBody->GetWorldCenter();

            modelMatrix = 
                glm::translate(modelMatrix, glm::vec3(position.x, position.y, 0.0f)); 
    

            glm::mat4 mvpMat = m_projMat * modelMatrix;
            glUniformMatrix4fv(locationMVP, 1, GL_FALSE, glm::value_ptr(mvpMat));

            // Draw the octogon!
            glDrawArrays ( GL_TRIANGLE_FAN , 0 , m_numVertices ); 
            glDisableVertexAttribArray ( positionAttribute ) ;
            glDisableVertexAttribArray ( colorAttribute ) ;
        }
    }
}


void Bullet::DestroyObject()
{
    m_pBody->SetActive(false);
    m_pShip->AddBullet(this);
}
