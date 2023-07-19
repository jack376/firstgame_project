#include "stdafx.h"
#include "AnimationController.h"
#include "ResourceMgr.h"

void AnimationController::AddClip(const AnimationClip& newClip)
{
    if (clips.find(newClip.id) == clips.end())
    {
        clips.insert({ newClip.id, newClip });
    }
}

void AnimationController::Update(float dt)
{
    if (!isPlaying)
    {
        return;
    }
    accumTime += dt * speed;

    if (accumTime < clipDuration)
    {
        return;
    }
    accumTime = 0.0f;
    ++currentFrame;

    if (currentFrame == totalFrame) // ���� �������� �ִ� ������(������)�� ��
    {
        if (!queue.empty())
        {
            std::string id = queue.front();
            queue.pop();
            Play(id, false);
            return;
        }
        else
        {
            switch (currentClip->loopType)
            {
            case AnimationLoopTypes::Single:
                currentFrame = totalFrame - 1;
                return;

            case AnimationLoopTypes::Loop:
                currentFrame = 0;
                break;
            }
        }
    }
    
    if (currentClip->frames[currentFrame].action != nullptr)
    {
        currentClip->frames[currentFrame].action();
    }

    if (target != nullptr) 
    {
        SetFrame(currentClip->frames[currentFrame]); // target�� ������Ʈ�� �ƴ� �� ���� ���������� �Ѿ
    }
}

void AnimationController::Play(const std::string& clipid, bool clearQueue)
{
    if (clearQueue)
    {
        while (!queue.empty()) // ������� ������ pop (clear�� ��� �� ������ ������ ���� �ݺ���)
        {
            queue.pop(); // pop ���� ���� ���Դ� ���� ����
        }
    }

    auto find = clips.find(clipid);
    if (find == clips.end())
    {
        Stop();
        std::cout << "ERROR : NOT EXIST CLIP" << std::endl;
        return;
    }
    
    isPlaying    = true;
    currentClip  = &find-> second;
    currentFrame = 0;
    totalFrame   = (int)currentClip-> frames.size();
    clipDuration = 1.0f / currentClip-> fps;
    accumTime    = 0.0f;

    SetFrame(currentClip->frames[currentFrame]);
}

void AnimationController::PlayQueue(const std::string& clipid)
{
    queue.push(clipid);
}

void AnimationController::Stop()
{
    isPlaying = false;
}

void AnimationController::SetFrame(const AnimationFrame& frame)
{
    //const AnimationFrame& frame = currentClip->frames[currentFrame];
    sf::Texture* tex = RESOURCE_MGR.GetTexture(frame.textureId);
    target->setTexture(*tex);
    target->setTextureRect(frame.texCoord);
}

const std::string& AnimationController::GetCurrentClipId() const
{
    if (currentClip == nullptr)
    {
        return std::string("");
    }
    return currentClip->id;
}