#include "Game.h"
#include "Actor/Player.h"
#include "Level/GameLevel.h"
#include <string>

Game::Game(HINSTANCE hInstance, const std::wstring& title, uint32 width, uint32 height) : Engine(hInstance, title, width, height)
{
	SetLevel(std::make_shared<GameLevel>());
}

Game::~Game()
{
}