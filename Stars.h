#ifndef STARS_H
#define STARS_H

#include <vector>

class Star;

class Stars
{
public:
//Constructor
    explicit Stars();
    void Draw();
    ~Stars();
private:
    std::vector<Star*> m_stars;
};

#endif
