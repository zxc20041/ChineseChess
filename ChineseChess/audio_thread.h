#pragma once
#include "framework_base.h"
#include <al.h>
#include <alut.h>
#include <alc.h>
#include <vorbis/vorbisfile.h>
#include"audio_base.h"



//载入ogg文件
//bool LoadOGG(const char* name, ALuint* buffer, ALsizei* freq, ALenum* format, ALsizei* size);

//载入wave文件
//bool loadWavFile(const string filename, ALuint* buffer, ALsizei* size, ALsizei* frequency, ALenum* format);



//音乐&音效管理线程
unsigned __stdcall ThreadPlayMusic(LPVOID lpParameter);

class AudioManager
{
private:
    //SE_INFO se_p[SE_AUDIO_NUM];  //se sources
    SE_DESC se[SE_MAX]; //se requests
    int thread_Audio_target_music;
    int current_playing_music, last_music_request;
    int thread_smusic_response, isloop;
    int audio_latency_update_count;
    weak_ptr<float> pAvgLatency;
    TIMER timer;
    bool device_created;
    bool thread_Audio_volume_changed, thread_Audio_volume_changed_music, thread_Audio_switch_immediately, thread_Audio_quit_single;
    
    friend unsigned __stdcall ThreadPlayMusic(LPVOID lpParameter);
    friend unsigned __stdcall ThreadPlaySingleMusic(LPVOID lpParameter);

    //create buffers and sources
    void Init();


    void ApplySEVolumeChange(shared_ptr<SE_INFO> se_info);

    //play se in SE_DESC
    void updateSE();
public:
    AudioManager();
    ~AudioManager();
    
    //播放音效
    void PlayEffectSound(string aliasName, float volumn = 1.0f);

    //播放指定空间位置的音效
    void PlayEffectSound(string aliasName, float x, float y, float z, float volumn = 1.0f);

    void PlayBGM(int music_index, bool immediate, bool loop);

    void PlayMusicGroup(int group);

    void setMusicVolume(int volume);

    void setSEVolume(int volume);

    void GetAudioLatency(weak_ptr<float> avgLatency);

    // Load the WAVE file into memory
    shared_ptr<SE_INFO> LoadWavFromFile(string aliasName, string filePath, AUDIO_DESC audio_desc);

    // Clean up sound buffer and source
    void Cleanup(shared_ptr<SE_INFO> se_info);
};

extern AudioManager g_am;