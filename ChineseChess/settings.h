#pragma once
#include<string>

struct setting_general_desc
{
    int extra_mode_size = 0;
    int demo_mode_timePerStep = 2;
    int extra_mode_timePerStep = 0;
    int color_limit = 4;
    bool language_translation = 1;
};

struct setting_graphics_desc
{
    int MSAA = 1;
    int sampleCount = 0;
    int resolution = 3;
    int particleDensity = 2;
    bool show_framerate = 0;
    bool window_mode = 1;
    bool vsync = 1;
};

struct setting_audio_desc
{
    int music_volume = 20;
    int se_volume = 50;
    int thread_method = 0;
    bool spatial_sound = 1;
};



struct stringTable
{
    std::string test = "test";
    std::string start = "Start";
    std::string history = "History";
    std::string connect = "Connect";
    std::string PVE = "PVE";
    std::string quit = "Quit";
    std::string str_return = "Return";
    std::string general = "General";
    std::string graphics = "Graphics";
    std::string audio = "Audio";

    std::string language_translation = "Language Translation";

    std::string window_mode = "Window Mode";
    std::string vsync = "V-Sync";
    std::string MSAA = "Anti-aliasing";
    std::string sampleCount = "Current SampleCount: ";
    std::string resolution = "Render Resolution";
    std::string show_framerate = "Show Framerate";

    std::string music_volume = "Music Volume";
    std::string se_volume = "SE Volume";
    std::string thread_method = "Thread Method";

    std::string on = "On";
    std::string off = "Off";
    std::string mode = "Mode";
    std::string windowed = "Windowed";
    std::string fullscreen = "Fullscreen";
    std::string lastpage = "Last Page";
    std::string nextpage = "Next Page";
    std::string end_of_list = "End of list.";
    std::string record = "Record";
    //string scoresum = "Sum of Score:";
    std::string timesum = "Play time:";
    std::string accountTime = "Account time at";
    std::string save_succeed = "Save Succeed.";
    std::string save_failed = "Save Failed.";
    std::string load_failed = "Load Failed.";
    std::string enter_username = "Enter Username:";
    std::string loading = "Loading";
    std::string no_limit = "No Limit";
    std::string easy = "Easy";
    std::string normal = "Normal";
    std::string hard = "Hard";
    std::string low = "Low";
    std::string mid = "Mid";
    std::string high = "High";
    std::string free_view = "View Mode: Free";
    


    std::string language_translation_description = "Use external translation data.";

    
    std::string window_mode_description = "Change window display mode.";
    std::string vsync_description = "Enable vertical synchronization to decrease screen tearing and save power.";
    std::string MSAA_description = "Enable Anti-Aliasing to make the image smoother. Restart application to completely apply.";
    
    std::string resolution_description = "Change the resolution of the program.";
    std::string show_framerate_description = "Show realtime fps and drawcall times per frame.";
    
    std::string thread_method_description = "Change sleeping methed of the audio thread. Mode1/Mode2=Balance Mode3=Performance  Avg Latency: ";

    std::string save_overwrite_warning = "Overwrite current save, Sure?";
    std::string load_warning = "Load save and LOSE current progress, Sure?";

    std::string agreement_status_on_description = "Network feature enabled.";
    std::string agreement_status_off_description = "Enable network feature. You can't disable this feature once enabled.";


    std::string paste_ip = "Paste IP";
    std::string paste_port = "Paste Port";

    std::string copy_succeed = "Copy Succeed";
    std::string copy_failed = "Copy Failed";

    std::string connect_succeed = "Connected";
    std::string connect_failed = "Connect Failed";
    std::string disconnect = "Disconnected";
    std::string require_ip = "IP Address Required";

    std::string io_error = "Failed to access file, Check your permissions";
    std::string file_corrupt = "File Corrupted";


    std::string agree = "I see";
    std::string enable = "Enable Feature";
    std::string user_agreement = "This program is for learning purpose only.\nOnline game allows you match with other players under the same local area network, hotspots are recommended. You hold responsible for your own device's safety.";
    
    std::string match_num = "Numbers of Matches";
    std::string win_num = "Numbers of Winnings";

    std::string color_limit = "Color Num";
    std::string color_limit_description = "Change Color Num limitation in coloring";

    std::string demo_mode_timePerStep = "Demo Mode Time Per Step";
    std::string demo_mode_timePerStep_description = "Change time per step in demo mode";
    std::string extra_mode_timePerStep = "Extra Mode Time Per Step";
    std::string extra_mode_timePerStep_description = "Change time per step in extra mode";
    std::string extra_mode_size = "Extra Mode Size";
    std::string extra_mode_size_description = "Select map size in extra mode";

    std::string PE_warning = "A very small percentage of people may experience a seizure when exposed to certain visual images, including flashing lights or patterns that may appear in video games. Even people who have no history of seizures or epilepsy may have an undiagnosed condition that can cause these \"photosensitive epileptic seizures\" while playing video games. If you or any of your relatives have a history of seizures or epilepsy, consult a doctor before playing video games.\nImmediately stop playing and consult a doctor if you experience any symptoms.";
};

extern setting_general_desc set1[2];
extern setting_graphics_desc set2[2];
extern setting_audio_desc set3[2];

extern stringTable lan[3];

//保存设置至文件
void saveConfig();

//读取设置文件
void readConfig();

std::string addFormat(std::string ori);

