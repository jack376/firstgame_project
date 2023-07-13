#include "stdafx.h"
#include "Framework.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
#include "Scene.h"

Framework::Framework(int w, int h, const std::string& t)
    : screenWidth(w), screenHeight(h), title(t)
{
}

void Framework::Init(int width, int height, const std::string& title)
{
	window.create(sf::VideoMode(width, height), title);

    RESOURCE_MGR.Init();
    DATATABLE_MGR.LoadAll();
    SCENE_MGR.Init();
}

void Framework::Release()
{
    SCENE_MGR.Release();
    DATATABLE_MGR.ReleaseAll();
}

void Framework::Update(float dt)
{
    SCENE_MGR.Update(dt);
}

void Framework::Draw()
{
    SCENE_MGR.Draw(window);
}

void Framework::Run()
{
    Init(screenWidth, screenHeight, title);
    clock.restart();

    while (window.isOpen())
    {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        INPUT_MGR.Update(dt);

        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                SCENE_MGR.GetCurrScene()->Exit();
                window.close();
                break;

            case sf::Event::GainedFocus:
                break;
            }
            INPUT_MGR.Update(event);
        }

        if (window.isOpen())
        {
            Update(dt);

            window.clear();
            Draw();
            window.display();
        }
    }

    Release();
}

sf::Vector2f Framework::GetWindowSize()
{
    return sf::Vector2f((float)screenWidth, (float)screenHeight);
}

sf::RenderWindow& Framework::GetWindow()
{
    return window;
}
