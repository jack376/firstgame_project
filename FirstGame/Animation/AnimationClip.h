#pragma once

enum class AnimationLoopTypes
{
    Single,
    Loop,
};

struct AnimationFrame
{
    std::string textureId;         // �ؽ�ó ID
    sf::IntRect texCoord;          // Ŭ�� �� �ؽ�ó ��ġ
    std::function<void()> action;  // �Լ� ������ ���� ȣȯ
    // �پ��� ������ ȣ�� ������ ��ü�� ������ �� �ִ� ��ü
};

struct AnimationClip
{
    std::string id;                      // �ִϸ��̼� Ŭ�� ID  // std::string
    AnimationLoopTypes loopType;          // � �ݺ� �����ΰ�  // int �� ��ȯ
    int fps;                             // �ʴ� ������ ��      // int

    std::vector<AnimationFrame> frames;  // �� ���������� ������ Ŭ���ΰ�

    bool LoadFromFile(const std::string path);

    //AnimationClip(const AnimationClip& other) = delete;
};