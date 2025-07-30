#pragma once

#include"framework_base.h"
#include"CChessUI.h"
#include"localPVP.h"

class LocalGame_Page :public PAGE
{
public:
	constexpr static int PAGE_INDEX = PAGE_GAME;

    LocalGame_Page();
    ~LocalGame_Page();
    bool Init() override;
    void Update() override;
    void Rend() override;
    bool EnterPage() override;
    bool ExitPage() override;


private:
    shared_ptr<Button> switchSideButton, returnButton;

    CChessUI CUI;
    shared_ptr<CChessEngine>chessEngine;
    void OnLoad();
};