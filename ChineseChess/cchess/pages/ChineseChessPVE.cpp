#include"ChineseChessPVE.h"
#include"settings.h"
#include"audio_thread.h"

using namespace debugger;

LocalGamePVE_Page::LocalGamePVE_Page() :PAGE(PAGE_INDEX, PAGE_CREATED_STATUS, Effect::WHITE_CIRCLE_SWITCH, 2, "LocalGamePVE")
{
}

LocalGamePVE_Page::~LocalGamePVE_Page()
{
}

bool LocalGamePVE_Page::Init()
{
	PAGE::Init();

	returnButton = make_shared<Button>(100, 50, 250, 100, ""/*lan[0].str_return*/, g_pBrushYellow, g_pBrushBlue, g_pBrushGreen, g_pD2DBimtapUI[10]);
	switchSideButton = make_shared<Button>(80, 400, 170, 500, "Black", g_pBrushYellow, g_pBrushBlue, g_pBrushGreen);//todo: use icon
	return 1;
}

void LocalGamePVE_Page::Update()
{
	PAGE::Update();
	if (Page_status < PAGE_PREPARED_STATUS)
	{
		auto status = task.wait_for(0ms);
		if (status != std::future_status::ready)
		{
			//debugger_main.writelog(DDEBUG, "in LocalGame_Page::Update() waiting FinishLoading", __LINE__);
			if (returnButton->getClicked())
			{
				task.wait();
				g_cm.CreateEffect(9);
				g_PageManager.SwitchPageTo(HOME_PAGE_INDEX);
			}
			return;
		}
		Page_status = PAGE_PREPARED_STATUS;

	}

	CUI->Update();
	if (returnButton->getClicked())
	{
		g_cm.CreateEffect(Effect::WHITE_SWITCH, cpos.x, cpos.y);
		g_PageManager.SwitchPageTo(HOME_PAGE_INDEX);
	}
	else if (switchSideButton->getClicked())
	{

	}
	return;
}

void LocalGamePVE_Page::Rend()
{
	g_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::LightGoldenrodYellow));
	PAGE::Rend();
	if (Page_status < PAGE_PREPARED_STATUS)
	{
		return;
	}
	CUI->Rend();

	return;
}

bool LocalGamePVE_Page::EnterPage()
{
	g_am.PlayBGM(BGM_INGAME, 1, 1);
	PAGE::EnterPage();
	//create async task
	task = std::async(std::launch::async, [&]
		{
			chessEngine = make_shared<CChessLocalPVE>();
			chessEngine->Reset();
			CUI = make_unique<CChessUIPVE>();
			CUI->SetEngine(chessEngine);

			while (!g_rm.GetFinishLoading())
			{
				Sleep(1);
			}
			//resolve atlas data
			if (!CUI->LoadPiecesAtlasInfo())
			{
				g_PageManager.SwitchPageTo(HOME_PAGE_INDEX);
				g_am.playEffectSound(8);
			}
			CUI->Reset();
			return;
		});
	

	g_rm.AddResource("bg board", ".\\game\\pic\\board.jpg", "DE601E07F5C7D8B8DDD81F521A458510", RESOURCE_INFO::BRUSH_ONLY);

	g_rm.AddResource("bg river", ".\\game\\pic\\bg_river.png", "pass", RESOURCE_INFO::DEFAULT_Bitmap);

	g_rm.AddResource("pieces", ".\\game\\pic\\pieces.png", "pass", RESOURCE_INFO::DEFAULT_Bitmap);
	g_rm.AddResource("pieces atlas info", ".\\game\\pic\\pieces.dat", "pass", RESOURCE_INFO::DEFAULT_TEXT);
	g_rm.LoadAll();

	g_cm.AddButton(returnButton);
	g_cm.AddButton(switchSideButton);


	return 1;
}

bool LocalGamePVE_Page::ExitPage()
{
	chessEngine = nullptr;
	CUI->SetEngine(nullptr);
	CUI = nullptr;
	g_rm.ReleaseAll();
	return 1;
}

