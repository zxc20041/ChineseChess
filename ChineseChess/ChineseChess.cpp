// ChineseChess.cpp : 定义应用程序的入口点。
//

#include "ChineseChess.h"
#include"settings.h"
#include"audio_thread.h"

using namespace debugger;

LocalGame_Page::LocalGame_Page() :PAGE(PAGE_INDEX, PAGE_CREATED_STATUS, Effect::WHITE_CIRCLE_SWITCH, 2, "LocalGame")
{


}

LocalGame_Page::~LocalGame_Page()
{
}

bool LocalGame_Page::Init()
{
	PAGE::Init();

	returnButton = make_shared<Button>(100, 50, 250, 100, ""/*lan[0].str_return*/, g_pBrushYellow, g_pBrushBlue, g_pBrushGreen, g_pD2DBimtapUI[10]);
	switchSideButton = make_shared<Button>(80, 400, 170, 500, "Black", g_pBrushYellow, g_pBrushBlue, g_pBrushGreen);//todo: use icon
	return 1;
}

void LocalGame_Page::Update()
{

	PAGE::Update();
	if (Page_status < PAGE_PREPARED_STATUS)
	{
		if (!g_rm.GetFinishLoading())
		{
			//debugger_main.writelog(DDEBUG, "in LocalGame_Page::Update() waiting FinishLoading", __LINE__);
			if (returnButton->getClicked())
			{
				g_cm.CreateEffect(9);
				g_PageManager.SwitchPageTo(HOME_PAGE_INDEX);
			}
			return;
		}

		OnLoad();
		Page_status = PAGE_PREPARED_STATUS;

	}
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

void LocalGame_Page::Rend()
{
	g_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::LightGoldenrodYellow));
	PAGE::Rend();
	if (Page_status < PAGE_PREPARED_STATUS)
	{
		return;
	}
	CUI.Rend();

	return;
}

bool LocalGame_Page::EnterPage()
{
	g_am.PlayBGM(BGM_INGAME, 1, 1);
	PAGE::EnterPage();

	TEXTURE_DESC brush_only{ 0,0,0,0,1 };
	g_rm.AddResource("bg board", ".\\game\\pic\\board.jpg", ResourceManager::ResourceType::Resource_Texture, "DE601E07F5C7D8B8DDD81F521A458510", brush_only);

	g_rm.AddResource("bg river", ".\\game\\pic\\bg_river.png", ResourceManager::ResourceType::Resource_Texture, "pass");
	g_rm.LoadAll();

	g_cm.AddButton(returnButton);
	g_cm.AddButton(switchSideButton);


	return 1;
}

bool LocalGame_Page::ExitPage()
{
	g_rm.ReleaseAll();
	return 1;
}

void LocalGame_Page::OnLoad()
{
	//resolve dat

	return;
}
