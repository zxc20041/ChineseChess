#include "ChineseChessHistory.h"

GameHistory_Page::GameHistory_Page() :PAGE(PAGE_INDEX, PAGE_CREATED_STATUS, Effect::WHITE_CIRCLE_SWITCH, 2, "GameHistory")
{
}

GameHistory_Page::~GameHistory_Page()
{
}

bool GameHistory_Page::Init()
{
    return false;
}

void GameHistory_Page::Update()
{
}

void GameHistory_Page::Rend()
{
}

bool GameHistory_Page::EnterPage()
{
    return false;
}

bool GameHistory_Page::ExitPage()
{
    return false;
}
