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

    if (currentFrame == totalFrame) // 현재 프레임이 최대 프레임(마지막)일 때
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
        SetFrame(currentClip->frames[currentFrame]); // target이 널포인트가 아닐 때 다음 프레임으로 넘어감
    }
}

void AnimationController::Play(const std::string& clipid, bool clearQueue)
{
    if (clearQueue)
    {
        while (!queue.empty()) // 비어있지 않으면 pop (clear가 없어서 다 없어질 때까지 비우는 반복문)
        {
            queue.pop(); // pop 제일 먼저 나왔던 놈이 빠짐
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