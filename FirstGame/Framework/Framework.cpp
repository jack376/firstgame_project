#include "stdafx.h"
#include "Framework.h"
#include "InputMgr.h"
#include "SceneMgr.h"
#include "DataTableMgr.h"
#include "ResourceMgr.h"
#include "Scene.h"
#include "TextGo.h"
#include <sstream>

Framework::Framework(int w, int h, const std::string& t)
    : screenWidth(w), screenHeight(h), title(t)
{
}

void Framework::Init(int width, int height, const std::string& title)
{
	window.create(sf::VideoMode(width, height), title);
    window.setVerticalSyncEnabled(true);

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

    TextGo fpsViewer("fonts/RoboNoto-Medium.ttf", "fpsViewer");
    sf::Font systemFont;
    systemFont.loadFromFile("fonts/RoboNoto-Medium.ttf");
    fpsViewer.SetFont(systemFont);
    fpsViewer.SetCharacterSize(20);
    fpsViewer.SetOrigin(Origins::TL);
    fpsViewer.SetPosition(0.0f, 0.0f);
    fpsViewer.SetActive(false);

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
                SCENE_MGR.GetCurrentScene()->Exit();
                window.close();
                break;

            case sf::Event::GainedFocus:
                break;
            }
            INPUT_MGR.Update(event);
        }

        if (window.isOpen())
        {
            fpsDelay -= dt;

            if (INPUT_MGR.GetKeyDown(sf::Keyboard::Tilde))
            {
                fpsViewer.SetActive(!fpsViewer.GetActive());
            }
            if (fpsDelay <= 0.0f && fpsViewer.GetActive())
            {
                std::stringstream ss;
                ss << 1.0f / dt;
                fpsViewer.SetString(ss.str());
                fpsDelay = 1.0f;
            }

            Update(dt);

            window.clear();
            Draw();
            if (fpsViewer.GetActive())
            {
                fpsViewer.Draw(window);
            }

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
