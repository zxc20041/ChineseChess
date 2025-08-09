#include"framework.h"
#include"settings.h"
#include"framework_base.h"
#include"thread_communicate.h"
#include"audio_thread.h"
#define ReadFile(filename,buf) ReadFile_1(filename,buf,sizeof(buf)/sizeof(buf[0]))
using namespace std;

setting_general_desc set1[2];
setting_graphics_desc set2[2];
setting_audio_desc set3[2];

stringTable lan[3];

void saveConfig()
{
    string  writebuf[64] = { "" };
    int index = 0;
    Sleep(1);
    writebuf[index++] = addFormat(lan[1].language_translation);
    writebuf[index++] = to_string(set1[0].language_translation);
    
    writebuf[index++] = addFormat(lan[1].demo_mode_timePerStep);
    writebuf[index++] = to_string(set1[0].demo_mode_timePerStep);
    writebuf[index++] = addFormat(lan[1].extra_mode_size);
    writebuf[index++] = to_string(set1[0].extra_mode_size);
    writebuf[index++] = addFormat(lan[1].extra_mode_timePerStep);
    writebuf[index++] = to_string(set1[0].extra_mode_timePerStep);
    writebuf[index++] = addFormat(lan[1].color_limit);
    writebuf[index++] = to_string(set1[0].color_limit);
    
    writebuf[index++] = addFormat(lan[1].window_mode);
    writebuf[index++] = to_string(set2[0].window_mode);
    writebuf[index++] = addFormat(lan[1].vsync);
    writebuf[index++] = to_string(set2[0].vsync);
    writebuf[index++] = addFormat(lan[1].MSAA);
    writebuf[index++] = to_string(set2[0].MSAA);
    writebuf[index++] = addFormat(lan[1].resolution);
    writebuf[index++] = to_string(set2[0].resolution);
    writebuf[index++] = addFormat(lan[1].show_framerate);
    writebuf[index++] = to_string(set2[0].show_framerate);
    writebuf[index++] = addFormat(lan[1].music_volume);
    writebuf[index++] = to_string(set3[0].music_volume);
    writebuf[index++] = addFormat(lan[1].se_volume);
    writebuf[index++] = to_string(set3[0].se_volume);
    writebuf[index++] = addFormat(lan[1].thread_method);
    writebuf[index++] = to_string(set3[0].thread_method);
    
    WriteFile("./save/config.dat", writebuf);
    return;
}



void readConfig_config()
{
    string readbuf[128] = { "" };
    //fileinit();
    if (ReadFile("./save/config.dat", readbuf) == -1)
    {
        writelog("read config failed!");
        return;
    }

    for (int i = 0; i < 127; i++)
    {
        if (readbuf[i].empty())
        {
            break;
        }
        if (readbuf[i] == addFormat(lan[1].language_translation))
        {
            set1[0].language_translation = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].demo_mode_timePerStep))
        {
            set1[0].demo_mode_timePerStep = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].extra_mode_size))
        {
            set1[0].extra_mode_size = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].extra_mode_timePerStep))
        {
            set1[0].extra_mode_timePerStep = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].color_limit))
        {
            set1[0].color_limit = atoi(readbuf[i + 1].c_str());
        }

        else if (readbuf[i] == addFormat(lan[1].window_mode))
        {
            set2[0].window_mode = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].vsync))
        {
            set2[0].vsync = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].MSAA))
        {
            set2[0].MSAA = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].resolution))
        {
            set2[0].resolution = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].show_framerate))
        {
            set2[0].show_framerate = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].music_volume))
        {
            set3[0].music_volume = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].se_volume))
        {
            set3[0].se_volume = atoi(readbuf[i + 1].c_str());
        }
        else if (readbuf[i] == addFormat(lan[1].thread_method))
        {
            set3[0].thread_method = atoi(readbuf[i + 1].c_str());
        }
    }
    set2[1] = set2[0];
    thread_IO_config_read = 1;
    g_am.setMusicVolume(set3[0].music_volume);
    g_am.setSEVolume(set3[0].se_volume);    //应用更改
    return;
}

void readConfig_lan()
{
    string readbuf[256] = { "" };
    //cleanStrBuff(readbuf);
    if (ReadFile("./lan/language.dat", readbuf) < 1)
    {
        writelog("read language failed!");
        return;
    }
    for (int i = 0; i < 255; i++)
    {
        if (readbuf[i].empty())
        {
            break;
        }
        if (readbuf[i] == addFormat("start"))
        {
            lan[2].start = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("history"))
        {
            lan[2].history = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("connect"))
        {
            lan[2].connect = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("general"))
        {
            lan[2].general = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("window_mode"))
        {
            lan[2].window_mode = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("vsync"))
        {
            lan[2].vsync = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("MSAA"))
        {
            lan[2].MSAA = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("resolution"))
        {
            lan[2].resolution = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("show_framerate"))
        {
            lan[2].show_framerate = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("music_volume"))
        {
            lan[2].music_volume = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("se_volume"))
        {
            lan[2].se_volume = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("thread_method"))
        {
            lan[2].thread_method = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("graphics"))
        {
            lan[2].graphics = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("audio"))
        {
            lan[2].audio = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("sampleCount"))
        {
            lan[2].sampleCount = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("on"))
        {
            lan[2].on = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("off"))
        {
            lan[2].off = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("mode"))
        {
            lan[2].mode = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("windowed"))
        {
            lan[2].windowed = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("fullscreen"))
        {
            lan[2].fullscreen = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("lastpage"))
        {
            lan[2].lastpage = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("nextpage"))
        {
            lan[2].nextpage = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("record"))
        {
            lan[2].record = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("no_limit"))
        {
            lan[2].no_limit = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("timesum"))
        {
            lan[2].timesum = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("accountTime"))
        {
            lan[2].accountTime = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("save_succeed"))
        {
            lan[2].save_succeed = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("save_failed"))
        {
            lan[2].save_failed = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("load_failed"))
        {
            lan[2].load_failed = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("demo_mode_timePerStep"))
        {
            lan[2].demo_mode_timePerStep = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("extra_mode_timePerStep"))
        {
            lan[2].extra_mode_timePerStep = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("extra_mode_size"))
        {
            lan[2].extra_mode_size = readbuf[i + 1];
        }

        else if (readbuf[i] == addFormat("enter_username"))
        {
            lan[2].enter_username = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("easy"))
        {
            lan[2].easy = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("normal"))
        {
            lan[2].normal = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("hard"))
        {
            lan[2].hard = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("paste ip"))
        {
            lan[2].paste_ip = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("paste port"))
        {
            lan[2].paste_port = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("low"))
        {
            lan[2].low = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("mid"))
        {
            lan[2].mid = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("high"))
        {
            lan[2].high = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("copy succeed"))
        {
            lan[2].copy_succeed = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("color_limit"))
        {
            lan[2].color_limit = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("copy failed"))
        {
            lan[2].copy_failed = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("connect succeed"))
        {
            lan[2].connect_succeed = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("connect failed"))
        {
            lan[2].connect_failed = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("disconnect"))
        {
            lan[2].disconnect = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("require ip"))
        {
            lan[2].require_ip = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("io error"))
        {
            lan[2].io_error = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("file corrupt"))
        {
            lan[2].file_corrupt = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("agree"))
        {
            lan[2].agree = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("enable"))
        {
            lan[2].enable = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("match num"))
        {
            lan[2].match_num = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("win num"))
        {
            lan[2].win_num = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("free_view"))
        {
            lan[2].free_view= readbuf[i + 1];
        }
    }
    return;
}

void readConfig()
{
    string readbuf[128] = { "" };

    readConfig_config();
    
    //fileinit();
    /*if (ReadFile("./save/config.dat", readbuf) == -1)
    {
        writelog("read config failed!");
        goto read_lan;
    }*/

//read_lan:
    readConfig_lan();
//read_desc:
    //cleanStrBuff(readbuf);
    if (ReadFile("./lan/desc.dat", readbuf) < 1)
    {
        writelog("read desc failed!");
        return;
    }
    
    for (int i = 0; i < 127; i++)
    {
        if (readbuf[i].empty())
        {
            break;
        }
        if (readbuf[i] == addFormat("language_translation_description"))
        {
            lan[2].language_translation_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("demo_mode_timePerStep_description"))
        {
            lan[2].demo_mode_timePerStep_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("extra_mode_timePerStep_description"))
        {
            lan[2].extra_mode_timePerStep_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("extra_mode_size_description"))
        {
            lan[2].extra_mode_size_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("color_limit_description"))
        {
            lan[2].color_limit_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("window_mode_description"))
        {
            lan[2].window_mode_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("vsync_description"))
        {
            lan[2].vsync_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("MSAA_description"))
        {
            lan[2].MSAA_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("resolution_description"))
        {
            lan[2].resolution_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("show_framerate_description"))
        {
            lan[2].show_framerate_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("music_volume"))
        {
            lan[2].music_volume = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("thread_method_description"))
        {
            lan[2].thread_method_description = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("save_overwrite_warning"))
        {
            lan[2].save_overwrite_warning = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("load_warning"))
        {
            lan[2].load_warning = readbuf[i + 1];
        }
        else if (readbuf[i] == addFormat("user_agreement"))
        {
            lan[2].user_agreement = readbuf[i + 1];
            lan[2].user_agreement += "\n";
            lan[2].user_agreement += readbuf[i + 2];
        }
        else if (readbuf[i] == addFormat("PE_warning"))
        {
            lan[2].PE_warning = readbuf[i + 1];
            lan[2].PE_warning += "\n";
            lan[2].PE_warning += readbuf[i + 2];
        }
    }
    if (set1[0].language_translation)
    {
        lan[0] = lan[2];
    }
    debugger_main.writelog(0,"read config and translation complete.");
    return;
}