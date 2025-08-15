#pragma once
#include"framework_base.h"
#include"cchess/CChessUI.h"
#include"cchess/localPVE.h"
#include <future>
class LocalGamePVE_Page :public PAGE
{
public:
    constexpr static int PAGE_INDEX = PAGE_GAME_PVE;

    LocalGamePVE_Page();
    ~LocalGamePVE_Page();
    bool Init() override;
    void Update() override;
    void Rend() override;
    bool EnterPage() override;
    bool ExitPage() override;


private:
    shared_ptr<Button> switchSideButton, returnButton;
    std::future<void> task;
    CChessUI CUI;
    shared_ptr<CChessEngine>chessEngine;
};