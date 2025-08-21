#pragma once
#include "framework_base.h"
#include <al.h>
#include <alut.h>
#include <alc.h>
#include <vorbis/vorbisfile.h>

constexpr int SE_MAX = 256;
constexpr int SE_AUDIO_NUM = 64;
constexpr int SE_AUDIO_SOURCE_NUM = 4;

constexpr int BGM_INGAME = 1;
constexpr int BGM_HOME = 2;

//extern int thread_smusic_response, isloop, last_music_request;
//extern int current_playing_music;




struct ALcoms
{
    string fileName;
    
    ALint state = NULL;                            // The state of the sound source
    ALuint bufferID = NULL;                        // The OpenAL sound buffer ID
    ALuint sourceID = NULL;                        // The OpenAL sound source
    ALsizei size2 = NULL;                          // For wave format
    ALenum format = NULL;                          // The sound data format
    ALsizei freq = NULL;                           // The frequency of the sound data
    bool loaded = 0;
    //vector<char> bufferData;                // The sound buffer data from file
};

struct WAVE_Data {
    char subChunkID[4]; //should contain the word data
    long subChunk2Size; //Stores the size of the data block
};

struct WAVE_Format {
    char subChunkID[4];
    long subChunkSize;
    short audioFormat;
    short numChannels;
    long sampleRate;
    long byteRate;
    short blockAlign;
    short bitsPerSample;
};

struct RIFF_Header {
    char chunkID[4];
    long chunkSize;//size not including chunkSize or chunkID
    char format[4];
};

struct musicinfo
{
    string name = "";
};


struct SE_INFO
{
    vector<ALcoms> ALwav;
    string aliasName;
    string filename;
    float loudnessOffset = 1.0f;
    int source_num = 0;
};

struct SE_DESC
{
    atomic<string> se_aliasName;
    float sound_positions[3];   //max 1,1 while z=0.5f
    float volumn;   //0-1
    bool sound_3d_position;
};


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

    // Load the WAVE file into memory
    shared_ptr<SE_INFO> LoadWavFromFile(string aliasName, string filePath, AUDIO_DESC audio_desc);

    // Clean up sound buffer and source
    void Cleanup(shared_ptr<SE_INFO> se_info);

    void ApplySEVolumeChange(shared_ptr<SE_INFO> se_info);

    //play se in SE_DESC
    void updateSE();
public:
    AudioManager();
    ~AudioManager();
    
    //播放音效
    void PlayEffectSound(const string aliasName, const float volumn = 1.0f);

    //播放指定空间位置的音效
    void PlayEffectSound(string aliasName, float x, float y, float z, float volumn = 1.0f);

    void PlayBGM(int music_index, bool immediate, bool loop);

    void PlayMusicGroup(int group);

    void setMusicVolume(int volume);

    void setSEVolume(int volume);

    void GetAudioLatency(weak_ptr<float> avgLatency);

    ALuint LoadWavFromFile(std::string fileName, float loudnessOffset = 1.0f);
};

extern AudioManager g_am;