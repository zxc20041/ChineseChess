#include"audio_thread.h"
#include"thread_communicate.h"
#include"settings.h"
using namespace debugger;

ALCdevice* device;
ALCcontext* context;
debug_ex debugger_audio(1);

ALcoms ALogg[32];
AudioManager g_am;

//载入wave文件
bool loadWavFile(const string filename, ALcoms* waveData);

AudioManager::AudioManager()
{

    device_created = 0;
    thread_Audio_switch_immediately = 0, thread_Audio_quit_single = 0, thread_Audio_volume_changed = 0;
    thread_Audio_volume_changed_music = 0;
    current_playing_music = 0, last_music_request = 6;
    thread_smusic_response = 0, isloop = 1;
    thread_Audio_target_music = 6;
    audio_latency_update_count = -1;
}

void AudioManager::Init()
{
    device = alcOpenDevice(0);
    context = alcCreateContext(device, 0);
    ALboolean initStatus = alcMakeContextCurrent(context);
    if (alGetError() != AL_NO_ERROR)
    {
        debugger_audio.writelog(2, "openal init failed!");
        return;
    }
    device_created = 1;
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    /*int alerr = AL_NO_ERROR;
    string se_filenames[11],aliasNames[11];
    se_filenames[0] = ".\\sounds\\Button1.wav";
    se_filenames[1] = ".\\sounds\\Button2.wav";
    se_filenames[2] = ".\\sounds\\save.wav";
    se_filenames[3] = ".\\sounds\\load.wav";
    se_filenames[4] = ".\\sounds\\gameover.wav";
    se_filenames[5] = ".\\sounds\\alertTick.wav";
    se_filenames[6] = ".\\sounds\\endofpage.wav";
    se_filenames[7] = ".\\sounds\\ioerror.wav";
    se_filenames[8] = ".\\sounds\\pass.wav";
    se_filenames[9] = ".\\sounds\\tick.wav";
    se_filenames[10] = ".\\sounds\\loading.wav";

    aliasNames[0] = "button1";
    aliasNames[1] = "button2";
    aliasNames[2] = "save";
    aliasNames[3] = "load";
    aliasNames[4] = "gameover";
    aliasNames[5] = "alertTick";
    aliasNames[6] = "endofpage";
    aliasNames[7] = "ioerror";
    aliasNames[8] = "pass";
    aliasNames[9] = "tick";
    aliasNames[10] = "loading";
    for (int i = 0; i < 11; i++)
    {
        g_rm.AddResource(aliasNames[i], se_filenames[i], "pass", RESOURCE_INFO::DEFAULT_WAVE);
    }
    g_rm.LoadAll();*/
    return;
}

shared_ptr<SE_INFO> AudioManager::LoadWavFromFile(string aliasName, string filePath, AUDIO_DESC audio_desc)
{
    int alerr = AL_NO_ERROR, cnt = 0;
    if (filePath.empty() || audio_desc.se_source_num <= 0 || audio_desc.type != AUDIO_DESC::AUDIO_TYPE::AUDIO_TYPE_WAV)
    {
        return nullptr;
    }
    while (!device_created && cnt < 100)
    {
        Sleep(10);
        cnt++;
    }
    if (!device_created)
    {
        debugger_audio.writelog(DWARNNING, "device not created in LoadWavFromFile()", __LINE__);
        return nullptr;
    }
    //prepare meta data
    shared_ptr<SE_INFO> se_info = make_shared<SE_INFO>();
    se_info->filename = filePath;
    se_info->aliasName = aliasName;
    se_info->source_num = audio_desc.se_source_num;
    se_info->loudnessOffset = audio_desc.loudnessOffset;
    ALcoms wave_data;
    for (int i = 0; i < audio_desc.se_source_num; i++)
    {
        alGenSources(1, &wave_data.sourceID);
        alerr = alGetError();
        if (alerr != AL_NO_ERROR)
        {
            debugger_audio.writelog(1, "alGenSources error in ThreadPlayMusic loading wav! file=" + filePath + " err=" + to_string(alerr));
            return nullptr;
        }
        if (i == 0)
        {
            alGenBuffers(1, &wave_data.bufferID);
            alerr = alGetError();
            if (alerr != AL_NO_ERROR)
            {
                debugger_audio.writelog(1, "alGenBuffers error in ThreadPlayMusic loading wav! file=" + filePath + " err=" + to_string(alerr));
                return nullptr;
            }
            if (!loadWavFile(filePath, &wave_data))
            {
                debugger_audio.writelog(DWARNNING, "load wavefile failed");
                return nullptr;
            }
            alSourcei(wave_data.sourceID, AL_BUFFER, wave_data.bufferID);
            alSourcef(wave_data.sourceID, AL_GAIN, (float)set3[0].se_volume / 100.0F);  //音量
            alSourcei(wave_data.sourceID, AL_SOURCE_RELATIVE, 1);
            alSource3f(wave_data.sourceID, AL_POSITION, 0.0f, 0.0f, 1.0f);
        }
        else
        {
            alSourcei(wave_data.sourceID, AL_BUFFER, wave_data.bufferID);
            alSourcef(wave_data.sourceID, AL_GAIN, (float)set3[0].se_volume / 100.0F);  //音量
            alSourcei(wave_data.sourceID, AL_SOURCE_RELATIVE, 1);
            alSource3f(wave_data.sourceID, AL_POSITION, 0.0f, 0.0f, 1.0f);
        }
        se_info->ALwav.push_back(wave_data);

        /*alSourcef(se_p[se[i] - 1].ALwav[j].sourceID, AL_ROLLOFF_FACTOR, 1.0f);
        alSourcef(se_p[se[i] - 1].ALwav[j].sourceID, AL_REFERENCE_DISTANCE, 1.0f);
        alSourcef(se_p[se[i] - 1].ALwav[j].sourceID, AL_MIN_GAIN, 0.0f);
        alSourcef(se_p[se[i] - 1].ALwav[j].sourceID, AL_MAX_GAIN, 0.9f);*/
        alerr = alGetError();
        if (alerr != AL_NO_ERROR)
        {
            debugger_audio.writelog(DWARNNING, "alSourcef error in after-load wavefile! " + to_string(alerr) + " index " + to_string(i));
            return nullptr;
        }
        
    }
    //debugger_audio.writelog(DDEBUG, "load wav completed: " + filePath + " as " + aliasName);
    return se_info;
}

void AudioManager::Cleanup(shared_ptr<SE_INFO> se_info)
{
    if (se_info->filename.empty() || se_info->source_num == 0)
    {
        return;
    }
    while (se_info->playing.load());

    for (int i = 0; i < se_info->source_num; i++)
    {
        alSourceStop(se_info->ALwav[i].sourceID);
        alDeleteSources(1, &se_info->ALwav[i].sourceID);
    }
    alDeleteBuffers(1, &se_info->ALwav[0].bufferID);
    int alerr = alGetError();
    if (alerr != AL_NO_ERROR)
    {
        debugger_audio.writelog(DWARNNING, "alDeleteBuffers error in AudioManager::Cleanup(shared_ptr<SE_INFO> se_info)! " + to_string(alerr), __LINE__);
    }
    return;
}

void AudioManager::ApplySEVolumeChange(shared_ptr<SE_INFO> se_info)
{
    if (se_info->filename.empty()|| se_info->source_num == 0)
    {
        return;
    }
    for (int i = 0; i < se_info->source_num; i++)
    {
        alSourcef(se_info->ALwav[i].sourceID, AL_GAIN, (float)set3[0].se_volume / 100.0F);  //音量
    }
    thread_Audio_volume_changed = 0;
    return;
}

void AudioManager::updateSE()
{
    int alerr = AL_NO_ERROR;
    for (int i = 0; i < SE_MAX; i++)
    {
        if (se[i].valid.load() == 0)
        {
            continue;
        }
        if (se[i].se_aliasName.empty())
        {
            debugger_audio.writelog(DWARNNING, "null se_aliasName in AudioManager::updateSE()", __LINE__);
            se[i].valid.store(0);
            continue;
        }
        shared_ptr<SE_INFO> se_info = g_rm.getAudio(se[i].se_aliasName);
        se_info->playing.store(1);
        if (se_info == nullptr)
        {
            debugger_audio.writelog(DWARNNING, "audio not found in resource manager: " + se[i].se_aliasName, __LINE__);
            se[i].valid.store(0);
            continue;
        }
        //debugger_audio.writelog(DDEBUG, "play se: " + se[i].se_aliasName);
        for (int j = 0; j < se_info->source_num; j++)
        {
            alGetSourcei(se_info->ALwav[j].sourceID, AL_SOURCE_STATE, &se_info->ALwav[j].state);
            alerr = alGetError();
            if (alerr != AL_NO_ERROR)
            {
                debugger_audio.writelog(DWARNNING, "alGetSourcei error in AudioManager::updateSE()! " + to_string(alerr), __LINE__);
            }
            if (se_info->ALwav[j].state == AL_STOPPED || se_info->ALwav[j].state == AL_INITIAL)
            {
                alSourcef(se_info->ALwav[j].sourceID, AL_GAIN, (float)set3[0].se_volume * se[i].volumn * se_info->loudnessOffset / 100.0F);  //音量
                if (se[i].sound_3d_position)
                {
                    alSource3f(se_info->ALwav[j].sourceID, AL_POSITION, se[i].sound_positions[0], se[i].sound_positions[1], se[i].sound_positions[2]);
                }

                alerr = alGetError();
                if (alerr != AL_NO_ERROR)
                {
                    debugger_audio.writelog(1, "alSource3f error in AL_POSITION! " + to_string(alerr) + " file= " + se_info->filename + " 3d_pos " + to_string(se[i].sound_3d_position));
                    if (se[i].sound_3d_position)
                    {
                        debugger_audio.writelog(0, "pos x= " + to_string(se[i].sound_positions[0]) + " y= " + to_string(se[i].sound_positions[1]) + " z= " + to_string(se[i].sound_positions[2]));
                    }
                }
                alSourcePlay(se_info->ALwav[j].sourceID);
                alerr = alGetError();
                if (alerr != AL_NO_ERROR)
                {
                    debugger_audio.writelog(DWARNNING, "alSourcePlay error in AudioManager::updateSE()! " + to_string(alerr), __LINE__);
                }
                break;
            }
            if (j == se_info->source_num - 1)
            {
                alSourceStop(se_info->ALwav[0].sourceID);
                alSourcef(se_info->ALwav[0].sourceID, AL_GAIN, (float)set3[0].se_volume * se[i].volumn / 100.0F);  //音量
                if (se[i].sound_3d_position)
                {
                    alSource3f(se_info->ALwav[0].sourceID, AL_POSITION, se[i].sound_positions[0], se[i].sound_positions[1], se[i].sound_positions[2]);
                }
                alerr = alGetError();
                if (alerr != AL_NO_ERROR)
                {
                    debugger_audio.writelog(1, "alSource3f error in AL_POSITION 0! " + to_string(alerr) + " file= " + se_info->filename + " 3d_pos " + to_string(se[i].sound_3d_position));
                    if (se[i].sound_3d_position)
                    {
                        debugger_audio.writelog(0, "pos x= " + to_string(se[i].sound_positions[0]) + " y= " + to_string(se[i].sound_positions[1]) + " z= " + to_string(se[i].sound_positions[2]));
                    }
                }
                alSourcePlay(se_info->ALwav[0].sourceID);
            }
        }
        se[i].valid.store(0);
        se_info->playing.store(0);
    }
    return;
}

AudioManager::~AudioManager()
{
    if (device_created)
    {
        alcDestroyContext(context);
        alcCloseDevice(device);
    }
    
}

void AudioManager::PlayEffectSound(string aliasName, float volumn)
{
    for (int i = 0; i < SE_MAX; i++)
    {
        if (!se[i].se_aliasName.empty())
        {
            continue;
        }
        se[i].sound_3d_position = 0;
        se[i].volumn = volumn;
        se[i].se_aliasName = aliasName;
        se[i].valid.store(1);
        break;
    }
    return;
}


void AudioManager::PlayEffectSound(string aliasName, float x, float y, float z, float volumn)
{
    if (x < -1)
    {
        debugger_audio.writelog(DWARNNING, "unexpected x pos in AudioManager::playEffectSound: " + to_string(x) + " aliasName= " + aliasName);
        x = -0.99f;
    }
    else if (x > 1)
    {
        debugger_audio.writelog(DWARNNING, "unexpected x pos in AudioManager::playEffectSound: " + to_string(x) + " aliasName= " + aliasName);
        x = 0.99f;
    }
    if (y < -1)
    {
        debugger_audio.writelog(DWARNNING, "unexpected y pos in AudioManager::playEffectSound: " + to_string(y) + " aliasName= " + aliasName);
        y = -0.99f;
    }
    else if (y > 1)
    {
        debugger_audio.writelog(DWARNNING, "unexpected y pos in AudioManager::playEffectSound: " + to_string(y) + " aliasName= " + aliasName);
        y = 0.99f;
    }
    if (z > 1)
    {
        debugger_audio.writelog(DWARNNING, "unexpected z pos in AudioManager::playEffectSound: " + to_string(z) + " aliasName= " + aliasName);
        z = 1;
    }
    else if (z < 0)
    {
        debugger_audio.writelog(DWARNNING, "unexpected z pos in AudioManager::playEffectSound: " + to_string(z) + " aliasName= " + aliasName);
        z = 0.1f;
    }
    for (int i = 0; i < SE_MAX; i++)
    {
        if (!se[i].se_aliasName.empty())
        {
            continue;
        }
        if (set3[0].spatial_sound)
        {
            se[i].sound_3d_position = 1;
            se[i].sound_positions[0] = x;
            se[i].sound_positions[1] = y;
            se[i].sound_positions[2] = z;
        }
        else
        {
            se[i].sound_3d_position = 0;
        }

        se[i].volumn = 1 - (0.5f * sqrtf(2 * x * x + 2 * y * y)) * volumn;  // 1-sqrt(2)/2 ~ 1

        se[i].se_aliasName = aliasName;
        se[i].valid.store(1);
        break;
    }
    return;
}

void AudioManager::PlayBGM(int music_index, bool immediate, bool loop)
{
    thread_Audio_target_music = music_index;
    thread_Audio_switch_immediately = immediate;
    isloop = loop;
    return;
}



void AudioManager::PlayMusicGroup(int group)
{
    return;
}

void AudioManager::setMusicVolume(int volume)
{
    set3[0].music_volume = volume;
    thread_Audio_volume_changed_music = 1;
    return;
}

void AudioManager::setSEVolume(int volume)
{
    set3[0].se_volume = volume;
    thread_Audio_volume_changed = 1;
    return;
}

void AudioManager::GetAudioLatency(weak_ptr<float> avgLatency)
{
    pAvgLatency = avgLatency;
    
    audio_latency_update_count = 10;    // avg = sum / 10
    return;
}

bool LoadOGG(const char* name, ALuint* buffer, ALsizei* freq, ALenum* format, ALsizei* size)
{

    string buf;
    if (strcmp(name, "") == 0)
    {
        debugger_audio.writelog(1,"loadogg : filename cannot be null!");
        return 0;
    }
    vector<char> bufferData;                // The sound buffer data from file
    bool endian = 0;                         // 0 for Little-Endian, 1 for Big-Endian
    int bitStream;
    long bytes;
    char array[BUFFER_SIZE];                // Local fixed size array
    FILE* f;
    fopen_s(&f, name, "rb");

    if (f == NULL)
    {
        buf = "loadogg : can not open file: ";
        buf += name;
        debugger_audio.writelog(1,buf);
        return false;
    }

    vorbis_info* pInfo;
    OggVorbis_File oggFile;

    // Try opening the given file
    if (ov_open(f, &oggFile, NULL, 0) != 0)
    {
        buf = "load ogg : format error! ";
        buf += name;
        debugger_audio.writelog(1,buf);
        fclose(f);
        return false;
    }
    pInfo = ov_info(&oggFile, -1);
    if (pInfo->channels == 1)
        *format = AL_FORMAT_MONO16;
    else
        *format = AL_FORMAT_STEREO16;

    *freq = pInfo->rate;


    do
    {
        bytes = ov_read(&oggFile, array, BUFFER_SIZE, endian, 2, 1, &bitStream);

        if (bytes < 0)
        {
            ov_clear(&oggFile);
            fclose(f);
            buf = "load ogg : format error 2 ! ";
            buf += name;
            debugger_audio.writelog(1,buf);
            bufferData.clear();
            return 0;
        }

        bufferData.insert(bufferData.end(), array, array + bytes);
    } while (bytes > 0);
    ov_clear(&oggFile);
    fclose(f);
    *size = bufferData.size();
    int t = alGetError();

    if (t != AL_NO_ERROR)
    {
        buf = "alBufferData preparation error in load ogg! ";
        buf += name;
        buf += " ";
        buf += to_string(t);
        debugger_audio.writelog(1,buf);

    }
    alBufferData(*buffer, *format, &bufferData[0],
        *size, *freq);

    t = alGetError();
    if (t != AL_NO_ERROR)
    {
        buf = "alBufferData error in load ogg! ";
        buf += name;
        buf += " ";
        buf += to_string(t);
        debugger_audio.writelog(1,buf);
    }
    bufferData.clear();

    return true;
}



bool loadWavFile(const string filename, ALcoms* waveData) 
{
    if (waveData == nullptr)
    {
        return 0;
    }
    //Local Declarations
    FILE* soundFile = NULL;
    WAVE_Format wave_format;
    RIFF_Header riff_header;
    WAVE_Data wave_data;
    unsigned char* data;
    try {
        fopen_s(&soundFile, filename.c_str(), "rb");
        if (!soundFile)
            throw (filename);

        // Read in the first chunk into the struct
        fread(&riff_header, sizeof(RIFF_Header), 1, soundFile);
        //check for RIFF and WAVE tag in memeory
        if ((riff_header.chunkID[0] != 'R' ||
            riff_header.chunkID[1] != 'I' ||
            riff_header.chunkID[2] != 'F' ||
            riff_header.chunkID[3] != 'F') ||
            (riff_header.format[0] != 'W' ||
                riff_header.format[1] != 'A' ||
                riff_header.format[2] != 'V' ||
                riff_header.format[3] != 'E'))
            throw ("Invalid RIFF or WAVE Header");

        //Read in the 2nd chunk for the wave info
        fread(&wave_format, sizeof(WAVE_Format), 1, soundFile);
        
        //check for fmt tag in memory
        if (wave_format.subChunkID[0] != 'f' ||
            wave_format.subChunkID[1] != 'm' ||
            wave_format.subChunkID[2] != 't' ||
            wave_format.subChunkID[3] != ' ')
            throw ("Invalid Wave Format");
        
        //check for extra parameters;
        if (wave_format.subChunkSize > 16)
            fseek(soundFile, sizeof(short), SEEK_CUR);

        //Read in the the last byte of data before the sound file
        fread(&wave_data, sizeof(WAVE_Data), 1, soundFile);
        
        //check for data tag in memory
        if (wave_data.subChunkID[0] != 'd' ||
            wave_data.subChunkID[1] != 'a' ||
            wave_data.subChunkID[2] != 't' ||
            wave_data.subChunkID[3] != 'a')
            throw ("Invalid data header");

        //Allocate memory for data
        data = new unsigned char[wave_data.subChunk2Size];

        // Read in the sound data into the soundData variable
        if (!fread(data, wave_data.subChunk2Size, 1, soundFile))
            throw ("error loading WAVE data into struct!");

        //Now we set the variables that we passed in with the
        //data from the structs
        waveData->size2 = wave_data.subChunk2Size;
        waveData->freq = wave_format.sampleRate;
        //The format is worked out by looking at the number of
        //channels and the bits per sample.
        if (wave_format.numChannels == 1) {
            if (wave_format.bitsPerSample == 8)
                waveData->format = AL_FORMAT_MONO8;
            else if (wave_format.bitsPerSample == 16)
                waveData->format = AL_FORMAT_MONO16;
        }
        else if (wave_format.numChannels == 2) {
            if (wave_format.bitsPerSample == 8)
                waveData->format = AL_FORMAT_STEREO8;
            else if (wave_format.bitsPerSample == 16)
                waveData->format = AL_FORMAT_STEREO16;
        }

        //errorCheck();
        //now we put our data into the openAL buffer and
        //check for success

        alBufferData(waveData->bufferID, waveData->format, (void*)data,
            waveData->size2, waveData->freq);
        delete[] data;
        if (alGetError() != AL_NO_ERROR)
        {
            throw ("alBufferData error in load wav!");
        }

        //errorCheck();
        //clean up and return true if successful
        fclose(soundFile);
        return true;
    }
    catch (std::exception error) {
        //our catch statement for if we throw a string
        debugger_audio.writelog(1, "failed to load wave file: " + filename + " " + error.what());

        Sleep(100);
        //clean up memory if wave loading fails
        if (soundFile != NULL)
            fclose(soundFile);
        //return false to indicate the failure to load wave
        return false;
    }
}


unsigned __stdcall ThreadPlaySingleMusic(LPVOID lpParameter)
{
    
    g_am.thread_smusic_response = 1;
    int current_playing_music_s = g_am.current_playing_music;
    int t;
    //writelog("select music " + to_string(current_playing_music_s));
    if (!ALogg[current_playing_music_s].loaded)
    {
        alGenBuffers(1, &ALogg[current_playing_music_s].bufferID);

        alGenSources(1, &ALogg[current_playing_music_s].sourceID);
        t = alGetError();
        if (t != AL_NO_ERROR && t != AL_INVALID_NAME)    //bug?
        {
            debugger_audio.writelog(1,ALogg[current_playing_music_s].fileName + " alGenSources error in load ogg!  " + to_string(t));

        }

        if (!LoadOGG(ALogg[current_playing_music_s].fileName.c_str(), &ALogg[current_playing_music_s].bufferID, &ALogg[current_playing_music_s].freq, &ALogg[current_playing_music_s].format, &ALogg[current_playing_music_s].size2))
        {
            debugger_audio.writelog(1,"load oggfile failed");
            alDeleteSources(1, &ALogg[current_playing_music_s].sourceID);
            alDeleteBuffers(1, &ALogg[current_playing_music_s].bufferID);
            Sleep(1000);
            g_am.thread_smusic_response = 0;
            return 0;
        }
        t = alGetError();
        if (t != AL_NO_ERROR)
        {
            debugger_audio.writelog(1,"error in ThreadPlaySingleMusic!  " + to_string(t) + " " + ALogg[current_playing_music_s].fileName);
            alDeleteSources(1, &ALogg[current_playing_music_s].sourceID);
            alDeleteBuffers(1, &ALogg[current_playing_music_s].bufferID);
            Sleep(1000);
            g_am.thread_smusic_response = 0;
            return 0;
        }

        alSourcei(ALogg[current_playing_music_s].sourceID, AL_BUFFER, ALogg[current_playing_music_s].bufferID);
        t = alGetError();
        if (t != AL_NO_ERROR /*&& t != AL_INVALID_NAME*/)   //same bug?
        {
            debugger_audio.writelog(1,"AL_BUFFER error in ThreadPlaySingleMusic! "+ to_string(t));
        }
        ALogg[current_playing_music_s].loaded = 1;
    }

    if (ALogg[current_playing_music_s].size2 > 1000)
    {
        // Attach sound buffer to source
        alSourcef(ALogg[current_playing_music_s].sourceID, AL_GAIN, (float)set3[0].music_volume / 100.0F);  //音量
        if (g_am.isloop)
        {
            alSourcei(ALogg[current_playing_music_s].sourceID, AL_LOOPING, AL_TRUE);
        }
        else
        {
            alSourcei(ALogg[current_playing_music_s].sourceID, AL_LOOPING, AL_FALSE);
        }

        alSourcePlay(ALogg[current_playing_music_s].sourceID);
        //writelog("now playing  " + ALogg[current_playing_music_s].name);
    }
    else
    {
        debugger_audio.writelog(1,"Exception: bufferData size too low!   " + to_string(current_playing_music_s));
        alSourcei(ALogg[current_playing_music_s].sourceID, AL_BUFFER, NULL);
        alDeleteSources(1, &ALogg[current_playing_music_s].sourceID);
        alDeleteBuffers(1, &ALogg[current_playing_music_s].bufferID);
        Sleep(1000);
        g_am.thread_smusic_response = 0;
        ALogg[current_playing_music_s].loaded = 0;
        return 0;
    }

    while (1)
    {
        Sleep(100);
        if (g_am.thread_smusic_response == -1 || normal_quit||quit_single)
        {
            alSourceStop(ALogg[current_playing_music_s].sourceID);

            g_am.thread_smusic_response = 0;
            return 0;
        }
        if (g_am.thread_Audio_volume_changed_music)
        {
            alSourcef(ALogg[current_playing_music_s].sourceID, AL_GAIN, (float)set3[0].music_volume / 100.0F);  //音量

            g_am.thread_Audio_volume_changed_music = 0;
        }
        // Query the state of the souce
        alGetSourcei(ALogg[current_playing_music_s].sourceID, AL_SOURCE_STATE, &ALogg[current_playing_music_s].state);
        if (ALogg[current_playing_music_s].state == AL_STOPPED || g_am.thread_Audio_switch_immediately)
        {
            //writelog("check music target " + to_string(thread_Audio_target_music));
            g_am.thread_Audio_switch_immediately = 0;

            if (g_am.last_music_request == g_am.thread_Audio_target_music && g_am.thread_Audio_target_music != 1)
            {
                continue;
            }
            if (g_am.thread_Audio_target_music == 0)
            {
                if (ALogg[current_playing_music_s].state != AL_STOPPED)
                {
                    Sleep(500);
                    alSourceStop(ALogg[current_playing_music_s].sourceID);
                }
                continue;
            }
            g_am.last_music_request = g_am.thread_Audio_target_music;
            alSourceStop(ALogg[current_playing_music_s].sourceID);
            alSourcei(ALogg[current_playing_music_s].sourceID, AL_BUFFER, NULL);
            t = alGetError();
            if (t != AL_NO_ERROR)
            {
                debugger_audio.writelog(1,"AL_BUFFER error in ThreadPlaySingleMusic!"+ to_string(t));
            }
            alDeleteSources(1, &ALogg[current_playing_music_s].sourceID);
            t = alGetError();
            if (t != AL_NO_ERROR)
            {
                debugger_audio.writelog(1,"alDeleteSources error in ThreadPlaySingleMusic!" + to_string(t));
            }
            alDeleteBuffers(1, &ALogg[current_playing_music_s].bufferID);
            t = alGetError();
            if (t != AL_NO_ERROR)
            {
                debugger_audio.writelog(1,"alDeleteBuffers error in ThreadPlaySingleMusic!" + to_string(t));
            }
            ALogg[current_playing_music_s].loaded = 0;
            g_am.thread_smusic_response = 0;
            return 0;
        }

    }
}


unsigned __stdcall ThreadPlayMusic(LPVOID lpParameter)
{
    debugger_audio.setSurfix("[audio]");
    debugger_audio.writelog(0,"ThreadPlayMusic started.");

    srand((unsigned int)GetTickCount64());
    g_am.Init();
    
    musicinfo stage1[16], stage2[16], sys[5];
    int musicnum1 = 0, musicnum2 = 0;
    bool no_exmusic = 0;
    int alerr = 0;

    sys[1].name = ".\\music\\sys\\credit.ogg";
    sys[2].name = ".\\music\\sys\\atmosphere_rainy.ogg";

    //文件句柄
    long long hFile = 0;

    //文件信息 
    struct _finddata_t fileinfo;
    string p;
    if ((hFile = _findfirst(p.assign(".\\music\\lobby").append("\\*.ogg").c_str(), &fileinfo)) != -1)
    {
        do
        {
            if (!(fileinfo.attrib & _A_SUBDIR))
            {
                musicnum1++;
                stage1[musicnum1 - 1].name = ".\\music\\lobby\\";
                stage1[musicnum1 - 1].name += fileinfo.name;
                if (musicnum1 >= 16)
                {
                    break;
                }
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }
    /*hFile = 0;
    p = ".\\music\\battle\\*.ogg";
    memset(&fileinfo, 0, sizeof(fileinfo));
    if ((hFile = _findfirst(p.c_str(), &fileinfo)) != -1)
    {
        do
        {
            if (!(fileinfo.attrib & _A_SUBDIR))
            {
                musicnum2++;
                stage2[musicnum2 - 1].name = ".\\music\\battle\\";
                stage2[musicnum2 - 1].name += fileinfo.name;
                if (musicnum2 >= 16)
                {
                    break;
                }
            }
        } while (_findnext(hFile, &fileinfo) == 0);
        _findclose(hFile);
    }*/



    debugger_audio.writelog(0,"found " + to_string(musicnum1+ musicnum2) + " external music file.");
    
    ALogg[1].fileName = sys[1].name;
    ALogg[2].fileName = sys[2].name;


    int curnum = 1;
    int mainpagemusic, historypagemusic;

    if (musicnum1 > 0)
    {
        for (int i = 0; i < musicnum1; i++)
        {
            ALogg[3 + i].fileName = stage1[i].name;
        }
        mainpagemusic = rand() % musicnum1 + 3;
        sys[3].name = ALogg[mainpagemusic].fileName;
        //writelog(ALogg[mainpagemusic].name);
    }
    else
    {
        no_exmusic = 1;
        mainpagemusic = -1;
    }
    historypagemusic = mainpagemusic;
    curnum = mainpagemusic;
    
    
    
    

    int oggnum = 1;

    g_am.thread_smusic_response = 0;
    HANDLE hThreadsAudio;

    //thread_smusic_response = 1;
    while (md5_result != 1)
    {
        Sleep(1);
        if (md5_result == -1)
        {
            debugger_audio.writelog(1,"Audio thread quit.(verification error)");
            return 0;
        }
        if (normal_quit || quit_single)
        {
            debugger_audio.writelog(1,"Audio thread quit.(quit_single)");
            return 0;
        }
    }
    
    alDistanceModel(AL_INVERSE_DISTANCE);

    int current_playing_wav = 0;
    //bool dbg_wav_wait = 0;
    int threadfailed = 0;

    //audio loop
    while (1)
    {
        if (g_output && set3[0].thread_method == 0)
        {
            g_output->WaitForVBlank();
        }
        else if (set3[0].thread_method == 2)
        {
            Sleep(0);
        }
        else
        {
            Sleep(1);
        }

        if (g_am.audio_latency_update_count >= 0)
        {
            if (g_am.pAvgLatency.use_count() == 0)
            {
                debugger_audio.writelog(DWARNNING, "(weak_ptr)pAvgLatency use_count=0 in latency detection", __LINE__);
                g_am.audio_latency_update_count = -1;
            }
            else if (g_am.audio_latency_update_count == 10)
            {
                *g_am.pAvgLatency.lock() = 0;
                g_am.timer.start_timer();
            }
            else if (g_am.audio_latency_update_count == 0)
            {
                *g_am.pAvgLatency.lock() += g_am.timer.stop_timer();
                *g_am.pAvgLatency.lock() *= 0.1f;
            }
            else
            {
                *g_am.pAvgLatency.lock() += g_am.timer.stop_timer();
                g_am.timer.start_timer();
            }
            g_am.audio_latency_update_count--;
        }
        
        if (normal_quit || g_am.thread_Audio_quit_single || quit_single)
        {
            
            g_am.thread_smusic_response = -1;
            
            //g_am.Cleanup();
            return 0;
        }

 
        if (g_am.thread_Audio_volume_changed)
        {
            unordered_map<string, shared_ptr<SE_INFO>> audioMap = g_rm.getAudioMap();
            for (auto i : audioMap)
            {
                g_am.ApplySEVolumeChange(i.second);
            }
        }

        g_am.updateSE();

        if (threadfailed == 8)
        {
            debugger_audio.writelog(2,"stop trying to create audio thread!");
        }
        if (g_am.thread_smusic_response == 0 && threadfailed < 8)
        {
            if (g_am.thread_Audio_target_music == BGM_INGAME)//in game
            {
                if (!no_exmusic)
                {
                    g_am.isloop = 0;

                    oggnum = rand() % musicnum1;
                    while (oggnum == g_am.current_playing_music && musicnum1 > 1)
                    {
                        oggnum = rand() % musicnum1;
                    }
                    //musicpathbuf = stage1[oggnum].name;

                    curnum = oggnum + 3;
                }
                else
                {
                    curnum = -1;
                }
            }
            else if (g_am.thread_Audio_target_music == BGM_HOME)//mainpage
            {
                g_am.isloop = 1;
                curnum = mainpagemusic;
                //musicpathbuf = sys[1].name;
            }
            else if (g_am.thread_Audio_target_music == 6)
            {
                g_am.isloop = 1;
                curnum = 2;
                //musicpathbuf = sys[9].name;

            }
            else
            {
                debugger_audio.writelog(DWARNNING, "invalid thread_Audio_target_music in sAudio thread! "+to_string(g_am.thread_Audio_target_music));
            }
            g_am.current_playing_music = curnum;

            if (curnum != -1)
            {
                g_am.thread_smusic_response = 1;
                hThreadsAudio = (HANDLE)_beginthreadex(NULL, 0, ThreadPlaySingleMusic, NULL, 0, NULL);	//创建线程
                if (hThreadsAudio == 0)
                {
                    debugger_audio.writelog(1,"Failed to create sAudio thread!");
                    threadfailed++;
                    g_am.thread_smusic_response = 0;
                }
            }
        }
    }

    return 0;
}


//音频线程部分结束