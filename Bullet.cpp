#include "Bullet.h"

Bullet::Bullet(b2World& world):
  m_pBody(0),
  m_world(world)
{
  b2BodyDef bulletBodyDef;
  bulletBodyDef.bullet = true;
  bulletBodyDef.active = false;
  bulletBodyDef.fixedRotation = true;
  bulletBodyDef.type = b2_dynamicBody;

  m_pBody = m_world.CreateBody(&bulletBodyDef);

  b2CircleShape bulletShape;
  bulletShape.m_radius = 0.1f;

  b2FixtureDef bulletFixture;
  bulletFixture.restitution = 0.3f;
  bulletFixture.shape = &bulletShape;

  m_pBody->CreateFixture(&bulletFixture);
}


Bullet::~Bullet()
{
  m_world.DestroyBody(m_pBody);
}


void Bullet::Fire(const b2Transform& xf)
{
  //get a direction and a position to start from
  //This should probably be based on the transform
  //from the ship
  m_pBody->SetTransform(xf.p,xf.q.GetAngle());
  //Get/Set the starting timestamp
  m_timestamp = SDL_GetTicks();
  SetActive();
}

void Bullet::Update()
{
  if(m_pBody->IsActive())
    {
      if(m_timestamp - SDL_GetTicks() <= 0)
	{
	  SetInactive();
	}
    }
}

void Bullet::SetActive()
{
  m_pBody->SetActive(true);
}
void Bullet::SetInactive()
{
  m_pBody->SetActive(false);
}

