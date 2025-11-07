#include"CChessBase.h"
#include"framework_base.h"
#include"cchess/CChessUI.h"
#include <future>

class GameHistory_Page :public PAGE
{
public:
	constexpr static int PAGE_INDEX = PAGE_HISTORY;
	GameHistory_Page();
	~GameHistory_Page();

	bool Init() override;
	void Update() override;
	void Rend() override;
	bool EnterPage() override;
	bool ExitPage() override;

private:
	void CreateAndAddBrush();
	shared_ptr<Button> backButton;
	std::future<void> task;
	unique_ptr<CChessUI> CUI;
	shared_ptr<CChessEngine>chessEngine;
};