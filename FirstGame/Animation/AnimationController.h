#pragma once

class AnimationController
{
protected:
    std::unordered_map<std::string, AnimationClip> clips;

    std::queue<std::string> queue;   // 애니메이션 ID queue 자료구조
    float speed     = 1.0f;          // 애니메이션 속도 배율
    bool  isPlaying = false;

    AnimationClip* currentClip;
    int   currentFrame = 0;
    int   totalFrame   = 0;   
    float clipDuration = 0.0f;
    float accumTime    = 0.0f;

    sf::Sprite* target = nullptr;

public:
    void AddClip(const AnimationClip& newClip);

    void SetTarget(sf::Sprite* sprite) { target = sprite; }
    sf::Sprite* GetTarget() const { return target; }

    bool IsPlaying() { return isPlaying; }

    void SetSpeed(float s) { speed = s; }

    void Update(float dt);
    
    void Play(const std::string& clipid, bool clearQueue = true);
    void PlayQueue(const std::string& clipid);

    void Stop();

    void SetFrame(const AnimationFrame& frame);
    const std::string& GetCurrentClipId() const;
};

