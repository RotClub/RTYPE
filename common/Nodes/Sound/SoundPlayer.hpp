//
// Created by Théo Bary on 17/01/2025.
//

#ifndef SOUND_HPP
    #define SOUND_HPP

    #include <Engine.hpp>
    #include <Nodes/Node.hpp>

class SoundPlayer : public Node {
    public:
        explicit SoundPlayer(const std::string &name , const std::string &sound);
        ~SoundPlayer();

        void play();
        void stop();
        void pause();
        void resume();
        void setVolume(float volume);
        void setPitch(float pitch);
        void setPan(float pan);
        void setSound(const std::string &sound);
        [[nodiscard]] const std::string &getSound() const;
        [[nodiscard]] bool isPlaying() const;
        [[nodiscard]] bool isStopped() const;

    private:
        std::string _sound;
};

#endif //SOUND_HPP
