#include "Stars.h"
#include "Star.h"

Stars::Stars():
{
    
}



Stars::Stars():
{
    for(int i=0; i< m_stars.size(); ++i)
    {
        delete m_stars[i];
    }
    
}


void Stars::Draw()
{
    for(int i=0; i< m_stars.size(); ++i)
    {
        m_stars[i]->Draw();
    }
}
