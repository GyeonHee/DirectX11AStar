#pragma once
#include "Engine.h"
#include "Level/Level.h"

class Game : public Engine
{
public:
    Game(HINSTANCE hInstance, const std::wstring& title, uint32 width, uint32 height);
    ~Game();

    Level* GetMainLevel() const { return mainLevel; }


private:
    Level* mainLevel;
};