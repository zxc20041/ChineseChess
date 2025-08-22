#pragma once
#include"framework_base.h"
#include"cchess\pages\ChineseChess.h"
#include"cchess\pages\ChineseChessPVE.h"


class Start_Page :public PAGE
{
public:
    constexpr static int PAGE_INDEX = PAGE_INTRO;
    Start_Page();

    bool Init() override;
    void Update() override;
    void Rend() override;
    bool EnterPage() override;
    bool ExitPage() override;



private:
    float text_rect_y = 700, text_opacity = 0, logo_opacity = 0, loading_progress_show = 0;
    int start_page_stage = 0, loading_progress = 0, effect_wait_changing = 0;
    
    class StartPageRectangle
    {
        static constexpr int rect_num = 36;
        static constexpr float target_rect_length = 30;
        static constexpr float target_rect_interval = 20;
        static constexpr float rect_opacity_min = -0.25f;
        static constexpr float rect_opacity_max = 0.25f;
        static constexpr float rect_length_multi_min = 0.25f;
        static constexpr float rect_length_multi_max = 3;
        static constexpr float rect_interval_multi_min = 0.75f;
        static constexpr float rect_interval_multi_max = 1.5f;
    private:
        float x1[rect_num], x2[rect_num], opacity[rect_num];
        int brush_index;
    public:
        StartPageRectangle();
        void rend(float limit_x);
    };
    StartPageRectangle rects;
};

class Home_Page :public PAGE
{
public:
    Home_Page();
    ~Home_Page();
    bool Init() override;
    void Update() override;
    void FixedUpdate() override;
    // 通过 PAGE 继承

    void Rend() override;
    bool EnterPage();
    bool ExitPage() override;
    constexpr static int PAGE_INDEX = PAGE_HOME;
private:
    shared_ptr<Button> startButton, quitButton, settingButton, PVEButton, PVPOnlineButton;
    int homepage_bg_type, homepage_text_type;
    float bg_transform_x, bg_transform_y, bg_light_opacity;
    bool text_type;
};


class Setting_Page :public PAGE
{
public:
    Setting_Page();
    ~Setting_Page();
    bool Init() override;
    void Update() override;
    void FixedUpdate() override;
    // 通过 PAGE 继承

    void Rend() override;
    bool EnterPage() override;
    bool ExitPage() override;
    constexpr static int PAGE_INDEX = PAGE_SETTING;
private:
    int setting_page_index = 1;
    string setting_descreption = "";
    int button_drag = 0;
    bool graphic_changed = 0;

    float deviation = 0;
    float chosen_block_posx = 420;
    float chosen_block_target_posx = 420;
    float chosen_block_speed_base = 0;
    float chosen_block_move_status = 1;  //> 1 for static, 0 for start moving, 0-1 for moving
    float chosen_block_color_r = 1, chosen_block_color_g = 0.5f, chosen_block_color_b = 0;
    float chosen_block_target_color_r = 0, chosen_block_target_color_g = 0, chosen_block_target_color_b = 0;
    float chosen_block_color_base_r = 0, chosen_block_color_base_g = 0, chosen_block_color_base_b = 0;

    shared_ptr<float> audio_latency;

    shared_ptr<Button> returnButton,  generalButton,  graphicsButton,  audioButton;
    shared_ptr<Button> languageButton,  extra_mode_sizeButton,  demo_mode_timePerStepButton,  extra_mode_timePerStepButton,  colorLimitButton; //gameplay
    shared_ptr<Button> WindowModeButton,  VsyncButton,  AntialiasingButton,  RenderResolutionButton,  ShowFramerateButton;  //graphics
    shared_ptr<Button> MusicVolumeButton,  SEVolumeButton,  ThreadMethodButton;
    shared_ptr<LABEL> describeLabel;

    shared_ptr<LABEL> languageLabel,  extra_mode_sizeLabel,  demo_mode_timePerStepLabel,  extra_mode_timePerStepLabel,  colorLimitLabel;
    shared_ptr<LABEL> WindowModeLabel,  VsyncLabel,  AntialiasingLabel,  RenderResolutionLabel,  ShowFramerateLabel;
    shared_ptr<LABEL> MusicVolumeLabel,  SEVolumeLabel,  ThreadMethodLabel,  MusicVolumeBar,  SEVolumeBar;

    void ResetPageComponents();
    void MoveBlock(float target_posx, float target_color_r, float target_color_g, float target_color_b);

    constexpr static int circle_speed = 60;

    void rend_circles_BG(int refresh);
};


class New_User_Page :public PAGE
{
public:
    New_User_Page();
    ~New_User_Page();
    bool Init() override;
    void Update() override;
    void FixedUpdate() override;
    // 通过 PAGE 继承

    void Rend() override;
    bool EnterPage();
    bool ExitPage() override;
    constexpr static int PAGE_INDEX = PAGE_REGISTER;
private:
    shared_ptr<Button> okButton;
    shared_ptr<LABEL> inputLabel;
    string input_username = "defaultUser";
    float enter_notice_time = 0;
    int page_status;
    //after clicking okButton
    void Update2(); 
};


class VERIFY_FAILED_PAGE :public PAGE
{
public:
    VERIFY_FAILED_PAGE();

    void Rend() override;
    bool EnterPage() override;

    constexpr static int PAGE_INDEX = PAGE_VERIFY_FAILED;
private:
    // 通过 PAGE 继承
    bool ExitPage() override;

};