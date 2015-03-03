#include "SoundBank.h"

//static accessor
SoundBank SoundBank::SoundControl;

unsigned int SoundBank::LoadSoundFile(const std::string& filename)
{
    Mix_Chunk* tempSound = 0;
    if((tempSound = Mix_LoadWAV(filename.c_str())) == 0)
    {
        return -1;
    }
    m_sounds.push_back(tempSound);
    return (m_sounds.size() - 1);
}

void SoundBank::Play(unsigned int soundId)
{
    if(soundId >= m_sounds.size()) 
    {
        return;
    }
    if(m_sounds[soundId] == 0)
    {
        return;
    }
    
    Mix_PlayChannel(-1, m_sounds[soundId], 0);
}

SoundBank::SoundBank()
{
    //Do nothing
}

SoundBank::~SoundBank()
{
    for(unsigned int i = 0;i < m_sounds.size();++i) 
    {
        Mix_FreeChunk(m_sounds[i]);
    }
    Mix_CloseAudio();
}
    
