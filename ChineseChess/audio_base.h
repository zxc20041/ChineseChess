#pragma once
#include <al.h>
#include<string>
#include<vector>
#include<atomic>
constexpr int SE_MAX = 256;
constexpr int SE_AUDIO_NUM = 64;
constexpr int SE_AUDIO_SOURCE_NUM = 4;

constexpr int BGM_INGAME = 1;
constexpr int BGM_HOME = 2;


struct ALcoms
{
    std::string fileName;

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
    std::string name = "";
};


struct SE_INFO
{
    std::string aliasName;
    std::string filename;
    std::vector<ALcoms> ALwav;
    float loudnessOffset = 1.0f;
    int source_num = 0;
    std::atomic<bool> playing = 0;
};

struct SE_DESC
{
    float sound_positions[3] = { 0,0,0 };   //max 1,1 while z=0.5f
    float volumn = 0;   //0-1
    std::string se_aliasName;
    bool sound_3d_position = 0;
    std::atomic<bool> valid = 0;
};