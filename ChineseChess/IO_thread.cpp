
#include "framework_base.h"

#include "audio_thread.h"
#include "settings.h"

#include "thread_communicate.h"

using namespace std;

extern debug_ex debugger_audio;

bool md5_verify(const char* filename, const char* expected_md5)
{
    if (NO_HASH_CHECK)
    {
        return 1;
    }
    MD5_CTX ctx;
    int len = 0;
    unsigned char buffer[BUFFER_SIZE] = { 0 };
    unsigned char digest[16] = { 0 };
    FILE* pFile;
    errno_t err;

    if (err = fopen_s(&pFile, filename, "rb") != 0)
    {
        
        return 0;
    }
    if (pFile == 0)
    {
        return 0;
    }

    MD5_Init(&ctx);

    while ((len = fread(buffer, sizeof(unsigned char), BUFFER_SIZE-16, pFile)) > 0)
    {
        MD5_Update(&ctx, buffer, len);
    }

    MD5_Final(digest, &ctx);

    fclose(pFile);

    int i = 0;
    char buf[33] = { 0 };
    char tmp[3] = { 0 };
    for (i = 0; i < 16; i++)
    {
        sprintf_s(tmp, "%02X", digest[i]);
        strcat_s(buf, tmp);
    }

    return strcmp(expected_md5, buf) == 0;
}


//校验存档，存入md5
//返回md5的特征值
int md5_private_verify(const char* filename, string* md5)
{
    if (NO_HASH_CHECK)
    {
        return 1;
    }
    MD5_CTX ctx;
    int len = 0, check = 0;
    unsigned char buffer[1024] = { 0 };
    unsigned char digest[16] = { 0 };

    FILE* pFile = 0;
    errno_t err;

    err = fopen_s(&pFile, filename, "rb");

    if (pFile == 0 || err != 0)
    {
        writelog("Open file failed! "+ string(filename));
        return -1;
    }
    MD5_Init(&ctx);
    while ((len = fread(buffer, 1, 1020, pFile)) > 0)
    {
        MD5_Update(&ctx, buffer, len);
    }

    MD5_Final(digest, &ctx);

    fclose(pFile);


    int i = 0;
    char buf[33] = { 0 };
    char tmp[3] = { 0 };
    for (i = 0; i < 16; i++)
    {
        sprintf_s(tmp, "%02X", digest[i]);
        strcat_s(buf, tmp);
    }

    *md5 = buf;

    check = (int)buf[1] * (int)buf[10] + (int)buf[2] * (int)buf[9] + (int)buf[4] * (int)buf[8];
    check *= (int)buf[14] + (int)buf[6];
    check /= (int)buf[3] / 3 + 1;
    check += (int)buf[5] * (int)buf[7];

    return check;
}


int ReadFile_1(const char* filename, string filebuf[],int max_num)
{
    ifstream file;
    int num = 0;
    file.open(filename, ios::in);
    if (file.is_open())
    {
        while (getline(file, filebuf[num]))
        {
            if (filebuf[num] == "")
            {
                continue;
            }
            else
            {
                num++;
                if (num == max_num)
                {
                    writelog("lines beyond max_num limit in ReadFile_1()! "+to_string(max_num)+" "+string(filename));
                    break;
                }
            }
        }
        file.close();
        return num;
    }
    else
    {
        g_am.playEffectSound(8);
        g_cm.CreateEffect(301, 400, 50);
        string err = "ReadFile failed! ";
        err += filename;
        writelog(err);
        return -1;
    }
}

bool WriteFile(const char* filename, string filebuf[], int max_num)
{
    ofstream file;
    file.open(filename, ios::out);
    int end = 0;
    if (!file.is_open())
    {
        g_am.playEffectSound(8);
        g_cm.CreateEffect(301, 400, 50);
        string err = "WriteFile Error! ";
        err += filename;
        writelog(err);
        return 0;
    }
    for (int i = 0; i < max_num; i++)
    {
        if (filebuf[i] == "")
        {
            end = 1;
            continue;
        }
        if (!end)
        {
            file << filebuf[i] << endl;
        }
        else
        {
            writelog("Possible file corruption: Discontinuous writebuf!");
            g_am.playEffectSound(8);
            writelog(filename);
            for (int j = 0; j < max_num; j++)
            {
                if (filebuf[i] == "")
                {
                    writelog("[null]");
                    end++;
                    if (end > 10)
                    {
                        break;
                    }
                }
                else
                {
                    writelog(filebuf[i]);
                }
            }
            break;
        }
    }
    file.close();
    return 1;
}


bool file_private_verify(string filename)
{
    if (NO_HASH_CHECK)
    {
        return 1;
    }
    string checkfilename = filename.substr(0, filename.length() - 3), filemd5 = "", readbuf[64] = { "" };
    checkfilename += "check";
    int privatesum = 0, lines = 0;
    privatesum = md5_private_verify(filename.c_str(), &filemd5);
    lines = ReadFile(checkfilename.c_str(), readbuf);
    if (lines > 0)
    {
        if (filemd5 == readbuf[0] && to_string(privatesum) == readbuf[1])
        {
            return 1;
        }
        else
        {
            g_cm.CreateEffect(302, 600, 50);
        }
    }
    else
    {
        writelog("file_private_verify() failed! "+ filename);
    }
    return 0;
}



void certfile(string filename)
{
    int check = 0;
    string writebuff[64] = { "" }, md5buf = "", checkfilename = filename.substr(0, filename.length() - 3);
    checkfilename += "check";
    check = md5_private_verify(filename.c_str(), &md5buf);

    if (check > 0)
    {
        writebuff[0] = md5buf;
        writebuff[1] = to_string(check);
        WriteFile(checkfilename.c_str(), writebuff);
    }
    
    return;
}



void fileinit()
{
    string writebuf[64] = { "" }, readbuf[64] = { "" };
    int flag = 0, lines = 0, index = 0;
    string pathname = "./save/", fullfilename;
    fullfilename = pathname;
    fullfilename += "AllUsersProfile.dat";
    if (_access(fullfilename.c_str(), 4) != 0)
    {
        writelog("File not found: ./save/AllUsersProfile.dat not exist!");
        usernameC = "";
        return;
    }
    if (file_private_verify(fullfilename))
    {
        lines = ReadFile(fullfilename.c_str(), readbuf);
        if (lines < 1)
        {
            writelog("file corrupted! " + fullfilename);
            usernameC = "";
            return;
        }
        else
        {
            for (int i = 0; i < 63; i++)
            {
                if (readbuf[i] == "[Username]")
                {
                    usernameC = readbuf[i + 1];
                }
                else if (readbuf[i] == "[long_credit]")
                {
                    long_credit = (bool)stoi(readbuf[i + 1]);
                }
            }
        }
    }
    else
    {
        writelog("file cert failed! " + fullfilename);
        usernameC = "";
        return;
    }
    pathname += usernameC;
    //int flag = 0, result = 0;

    fullfilename = pathname;
    fullfilename += "/account.dat";
    if (_access(fullfilename.c_str(), 4) != 0)
    {
        writelog("File not found: ./save/username/account.dat not exist!");
        flag = CreateDirectory(pathname.c_str(), NULL);
        if (flag == 0)
        {
            writelog("CreateDirectory Error or folder has existed!(./save/username)");
            
        }
        cleanStrBuff(writebuf);
        writebuf[index++] = "[user]";
        writebuf[index++] = usernameC;
        
        writebuf[index++] = "[time_sum]";
        writebuf[index++] = to_string(0.0f);
        writebuf[index++] = to_string(0.0f);
        //writebuf[index++] = "[match_num]";
        //writebuf[index++] = to_string(0);
        //writebuf[index++] = to_string(0);
        writebuf[index++] = "[date]";
        writebuf[index++] = getTimeStr();
        WriteFile(fullfilename.c_str(), writebuf);

        certfile(fullfilename);
    }
    cleanStrBuff(writebuf);
    index = 0;
    if (_access("./save/config.dat", 4) != 0)
    {
        writelog("File not found: ./save/config.dat not exist!");
        writebuf[index++] = addFormat(lan[1].language_translation);
        writebuf[index++] = to_string(set1[1].language_translation);
        writebuf[index++] = addFormat(lan[1].extra_mode_size);
        writebuf[index++] = to_string(set1[1].extra_mode_size);
        writebuf[index++] = addFormat(lan[1].demo_mode_timePerStep);
        writebuf[index++] = to_string(set1[1].demo_mode_timePerStep);
        writebuf[index++] = addFormat(lan[1].extra_mode_timePerStep);
        writebuf[index++] = to_string(set1[1].extra_mode_timePerStep);
        writebuf[index++] = addFormat(lan[1].color_limit);
        writebuf[index++] = to_string(set1[1].color_limit);
        writebuf[index++] = addFormat(lan[1].window_mode);
        writebuf[index++] = to_string(set2[1].window_mode);
        writebuf[index++] = addFormat(lan[1].vsync);
        writebuf[index++] = to_string(set2[1].vsync);
        writebuf[index++] = addFormat(lan[1].MSAA);
        writebuf[index++] = to_string(set2[1].MSAA);
        writebuf[index++] = addFormat(lan[1].resolution);
        writebuf[index++] = to_string(set2[1].resolution);
        writebuf[index++] = addFormat(lan[1].show_framerate);
        writebuf[index++] = to_string(set2[1].show_framerate);
        writebuf[index++] = addFormat(lan[1].music_volume);
        writebuf[index++] = to_string(set3[1].music_volume);
        writebuf[index++] = addFormat(lan[1].se_volume);
        writebuf[index++] = to_string(set3[1].se_volume);
        writebuf[index++] = addFormat(lan[1].thread_method);
        writebuf[index++] = to_string(set3[1].thread_method);
        
        WriteFile("./save/config.dat", writebuf);
    }

    wchar_t* pathbuf1 = NULL;
    string pathbuf;
    SHGetKnownFolderPath(FOLDERID_ProgramData, KF_FLAG_DEFAULT_PATH, NULL, &pathbuf1);
    Wchar_tToString(pathbuf, pathbuf1);
    cleanStrBuff(writebuf);
    writebuf[0] = VERSION;
    pathbuf += "\\";
    pathbuf += PRODUCT_NAME;

    string filepath = pathbuf+ "\\init_ver.dat";
    
    if (_access(filepath.c_str(), 4) != 0)
    {
        //pathbuf = "\"" + pathbuf + "\"";
        flag = CreateDirectory(pathbuf.c_str(), NULL);
        if (flag == 0)
        {
            writelog("CreateDirectory failed! "+ pathbuf);
        }
        WriteFile(filepath.c_str(), writebuf);
    }
    return;
}


void userinit()
{

    string pathname = "./save/", fullfilename = "", writebuf[64] = { "" };
    int flag = 0,index=0;
    flag = CreateDirectory(pathname.c_str(), NULL);
    if (flag == 0)
    {
        writelog("CreateDirectory Error or folder has existed!(./save)");
    }
    fullfilename = pathname;
    fullfilename += "/AllUsersProfile.dat";
    
    writebuf[index++] = "[AllUsers]";
    writebuf[index++] = "[Username]";
    writebuf[index++] = usernameC;
    writebuf[index++] = "[long_credit]";
    writebuf[index++] = "1";
    if (WriteFile(fullfilename.c_str(), writebuf))
    {
        certfile(fullfilename);
    }
    else
    {
        thread_IO_request_userinit = -1;
        return;
    }
    
    pathname += usernameC;
    flag = CreateDirectory(pathname.c_str(), NULL);
    if (flag == 0)
    {
        writelog("CreateDirectory Error or folder has existed!(./save/username)");
    }

    fileinit();
    if (_access(fullfilename.c_str(), 4) != 0)
    {
        thread_IO_request_userinit = -1;
    }
    else
    {
        thread_IO_request_userinit = 0;
    }
    return;
}


unsigned __stdcall File_IO_init_res(LPVOID lpParameter)
{
    initgame();
    return 0;
}



//文件读写线程
unsigned __stdcall File_IO(LPVOID lpParameter)
{
    ofstream file;
    
    int waitnum = 0, errnum = 0;
    //刚写入过标识
    bool written = 0;
    string err = "Open File Failed!\n";
    err += filename_dbg;
    err += "\nPlease check your permissions.";

    debugger_main.writelog(0,"Program start at " + to_string(st.wYear) + "-" + to_string(st.wMonth) + "-" + to_string(st.wDay) + " " + to_string(st.wHour) + ":" + to_string(st.wMinute) + ":" + to_string(st.wSecond));
    
    file.open(filename_dbg, ios::app);
    readConfig();
    while (thread_IO_config_read !=1 )
    {
        Sleep(1);
    }
    fileinit();
    //MessageBoxEx(NULL, "dbg00 pass", "debug", MB_OK | MB_ICONSTOP, NULL);
    while (!file.is_open())
    {
        file.open(filename_dbg, ios::app);
        errnum++;
        if (errnum > 3)
        {
            quit_single = 1;
            return 0;
        }

        MessageBoxEx(NULL, err.c_str(), "IO Error", MB_OK | MB_ICONSTOP, NULL);
        Sleep(500);
    }

    while (1)
    {
        waitnum++;
        for (int i = 0; i < logbuf_length; i++)
        {
            if (!debugger_main.logbuf[i].empty())
            {
                file << debugger_main.logbuf[i] << endl;
                debugger_main.logbuf[i].clear();
                written = 1;
            }
            if (!debugger_audio.logbuf[i].empty())
            {
                file << debugger_audio.logbuf[i] << endl;
                debugger_audio.logbuf[i].clear();
                written = 1;
            }
        }
        if (quit_single)
        {
            file.close();
            return 0;
        }
        if (waitnum == 200)
        {
            waitnum = 0;
            if (written)
            {
                file.close();
                Sleep(100);
                file.open(filename_dbg, ios::app);
                while (!file.is_open())
                {
                    errnum++;
                    if (errnum > 5)
                    {
                        quit_single = 1;
                        return 0;
                    }
                    MessageBoxEx(NULL, err.c_str(), "IO Error", MB_OK | MB_ICONSTOP, NULL);
                    Sleep(500);
                    file.open(filename_dbg, ios::app);
                }
                written = 0;
            }
        }
        if (thread_IO_request_verify_res ==-1)
        {
            thread_IO_request_verify_res = 0;

            _beginthreadex(NULL, 0, File_IO_init_res, NULL, 0, NULL);	//创建线程
            continue;
        }
        if (thread_IO_request_update_profile)
        {
            update_profile();
        }
        /*if (thread_IO_request_record_game > 0)
        {
            game_record();
        }*/

        if (thread_IO_request_save_config)
        {
            Sleep(100);
            saveConfig();
            thread_IO_request_save_config = 0;
        }
        /*if (thread_IO_request_read_history > 0)
        {
            game_read_history();
        }*/
        if (thread_IO_request_read_record > 0)
        {
            thread_IO_request_read_record = 0;
            game_read_record();
        }
        if (thread_IO_request_userinit > 0)
        {
            userinit();
        }
        if (thread_IO_request_read_config)
        {
            thread_IO_request_read_config = 0;
            readConfig();
        }
        if (thread_IO_request_rend_taglines)
        {
            thread_IO_request_rend_taglines = 0;
            rend_taglines(1);
        }
        if (g_rm.releaseAllSignal.load() == 1)
        {
            g_rm.releaseAll_implementation();
            g_rm.releaseAllSignal.store(0);
        }
        if (g_rm.loadAllSignal.load() == 1)
        {
            if (g_rm.LoadAll_implementation())
            {
                g_rm.loadAllSignal.store(0);
            }
            else
            {
                g_rm.loadAllSignal.store(-1);
            }
        }
        
        Sleep(10);
    }
}

void game_read_record()
{
    if (thread_IO_request_read_record <= 0)
    {
        return;
    }
    string pathname = "./save/", fullfilename = "";
    pathname += usernameC;
    int lines = 0;
    fullfilename = pathname;
    fullfilename += "/account.dat";

    if (!file_private_verify(fullfilename))
    {
        g_am.playEffectSound(6);
        //writelog("cert file failed! "+ fullfilename);
        thread_IO_request_read_record = -1;
        return;
    }
    string readbuf[64] = { "" };
    lines = ReadFile(fullfilename.c_str(), readbuf);
    if (lines > 0)
    {
        for (int i = 0; i < 64; i++)
        {
            if (readbuf[i] == "[user]")
            {
                if (readbuf[i + 1] != usernameC)
                {
                    g_am.playEffectSound(6);
                    g_cm.CreateEffect(302, 600, 50);
                    thread_IO_request_read_record = -1;
                    writelog("account info cert failed! (username) " + fullfilename);
                    return;
                }
            }
            else if (readbuf[i] == "[time_sum]")
            {
                record.time = getTimeDigit_minute(stof(readbuf[i + 1]));
                record.time_online = getTimeDigit_minute(stof(readbuf[i + 2]));
            }
            else if (readbuf[i] == "[match_num]")
            {
                record.match_num = readbuf[i + 1];
                record.match_num_online = readbuf[i + 2];
            }
            else if (readbuf[i] == "[online_win_num]")
            {
                record.win_num_online = readbuf[i + 1];
            }
            else if (readbuf[i] == "[date]")
            {
                record.date = readbuf[i + 1];
            }
        }
    }
    else
    {
        writelog("read highscore failed!");
        thread_IO_request_read_record = -1;
        return;
    }
    thread_IO_request_read_record = 0;
    return;
}


void update_profile()
{
    string readbuf[64] = { "" }, writebuf[64] = { "" };
    string fullfilename = "./save/AllUsersProfile.dat";
    int lines;
    thread_IO_request_update_profile = 0;
    if (!file_private_verify(fullfilename))
    {
        writelog("file cert failed! " + fullfilename);
        return;
    }
    else
    {
        lines = ReadFile(fullfilename.c_str(), readbuf);
    }

    if (lines < 1)
    {
        writelog("corrupted file! "+ fullfilename);
    }
    else
    {
        writebuf[0] = "[AllUsers]";
        writebuf[1] = "[Username]";
        writebuf[2] = usernameC;
        writebuf[3] = "[long_credit]";
        writebuf[4] = "0";
        WriteFile(fullfilename.c_str(), writebuf);
        certfile(fullfilename);
    }
    return;
}