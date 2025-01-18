//
// Created by Théo Bary on 17/01/2025.
//

#include "SoundPlayer.hpp"

SoundPlayer::SoundPlayer(const std::string &name, const std::string &sound)
    : Node(name)
{
    setSound(sound);
}

SoundPlayer::~SoundPlayer()
{
    stop();
    Engine::GetInstance().getResourceManager().unloadResource(_sound);
}

void SoundPlayer::play()
{
    raylib::Sound &sound = Engine::GetInstance().getResourceManager().getSound(_sound);
    sound.Play();
}

void SoundPlayer::stop()
{
    raylib::Sound &sound = Engine::GetInstance().getResourceManager().getSound(_sound);
    sound.Stop();
}

void SoundPlayer::pause()
{
    raylib::Sound &sound = Engine::GetInstance().getResourceManager().getSound(_sound);
    sound.Pause();
}

void SoundPlayer::resume()
{
    raylib::Sound &sound = Engine::GetInstance().getResourceManager().getSound(_sound);
    sound.Resume();
}

void SoundPlayer::setVolume(float volume)
{
    raylib::Sound &sound = Engine::GetInstance().getResourceManager().getSound(_sound);
    sound.SetVolume(volume);
}

void SoundPlayer::setPitch(float pitch)
{
    raylib::Sound &sound = Engine::GetInstance().getResourceManager().getSound(_sound);
    sound.SetPitch(pitch);
}

void SoundPlayer::setPan(float pan)
{
    raylib::Sound &sound = Engine::GetInstance().getResourceManager().getSound(_sound);
    sound.SetPan(pan);
}

void SoundPlayer::setSound(const std::string &sound)
{
    _sound = sound;
    Engine::GetInstance().getResourceManager().loadResource(_sound);
}

const std::string &SoundPlayer::getSound() const
{
    return _sound;
}

bool SoundPlayer::isPlaying() const
{
    raylib::Sound &sound = Engine::GetInstance().getResourceManager().getSound(_sound);
    return sound.IsPlaying();
}

bool SoundPlayer::isStopped() const
{
    raylib::Sound &sound = Engine::GetInstance().getResourceManager().getSound(_sound);
    return !sound.IsPlaying();
}
