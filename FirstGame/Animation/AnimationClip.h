#pragma once

enum class AnimationLoopTypes
{
    Single,
    Loop,
};

struct AnimationFrame
{
    std::string textureId;         // 텍스처 ID
    sf::IntRect texCoord;          // 클립 내 텍스처 위치
    std::function<void()> action;  // 함수 포인터 상위 호환
    // 다양한 종류의 호출 가능한 객체를 저장할 수 있는 객체
};

struct AnimationClip
{
    std::string id;                      // 애니메이션 클립 ID  // std::string
    AnimationLoopTypes loopType;          // 어떤 반복 유형인가  // int 형 변환
    int fps;                             // 초당 프레임 수      // int

    std::vector<AnimationFrame> frames;  // 몇 프레임으로 구성된 클립인가

    bool LoadFromFile(const std::string path);

    //AnimationClip(const AnimationClip& other) = delete;
};