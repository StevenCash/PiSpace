#ifndef SOUNDBANK_H
#define SOUNDBANK_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <vector>
#include <string>

class SoundBank
{
public:
    static SoundBank SoundControl;
    unsigned int LoadSoundFile(const std::string& filename);
    void Play(unsigned int soundId);

private:
    SoundBank();
    ~SoundBank();
    
    std::vector<Mix_Chunk*> m_sounds;
};



#endif
