#include"framework_base.h"
#include"framework_pages.h"
#include "audio_thread.h"
#include "thread_communicate.h"
#include "settings.h"

using namespace debugger;

Start_Page::StartPageRectangle::StartPageRectangle()
{
    x1[0] = 0, x2[0] = 0;
    brush_index = rand() % 8;
    for (int i = 1; i < rect_num; i++)
    {
        x1[i] = x2[i - 1] + target_rect_interval * (rect_interval_multi_min + (rect_interval_multi_max - rect_interval_multi_min) * 0.01f * (rand() % 101));
        x2[i] = x1[i] + target_rect_length * (rect_length_multi_min + (rect_length_multi_max - rect_length_multi_min) * 0.01f * (rand() % 101));
        opacity[i] = rect_opacity_min + (rect_opacity_max - rect_opacity_min) * 0.01f * (rand() % 101);
        if (x2[i] > 1600)
        {
            break;
        }
    }
}

void Start_Page::StartPageRectangle::rend(float limit_x)
{
    FillRectangle_1(0, 870, limit_x, 900, BrushRand[brush_index]);
    for (int i = 1; i < rect_num; i++)
    {
        if (x1[i] < limit_x)
        {
            if (opacity[i] < 0)
            {
                FillRectangle_1(x1[i], 870, min(x2[i], limit_x), 900, g_pBrushDark, max(0.1f, -opacity[i]));
            }
            else
            {
                FillRectangle_1(x1[i], 870, min(x2[i], limit_x), 900, g_pBrushLight, max(0.1f, opacity[i]));
            }
            if (x2[i] > 1600)
            {
                break;
            }
        }
    }
    return;
}

Start_Page::Start_Page() :PAGE(PAGE_INDEX, PAGE_CREATED_STATUS, Effect::EFF_NONE, 2,"Start Page")
{

}

bool Start_Page::Init()
{
    PAGE::Init();
    return 1;
}

void Start_Page::Update()
{
    if (md5_result < 0)
    {
        g_PageManager.SwitchPageTo(PAGE_VERIFY_FAILED, 0);  //verify failed
        return;
    }

    if (loading_progress_show < loading_progress * 160)
    {
        if (loading_progress_show < 0.1f)
        {
            effect_wait_changing = 0;
        }
        loading_progress_show += frmtm * max(600, (loading_progress * 160 - loading_progress_show) * 2);
    }
    loading_progress = 1 + thread_IO_request_verify_res;

    if (thread_IO_request_verify_res >= 8)
    {
        if (text_rect_y < 800)
        {
            text_rect_y += frmtm * 100;
        }

        if (usernameC == "")
        {
            //set_target_page(PAGE_REGISTER, 0, 0.4f);
            g_PageManager.SwitchPageTo(PAGE_REGISTER); //sign up
        }
        if (thread_IO_request_read_record != 1)
        {
            loading_progress = 10;
        }
        else
        {
            loading_progress = 9;
        }

        text_opacity += frmtm;

        if (clicking && !effect_wait_changing && cpos.x > 0 && cpos.y > 0 && cpos.x < to_screen(1600) && cpos.y < to_screen(900) && loading_progress == 10)
        {
            g_cm.CreateEffect(8);
            g_PageManager.SwitchPageTo(PAGE_HOME, 0.4f);
            //g_am.playEffectSound(SE_arrow_fly1 + rand() % 2);

            effect_wait_changing = 1;
        }
    }

    return;
}

void Start_Page::Rend()
{
    g_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::LightGoldenrodYellow));
    rects.rend(loading_progress_show);

    if (text_opacity < 2)
    {
        DrawTextA_1("Z", g_pTextFormatNormal, D2D1::RectF(to_screen(1190), to_screen(text_rect_y), to_screen(1390), to_screen(text_rect_y + 100)), g_pBrushRed, 2 - text_opacity);
        DrawTextA_1("Z", g_pTextFormatNormal, D2D1::RectF(to_screen(1200), to_screen(text_rect_y + 10), to_screen(1400), to_screen(text_rect_y + 110)), g_pBrushGreen, 2 - text_opacity);
        DrawTextA_1("Z", g_pTextFormatNormal, D2D1::RectF(to_screen(1210), to_screen(text_rect_y + 20), to_screen(1410), to_screen(text_rect_y + 120)), g_pBrushLightBlue, 2 - text_opacity);
        DrawTextA_1("C/C++ & Direct2D Project\n2D Application Framework", g_pTextFormat, D2D1::RectF(to_screen(1000), to_screen(text_rect_y - 160), to_screen(1600), to_screen(text_rect_y - 10)), g_pBrushBlue, 2 - text_opacity);
    }
    else
    {
        DrawTextA_1("Click to Start", g_pTextFormat, 0, 700, 1600, 800, g_pBrushDark, fabs(cosf(text_opacity * 2)));
    }

    return;
}

bool Start_Page::EnterPage()
{
    PAGE::EnterPage();
    text_rect_y = 700, text_opacity = 0;
    loading_progress = 0;
    logo_opacity = 0;
    effect_wait_changing = 0;
    loading_progress = 1 + thread_IO_request_verify_res;
    g_rm.LoadAll();
    return 1;
}

bool Start_Page::ExitPage()
{
    g_rm.ReleaseAll();
    return 1;
}

Home_Page::Home_Page() :PAGE(PAGE_INDEX, PAGE_CREATED_STATUS, Effect::EFF_NONE, 2,"Home Page")
{
    startButton = quitButton = settingButton = PVEButton = PVPOnlineButton = nullptr;
    homepage_bg_type = homepage_text_type = 0;
    bg_transform_x = 0, bg_transform_y = 0, bg_light_opacity = 0.3f;
    text_type = 0;
}

Home_Page::~Home_Page()
{
    
}

bool Home_Page::Init()
{
    PAGE::Init();
    startButton = make_shared<Button>(280, 700, 470, 800, lan[0].start, g_pBrushYellow, g_pBrushBlue, g_pBrushGreen);
    quitButton = make_shared<Button>(900, 670, 1050, 750, ""/*lan[0].quit*/, g_pBrushYellow, g_pBrushBlue, g_pBrushGreen, g_pD2DBimtapUI[1]);
    settingButton = make_shared<Button>(600, 680, 750, 750, "", g_pBrushYellow, g_pBrushBlue, g_pBrushGreen, g_pD2DBimtapUI[0]);
    PVEButton = make_shared<Button>(280, 530, 470, 630, lan[0].PVE, g_pBrushGreen, g_pBrushYellow, g_pBrushBlue);
    return 1;
}

void Home_Page::Update()
{
    PAGE::Update();

    bg_transform_x += frmtm * 20;
    bg_transform_y += frmtm * 20;
    g_pBitmapBrushUI[12]->SetTransform(D2D1::Matrix3x2F::Translation(D2D1::SizeF(to_screen(bg_transform_x), to_screen(bg_transform_y))));
    g_pBitmapBrushUI[13]->SetTransform(D2D1::Matrix3x2F::Translation(D2D1::SizeF(to_screen(bg_transform_x), to_screen(bg_transform_y))));

    if (quitButton->getClicked())
    {
        normal_quit = 1;
    }
    else if (settingButton->getClicked())
    {
        g_cm.CreateEffect(3, cpos.x, cpos.y);
        //set_target_page(PAGE_SETTING, 0, 0.4f);
        g_PageManager.SwitchPageTo(Setting_Page::PAGE_INDEX);
    }
    else if (startButton->getClicked())
    {
        g_PageManager.SwitchPageTo(LocalGame_Page::PAGE_INDEX);
    }
    else if (PVEButton->getClicked())
    {
        g_PageManager.SwitchPageTo(LocalGamePVE_Page::PAGE_INDEX);
    }
    return;
}

void Home_Page::FixedUpdate()
{
    PAGE::FixedUpdate();
    return;
}


void Home_Page::Rend()
{
    g_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateBlue));

    DrawTextA_1(PRODUCT_NAME, g_pTextFormatLarge, D2D1::RectF(to_screen(100), to_screen(100), to_screen(1700), to_screen(650)), g_pBitmapBrushUI[homepage_text_type]);
    return;
}

bool Home_Page::EnterPage()
{
    g_am.PlayBGM(2, 1, 1);
    PAGE::EnterPage();

    g_rm.LoadAll();

    g_cm.AddButton(quitButton);
    g_cm.AddButton(startButton);
    g_cm.AddButton(settingButton);
    g_cm.AddButton(PVEButton);

    bg_transform_x = rand() % 4000;
    bg_transform_y = rand() % 4000;
    if (st.wHour > 8 && st.wHour < 19)
    {
        homepage_bg_type = 12;
        homepage_text_type = 13;
        bg_light_opacity = 0.3f;
    }
    else
    {
        homepage_bg_type = 13;
        homepage_text_type = 12;
        bg_light_opacity = 0.15f;
    }
    return 1;
}

bool Home_Page::ExitPage()
{
    g_rm.ReleaseAll();
    return 1;
}

Setting_Page::Setting_Page() :PAGE(PAGE_INDEX, PAGE_CREATED_STATUS, Effect::EFF_NONE, 2, "Setting Page")
{ 
}

Setting_Page::~Setting_Page()
{
}

bool Setting_Page::Init()
{
    PAGE::Init();

    audio_latency = make_shared<float>(0);

    //todo: use shared_ptr
    returnButton = make_shared<Button>(100, 50, 250, 100, ""/*lan[0].str_return*/, g_pBrushYellow, g_pBrushBlue, g_pBrushGreen, g_pD2DBimtapUI[10]);
    //returnButton = new Button(100, 50, 250, 100, ""/*lan[0].str_return*/, g_pBrushYellow, g_pBrushBlue, g_pBrushGreen, g_pD2DBimtapUI[10]);

    describeLabel = make_shared<LABEL>(100, 790, 1500, 880, "", nullptr, g_pBrushPurple, g_pBrushBlack, nullptr, 0);
    generalButton = make_shared<Button>(420, 130, 580, 240, lan[0].general, nullptr, nullptr, g_pBrushGreen, nullptr);
    graphicsButton = make_shared<Button>(620, 130, 780, 240, lan[0].graphics, nullptr, nullptr, g_pBrushGreen, nullptr);
    audioButton = make_shared<Button>(820, 130, 980, 240, lan[0].audio, nullptr, nullptr, g_pBrushGreen, nullptr);


    languageButton = make_shared<Button>(1250, 300, 1400, 380, lan[0].on, g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);
    demo_mode_timePerStepButton = make_shared<Button>(1250, 400, 1400, 480, "", g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);
    extra_mode_sizeButton = make_shared<Button>(1250, 500, 1400, 580, "", g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);
    extra_mode_timePerStepButton = make_shared<Button>(1250, 600, 1400, 680, "", g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);
    colorLimitButton = make_shared<Button>(1250, 700, 1400, 780, "", g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);

    WindowModeButton = make_shared<Button>(1250, 300, 1400, 380, lan[0].on, g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);
    VsyncButton = make_shared<Button>(1250, 400, 1400, 480, lan[0].on, g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);
    AntialiasingButton = make_shared<Button>(1250, 500, 1400, 580, lan[0].on, g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);
    RenderResolutionButton = make_shared<Button>(1250, 600, 1400, 680, lan[0].on, g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);
    ShowFramerateButton = make_shared<Button>(1250, 700, 1400, 780, lan[0].on, g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);

    MusicVolumeButton = make_shared<Button>(600, 350, 650, 380, "", g_pBrushLightBlue, g_pBrushBlue, g_pBrushBlack, nullptr);
    SEVolumeButton = make_shared<Button>(600, 450, 650, 480, "", g_pBrushLightBlue, g_pBrushBlue, g_pBrushBlack, nullptr);
    ThreadMethodButton = make_shared<Button>(1250, 600, 1400, 680, lan[0].mode+"1", g_pBrushYellow, g_pBrushBlue, g_pBrushBlack, nullptr);
    

    MusicVolumeButton->setUItype(UI_TYPE_BUTTON_STATIC2);
    SEVolumeButton->setUItype(UI_TYPE_BUTTON_STATIC2);
    generalButton->setUItype(UI_TYPE_BUTTON_STATIC2);
    graphicsButton->setUItype(UI_TYPE_BUTTON_STATIC2);
    audioButton->setUItype(UI_TYPE_BUTTON_STATIC2);



    languageLabel = make_shared<LABEL>(100, 300, 900, 380, lan[0].language_translation, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    demo_mode_timePerStepLabel = make_shared<LABEL>(100, 400, 900, 480, lan[0].demo_mode_timePerStep, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    extra_mode_sizeLabel = make_shared<LABEL>(100, 500, 900, 580, lan[0].extra_mode_size, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    extra_mode_timePerStepLabel = make_shared<LABEL>(100, 600, 900, 680, lan[0].extra_mode_timePerStep, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    colorLimitLabel = make_shared<LABEL>(100, 700, 900, 780, lan[0].color_limit, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);

    WindowModeLabel = make_shared<LABEL>(100, 300, 900, 380, lan[0].window_mode, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    VsyncLabel = make_shared<LABEL>(100, 400, 900, 480, lan[0].vsync, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    AntialiasingLabel = make_shared<LABEL>(100, 500, 900, 580, lan[0].MSAA, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    RenderResolutionLabel = make_shared<LABEL>(100, 600, 900, 680, lan[0].resolution, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    ShowFramerateLabel = make_shared<LABEL>(100, 700, 900, 780, lan[0].show_framerate, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);

    MusicVolumeLabel = make_shared<LABEL>(100, 280, 360, 380, lan[0].music_volume + "\n" + to_string(set3[0].music_volume), g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    SEVolumeLabel = make_shared<LABEL>(100, 420, 360, 520, lan[0].se_volume + "\n" + to_string(set3[0].se_volume), g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    ThreadMethodLabel = make_shared<LABEL>(100, 600, 500, 680, lan[0].thread_method, g_pBrushPurple, g_pBrushBlue, g_pBrushBlack, nullptr, 1);
    MusicVolumeBar = make_shared<LABEL>(400, 360, 1300, 370, "", nullptr, g_pBrushPurple, g_pBrushBlack, nullptr, 1);
    SEVolumeBar = make_shared<LABEL>(400, 460, 1300, 470, "", nullptr, g_pBrushPurple, g_pBrushBlack, nullptr, 1);
    return 1;
}

void Setting_Page::Update()
{
    if (chosen_block_move_status < 1)
    {
        chosen_block_move_status += frmtm * 2;
        chosen_block_posx += chosen_block_speed_base * frmtm * PI * cosf(chosen_block_move_status * 0.5f * PI);
        chosen_block_color_r += chosen_block_color_base_r * frmtm * PI * cosf(chosen_block_move_status * 0.5f * PI);
        chosen_block_color_g += chosen_block_color_base_g * frmtm * PI * cosf(chosen_block_move_status * 0.5f * PI);
        chosen_block_color_b += chosen_block_color_base_b * frmtm * PI * cosf(chosen_block_move_status * 0.5f * PI);
    }

    if (setting_page_index == 1)
    {
        DrawBitmap_1(g_pD2DBimtapUI[2], D2D1::RectF(600, 250, 1000, 660), 0.5f);
        if (languageButton->getFocus())
        {
            describeLabel->setText(lan[0].language_translation_description);
        }
        else if (extra_mode_sizeButton->getFocus())
        {
            describeLabel->setText(lan[0].extra_mode_size_description);
        }
        else if (demo_mode_timePerStepButton->getFocus())
        {
            describeLabel->setText(lan[0].demo_mode_timePerStep_description);
        }
        else if (extra_mode_timePerStepButton->getFocus())
        {
            describeLabel->setText(lan[0].extra_mode_timePerStep_description);
        }
        else if (colorLimitButton->getFocus())
        {
            describeLabel->setText(lan[0].color_limit_description);
        }

        if (languageButton->getClicked())
        {
            if (set1[0].language_translation == 0)
            {
                lan[0] = lan[2];
                set1[0].language_translation = 1;
                languageButton->setBrush2(g_pBrushLightGreen);
                languageButton->setText(lan[0].on);
            }
            else
            {
                lan[0] = lan[1];
                set1[0].language_translation = 0;
                languageButton->setBrush2(g_pBrushRed);
                languageButton->setText(lan[0].off);
            }
            thread_IO_request_rend_taglines = 1;
            generalButton->setClicked();
        }

        if (extra_mode_sizeButton->getClicked())
        {
            set1[0].extra_mode_size++;
            if (set1[0].extra_mode_size > 2)
            {
                set1[0].extra_mode_size = 0;
            }
            if (set1[0].extra_mode_size == 1)
            {
                extra_mode_sizeButton->setText(lan[0].mid);
            }
            else if (set1[0].extra_mode_size == 2)
            {
                extra_mode_sizeButton->setText(lan[0].high);
            }
            else
            {
                extra_mode_sizeButton->setText(lan[0].low);
            }
        }
        if (demo_mode_timePerStepButton->getClicked())
        {
            set1[0].demo_mode_timePerStep++;
            if (set1[0].demo_mode_timePerStep > 5)
            {
                set1[0].demo_mode_timePerStep = 0;
            }
            demo_mode_timePerStepButton->setText(to_string(set1[0].demo_mode_timePerStep * 50) + "ms");
        }
        if (extra_mode_timePerStepButton->getClicked())
        {
            set1[0].extra_mode_timePerStep++;
            if (set1[0].extra_mode_timePerStep > 10)
            {
                set1[0].extra_mode_timePerStep = 0;
            }
            extra_mode_timePerStepButton->setText(to_string(set1[0].extra_mode_timePerStep) + "ms");
        }
        if (colorLimitButton->getClicked())
        {
            set1[0].color_limit++;
            if (set1[0].color_limit > 8)
            {
                set1[0].color_limit = 3;
            }
            colorLimitButton->setText(to_string(set1[0].color_limit));
        }
    }
    else if (setting_page_index == 2)
    {
        if (graphic_changed)
        {
            DrawBitmap_1(g_pD2DBimtapUI[4], D2D1::RectF(600, 250, 1000, 660), 0.5f);
        }
        else
        {
            DrawBitmap_1(g_pD2DBimtapUI[3], D2D1::RectF(600, 250, 1000, 660), 0.5f);
        }


        if (WindowModeButton->getFocus())
        {
            describeLabel->setText(lan[0].window_mode_description);
        }
        else if (VsyncButton->getFocus())
        {
            describeLabel->setText(lan[0].vsync_description);
        }
        else if (AntialiasingButton->getFocus())
        {
            describeLabel->setText(lan[0].MSAA_description + "\n" + lan[0].sampleCount + to_string(sampleCountOut) + "x");
        }
        else if (RenderResolutionButton->getFocus())
        {
            describeLabel->setText(lan[0].resolution_description);
        }
        else if (ShowFramerateButton->getFocus())
        {
            describeLabel->setText(lan[0].show_framerate_description);
        }

        if (WindowModeButton->getClicked())
        {
            if (set2[0].window_mode == 0)
            {
                set2[0].window_mode = 1;
                g_pSwapChain->SetFullscreenState(0, NULL);
                WindowModeButton->setText(lan[0].windowed);
            }
            else
            {
                set2[0].window_mode = 0;
                g_pSwapChain->SetFullscreenState(1, NULL);
                WindowModeButton->setText(lan[0].fullscreen);
            }
        }
        if (VsyncButton->getClicked())
        {
            if (set2[0].vsync == 0)
            {
                set2[0].vsync = 1;
                VsyncButton->setBrush2(g_pBrushLightGreen);
                VsyncButton->setText(lan[0].on);
            }
            else
            {
                set2[0].vsync = 0;
                VsyncButton->setBrush2(g_pBrushRed);
                VsyncButton->setText(lan[0].off);
            }
        }
        if (AntialiasingButton->getClicked())
        {
            graphic_changed = 1;
            if (set2[0].MSAA == 0)
            {
                set2[0].MSAA = 1;
                AntialiasingButton->setBrush2(g_pBrushLightGreen);
                AntialiasingButton->setText(lan[0].low);
                g_pD2DDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
                g_pD2DDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
            }
            else if (set2[0].MSAA == 1)
            {
                set2[0].MSAA = 2;
                AntialiasingButton->setBrush2(g_pBrushLightGreen);
                AntialiasingButton->setText(lan[0].high);
                g_pD2DDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
                g_pD2DDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
            }
            else
            {
                set2[0].MSAA = 0;
                AntialiasingButton->setBrush2(g_pBrushRed);
                AntialiasingButton->setText(lan[0].off);
                g_pD2DDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
                g_pD2DDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
            }
        }
        if (RenderResolutionButton->getClicked())
        {
            graphic_changed = 1;
            set2[0].resolution++;
            if (set2[0].resolution > 7)
            {
                set2[0].resolution = 1;
            }
            switch (set2[0].resolution)
            {
            case 1:
                RenderResolutionButton->setText("450P");
                break;
            case 2:
                RenderResolutionButton->setText("720P");
                break;
            case 3:
                RenderResolutionButton->setText("900P");
                break;
            case 4:
                RenderResolutionButton->setText("1080P");
                break;
            case 5:
                RenderResolutionButton->setText("2K");
                break;
            case 6:
                RenderResolutionButton->setText("4K");
                break;
            case 7:
                RenderResolutionButton->setText("8K");
                break;
            default:
                break;
            }
            RebuildRenderContext();
        }
        if (ShowFramerateButton->getClicked())
        {
            if (set2[0].show_framerate == 0)
            {
                set2[0].show_framerate = 1;
                ShowFramerateButton->setBrush2(g_pBrushLightGreen);
                ShowFramerateButton->setText(lan[0].on);
            }
            else
            {
                set2[0].show_framerate = 0;
                ShowFramerateButton->setBrush2(g_pBrushRed);
                ShowFramerateButton->setText(lan[0].off);
            }
        }
    }
    else if (setting_page_index == 3)
    {
        if (set3[0].thread_method == 2)
        {
            DrawBitmap_1(g_pD2DBimtapUI[6], D2D1::RectF(600, 250, 1000, 660), 0.5f);
        }
        else
        {
            DrawBitmap_1(g_pD2DBimtapUI[5], D2D1::RectF(600, 250, 1000, 660), 0.5f);
        }


        if (ThreadMethodButton->getFocus())
        {
            describeLabel->setText(lan[0].thread_method_description + to_string(int(*audio_latency * 1000)) + to_string(*audio_latency * 1000 - int(*audio_latency * 1000)).substr(1, 3) + "ms");
        }

        if (ThreadMethodButton->getClicked())
        {
            if (set3[0].thread_method == 0)
            {
                set3[0].thread_method = 1;
                ThreadMethodButton->setText(lan[0].mode + "2");
            }
            else if (set3[0].thread_method == 1)
            {
                set3[0].thread_method = 2;
                ThreadMethodButton->setText(lan[0].mode + "3");
            }
            else if (set3[0].thread_method == 2)
            {
                set3[0].thread_method = 0;
                ThreadMethodButton->setText(lan[0].mode + "1");
            }
            g_am.GetAudioLatency(audio_latency);
        }
        if (MusicVolumeButton->getClickStatus() && button_drag == 0 || button_drag == 1)
        {
            if (MusicVolumeButton->getClickStatus())
            {
                button_drag = 1;
            }
            float x1, y1, x2, y2;
            MusicVolumeButton->getBound(&x1, &y1, &x2, &y2);
            x1 = cpos.x - 25;
            if (x1 < 375)
            {
                x1 = 375;
            }
            else if (x1 > 1275)
            {
                x1 = 1275;
            }

            x2 = x1 + 50;
            MusicVolumeButton->setBound(x1, y1, x2, y2);
            set3[1].music_volume = (cpos.x - 400) / 9;
            if (set3[1].music_volume < 0)
            {
                set3[1].music_volume = 0;
            }
            else if (set3[1].music_volume > 100)
            {
                set3[1].music_volume = 100;
            }

            MusicVolumeLabel->setText(lan[0].music_volume + "\n" + to_string(set3[1].music_volume));

            if (!clicking)
            {
                button_drag = 0;

                g_am.setMusicVolume(set3[1].music_volume);
            }
        }
        if (SEVolumeButton->getClickStatus() && button_drag == 0 || button_drag == 2)
        {
            if (SEVolumeButton->getClickStatus())
            {
                button_drag = 2;
            }
            float x1, y1, x2, y2;
            SEVolumeButton->getBound(&x1, &y1, &x2, &y2);
            x1 = cpos.x - 25;

            if (x1 < 375)
            {
                x1 = 375;
            }
            else if (x1 > 1275)
            {
                x1 = 1275;
            }
            x2 = x1 + 50;
            SEVolumeButton->setBound(x1, y1, x2, y2);
            set3[1].se_volume = (cpos.x - 400) / 9;
            if (set3[1].se_volume < 0)
            {
                set3[1].se_volume = 0;
            }
            else if (set3[1].se_volume > 100)
            {
                set3[1].se_volume = 100;
            }
            SEVolumeLabel->setText(lan[0].se_volume + "\n" + to_string(set3[1].se_volume));
            if (!clicking)
            {
                button_drag = 0;
                g_am.setSEVolume(set3[1].se_volume);
            }
        }

    }

    if (returnButton->getClicked())
    {
        g_cm.CreateEffect(9);
        g_PageManager.SwitchPageTo(Home_Page::PAGE_INDEX, 0);
    }
    else if (generalButton->getClicked())
    {
        g_cm.CreateEffect(9);
        setting_page_index = 1;
        ResetPageComponents();
        MoveBlock(420, 1, 0.5f, 0);

        generalButton->setBrush1(g_pBrushRed);

        languageButton->activate();
        extra_mode_sizeButton->activate();
        demo_mode_timePerStepButton->activate();
        extra_mode_timePerStepButton->activate();
        colorLimitButton->activate();

        languageLabel->setShow(1);
        extra_mode_sizeLabel->setShow(1);
        demo_mode_timePerStepLabel->setShow(1);
        extra_mode_timePerStepLabel->setShow(1);
        colorLimitLabel->setShow(1);

        if (set1[0].language_translation == 0)
        {
            languageButton->setBrush2(g_pBrushRed);
            languageButton->setText(lan[0].off);
        }
        else
        {
            languageButton->setBrush2(g_pBrushLightGreen);
            languageButton->setText(lan[0].on);
        }
        if (set1[0].extra_mode_size == 1)
        {
            extra_mode_sizeButton->setText(lan[0].mid);
        }
        else if (set1[0].extra_mode_size == 2)
        {
            extra_mode_sizeButton->setText(lan[0].high);
        }
        else
        {
            extra_mode_sizeButton->setText(lan[0].low);
        }
        demo_mode_timePerStepButton->setText(to_string(set1[0].demo_mode_timePerStep * 50) + "ms");
        extra_mode_timePerStepButton->setText(to_string(set1[0].extra_mode_timePerStep) + "ms");
        colorLimitButton->setText(to_string(set1[0].color_limit));
        describeLabel->setText(VERSION_SHORT);
    }
    else if (graphicsButton->getClicked())
    {
        g_cm.CreateEffect(9);
        setting_page_index = 2;
        ResetPageComponents();
        MoveBlock(620, 0, 1, 0.5f);

        describeLabel->setText("Supported by DirectX 11");
        graphicsButton->setBrush1(g_pBrushRed);

        WindowModeButton->activate();
        VsyncButton->activate();
        AntialiasingButton->activate();
        RenderResolutionButton->activate();
        ShowFramerateButton->activate();

        WindowModeLabel->setShow(1);
        VsyncLabel->setShow(1);
        AntialiasingLabel->setShow(1);
        RenderResolutionLabel->setShow(1);
        ShowFramerateLabel->setShow(1);

        if (set2[0].window_mode == 0)
        {
            WindowModeButton->setText(lan[0].fullscreen);
        }
        else
        {
            WindowModeButton->setText(lan[0].windowed);
        }
        if (set2[0].vsync == 0)
        {
            VsyncButton->setBrush2(g_pBrushRed);
            VsyncButton->setText(lan[0].off);
        }
        else
        {
            VsyncButton->setBrush2(g_pBrushLightGreen);
            VsyncButton->setText(lan[0].on);
        }
        if (set2[0].MSAA == 0)
        {
            AntialiasingButton->setBrush2(g_pBrushRed);
            AntialiasingButton->setText(lan[0].off);
        }
        else if (set2[0].MSAA == 1)
        {
            AntialiasingButton->setBrush2(g_pBrushLightGreen);
            AntialiasingButton->setText(lan[0].low);
        }
        else
        {
            AntialiasingButton->setBrush2(g_pBrushLightGreen);
            AntialiasingButton->setText(lan[0].high);
        }
        switch (set2[0].resolution)
        {
        case 1:
            RenderResolutionButton->setText("450P");
            break;
        case 2:
            RenderResolutionButton->setText("720P");
            break;
        case 3:
            RenderResolutionButton->setText("900P");
            break;
        case 4:
            RenderResolutionButton->setText("1080P");
            break;
        case 5:
            RenderResolutionButton->setText("2K");
            break;
        case 6:
            RenderResolutionButton->setText("4K");
            break;
        case 7:
            RenderResolutionButton->setText("8K");
            break;
        default:
            RenderResolutionButton->setText("~900P");
            break;
        }

        if (set2[0].show_framerate == 0)
        {
            ShowFramerateButton->setBrush2(g_pBrushRed);
            ShowFramerateButton->setText(lan[0].off);
        }
        else
        {
            ShowFramerateButton->setBrush2(g_pBrushLightGreen);
            ShowFramerateButton->setText(lan[0].on);
        }
    }
    else if (audioButton->getClicked())
    {
        g_cm.CreateEffect(9);
        setting_page_index = 3;
        ResetPageComponents();
        MoveBlock(820, 0.5f, 0, 1);

        describeLabel->setText("Supported by OpenAL");
        audioButton->setBrush1(g_pBrushRed);

        MusicVolumeButton->activate();
        SEVolumeButton->activate();
        ThreadMethodButton->activate();

        MusicVolumeBar->setShow(1);
        SEVolumeBar->setShow(1);
        MusicVolumeLabel->setShow(1);
        SEVolumeLabel->setShow(1);
        ThreadMethodLabel->setShow(1);

        float x1 = 0, y1 = 0, x2 = 0, y2 = 0;
        MusicVolumeButton->getBound(&x1, &y1, &x2, &y2);
        MusicVolumeButton->setBound(400 + 9.0f * set3[0].music_volume - 25, y1, 400 + 9.0f * set3[0].music_volume + 25, y2);

        SEVolumeButton->getBound(&x1, &y1, &x2, &y2);
        SEVolumeButton->setBound(400 + 9.0f * set3[0].se_volume - 25, y1, 400 + 9.0f * set3[0].se_volume + 25, y2);

        if (set3[0].thread_method == 0)
        {
            ThreadMethodButton->setText(lan[0].mode + "1");
        }
        else if (set3[0].thread_method == 1)
        {
            ThreadMethodButton->setText(lan[0].mode + "2");
        }
        else
        {
            set3[0].thread_method = 2;
            ThreadMethodButton->setText(lan[0].mode + "3");
        }
        g_am.GetAudioLatency(audio_latency);
    }

    return;
}

void Setting_Page::FixedUpdate()
{
}

void Setting_Page::Rend()
{
    g_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::LightBlue));
    
    DrawRoundedRectangle_1(420, 130, 980, 240, 2.5f, g_pBrushPink, 1.0f);
    if (chosen_block_move_status < 0.999f)
    {
        g_pBrushBGSelect->SetColor(D2D1::ColorF(chosen_block_color_r, chosen_block_color_g, chosen_block_color_b));
    }
    FillRoundedRectangle_1(chosen_block_posx, 130, chosen_block_posx + 160, 240, g_pBrushBGSelect, 0.4f);
    FillRoundedRectangle_1(chosen_block_posx + 20, 140, chosen_block_posx + 140, 230, g_pBrushLight, 0.3f);

    rend_circles_BG(0);

    if (setting_page_index == 1)
    {
        DrawBitmap_1(g_pD2DBimtapUI[2], D2D1::RectF(600, 250, 1000, 660), 0.5f);
    }
    else if (setting_page_index == 2)
    {
        if (graphic_changed)
        {
            DrawBitmap_1(g_pD2DBimtapUI[4], D2D1::RectF(600, 250, 1000, 660), 0.5f);
        }
        else
        {
            DrawBitmap_1(g_pD2DBimtapUI[3], D2D1::RectF(600, 250, 1000, 660), 0.5f);
        }
    }
    else if (setting_page_index == 3)
    {
        if (set3[0].thread_method)
        {
            DrawBitmap_1(g_pD2DBimtapUI[6], D2D1::RectF(600, 250, 1000, 660), 0.5f);
        }
        else
        {
            DrawBitmap_1(g_pD2DBimtapUI[5], D2D1::RectF(600, 250, 1000, 660), 0.5f);
        }
    }
    return;
}

bool Setting_Page::EnterPage()
{
    PAGE::EnterPage();

    g_rm.LoadAll();

    g_cm.AddButton(returnButton);
    g_cm.AddButton(generalButton);
    g_cm.AddButton(graphicsButton);
    g_cm.AddButton(audioButton);

    g_cm.AddButton(languageButton);
    g_cm.AddButton(demo_mode_timePerStepButton);
    g_cm.AddButton(extra_mode_sizeButton);
    g_cm.AddButton(extra_mode_timePerStepButton);
    g_cm.AddButton(colorLimitButton);

    g_cm.AddButton(WindowModeButton);
    g_cm.AddButton(VsyncButton);
    g_cm.AddButton(AntialiasingButton);
    g_cm.AddButton(RenderResolutionButton);
    g_cm.AddButton(ShowFramerateButton);

    g_cm.AddButton(MusicVolumeButton);
    g_cm.AddButton(SEVolumeButton);
    g_cm.AddButton(ThreadMethodButton);

    g_cm.AddLabel(describeLabel);

    g_cm.AddLabel(languageLabel);
    g_cm.AddLabel(demo_mode_timePerStepLabel);
    g_cm.AddLabel(extra_mode_sizeLabel);
    g_cm.AddLabel(extra_mode_timePerStepLabel);
    g_cm.AddLabel(colorLimitLabel);

    g_cm.AddLabel(WindowModeLabel);
    g_cm.AddLabel(VsyncLabel);
    g_cm.AddLabel(AntialiasingLabel);
    g_cm.AddLabel(RenderResolutionLabel);
    g_cm.AddLabel(ShowFramerateLabel);

    g_cm.AddLabel(MusicVolumeLabel);
    g_cm.AddLabel(SEVolumeLabel);
    g_cm.AddLabel(ThreadMethodLabel);
    g_cm.AddLabel(MusicVolumeBar);
    g_cm.AddLabel(SEVolumeBar);

    if (setting_page_index == 1)
    {
        generalButton->setClicked();
    }
    else if (setting_page_index == 2)
    {
        graphicsButton->setClicked();
    }
    else if (setting_page_index == 3)
    {
        audioButton->setClicked();
    }

    rend_circles_BG(1);
    return 1;
}

bool Setting_Page::ExitPage()
{
    g_rm.ReleaseAll();
    return 1;
}

void Setting_Page::ResetPageComponents()
{
    g_cm.hideAll();

    returnButton->activate();
    describeLabel->setShow(1);

    generalButton->activate();
    generalButton->setBrush1(NULL);
    graphicsButton->activate();
    graphicsButton->setBrush1(NULL);
    audioButton->activate();
    audioButton->setBrush1(NULL);
    return;
}

void Setting_Page::MoveBlock(float target_posx, float target_color_r, float target_color_g, float target_color_b)
{
    chosen_block_target_posx = target_posx;
    chosen_block_target_color_r = target_color_r;
    chosen_block_target_color_g = target_color_g;
    chosen_block_target_color_b = target_color_b;

    chosen_block_speed_base = chosen_block_target_posx - chosen_block_posx;
    chosen_block_color_base_r = chosen_block_target_color_r - chosen_block_color_r, chosen_block_color_base_g = chosen_block_target_color_g - chosen_block_color_g, chosen_block_color_base_b = chosen_block_target_color_b - chosen_block_color_b;
    chosen_block_move_status = 0;
    return;
}

void Setting_Page::rend_circles_BG(int refresh)
{
    static bool circles_pos_init = 1;
    static float circles_posx[32], circles_posy[32], circles_speedy_acc_time = 0, circle_range = 70, circle_opacity = 0.4f, circle_zoom_time = 0, circle_zoom_speed_base = 0;;
    int rand_temp1 = 0, rand_temp2 = 0;
    if (refresh == 1)
    {
        circles_speedy_acc_time = 2;
        return;
    }
    else if (refresh == 2)
    {
        g_pBrushBGSelect->SetColor(D2D1::ColorF(1, 1, 1));
    }
    if (circles_speedy_acc_time > 0)
    {
        circles_speedy_acc_time -= frmtm;
    }
    if (circles_pos_init)
    {
        rand_temp1 = rand() % 250;
        rand_temp2 = rand() % 150;
        circles_pos_init = 0;
        for (int i = 0; i < 32; i++)
        {
            circles_posx[i] = i / 4 * 250 + rand_temp1;
            if (i / 4 % 2)
            {
                circles_posy[i] = i % 4 * 300 + rand_temp2;
            }
            else
            {
                circles_posy[i] = i % 4 * 300 + 150 + rand_temp2;
            }
        }
    }
    if (clicking)
    {
        circle_zoom_time = 0.5f;
        circle_zoom_speed_base = (float)(circle_range - 40) / 60;
    }
    if (circle_zoom_time > 0.01f)
    {
        circle_range -= circle_speed * circle_zoom_speed_base * frmtm * sinf(circle_zoom_time * PI);

        circle_zoom_time -= 2 * frmtm;
        if (circle_zoom_time < 0.01f)
        {
            circle_zoom_time = -0.5f;
            circle_zoom_speed_base = (float)(120 - circle_range) / 60;
        }
    }
    else if (circle_zoom_time < -0.01f)
    {
        //circle_zoom_speed_base = (float)(140 - circle_range) / 60;
        circle_range -= circle_speed * circle_zoom_speed_base * frmtm * sinf(circle_zoom_time * PI);
        circle_zoom_time += 2 * frmtm;
        if (circle_zoom_time > -0.01f)
        {
            circle_zoom_time = 0;
        }
    }
    for (int i = 0; i < 32; i++)
    {
        if (refresh <= 2)
        {
            circles_posx[i] -= frmtm * circle_speed;
            if (circles_posx[i] < -200)
            {
                circles_posx[i] = 1800;
            }
            if (circles_speedy_acc_time > 0)
            {
                circles_posy[i] -= frmtm * sinf(circles_speedy_acc_time * 0.25 * PI) * circle_speed * 10;
            }
        }
        if (refresh == 3)
        {
            circles_posy[i] -= frmtm * circle_speed * 0.5f;
            if (circles_posy[i] < -150)
            {
                circles_posy[i] = 1050;
            }
        }
        else
        {
            circles_posy[i] -= frmtm * circle_speed;
            if (circles_posy[i] < -150)
            {
                circles_posy[i] = 1050;
            }
        }
        circle_opacity = 0.1f + 0.1f * sinf((70 - circle_range) / 70 * PI);
        if (refresh <= 2)
        {
            FillEllipse_1(circles_posx[i], circles_posy[i], circle_range, circle_range, g_pBrushBGSelect, circle_opacity);
        }
        else
        {
            g_pBrushBGSelect->SetColor(D2D1::ColorF(D2D1::ColorF::LightSeaGreen));
            FillEllipse_1(circles_posx[i], circles_posy[i], circle_range, circle_range, g_pBrushBGSelect, circle_opacity * 0.5f);
        }
    }
    return;
}


New_User_Page::New_User_Page() :PAGE(PAGE_INDEX, PAGE_CREATED_STATUS, Effect::EFF_NONE, 0, "Register Page")
{

}

New_User_Page::~New_User_Page()
{

}

bool New_User_Page::Init()
{
    PAGE::Init();
    okButton = make_shared<Button>(730, 650, 870, 720, "OK", g_pBrushBlack, g_pBrushYellow, g_pBrushLightGreen);
    inputLabel = make_shared<LABEL>(600, 450, 1000, 550, "Please wait", g_pBrushPink, nullptr, g_pBrushBlack, nullptr, 1);
    return 1;
}

void New_User_Page::Update()
{
    enter_notice_time -= frmtm;

    if (enter_notice_time > 0)
    {
        inputLabel->setText("Please wait_");
    }
    else
    {
        inputLabel->setText("Please wait ");
    }
    if (enter_notice_time < -0.5)
    {
        enter_notice_time = 0.5;
    }

    if (page_status == 3)
    {
        Update2();
    }
    else
    {
        if (okButton->getClicked())
        {
            g_am.playEffectSound(9);
            okButton->setText("loading...");
            okButton->setBrush3(g_pBrushYellow);
            usernameC = input_username;
            thread_IO_request_userinit = 1;
            page_status = 3;

        }
    }
    return;
}

void New_User_Page::Update2()
{
    if (thread_IO_request_userinit == 0)
    {
        okButton->setText("Done");
        okButton->setBrush3(g_pBrushLightGreen);
        self_restart();
        thread_IO_request_userinit = -3;
    }
    else if (thread_IO_request_userinit == -1)
    {
        okButton->setText("Error");
        okButton->setBrush3(g_pBrushRed);

    }
    else if (thread_IO_request_userinit == -2)
    {
        thread_IO_request_userinit = -3;
    }
    return;
}

void New_User_Page::FixedUpdate()
{

}

void New_User_Page::Rend()
{
    g_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::Snow));
    DrawTextA_1(
        "Setup",
        g_pTextFormatNormal,
        D2D1::RectF(to_screen(200), to_screen(150), to_screen(600), to_screen(300)),
        g_pBrushBlack
    );

    return;
}
bool New_User_Page::EnterPage()
{
    PAGE::EnterPage();

    g_rm.LoadAll();

    g_cm.AddButton(okButton);
    g_cm.AddLabel(inputLabel);
    okButton->setClicked();
    g_am.playEffectSound(10);
    return 1;
}


bool New_User_Page::ExitPage()
{
    g_rm.ReleaseAll();
    return 1;
}

VERIFY_FAILED_PAGE::VERIFY_FAILED_PAGE():PAGE(PAGE_INDEX,PAGE_CREATED_STATUS, Effect::EFF_NONE, 0, "VERIFY FAILED PAGE")
{
}

void VERIFY_FAILED_PAGE::Rend()
{
    g_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::LightSlateGray));
    g_pD2DDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2(to_screen(800), to_screen(350)), to_screen(200), to_screen(200)), g_pBrushRed);
    g_pD2DDeviceContext->FillRoundedRectangle(
        D2D1::RoundedRect(
            D2D1::RectF(to_screen(780), to_screen(200), to_screen(820),
                to_screen(430)),
            to_screen(10.0f),
            to_screen(10.0f)),
        g_pBrushYellow
    );
    g_pD2DDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2(to_screen(800), to_screen(470)), to_screen(20), to_screen(20)), g_pBrushYellow);
    DrawTextA_1(
        "Oops, resource file corrupted.\nPlease reinstall the application.\nYou may keep the \"save\" folder to keep your progress.",           // Text to render
        g_pTextFormatNormal,     // Text format
        D2D1::RectF(to_screen(300), to_screen(650), to_screen(1300),
            to_screen(850)),
        // The region of the window where the text will be rendered
        g_pBrushBlack      // The brush used to draw the text
    );
    if (clicking)
    {
        normal_quit = 1;
    }
    return;
}

bool VERIFY_FAILED_PAGE::ExitPage()
{
    return 1;
}
