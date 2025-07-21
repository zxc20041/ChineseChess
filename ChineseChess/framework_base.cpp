#include"framework_pages.h"
#include"framework_base.h"
#include "thread_communicate.h"
#include"settings.h"
#include "audio_thread.h"

//#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "dwrite.lib")
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment (lib ,"imm32.lib")

#pragma comment(lib,"libvorbisfile_static.lib")
#pragma comment(lib,"libogg.lib")
#pragma comment(lib,"libvorbis_static.lib")
#pragma comment(lib,"OpenAL32.lib")
#pragma comment(lib, "openssl/libcrypto_static.lib")
using namespace std;
using namespace debugger;

SYSTEMTIME st;
bool quit_single = 0, normal_quit = 0;
IDXGIOutput* g_output = NULL;
HIMC g_hIMC = NULL;
string usernameC = "";  //default

int md5_result = 0;
int page_index = -1, page_status = 0;
bool self_restarted = 0;
int ed_stage = 0;
bool long_credit = 1;

int tip_used_num = 0;
float frmtm = 0;    //frametime
float scale = 1;    //rendering scale
bool clicking = 0;
bool rightclick = 0;

const char* filename_dbg = "debug.log";
string tips[16] = { "" };
Resource res[128];

Read_info record;
debug_ex debugger_main(1);
ComponentManager g_cm;
ResourceManager g_rm;
PageManager g_PageManager;


ID2D1Factory2* g_pD2DFactory = NULL;	// Direct2D factory

ID2D1SolidColorBrush* g_pBrushBlack = NULL;	// A brush
ID2D1SolidColorBrush* g_pBrushQuit = NULL;
ID2D1SolidColorBrush* g_pBrushGray = NULL;
ID2D1SolidColorBrush* g_pBrushLightGray = NULL;
ID2D1SolidColorBrush* g_pBrushBlue = NULL;
ID2D1SolidColorBrush* g_pBrushDarkBlue = NULL;
ID2D1SolidColorBrush* g_pBrushLightBlue = NULL;
ID2D1SolidColorBrush* g_pBrushYellow = NULL;
ID2D1SolidColorBrush* g_pBrushLightYellow = NULL;
ID2D1SolidColorBrush* g_pBrushFloralWhite = NULL;
ID2D1SolidColorBrush* g_pBrushGreen = NULL;
ID2D1SolidColorBrush* g_pBrushLightGreen = NULL;
//ID2D1SolidColorBrush* g_pBrushtext = NULL;
ID2D1SolidColorBrush* g_pBrushRed = NULL;
ID2D1SolidColorBrush* g_pBrushPurple = NULL;
ID2D1SolidColorBrush* g_pBrushBrown = NULL;
ID2D1SolidColorBrush* BrushRand[8] = { NULL };
ID2D1SolidColorBrush* g_pBrushBGSelect = NULL;
ID2D1SolidColorBrush* g_pBrushLight = NULL;
ID2D1SolidColorBrush* g_pBrushDark = NULL;
ID2D1SolidColorBrush* g_pBrushPink = NULL;
ID2D1SolidColorBrush* g_pBrushWhite = NULL;
ID2D1SolidColorBrush* g_pBrushNetwork = NULL;
ID2D1SolidColorBrush* g_pBrushGameTemp = NULL;

ID2D1LinearGradientBrush* g_pLinearGradientBrush = NULL;
ID2D1RadialGradientBrush* g_pRadialGradientBrush = NULL;
ID2D1RadialGradientBrush* g_pRadialGradientBrush2 = NULL;
ID2D1RadialGradientBrush* g_pRadialGradientBrush3 = NULL;

ID2D1BitmapBrush1* g_pBitmapBrushUI[20] = { NULL };

IDWriteFactory* g_pDWriteFactory = NULL;

IDWriteTextFormat* g_pTextFormat = NULL;
IDWriteTextFormat* g_pTextFormatLarge = NULL;
IDWriteTextFormat* g_pTextFormatL = NULL;
IDWriteTextFormat* g_pTextFormatmini = NULL;
IDWriteTextFormat* g_pTextFormatminiL = NULL; 
IDWriteTextFormat* g_pTextFormatQuarlowLarge = NULL;
IDWriteTextFormat* g_pTextFormat2 = NULL;
IDWriteTextFormat* g_pTextFormatNormal = NULL;
IDWriteTextFormat* g_pTextFormatNormalLarge = NULL;
IDWriteTextFormat* g_pTextFormatNormalmini = NULL;
IDWriteTextFormat* g_pTextFormatNetwork = NULL;
IDWriteTextFormat* g_pTextFormatLargeCN = NULL;
IDWriteTextFormat* g_pTextFormat_debug = NULL;
IDWriteTextFormat* g_pTextFormatGame1 = NULL;
IDWriteTextFormat* g_pTextFormatGame2 = NULL;

ID2D1StrokeStyle* g_pStrokeStyle;

ID2D1Effect* gaussianBlurEffect = NULL;
ID2D1Effect* OpacityEffect = NULL;
ID2D1Bitmap1* gaussianBlur_Bitmap1 = NULL;
ID2D1Bitmap1* shortcut_Bitmap = NULL;
ID2D1Bitmap1* content_blur_Bitmap = NULL;

ID2D1Bitmap1* g_pD2DTargetBimtap = NULL;
ID2D1Bitmap* g_pD2DBimtapUI[64] = { nullptr };

ID2D1Bitmap1* copy_succeed_tagline = NULL, * copy_failed_tagline = NULL;
ID2D1Bitmap1* io_error_tagline = NULL, * file_corrupt_tagline = NULL;

ID2D1Bitmap1* numberBitmaps;

IWICImagingFactory* pIWICFactory = NULL;
ID2D1Device1* g_pD2DDevice = NULL;
ID2D1DeviceContext1* g_pD2DDeviceContext;
//DXGI_ADAPTER_DESC Adapter;
D3D_FEATURE_LEVEL m_featureLevel;
ID3D11Device* pD3DDevice = NULL;
ID3D11DeviceContext* pD3DDeviceImmediateContext = NULL;

IDXGIDevice1* pDxgiDevice = NULL;
DXGI_SWAP_CHAIN_DESC1 sd;
IDXGIAdapter* pDxgiAdapter = NULL;
IDXGIFactory2* g_pDxgiFactory = NULL;
IDXGISwapChain1* g_pSwapChain = NULL;
IDXGISurface* pDxgiBackBuffer = nullptr;


Clock::time_point t_start, t_end;

DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc;
D2D1_BITMAP_PROPERTIES1 targetbitmapProperties = D2D1::BitmapProperties1(
    D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
    D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
);

RECT rc;
D2D1_RECT_F fpsRect, textrbRect;
UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
UINT sampleCountOut = 1;

//RECT windowrect;
int DrawCallNum = 0;
int fps = 0;
int dur20 = 0, nframe = 0, durnanosec = 0;
int orix = 0, oriy = 0;
int nWidth = GetSystemMetrics(SM_CXSCREEN);  //屏幕宽度    
int nHeight = GetSystemMetrics(SM_CYSCREEN); //屏幕高度
bool mEnable4xMsaa = 1;
bool input_expand = 0;
char expand_input_key = NULL;
bool keyw = 0, keya = 0, keys = 0, keyd = 0;
bool keyTab = 0, keyESC = 0, keyComb = 0, keySpace = 0;

bool rendUIboxes = 0;


debug_ex::debug_ex(bool enable)
{
    osd_enable = enable;
    on_screen_output_content.clear();
    log_surfix.clear();
    for (int i = 0; i < 16; i++)
    {
        tagline_content[i].clear();
        tagline_time_remain[i] = -1;
    }
    return;
}

void debug_ex::setSurfix(string surfix)
{
    log_surfix = surfix;
    return;
}

void debug_ex::reset()
{
    on_screen_output_content.clear();
    return;
}

void debug_ex::add_output_continuous(string content)
{
    on_screen_output_content += content;
    return;
}

void debug_ex::add_output_line(string content)
{
    on_screen_output_content += content;
    on_screen_output_content += "\n";
    return;
}

void debug_ex::add_output_space(string content)
{
    on_screen_output_content += content;
    on_screen_output_content += " ";
    return;
}

void debug_ex::add_tagline(string content)
{
    for (int i = 0; i < 16; i++)
    {
        if (tagline_time_remain[i] < 0)
        {
            tagline_content[i] = content + "\n";
            tagline_time_remain[i] = 3;
            break;
        }
        if (i == 15)
        {
            tagline_content[0] = content + "\n";
            tagline_time_remain[0] = 3;
        }
    }
    return;
}

void debug_ex::set_osd_enable(bool enable)
{
    osd_enable = enable;
    return;
}

void debug_ex::writelog(int type, string content,int line)
{
    if (normal_quit || quit_single)
    {
        return;
    }
    if (type == DERROR)
    {
        normal_quit = 1;
    }
#if not defined(_DEBUG)
    else if (type == DDEBUG)
    {
        return;
    }
#endif
    for (int i = 0; i < logbuf_length; i++)
    {
        if (logbuf[i].empty())
        {
            logbuf[i] = log_surfix;
            switch (type)
            {
            case DINFO:
                logbuf[i] += "[Info] ";
                break;
            case DWARNNING:
                logbuf[i] += "[Warning] ";
                break;
            case -1:
            case DERROR:
                logbuf[i] += "[Error] (page_index="+to_string(page_index)+" page_status=" + to_string(page_status)+") ";
                break;
            case DDEBUG:
                logbuf[i] += "[Debug] (" + g_PageManager.GetCurrentPageTag() + ") ";
                break;
            default:
                logbuf[i] += "[Unknown] ";
                break;
            }
            if (line != 0)
            {
                logbuf[i] += "[line " + to_string(line) + "] ";
            }
            logbuf[i] += content;
            break;
        }
    }

    return;
}

void debug_ex::rend()
{
    for (int i = 0; i < 16; i++)
    {
        if (tagline_time_remain[i] > 0)
        {
            tagline_time_remain[i] -= frmtm;
            on_screen_output_content += tagline_content[i];
        }
    }
    if (!osd_enable)
    {
        return;
    }
    DrawTextA_1(on_screen_output_content, g_pTextFormat_debug, D2D1::RectF(to_screen(50), to_screen(100), to_screen(800), to_screen(900)), g_pBrushGray);
    return;
}


void writelog(string content)
{
    debugger_main.writelog(1, content);
    return;
}

void writelog(int type, string content)
{
    debugger_main.writelog(type, content);
    return;
}

ResourceManager::ResourceManager()
{
    for (int i = 0; i < TEXT_MAX; i++)
    {
        texts[i] = nullptr;
    }
    for (int i = 0; i < RES_MAX; i++)
    {
        res_info[i] = nullptr;
    }
    res_num = 0;
    releaseAllSignal.store(0);
    loadAllSignal.store(0);
}

ResourceManager::~ResourceManager()
{
    for (auto &i : texts)
    {
        if (i != nullptr)
        {
            delete i;
        }
    }
    for (auto &i : res_info)
    {
        if (i != nullptr)
        {
            delete i;
        }
    }
}

void ResourceManager::AddResource(string aliasName, string filePath, ResourceType type, string md5, TEXTURE_DESC texture_desc, FONT_DESC font_desc, TEXT_DESC text_desc, void* func)
{
    for (int i = 0; i < res_num; i++)
    {
        if (res_info[i]->aliasName == aliasName&& type!= ResourceType::Resource_Font)
        {
            debugger_main.writelog(1, "Repeated resource aliasName: " + aliasName);
            return;
        }
    }
    res_info[res_num] = new RESOURCE_INFO(aliasName, filePath, type, md5, texture_desc, font_desc, text_desc, func);
    res_num++;
    
    //prepare res
    if (type == ResourceType::Resource_Texture)
    {

    }
    else if (type == ResourceType::Resource_Font)
    {
        int font_num = 0;
        font_num = AddFontResourceA(filePath.c_str());  //添加字体文件
        //判断字体文件是否可用
        if (font_num <= 0)
        {
            debugger_main.writelog(DWARNNING, "Load Font Resource failed! " + filePath, __LINE__);
        }
    }
    else if (type == ResourceType::Resource_Text)
    {

    }
    return;
}

void ResourceManager::LoadAll()
{
    loadAllSignal.store(1);
    return;
}

void ResourceManager::ReleaseAll()
{
    releaseAllSignal.store(1);
    return;
}

void ResourceManager::FinishLoading()
{
    while (loadAllSignal.load() == 1);
    return;
}

bool ResourceManager::GetFinishLoading()
{
    return loadAllSignal.load() == 0;
}

void ResourceManager::AddThread(string aliasName, void* func)
{
    AddResource(aliasName, "", ResourceType::Resource_Handle, "", TEXTURE_DESC(), FONT_DESC(), TEXT_DESC(), func);
}

ID2D1Bitmap* ResourceManager::getTexture(string aliasName)
{
    static int notFoundNum = 0;
    if (textureMap.find(aliasName) == textureMap.end() && notFoundNum < 3)
    {
        notFoundNum++;
        debugger_main.writelog(2, "res not found in ResourceManager::getTexture() " + aliasName);
        return nullptr;
    }
    return textureMap[aliasName];
}

ID2D1BitmapBrush1* ResourceManager::getBrush(string aliasName)
{
    static int notFoundNum = 0;
    if (brushMap.find(aliasName) == brushMap.end() && notFoundNum < 3)
    {
        notFoundNum++;
        debugger_main.writelog(2, "res not found in ResourceManager::getBrush() " + aliasName);
        return nullptr;
    }
    return brushMap[aliasName];
}

int ResourceManager::getAudioIndex(string aliasName)
{
    static int notFoundNum = 0;
    if (audioMap.find(aliasName) == audioMap.end() && notFoundNum < 3)
    {
        notFoundNum++;
        debugger_main.writelog(1, "res not found in ResourceManager::getAudioIndex() " + aliasName);
        return 0;
    }
    return audioMap[aliasName];
}

IDWriteTextFormat* ResourceManager::getFont(string aliasName)
{
    static int notFoundNum = 0;
    if (fontMap.find(aliasName) == fontMap.end() && notFoundNum < 3)
    {
        notFoundNum++;
        debugger_main.writelog(DWARNNING, "res not found in ResourceManager::getFont() " + aliasName);
        return fontMap["default"];
    }
    return fontMap[aliasName];
}

int ResourceManager::getFontIndex(string aliasName)
{
    static int notFoundNum = 0;
    if (fontIndexMap.find(aliasName) == fontIndexMap.end() && notFoundNum < 3)
    {
        notFoundNum++;
        debugger_main.writelog(DWARNNING, "res not found in ResourceManager::getFontIndex() " + aliasName);
        return 0;
    }
    return fontIndexMap[aliasName];
}

TEXT_RESOURCE* ResourceManager::getText(string aliasName)
{
    if (textMap.find(aliasName) == textMap.end())
    {
        debugger_main.writelog(DWARNNING, "res not found in ResourceManager::getText() " + aliasName);
        return nullptr;
    }
    return textMap[aliasName];
}

void ResourceManager::setFontIndex(string aliasName, int index)
{
    fontIndexMap[aliasName] = index;
    return;
}

void ResourceManager::setFont(string aliasName, IDWriteTextFormat* font)
{
    fontMap[aliasName] = font;
    return;
}

bool ResourceManager::verifyFile(RESOURCE_INFO* res)
{
    if (res == nullptr)
    {
        debugger_main.writelog(DWARNNING, "res nullptr in ResourceManager::verifyFile()", __LINE__);
        return 0;
    }
    if (res->md5 == "pass")
    {
        return 1;
    }

    
    return md5_verify_implementation(res->filePath, res->md5);
}

bool ResourceManager::md5_verify_implementation(string filename, string md5)
{
    return md5_verify(filename.c_str(), md5.c_str());
}

bool ResourceManager::LoadAll_implementation()
{
    debugger_main.writelog(DDEBUG, "IN ResourceManager::LoadAll_implementation()", __LINE__);
    for (auto& i : res_info)
    {
        if (i == nullptr)
        {
            continue;
        }
        if (i->loaded)
        {
            continue;
        }
        if (i->type == ResourceType::Resource_Handle)
        {
            HANDLE newHandle = (HANDLE)_beginthreadex(NULL, 0, (_beginthreadex_proc_type)i->funcAddr, NULL, 0, NULL);
            if (newHandle == 0)
            {
                debugger_main.writelog(DWARNNING, "_beginthreadex failed!  " + i->aliasName, __LINE__);
                return 0;
            }
            handleMap[i->aliasName] = newHandle;
            i->loaded = 1;
            continue;
        }
        if (!verifyFile(i))
        {
            debugger_main.writelog(DWARNNING, "verifyFile failed: " + i->filePath, __LINE__);
            //set_target_page(PAGE_VERIFY_FAILED, 0, 0);
            return 0;
        }
        if (i->type == ResourceType::Resource_Texture)
        {
            ID2D1Bitmap* pBitmap = nullptr;
            IWICBitmapSource* pWicBitmap = nullptr;
            HRESULT hr = LoadBitmapFromFile(pIWICFactory, stringToLPCWSTR(i->filePath), i->textureDesc.width, i->textureDesc.height, &pBitmap, i->textureDesc.save_WicBitmapSource ? &pWicBitmap : nullptr);
            if (hr != S_OK || pBitmap == nullptr)
            {
                debugger_main.writelog(DWARNNING, "Load Bitmap From File failed!  " + i->filePath, __LINE__);
                return 0;
            }
            
            if (i->textureDesc.save_WicBitmapSource)
            {
                if (pWicBitmap == nullptr)
                {
                    debugger_main.writelog(DWARNNING, "Load IWICBitmapSource From File failed!  " + i->filePath, __LINE__);
                    return 0;
                }
                WICtextureMap[i->aliasName] = pWicBitmap;
            }
            if (i->textureDesc.save_Brush)
            {
                D2D1_BITMAP_BRUSH_PROPERTIES1 BITMAP_BRUSH_PROPERTY1;
                BITMAP_BRUSH_PROPERTY1.extendModeX = D2D1_EXTEND_MODE_MIRROR;
                BITMAP_BRUSH_PROPERTY1.extendModeY = D2D1_EXTEND_MODE_MIRROR;
                if (set2[0].MSAA == 2)
                {
                    BITMAP_BRUSH_PROPERTY1.interpolationMode = D2D1_INTERPOLATION_MODE_ANISOTROPIC;
                }
                else if (set2[0].MSAA == 1)
                {
                    BITMAP_BRUSH_PROPERTY1.interpolationMode = D2D1_INTERPOLATION_MODE_LINEAR;
                }
                else
                {
                    BITMAP_BRUSH_PROPERTY1.interpolationMode = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
                }
                ID2D1BitmapBrush1* g_pBitmapBrush = nullptr;
                hr = g_pD2DDeviceContext->CreateBitmapBrush(pBitmap, BITMAP_BRUSH_PROPERTY1, &g_pBitmapBrush);
                if (hr != S_OK || g_pBitmapBrush == nullptr)
                {
                    debugger_main.writelog(DWARNNING, "CreateBitmapBrush From File failed!  " + i->filePath, __LINE__);
                    return 0;
                }
                brushMap[i->aliasName] = g_pBitmapBrush;
            }
            if (!i->textureDesc.save_Bitmap)
            {
                pBitmap->Release();
            }
            else
            {
                textureMap[i->aliasName] = pBitmap;
            }
        }
        else if (i->type == ResourceType::Resource_Font)
        {
            IDWriteTextFormat* textFormat = nullptr;

            HRESULT hr = g_pDWriteFactory->CreateTextFormat(
                stringToLPCWSTR(i->fontDesc.FontFamilyName),
                NULL,
                DWRITE_FONT_WEIGHT_REGULAR,
                DWRITE_FONT_STYLE_NORMAL,
                DWRITE_FONT_STRETCH_NORMAL,
                to_screen(i->fontDesc.fontSize),
                L"en-us",
                &textFormat
            );

            if (hr != S_OK || textFormat == nullptr)
            {
                debugger_main.writelog(DWARNNING, "CreateTextFormat failed!  " + i->filePath, __LINE__);
                return 0;
            }

            fontMap[i->aliasName] = textFormat;
        }
        else if (i->type == ResourceType::Resource_Text)
        {
            string *readbuf = new string[i->textDesc.lines_max];
            int lines = ReadFile_1(i->filePath.c_str(), readbuf, i->textDesc.lines_max);
            if (lines <= 0)
            {
                debugger_main.writelog(DWARNNING, "Read text res failed!  " + i->filePath, __LINE__);
                return 0;
            }
            TEXT_RESOURCE* text = new TEXT_RESOURCE();
            text->lines = lines;
            text->content = readbuf;
            textMap[i->aliasName] = text;
        }
        i->loaded = 1;
    }
    for (auto& i : res_info)
    {
        if (i == nullptr || !i->loaded)
        {
            continue;
        }
        delete i;
        i = nullptr;
    }
    debugger_main.writelog(DDEBUG, "finishing ResourceManager::LoadAll_implementation()", __LINE__);
    return 1;
}

void ResourceManager::releaseAll_implementation()
{
    for (auto& i : res_info)
    {
        if (i == nullptr)
        {
            continue;
        }
        delete i;
        i = nullptr;
    }
    for (auto& i : textureMap)
    {
        if (i.second == nullptr)
        {
            continue;
        }
        i.second->Release();
    }
    textureMap.clear();

    for (auto& i : WICtextureMap)
    {
        if (i.second == nullptr)
        {
            continue;
        }
        i.second->Release();
    }
    WICtextureMap.clear();

    for (auto& i : brushMap)
    {
        if (i.second == nullptr)
        {
            continue;
        }
        i.second->Release();
    }
    brushMap.clear();

    /*for (auto& i : fontMap)
    {
        if (i.second == nullptr)
        {
            continue;
        }
        i.second->Release();
    }
    fontMap.clear();*/

    for (auto& i : textMap)
    {
        if (i.second == nullptr)
        {
            continue;
        }
        delete i.second;
        i.second = nullptr;
    }
    textMap.clear();

    res_num = 0;
    return;
}

Box::Box(float x1, float y1, float x2, float y2)
{
    active = 1;
    this->x1= x1, this->x2= x2, this->y1= y1, this->y2= y2;
    //debugger_main.writelog(DDEBUG, "box rect= " + to_string(x1) + ", " + to_string(y1) + ", " + to_string(x2) + ", " + to_string(y2));
    //聚焦
    focus = 0;
    //单击
    clicked = 0;
    click_status = 0;
    rightclick_status = 0;
    //右键
    right_clicked = 0;
    clicked_time = 0, rightclicked_time = 0;
    doubleclick_status = 0;
    //双键
    double_clicked = 0;
    lr_click_status_right = 0;
    return;
}

void Box::check()
{
    debugger_main.add_output_line("box rect= " + to_string(x1) + ", " + to_string(y1) + ", " + to_string(x2) + ", " + to_string(y2));
    if (!active)
    {
        return;
    }
    if (doubleclick_status)
    {
        clicked_time -= frmtm;
        if (clicked_time < 0)
        {
            doubleclick_status = 0;
        }
    }
    if (lr_click_status_right)
    {
        rightclicked_time -= frmtm;
        if (rightclicked_time < 0)
        {
            lr_click_status_right = 0;
        }
    }
    if (doubleclick_status && lr_click_status_right)
    {
        double_clicked = 1;
        doubleclick_status = 0;
        lr_click_status_right = 0;
    }
    if (cpos.x > x1 && cpos.x < x2 && cpos.y >y1 && cpos.y <y2)
    {
        focus = 1;

        if (click_status == 0 && clicking)
        {
            click_status = 1;

            g_am.playEffectSound(1);
        }
        else if (click_status == 1 && !clicking)
        {
            click_status = 0;
            clicked = 1;

            g_am.playEffectSound(2);


            if (doubleclick_status)
            {
                double_clicked = 1;
            }
            else
            {
                clicked_time = 0.3f;
            }
            doubleclick_status = !doubleclick_status;
            if (page_index == 1)
            {
                thread_IO_request_save_config = 1;
            }

        }
        if (rightclick_status == 0 && rightclick)
        {
            rightclick_status = 1;
        }
        else if (rightclick_status == 1 && !rightclick)
        {
            rightclick_status = 0;
            right_clicked = 1;
            rightclicked_time = 0.3f;
            lr_click_status_right = 1;
        }
    }
    else
    {
        focus = 0;
        click_status = 0;
        rightclick_status = 0;
        doubleclick_status = 0;
    }
    return;
}

void Box::deactivate()
{
    focus = 0;
    click_status = 0;
    rightclick_status = 0;
    doubleclick_status = 0;
    clicked = 0;
    right_clicked = 0;
    double_clicked = 0;

    active = 0;
    return;
}
void Box::activate()
{
    active = 1;
    return;
}

void Box::reset_click_status()
{
    clicked = 0;
    double_clicked = 0;
    right_clicked = 0;
    return;
}

bool Box::getClicked()
{
    if (!active)
    {
        return 0;
    }
    return clicked;
}
void Box::setClicked()
{
    clicked = 1;
    return;
}
bool Box::getRightClicked()
{
    if (!active)
    {
        return 0;
    }
    return right_clicked;
}
bool Box::getDoubleClicked()
{
    if (!active)
    {
        return 0;
    }
    return double_clicked;
}

bool Box::getClickStatus()
{
    return click_status;
}

bool Box::getFocus()
{
    return focus;
}


void Box::setBounds(float x1, float y1, float x2, float y2)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    return;
}


LABEL::LABEL(float x1, float y1, float x2, float y2, string text,
    ID2D1SolidColorBrush* Brush1,
    ID2D1SolidColorBrush* Brush2,
    ID2D1SolidColorBrush* Brush3,
    ID2D1Bitmap* Bitmap,
    int TextFormat)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    this->text = text;
    this->Brush1 = Brush1;
    this->Brush2 = Brush2;
    this->Brush3 = Brush3;
    this->TextFormat = TextFormat;
    this->Bitmap = Bitmap;
    show = 1;
    ui_type = 0;
}


void LABEL::setBound(float x1, float y1, float x2, float y2)
{
    this->x1 = x1;
    this->x2 = x2;
    this->y1 = y1;
    this->y2 = y2;
    return;
}

void LABEL::setText(string text)
{
    this->text = text;
    return;
}
string LABEL::getText()
{
    return text;
}
void LABEL::setBrush1(ID2D1SolidColorBrush* Brush)
{
    this->Brush1 = Brush;
    return;
}
void LABEL::setBrush2(ID2D1SolidColorBrush* Brush)
{
    this->Brush2 = Brush;
    return;
}
void LABEL::setBrush3(ID2D1SolidColorBrush* Brush)
{
    this->Brush3 = Brush;
    return;
}

void LABEL::getBound(float* x1, float* y1, float* x2, float* y2)
{
    *x1 = this->x1;
    *x2 = this->x2;
    *y1 = this->y1;
    *y2 = this->y2;
    return;
}

void LABEL::setUItype(int ui_type)
{
    this->ui_type = ui_type;
    return;
}

void LABEL::setBitmap(ID2D1Bitmap* Bitmap)
{
    this->Bitmap = Bitmap;
}

void LABEL::setShow(bool show)
{
    this->show = show;
    return;
}


void LABEL::rend()
{
    if (!show)
    {
        return;
    }
    if (Brush2 != NULL)
    {
        FillRoundedRectangle_1(x1, y1, x2, y2, Brush2, 0.6f);
        FillRoundedRectangle_1(x1, y1 * 0.1f + y2 * 0.9f, x2, y2, Brush2, 0.6f);
        FillRoundedRectangle_1(x1, y1 * 0.05f + y2 * 0.95f, x2, y2, Brush2, 0.6f);
    }

    if (Bitmap != NULL)
    {
        //Brush3->SetOpacity(0.4F);
        DrawBitmap_1(Bitmap,D2D1::RectF((x1 + x2 + y1 - y2) / 2, y1, (x1 + x2 - y1 + y2) / 2, y2),0.8f);
    }

    if (Brush1 != NULL)
    {
        DrawRoundedRectangle_1(x1, y1, x2, y2, 3.0f,Brush1, 1.0f);
    }
    IDWriteTextFormat* g_pTextFormat_T = g_pTextFormat;
    if (TextFormat == 0)
    {
        g_pTextFormat_T = g_pTextFormatL;
    }
    else if (TextFormat == 1)
    {
        g_pTextFormat_T = g_pTextFormatminiL;
    }
    else if (TextFormat == 2)
    {
        g_pTextFormat_T = g_pTextFormat2;
    }
    else if (TextFormat == 3)
    {
        g_pTextFormat_T = g_pTextFormatNormal;
    }
    else if (TextFormat == 4)
    {
        g_pTextFormat_T = g_pTextFormatmini;
    }
    else if (TextFormat == 5)
    {
        g_pTextFormat_T = g_pTextFormat;
    }
    if (!text.empty() && Brush3 != NULL)
    {
        if (TextFormat == 6)
        {
            g_pTextFormat_T = g_pTextFormat;
            DrawTextA_1(text, g_pTextFormat_T,x1 * 0.9f + x2 * 0.1f,y1,x1 * 0.1f + x2 * 0.9f,y2,g_pRadialGradientBrush);
            DrawTextA_1(text, g_pTextFormat_T,x1 * 0.9f + x2 * 0.1f,y1,x1 * 0.1f + x2 * 0.9f,y2,g_pBrushDark);
        }
        else
        {
            DrawTextA_1(text,g_pTextFormat_T,x1 * 0.9f + x2 * 0.1f,y1,x1 * 0.1f + x2 * 0.9f,y2,Brush3);
        }

        //Brush3->SetOpacity(1.0F);

    }
    return;
}

Button::Button(float x1, float y1, float x2, float y2, string text,
    ID2D1SolidColorBrush* Brush1,
    ID2D1SolidColorBrush* Brush2,
    ID2D1SolidColorBrush* Brush3,
    ID2D1Bitmap* Bitmap,
    int TextFormat) : LABEL(x1, y1, x2, y2,text,Brush1,Brush2,Brush3,Bitmap,TextFormat)
{
    ori_x1 = x1;
    ori_x2 = x2;
    ori_y1 = y1;
    ori_y2 = y2;
    
    Brush2_ori = Brush2;
    
    pos_move_status = 0;
    
    //buttons[k].box_index = SetBox(x1, x2, y1, y2);
    box = new Box(x1, y1, x2, y2);
    active = 1;
    return;
}

Button::~Button()
{
    delete box;
    return;
}

void Button::activate()
{
    active = 1;
    box->activate();
    setShow(1);
    return;
}

void Button::setBrush2(ID2D1SolidColorBrush* Brush)
{
    Brush2_ori = Brush;
    return;
}

void Button::deactivate()
{
    x1 = ori_x1;
    x2 = ori_x2;
    y1 = ori_y1;
    y2 = ori_y2;
    box->deactivate();
    active = 0;
    return;
}

void Button::ui_update()
{
    if (ui_type == UI_TYPE_BUTTON_STATIC)
    {
        return;
    }
    constexpr int UImoveSpeed = 120;
    constexpr float UIzoomSize = 1.06f;
    if (box->getClickStatus())
    {
        if (Brush2_ori != NULL)
        {
            Brush2 = g_pBrushGray;
        }
        if (ui_type == UI_TYPE_BUTTON_STATIC2|| ui_type == UI_TYPE_BUTTON_RECT_STATIC)
        {
            return;
        }

        if (pos_move_status == 1 || pos_move_status == 2)
        {
            pos_move_status = 3;
        }
        else if (pos_move_status == 3)
        {
            x1 += frmtm * UImoveSpeed;
            x2 -= frmtm * UImoveSpeed;
            y1 += frmtm * UImoveSpeed;
            y2 -= frmtm * UImoveSpeed;

            if (y1 > ori_y1)
            {
                x1 = ori_x1;
                x2 = ori_x2;
                y1 = ori_y1;
                y2 = ori_y2;
                pos_move_status = 4;
            }
        }

    }
    else if (box->getFocus())
    {
        if (Brush2_ori != NULL)
        {
            Brush2 = g_pBrushLightGray;
        }
        if (ui_type == UI_TYPE_BUTTON_STATIC2 || ui_type == UI_TYPE_BUTTON_RECT_STATIC)
        {
            return;
        }
        if (pos_move_status == 0)
        {
            pos_move_status = 1;

            box->setBounds(ori_x1 * UIzoomSize - ori_x2 * (UIzoomSize - 1), ori_y1 * UIzoomSize - ori_y2 * (UIzoomSize - 1), ori_x2 * UIzoomSize - ori_x1 * (UIzoomSize - 1), ori_y2 * UIzoomSize - ori_y1 * (UIzoomSize - 1));
        }
        else if (pos_move_status == 1 || pos_move_status >= 3)
        {
            x1 -= frmtm * UImoveSpeed;
            x2 += frmtm * UImoveSpeed;
            y1 -= frmtm * UImoveSpeed;
            y2 += frmtm * UImoveSpeed;

            if (y1 < ori_y1 * UIzoomSize - ori_y2 * (UIzoomSize - 1))
            {
                x1 = ori_x1 * UIzoomSize - ori_x2 * (UIzoomSize - 1);
                x2 = ori_x2 * UIzoomSize - ori_x1 * (UIzoomSize - 1);
                y1 = ori_y1 * UIzoomSize - ori_y2 * (UIzoomSize - 1);
                y2 = ori_y2 * UIzoomSize - ori_y1 * (UIzoomSize - 1);
                pos_move_status = 2;

            }
        }
    }
    else
    {
        Brush2 = Brush2_ori;
        if (ui_type == UI_TYPE_BUTTON_STATIC2 || ui_type == UI_TYPE_BUTTON_RECT_STATIC)
        {
            return;
        }
        if (pos_move_status != 0)
        {
            x1 += frmtm * UImoveSpeed;
            x2 -= frmtm * UImoveSpeed;
            y1 += frmtm * UImoveSpeed;
            y2 -= frmtm * UImoveSpeed;
            pos_move_status = 4;
            if (y1 > ori_y1)
            {
                setBound(ori_x1, ori_y1, ori_x2, ori_y2);
                pos_move_status = 0;
            }
        }
    }
    return;
}

bool Button::getFocus()
{
    if (!active)
    {
        return 0;
    }
    return box->getFocus();
}

bool Button::getClickStatus()
{
    if (!active)
    {
        return 0;
    }
    return box->getClickStatus();
}
bool Button::getClicked()
{
    if (!active)
    {
        return 0;
    }
    if (box->getClicked())
    {
        box->reset_click_status();
        return 1;
    }
    else
    {
        return 0;
    }
}

void Button::setClicked()
{
    box->setClicked();
    return;
}

void Button::setBound(float x1, float y1, float x2, float y2)
{
    ori_x1 = x1, ori_y1 = y1, ori_x2 = x2, ori_y2 = y2;;
    this->x1 = x1, this->y1 = y1, this->x2 = x2, this->y2 = y2;
    box->setBounds(x1, y1, x2, y2);
    return;
}

void Button::update()
{
    if (active)
    {
        box->check();
        ui_update();
        debugger_main.add_output_line("button rect= " + to_string(x1) + ", " + to_string(y1) + ", " + to_string(x2) + ", " + to_string(y2));
    }
    return;
}

void Button::rend()
{
    if (!show)
    {
        return;
    }
    float pic_opacity = 0.8F;
    float opacity_buf = 0;
    if (Brush2 != NULL)
    {
        opacity_buf = Brush2->GetOpacity();
        if (ui_type == UI_TYPE_BUTTON_RECT_STATIC)
        {
            FillRectangle_1(x1, y1, x2, y2, Brush2, 0.65f);

            FillRectangle_1(0.97f * x1 + 0.03f * x2, 0.97f * y1 + 0.03f * y2, 0.97f * x2 + 0.03f * x1, 0.03f * y1 + 0.97f * y2, Brush2, 0.85f);

            Brush2->SetOpacity(opacity_buf);
            FillRectangle_1(0.95f * x1 + 0.05f * x2, 0.9f * y1 + 0.1f * y2, 0.95f * x2 + 0.05f * x1, 0.5f * y1 + 0.5f * y2, g_pBrushLight, 0.3f);
        }
        else
        {
            FillRoundedRectangle_1(x1, y1, x2, y2, Brush2, 0.65f);

            FillRoundedRectangle_1(0.97f * x1 + 0.03f * x2, 0.97f * y1 + 0.03f * y2, 0.97f * x2 + 0.03f * x1, 0.03f * y1 + 0.97f * y2, Brush2, 0.85f);

            Brush2->SetOpacity(opacity_buf);
            FillRoundedRectangle_1(0.95f * x1 + 0.05f * x2, 0.9f * y1 + 0.1f * y2, 0.95f * x2 + 0.05f * x1, 0.5f * y1 + 0.5f * y2, g_pBrushLight, 0.3f);
        }
        
    }

    if (Bitmap != NULL)
    {
        DrawBitmap_1(Bitmap, D2D1::RectF((x1 + x2 + y1 - y2) / 2, y1, (x1 + x2 - y1 + y2) / 2, y2), pic_opacity);
    }

    if (Brush1 != NULL)
    {
        if (ui_type == UI_TYPE_BUTTON_RECT_STATIC)
        {
            DrawRectangle_1(x1, y1, x2, y2, 2.5f, Brush1, 1.0f);
        }
        else
        {
            DrawRoundedRectangle_1(x1, y1, x2, y2, 2.5f, Brush1, 1.0f);
        }
        
    }

    if (text != "" && Brush3 != NULL)
    {
        DrawTextA_1(text, g_pTextFormat, D2D1::RectF(to_screen(x1), to_screen(y1), to_screen(x2), to_screen(y2)), Brush3);
    }
    if (rendUIboxes)
    {
        DrawRectangle_1(x1, y1, x2, y2, 2.5f, g_pBrushBlack, 1.0f);
    }
    return;
}


ComponentManager::ComponentManager()
{
    Label_num = 0, Button_num = 0, Effect_num = 0, Anime_num = 0;
    managed_Label_num = 0, managed_Button_num = 0;
    for (int i = 0; i < LABEL_MAX; i++)
    {
        labels[i] = nullptr;
    }
    for (int i = 0; i < BUTTON_MAX; i++)
    {
        buttons[i] = nullptr;

    }
    managed_buttons.fill(weak_ptr<Button>());
    managed_labels.fill(weak_ptr<LABEL>());
    for (int i = 0; i < EFFECT_MAX; i++)
    {
        effects[i] = nullptr;
    }
    for (int i = 0; i < ANIME_MAX; i++)
    {
        anime[i] = nullptr;
    }

    return;
}

ComponentManager::~ComponentManager()
{
    Clear();
    return;
}

void ComponentManager::CleanEffects()
{
    for (int i = 0; i < Effect_num; i++)
    {
        if (effects[i] != nullptr)
        {
            delete effects[i];
        }
        effects[i] = nullptr;
    }
    Effect_num = 0;
    return;
}

void ComponentManager::CleanAnimations()
{
    for (int i = 0; i < Anime_num; i++)
    {
        if (anime[i] != nullptr)
        {
            delete anime[i];
        }
        anime[i] = nullptr;
    }
    Anime_num = 0;
    return;
}

void ComponentManager::Clear()
{
    CleanEffects();
    ClearPage();
    return;
}

void ComponentManager::ClearPage()
{
    //debugger_main.writelog(0,"clearing page");
    for (int i = 0; i < Label_num; i++)
    {
        delete labels[i];
        labels[i] = nullptr;
    }
    Label_num = 0;
    for (int i = 0; i < Button_num; i++)
    {
        delete buttons[i];
        buttons[i] = nullptr;
    }
    Button_num = 0;
    for (auto &i : managed_buttons)
    {
        if (!i.expired())
        {
            i.reset();
        }
    }
    managed_Button_num = 0;
    for (auto& i : managed_labels)
    {
        if (!i.expired())
        {
            i.reset();
        }
    }
    managed_Label_num = 0;
    CleanAnimations();
    //debugger_main.writelog(0, "page clear");
    return;
}


void ComponentManager::hideAll()
{
    for (int i = 0; i < Label_num; i++)
    {
        if (labels[i] != nullptr)
        {
            labels[i]->setShow(0);
        }
    }
    for (auto& i : managed_labels)
    {
        if (!i.expired())
        {
            i.lock()->setShow(0);
        }
    }

    for (int i = 0; i < Button_num; i++)
    {
        if (buttons[i] != nullptr)
        {
            buttons[i]->deactivate();
            buttons[i]->setShow(0);
        }
    }
    for (auto& i : managed_buttons)
    {
        if (!i.expired())
        {
            i.lock()->setShow(0);
            i.lock()->deactivate();
        }
    }
    return;
}


void ComponentManager::update()
{
    const constexpr float gc_cd = 5;
    static float gc_countdown = gc_cd;
    for (int i = 0; i < Button_num; i++)
    {
        if (buttons[i] != nullptr)
        {
            buttons[i]->update();
        }
    }

    for (auto& i : managed_buttons)
    {
        if (!i.expired())
        {
            i.lock()->update();
        }
    }
    for (int i = 0; i < Effect_num; i++)
    {
        if (effects[i] != nullptr)
        {
            effects[i]->update();
        }
    }
    for (int i = 0; i < Anime_num; i++)
    {
        if (anime[i] != nullptr)
        {
            anime[i]->update();
        }
    }
    gc_countdown -= frmtm;
    if (gc_countdown < 0)
    {
        gc_countdown = gc_cd;
        int max_effect_num = 0, max_anime_num = 0;
        for (int i = 0; i < Effect_num; i++)
        {
            if (effects[i] != nullptr)
            {
                if (effects[i]->getActive())
                {
                    max_effect_num = i;
                }
                else
                {
                    delete effects[i];
                    effects[i] = nullptr;
                }
            }
        }
        Effect_num = max_effect_num + 1;
        for (int i = 0; i < Anime_num; i++)
        {
            if (anime[i] != nullptr)
            {
                if (anime[i]->getActive())
                {
                    max_anime_num = i;
                }
                else
                {
                    delete anime[i];
                    anime[i] = nullptr;
                }
            }
        }
        Anime_num = max_anime_num + 1;
    }
    return;
}

void ComponentManager::rend()
{
    
    for (int i = 0; i < Anime_num; i++)
    {
        if (anime[i] != nullptr)
        {
            anime[i]->rend();
        }
        
    }
    for (int i = 0; i < Label_num; i++)
    {
        if (labels[i] != nullptr)
        {
            labels[i]->rend();
        }
        
    }
    for (auto& i : managed_labels)
    {
        if (!i.expired())
        {
            i.lock()->rend();
        }
    }
    for (int i = 0; i < Button_num; i++)
    {
        if (buttons[i] != nullptr)
        {
            buttons[i]->rend();
        }
    }
    for (auto& i: managed_buttons)
    {
        if (!i.expired())
        {
            i.lock()->rend();
        }
    }
    for (int i = 0; i < Effect_num; i++)
    {
        if (effects[i] != nullptr)
        {
            effects[i]->rend();
        }
        
    }
    if (rendUIboxes)
    {
        FillEllipse_1(cpos.x, cpos.y, 5, 5, g_pBrushBlack);
    }
    return;
}

//LABEL* ComponentManager::CreateLabel(float x1, float y1, float x2, float y2, string text, ID2D1SolidColorBrush* Brush1, ID2D1SolidColorBrush* Brush2, ID2D1SolidColorBrush* Brush3, ID2D1Bitmap* Bitmap, int TextFormat)
//{
//    if (Label_num == LABEL_MAX)
//    {
//        debugger_main.writelog(2, "failed to ComponentManager::CreateLabel() Label_num reach LABEL_MAX");
//        return nullptr;
//    }
//    labels[Label_num] = new LABEL(x1, y1, x2, y2, text, Brush1, Brush2, Brush3, Bitmap, TextFormat);
//    Label_num++;
//    return labels[Label_num-1];
//}
//
//Button* ComponentManager::CreateButton(float x1, float y1, float x2, float y2, string text, ID2D1SolidColorBrush* Brush1, ID2D1SolidColorBrush* Brush2, ID2D1SolidColorBrush* Brush3, ID2D1Bitmap* Bitmap, int TextFormat)
//{
//    if (Button_num == BUTTON_MAX)
//    {
//        debugger_main.writelog(1, "failed to ComponentManager::CreateButton() Button_num reach BUTTON_MAX");
//        return nullptr;
//    }
//    buttons[Button_num] = new Button(x1, y1, x2, y2, text, Brush1, Brush2, Brush3, Bitmap, TextFormat);
//    Button_num++;
//    return buttons[Button_num-1];
//}

Effect* ComponentManager::CreateEffect(int index, float posx, float posy)
{
    for (int i = 0; i < Effect_num; i++)
    {
        if (effects[i]==nullptr)
        {
            effects[i] = new Effect(index, posx, posy);
            return effects[Effect_num];
        }
    }
    effects[Effect_num] = new Effect(index, posx, posy);
    Effect_num++;
    return effects[Effect_num-1];
}


Animation* ComponentManager::CreateAnimation(int type, int destx, int desty, int destw, int destl)
{
    for (int i = 0; i < Anime_num; i++)
    {
        if (anime[i]==nullptr)
        {
            anime[i] = new Animation(type, destx, desty, destw, destl);
            return anime[i];
        }
    }
    anime[Anime_num] = new Animation(type, destx, desty, destw, destl);
    Anime_num++;
    return anime[Anime_num];
}

void ComponentManager::AddLabel(weak_ptr<LABEL> label)
{
    if (label.use_count() == 0)
    {
        debugger_main.writelog(DWARNNING, "failed in ComponentManager::AddButton() while button nullptr", __LINE__);
        return;
    }
    if (managed_Label_num == LABEL_MAX)
    {
        debugger_main.writelog(DWARNNING, "failed in ComponentManager::AddLabel() Label_num reach LABEL_MAX",__LINE__);
        return;
    }
    managed_labels[managed_Label_num] = label;
    managed_Label_num++;
    return;
}

void ComponentManager::AddButton(weak_ptr<Button> button)
{
    if (button.use_count() == 0)
    {
        debugger_main.writelog(DWARNNING, "failed in ComponentManager::AddButton() while button nullptr", __LINE__);
        return;
    }
    if (managed_Button_num == BUTTON_MAX)
    {
        debugger_main.writelog(DWARNNING, "failed in ComponentManager::AddButton() Button_num reach BUTTON_MAX",__LINE__);
        return;
    }
    managed_buttons[managed_Button_num] = button;
    managed_Button_num++;
    return;
}

Effect::Effect(int index, float posx, float posy)
{
    switch (index)
    {
        //case 1:
    case 9:

        if (!createShortcutBitmap(shortcut_Bitmap))
        {
            return;
        }
        time = 0;
        break;
    case 2:
    case 3:
    case 4:
    case 5:
    case 7:
    case 8:
    case 101:
    case 102:
    case 103:
    case 201:
    case 202:
    case 211:
    case 212:
    case 213:
    case 221:
    case 301:
    case 302:
    case 401:
    case 501:
        time = 0;
        
        break;

    default:
        debugger_main.writelog(1,"excepted index in createEffect()! " + to_string(index));
        return;

    }
    this->posx = posx;
    this->posy = posy;
    this->index = index;
    active = 1;
}

bool Effect::getActive()
{
    return active;
}


void Effect::update()
{
    switch (index)
    {
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
        //debugger_main.writelog(DDEBUG, "update index= "+ to_string(index)+"time= " + to_string(time)+"  frmtm= "+to_string(frmtm));
        time += frmtm * 1.25f;
        if (time > 1)
        {
            active = 0;
        }
        break;
    case 101:
    case 102:
    case 201:
    case 202:
    case 211:
    case 212:
    case 213:
    case 221:
    case 301:
    case 302:
    case 401:
    
        time += frmtm;
        if (time > 4)
        {
            active = 0;
            //writelog("effect " + to_string(index) + " end.");
        }
        break;
    default:
        debugger_main.writelog(1,"unexpected index in Effect::update()! index=" + to_string(index) + " page_index=" + to_string(page_index));
        active = 0;
    }
    return;
}

void Effect::rend()
{
    if (!active|| thread_IO_request_verify_res < 8)
    {
        return;
    }
    float opacity_buf = 1;
    string timestr;
    switch (index)
    {
    case 2: //0-0.5s半径从0扩大至1600，透明度从0增至1，0.5-1s透明度减至0
        g_pBrushLightGreen->SetOpacity(sinf(time * PI));

        g_pD2DDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2(posx, posy), to_screen(time * 3200), to_screen(time * 3200)), g_pBrushLightGreen);
        g_pBrushLightGreen->SetOpacity(1.0f);
        DrawCallNum++;
        break;
    case 3:
        g_pBrushLightBlue->SetOpacity(sinf(time * PI));

        g_pD2DDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2(posx, posy), to_screen(time * 3200), to_screen(time * 3200)), g_pBrushLightBlue);
        g_pBrushLightBlue->SetOpacity(1.0f);
        DrawCallNum++;
        break;
    case 4:
        g_pBrushFloralWhite->SetOpacity(sinf(time * PI));

        g_pD2DDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2(posx, posy), to_screen(time * 3200), to_screen(time * 3200)), g_pBrushFloralWhite);
        g_pBrushFloralWhite->SetOpacity(1.0f);
        DrawCallNum++;
        break;
    case 5:
        
        g_pBrushBlack->SetOpacity(sinf(time * PI));
        g_pD2DDeviceContext->FillRectangle(D2D1::RectF(to_screen(0), to_screen(0), to_screen(1660), to_screen(960)), g_pBrushBlack);
        g_pBrushBlack->SetOpacity(1.0f);
        break;
    case 6:
        break;
    case 7:
        g_pBrushDarkBlue->SetOpacity(sinf(time * PI));
        g_pD2DDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2(posx, posy), to_screen(time * 3200), to_screen(time * 3200)), g_pBrushDarkBlue);
        g_pBrushDarkBlue->SetOpacity(1.0f);
        DrawCallNum++;
        break;
    case 8:
        g_pBrushLight->SetOpacity(sinf(time * PI));
        g_pD2DDeviceContext->FillRectangle(D2D1::RectF(to_screen(0), to_screen(0), to_screen(1600), to_screen(900)), g_pBrushLight);
        DrawCallNum++;
        break;
    case 9:
        gaussianBlurEffect->SetInput(0, shortcut_Bitmap);
        gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);
        gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, 8.0f);
        OpacityEffect->SetInputEffect(0, gaussianBlurEffect);
        OpacityEffect->SetValue(D2D1_OPACITY_PROP_OPACITY, 1 - sinf(time * 0.5f * PI));
        DrawEffect_1(OpacityEffect);
        break;

    case 201:
        if (time < 1)
        {
            opacity_buf = 0.8f * sinf(time * 0.5 * PI);
        }
        else if (time < 3)
        {
            opacity_buf = 0.8f;
        }
        else
        {
            opacity_buf = 0.8f * sinf((time - 2) * 0.5f * PI);
        }
        DrawBitmap_1(copy_succeed_tagline, D2D1::RectF(posx, posy, posx + 400, posy + 100), opacity_buf);
        break;
    case 202:
        if (time < 1)
        {
            opacity_buf = 0.8f * sinf(time * 0.5 * PI);
        }
        else if (time < 3)
        {
            opacity_buf = 0.8f;
        }
        else
        {
            opacity_buf = 0.8f * sinf((time - 2) * 0.5f * PI);
        }
        DrawBitmap_1(copy_failed_tagline, D2D1::RectF(posx, posy, posx + 400, posy + 100), opacity_buf);
        break;
    case 301:
        if (time < 1)
        {
            opacity_buf = 0.8f * sinf(time * 0.5 * PI);
        }
        else if (time < 3)
        {
            opacity_buf = 0.8f;
        }
        else
        {
            opacity_buf = 0.8f * sinf((time - 2) * 0.5f * PI);
        }
        DrawBitmap_1(io_error_tagline, D2D1::RectF(posx, posy, posx + 800, posy + 100), opacity_buf);
        break;
    case 302:
        if (time < 1)
        {
            opacity_buf = 0.8f * sinf(time * 0.5 * PI);
        }
        else if (time < 3)
        {
            opacity_buf = 0.8f;
        }
        else
        {
            opacity_buf = 0.8f * sinf((time - 2) * 0.5f * PI);
        }
        DrawBitmap_1(file_corrupt_tagline, D2D1::RectF(posx, posy, posx + 400, posy + 100), opacity_buf);
        break;
    case 401:
        if (time < 1)
        {
            opacity_buf = 0.8f * sinf(time * 0.5 * PI);
        }
        else if (time < 3)
        {
            opacity_buf = 0.8f;
        }
        else
        {
            opacity_buf = 0.8f * sinf((time - 2) * 0.5f * PI);
        }
        FillRectangle_1(posx, posy, posx + 400, posy + 100, g_pBrushWhite, opacity_buf);
        DrawTextA_1(lan[0].free_view, g_pTextFormat, posx, posy, posx + 400, posy + 100, g_pBrushBlack, opacity_buf);
        break;
    default:
        break;
    }

    g_pBrushLight->SetOpacity(0.5);
    return;
}


Animation::Animation(ID2D1Bitmap* bitmap, int src_length, int src_height, int src_dx, int src_dy, int src_dw, int src_dh, int num, float stime)
{
    ctime = 0;
    this->stime = stime;
    length = src_length, width = src_height;
    this->num = num;
    repeat = 1;
    delta_x1 = src_dx - src_dw * 0.5f, delta_x2 = src_dx + src_dw * 0.5f;
    delta_y1 = src_dy - src_dh * 0.5f, delta_y2 = src_dy + src_dh * 0.5f;
    destl = 0;
    destw = 0;
    destx = 0;
    desty = 0;
    this->bitmap = bitmap;
    type = 0;
    curnum = 1;//start from 1
    active = 1;
}

Animation::Animation(int type, int destx, int desty, int destw, int destl)
{
    ctime = 0;
    stime = 0;
    length = 0, width = 0;
    num=1;
    repeat = 1;
    delta_x1 = 0, delta_x2 = 0, delta_y1 = 0, delta_y2 = 0;
    this->destl = 0;
    this->destw = 0;
    bitmap = NULL;
    switch (type)
    {
    case 1: //1-1 
    case 2:
    case 3:
    case 4:
        bitmap = g_pD2DBimtapUI[21+ type];
        stime = 0.1f;
        length = 512;
        width = 512;
        num = 11;
        repeat = 1;
        this->destl = destl;
        this->destw = destw;
        break;

    default:
        
        debugger_main.writelog(1, "unexpected index in Animation::Animation()! " + to_string(type));
        break;
    }
    this->type = type;

    this->destx = destx;
    this->desty = desty;
    curnum = 1;//start from 1
    active = 1;
    return;
}

bool Animation::getActive()
{
    return active;
}

void Animation::update()
{
    int t = 0;
    ctime += frmtm;
    if (ctime > stime)
    {
        t = ctime / stime;
        curnum += t;
        ctime -= t * stime;
        if (curnum > num)
        {
            if (repeat)
            {
                curnum = 1;
            }
            else
            {
                active = 0;
            }
        }
    }
    return;
}

void Animation::rend(float destx, float desty, float destw, float desth,  bool direction, float opacity)
{
    if (!active || !bitmap)
    {
        return;
    }
    D2D_RECT_F src, dest;
    

    src = D2D1::RectF(((curnum - 1) % 10) * length + delta_x1, ((curnum - 1) / 10) * width + delta_y1, ((curnum - 1) % 10) * length + delta_x2, ((curnum - 1) / 10) * width + delta_y2);
    if (direction)
    {
        dest = D2D1::RectF(destx - destw * 0.5f, desty - desth * 0.5f, destx + destw * 0.5f, desty + desth * 0.5f);
        DrawBitmap_1(bitmap, dest, src, opacity);
    }
    else
    {
        //src = D2D1::RectF(((curnum - 1) % 10) * length + delta_x2, ((curnum - 1) / 10) * width + delta_y1, ((curnum - 1) % 10) * length + delta_x1, ((curnum - 1) / 10) * width + delta_y2);
        dest = D2D1::RectF(destx - destw * 0.5f, desty - desth * 0.5f, destx +destw * 0.5f, desty + desth * 0.5f);
        g_pD2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Scale(D2D1::SizeF(-1, 1), D2D1::Point2F(to_screen(destx), to_screen(desty))));
        DrawBitmap_1(bitmap, dest, src, opacity);
        g_pD2DDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
    }
    return;
}

void Animation::rend()
{
    if (!active|| !bitmap)
    {
        return;
    }
    D2D_RECT_F src, dest;
    int dlength = destl, dwidth = destw;
    
    if (destl == 0)
    {
        dlength = 1600;
        dwidth = 900;
    }

    src = D2D1::RectF(((curnum - 1) % 10) * length, (curnum - 1) / 10 * width, ((curnum - 1) % 10) * length + length, (curnum - 1) / 10 * width + width);
    dest = D2D1::RectF((float)destx, (float)desty, (float)(destx + dlength), (float)(desty + dwidth));
    DrawBitmap_1(bitmap, dest, src, 1.0f);
    return;
}

int Letter::num = 0, Letter::homepage_text_type = 0;
Letter::Letter(char single_char)
{
    if (st.wHour > 8 && st.wHour < 19)
    {
        homepage_text_type = 13;
    }
    else
    {
        homepage_text_type = 12;
    }
    index_num = num;
    num++;
    sLetter = single_char;
    posx = 0;
    posy = 0;
    target_posx = 0;
    target_posy = 0;
    speedx = 0;
    speedy = 0;
    opacity = 0.4f;
    effect = 0;//0 for dark,1 for light
    effect_level = 0;
    effect_stage = 0;
    effect_wait_time = 0;
}

void Letter::reset()
{
    posx = rand() % 1500 + 50;
    posy = rand() % 800 + 50;
    opacity = 0.4f;
    target_posx = 700 + index_num * 100;
    target_posy = 200 + rand() % 4 * 20;
    speedx = (target_posx - posx) / 0.8f;
    speedy = (target_posy - posy) / 0.8f;
    effect = rand() % 2;
    effect_stage = 0;
    effect_wait_time = index_num + 1;
    effect_level = 0;
    return;
}

void Letter::update()
{
    if (opacity < 1)
    {
        posx += speedx * frmtm;
        posy += speedy * frmtm;
        opacity += 0.75f * frmtm;
    }
    else
    {
        if (effect_wait_time < 0)
        {
            if (effect_stage == 0)
            {
                effect_level += frmtm;
                if (effect_level > 0.6)
                {
                    effect_stage = 1;
                }
            }
            else
            {
                effect_level -= frmtm;
                if (effect_level < 0)
                {
                    effect_level = 0;
                    effect_stage = 0;
                    effect = rand() % 2;
                    effect_wait_time = 4 + rand() % 5;
                }
            }
        }
        else
        {
            effect_wait_time -= frmtm;
        }
    }
    
    return;
}

void Letter::rend()
{
    g_pBitmapBrushUI[homepage_text_type]->SetOpacity(opacity);
    DrawTextA_1(
        sLetter,           // Text to render
        g_pTextFormatLarge,     // Text format
        posx - 30, posy - 30, posx + 30, posy + 30,    // The region of the window where the text will be rendered
        g_pBitmapBrushUI[homepage_text_type]      // The brush used to draw the text
    );
    g_pBitmapBrushUI[homepage_text_type]->SetOpacity(1.0f);
    if (!long_credit)
    {
        if (effect)
        {
            g_pBrushLight->SetOpacity(effect_level);
            DrawTextA_1(
                sLetter,
                g_pTextFormatLarge,
                posx - 30, posy - 30, posx + 30, posy + 30,    // The region of the window where the text will be rendered
                g_pBrushLight
            );
        }
        else
        {
            g_pBrushDark->SetOpacity(effect_level);
            DrawTextA_1(
                sLetter,
                g_pTextFormatLarge,
                posx - 30, posy - 30, posx + 30, posy + 30,    // The region of the window where the text will be rendered
                g_pBrushDark
            );
        }
    }
    return;
}


void self_restart()
{
    if (self_restarted)
    {
        return;
    }
    debugger_main.writelog(0,"try to restart.");
    string writebuff[64] = { "" };
    //writebuff[0] = "chcp 65001";
    writebuff[0] = "ping 127.1 /n 5";
    int ci = 1;

    writebuff[ci] = "start \"\" ";
    writebuff[ci + 1] = "del /f /q %0&exit";

    const int nBufSize = 512;
    string quote = "\"";
    char chBuf[nBufSize];
    ZeroMemory(chBuf, nBufSize);
    if (GetModuleFileName(NULL, chBuf, nBufSize))
    {
        writebuff[ci] += quote;
        writebuff[ci] += chBuf;
        writebuff[ci] += quote;
        WriteFile("./restart.bat", writebuff);
        debugger_main.writelog(0, "Now execute: " + string(chBuf));
        system("start /min .\\restart.bat");
        normal_quit = 1;
        self_restarted = 1;
    }
    else
    {
        debugger_main.writelog(1,"GetModuleFileName Failed!");
    }
    return;
}


void Wchar_tToString(string& szDst, wchar_t* wchar)
{
    wchar_t* wText = wchar;
    DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);//WideCharToMultiByte的运用
    char* psText;  // psText为char*的临时数组，作为赋值给std::string的中间变量
    psText = new char[dwNum];
    WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);//WideCharToMultiByte的再次运用
    szDst = psText;// std::string赋值
    delete[]psText;// psText的清除
    return;
}

LPCWSTR stringToLPCWSTR(string orig)
{
    size_t origsize = orig.length() + 1;
    const size_t newsize = 100;
    size_t convertedChars = 0;
    wchar_t* wcstring = (wchar_t*)malloc(sizeof(wchar_t) * (orig.length() + 1));
    mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);

    return wcstring;
}


void CleanStrBuff(string buf[],int size)
{
    for (int i = 0; i < size; i++)
    {
        buf[i].clear();
    }
    return;
}

string addFormat(string ori)
{
    return "[" + ori + "]";
}


string getTimeStr()
{
    GetLocalTime(&st);
    return to_string(st.wYear)+ "/"+ to_string(st.wMonth)+ "/"+ to_string(st.wDay)+ " "+ to_string(st.wHour)+ ":"+ to_string(st.wMinute);
}


string getTimeDigit(float time)
{
    string result = "";
    if (time < 100)
    {
        result = to_string(time).substr(0, 4);
    }
    else if (100 <= time && time < 1000)
    {
        result = to_string(time).substr(0, 5);
    }
    else if (1000 <= time && time < 10000)
    {
        result = to_string(time).substr(0, 4);
    }
    else
    {
        result = to_string(time);
    }
    result += "s";
    return result;
}

string getTimeDigit_minute(float time_sec)
{
    string result;
    if (time_sec < 60)
    {
        return getTimeDigit(time_sec);
    }
    else
    {
        result = to_string((int)(time_sec / 60));
        result += " min";
    }
    
    return result;
}

string md5sum(string& str) 
{
    string md5;
    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, str.c_str(), str.size());
    unsigned char digest[MD5_DIGEST_LENGTH];
    MD5_Final(digest, &ctx);
    char hex[35] = { 0 };
    char tmp[3] = { 0 };
    memset(hex, 0, sizeof(hex));
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) 
    {
        sprintf_s(tmp, "%02X", digest[i]);
        strcat_s(hex, tmp);
    }
    md5 = string(hex);
    return md5;
}

bool AllisNum(string str)
{
    if (str.empty())
    {
        return false;
    }
    for (int i = 0; i < str.length(); i++)
    {
        int tmp = (int)str[i];
        if (tmp >= 48 && tmp <= 57)
        {
            continue;
        }
        else
        {
            return false;
        }
    }
    return true;
}



void init_string()
{
    tips[0] = "";
    tips[1] = "";
    tips[2] = "";
    tips[3] = "";
    tips[4] = "";
    //tips[5] = "好友不在同一局域网下？安装附件中的Radmin_LAN以实现远程联机";
    //tips[6] = "尽管存档是以明文形式保存的——在尝试修改存档前记得先备份";
    tips[7] = "";
    //tips[8] = "音频设置在线程模式2时延迟更低，代价是会占满整个线程";
    //tips[9] = "阅读music文件夹下的readme.txt以了解如何使用你自己的bgm";
    tips[10] = "";
    tips[11] = "";
    tips[12] = "";
    tips[13] = "";
    tips[14] = "";
    return;
}


string getClipBoard()
{
    if (OpenClipboard(NULL))//打开剪贴板  
    {
        if (IsClipboardFormatAvailable(CF_TEXT))//判断格式是否是我们所需要  
        {
            HANDLE hClip;
            char* pBuf;
            //读取数据  
            hClip = GetClipboardData(CF_TEXT);
            pBuf = (char*)GlobalLock(hClip);
            GlobalUnlock(hClip);
            CloseClipboard();
            return pBuf;
        }
        else
        {
            debugger_main.writelog(1,"ClipboardFormat no text!");
            CloseClipboard();
        }

    }
    else
    {
        debugger_main.writelog(1,"OpenClipboard failed!");
    }
    return "";
}

bool setClipBoard(string content)
{
    if (content.empty())
    {
        return 1;
    }

    if (!OpenClipboard(NULL))//打开剪贴板  
    {
        debugger_main.writelog(1,"OpenClipboard failed!");
        return 0;
    }
    HANDLE hClip = NULL;
    char* pBuf = NULL;
    if (EmptyClipboard() == 0)   //清空剪贴板  
    {
        debugger_main.writelog(1, "EmptyClipboard() failed! " + to_string(GetLastError()));
        CloseClipboard();
        return 0;
    }
    
    hClip = GlobalAlloc(GMEM_FIXED, 512);

    if (hClip == NULL)
    {
        debugger_main.writelog(1, "GlobalAlloc() failed! " + to_string(content.length() + 1));
        CloseClipboard();
        return 0;
    }
    pBuf = (char*)GlobalLock(hClip);
    //写入数据  
    if (pBuf == NULL)
    {
        //GlobalFree(hClip);
        debugger_main.writelog(1, "GlobalLock() failed!");
        CloseClipboard();
        return 0;
    }
    strcpy_s(pBuf, 512, content.c_str());
    GlobalUnlock(hClip);//解锁
    if (SetClipboardData(CF_TEXT, hClip) == NULL)//设置格式  
    {
        //GlobalFree(hClip);
        debugger_main.writelog(1, "SetClipboardData failed! " + to_string(GetLastError()));
        CloseClipboard();
        return 0;
    }

    
    //关闭剪贴板
    GlobalFree(hClip);
    CloseClipboard();
    return 1;
}


void TIMER::start_timer()
{
    start = Clock::now();
    return;
}

float TIMER::stop_timer()
{
    end = Clock::now();
    return (float)chrono::duration_cast<std::chrono::nanoseconds>(this->end - this->start).count() / 1000000000;
}



D2D1_POINT_2F cpos;

HWND hWnd, FocusWindow = NULL;

Box* boxes[BOX_MAX] = { nullptr };




//使用g_am对象setMusic()方法替换此函数
void InitPage(int index)
{
    g_cm.CleanAnimations();
    switch (index)
    {
        
    case 0: //开始页面
        //thread_Audio_target_music = 2;
        //thread_Audio_switch_immediately = 1;
        g_am.PlayBGM(2, 1, 0);
        break;
    case 1: //设置页面
        break;
    case 2: //game
        //thread_Audio_target_music = 1;
        //thread_Audio_switch_immediately = 1;
        g_am.PlayBGM(1, 1, 1);
        break;
    case 3:

        break;
    case 4:
        break;
    case 5:
        //thread_Audio_target_music = 3;
        //thread_Audio_switch_immediately = 1;
        break;
    case 6: 
    //cleanAnimation();
        break;
    case 7: //connect page
        //cleanAnimation();
        break;
    case 1000:
    case 1001:
        //thread_Audio_target_music = 6;
        g_am.PlayBGM(6, 1, 1);
        break;
    case 3001:  //new user
        //thread_Audio_target_music = 6;
        //thread_Audio_switch_immediately = 1;
        //g_am.PlayBGM(6, 1, 1);
        break;
    /*case 3002:
        thread_Audio_target_music = 5;
        thread_Audio_switch_immediately = 1;
        break;*/
    default:
        debugger_main.writelog(1,"try to init unknown page " + to_string(index));
        break;
    }
    return;
}

int page_index_target = 0, page_status_target = 0;
float target_delay_time = 0;
bool target_valid = 0;
void set_target_page(int page_index, int page_status, float delay_time)
{
    if (target_valid)
    {
        return;
    }
    page_index_target = page_index;
    page_status_target = page_status;
    target_delay_time = delay_time;
    target_valid = 1;
    return;
}

void update_target_page()
{
    if (!target_valid)
    {
        return;
    }
    target_delay_time -= frmtm;
    if (target_delay_time < 0)
    {
        target_valid = 0;
        page_index = page_index_target;
        page_status = page_status_target;

    }
    return;
}


void OnSize()
{
    GetClientRect(hWnd, &rc);
    return;
}

void RebuildRenderContext()
{
    HRESULT hr = S_OK;
    
    switch (set2[0].resolution)
    {
    case 1:
        orix = 800;
        oriy = 450;
        scale = 0.5f;
        break;
    case 2:
        orix = 1280;
        oriy = 720;
        scale = 0.8f;
        break;
    case 3:
        orix = 1600;
        oriy = 900;
        scale = 1.0;
        break;
    case 4:
        orix = 1920;
        oriy = 1080;
        scale = 1.2f;
        break;
    case 5:
        orix = 2560;
        oriy = 1440;
        scale = 1.6f;
        break;
    case 6:
        orix = 3840;
        oriy = 2160;
        scale = 2.4f;
        break;
    case 7:
        orix = 7680;
        oriy = 4320;
        scale = 4.8f;
        break;
    default:
        orix = 1600;
        oriy = 900;
        scale = 1.0;
        set2[0].resolution = 3;
        break;
    }
    debugger_main.writelog(DINFO, "switch target resolution: " + to_string(orix) + "x" + to_string(oriy));
    pDxgiBackBuffer->AddRef();
    debugger_main.writelog(DDEBUG, "pDxgiBackBuffer refcount3=" + to_string(pDxgiBackBuffer->Release()));
    g_pD2DDeviceContext->SetTarget(nullptr);

    
    SAFE_RELEASE(g_pD2DTargetBimtap)
    SAFE_RELEASE(g_output)
    
    /*ULONG refCount = 0;
    if (g_pD2DTargetBimtap)
    {
        refCount = g_pD2DTargetBimtap->Release();
        while (refCount > 0)
        {
            debugger_main.writelog(DWARNNING, "ResizeBuffers may failed since RefCount of g_pD2DTargetBimtap = " + to_string(refCount), __LINE__);
            refCount = g_pD2DTargetBimtap->Release();
        }
    }*/
    g_pD2DDeviceContext->Flush();
    pDxgiBackBuffer->AddRef();
    debugger_main.writelog(DDEBUG, "pDxgiBackBuffer refcount4=" + to_string(pDxgiBackBuffer->Release()));

    SAFE_RELEASE(pDxgiBackBuffer)
    /*if (pDxgiBackBuffer)
    {
        refCount = pDxgiBackBuffer->Release();
        while (refCount > 0)
        {
            debugger_main.writelog(DWARNNING, "ResizeBuffers may failed since RefCount of pDxgiBackBuffer = " + to_string(refCount), __LINE__);
            refCount = pDxgiBackBuffer->Release();
        }
    }*/
    //g_pD2DDeviceContext->Flush();
    
    hr = g_pSwapChain->ResizeBuffers(0, orix, oriy, DXGI_FORMAT_B8G8R8A8_UNORM, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH);
    if (FAILED(hr))
    {
        debugger_main.writelog(-1, "ResizeBuffers Failed. " + to_string(hr),__LINE__);
        Sleep(200);
        quit_single = 1;
        return;
    }

    hr = g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pDxgiBackBuffer));
    if (FAILED(hr))
    {
        debugger_main.writelog(-1, "GetBuffer Failed. "+to_string(hr), __LINE__);
        Sleep(200);
        quit_single = 1;
        return;
    }

    hr = g_pSwapChain->GetContainingOutput(&g_output);

    if (FAILED(hr))
    {
        debugger_main.writelog(-1, "GetContainingOutput Failed. " + to_string(hr), __LINE__);
        Sleep(200);
        quit_single = 1;
        return;
    }
    
    hr = g_pD2DDeviceContext->CreateBitmapFromDxgiSurface(
        pDxgiBackBuffer,
        &targetbitmapProperties,
        &g_pD2DTargetBimtap
    );

    if (FAILED(hr))
    {
        debugger_main.writelog(-1, "CreateBitmapFromDxgiSurface Failed. " + to_string(hr), __LINE__);
        Sleep(200);
        quit_single = 1;
        return;
    }

    g_pD2DDeviceContext->SetTarget(g_pD2DTargetBimtap);

    SAFE_RELEASE(shortcut_Bitmap)

    hr = g_pD2DDeviceContext->CreateBitmap(D2D1::SizeU(orix, oriy), nullptr, 0,
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_NONE,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &shortcut_Bitmap);

    if (!SUCCEEDED(hr))
    {
        debugger_main.writelog(DERROR, "CreateBitmap shortcut_Bitmap failed!", __LINE__);
        Sleep(200);
        quit_single = 1;
        return;
    }
    /*if (!createShortcutBitmap(shortcut_Bitmap))
    {
        debugger_main.writelog(DWARNNING, "createShortcutBitmap() failed!", __LINE__);
    }*/

    //recreate fonts
    LoadFonts();

    //todo: switch page to setting page (reload page)

    return;
}

float wheel_move_value = 0, wheel_move_value_temp = 0;
void input_wheel(short value)
{
    wheel_move_value_temp += value/WHEEL_DELTA;
    return;
}

void input()
{
    static float expand_input_time = 0.1f;
    static char expand_input_last_key = NULL;
    
    
    clicking = 0, rightclick = 0, keyw = 0, keya = 0, keys = 0, keyd = 0, keyTab = 0, keyESC = 0, keyComb = 0, keySpace=0;
    if (hWnd == FocusWindow)
    {
        if (KEY_DOWN(27) && page_index != 0)
        {
            keyESC = 1;
        }
        if (KEY_DOWN('W'))
        {
            keyw = 1;
        }
        if (KEY_DOWN('A'))
        {
            keya = 1;
        }
        if (KEY_DOWN('S'))
        {
            keys = 1;
        }
        if (KEY_DOWN('D'))
        {
            keyd = 1;
        }
        if (KEY_DOWN('Z') && KEY_DOWN('X') && KEY_DOWN('C'))
        {
            keyComb = 1;
        }
        //debug
        
        if (KEY_DOWN(VK_F2))
        {
            debugger_main.set_osd_enable(1);
#ifdef _DEBUG
            rendUIboxes = 1;
#endif // _DEBUG

        }
        else
        {
            debugger_main.set_osd_enable(0);
#ifdef _DEBUG
            rendUIboxes = 0;
#endif // _DEBUG
        }

        if (KEY_DOWN(VK_TAB))
        {
            keyTab = 1;
        }
        if (KEY_DOWN(VK_SPACE))
        {
            keySpace = 1;
        }
        
        if (KEY_DOWN(VK_LBUTTON))
        {
            clicking = 1;
        }
        if (KEY_DOWN(VK_RBUTTON))
        {
            rightclick = 1;
        }
        POINT cursorPos;
        GetCursorPos(&cursorPos);                       //获取鼠标在屏幕上的位置
        ScreenToClient(hWnd, &cursorPos);
        //适应窗口大小
        cpos.x = 1600.0f * cursorPos.x / (rc.right - rc.left);
        cpos.y = 900.0f * cursorPos.y / (rc.bottom - rc.top);

        //debug
        if (input_expand)
        {
            expand_input_key = NULL;

            for (int i = 48; i < 91; i++)
            {
                if (KEY_DOWN(i))
                {
                    if (i < 58)
                    {
                        expand_input_key = i;
                    }
                    else if (GetKeyState(VK_CAPITAL))
                    {
                        expand_input_key = i;
                    }
                    else
                    {
                        expand_input_key = i + 32;
                    }


                }
            }
            if (KEY_DOWN(8))//backspace
            {
                expand_input_key = 35;
            }
            if (KEY_DOWN(13))//enter
            {
                expand_input_key = 36;
            }


            if (expand_input_key == expand_input_last_key && expand_input_last_key != NULL)
            {
                expand_input_time += frmtm;
                if (expand_input_time > 0.5f)
                {
                    expand_input_time = 0;
                }
                else
                {
                    expand_input_key = NULL;
                }
            }
            else
            {
                expand_input_time = 0;
                expand_input_last_key = expand_input_key;
            }

        }
    }

    return;
}



void CreateD3DResource(HWND hWnd)
{
    HRESULT hr;
    D3D_FEATURE_LEVEL featureLevels[] = {
            D3D_FEATURE_LEVEL_11_1,
            D3D_FEATURE_LEVEL_11_0,
    };
#if defined(_DEBUG)
    // If the project is in a debug build, enable the debug layer.
    creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    // 创建设备
    hr = D3D11CreateDevice(
        // 设为空指针选择默认设备
        NULL,
        // 强行指定硬件渲染
        D3D_DRIVER_TYPE_HARDWARE,
        // 没有软件接口
        nullptr,
        // 创建flag
        creationFlags,
        // 欲使用的特性等级列表
        featureLevels,
        // 特性等级列表长度
        ARRAYSIZE(featureLevels),
        // SDK 版本
        D3D11_SDK_VERSION,
        // 返回的D3D11设备指针
        &pD3DDevice,
        // 返回的特性等级
        &m_featureLevel,
        // 返回的D3D11设备上下文指针
        &pD3DDeviceImmediateContext
    );
    if (FAILED(hr))
    {
        debugger_main.writelog(-1,"D3D11CreateDevice Failed. "+to_string(hr));
        return;
    }
    ZeroMemory(&sd, sizeof(sd));
    // 是否使用 MSAA?
    if (set2[0].MSAA==2)
    {
        UINT m4xMsaaQuality = 0;
        for (UINT sampleCount = 1; sampleCount <= D3D11_MAX_MULTISAMPLE_SAMPLE_COUNT; sampleCount++)
        {
            UINT maxQualityLevel = 0;
            hr = pD3DDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_B8G8R8A8_UNORM, sampleCount, &maxQualityLevel);


            if (hr == S_OK && maxQualityLevel > 0) {
               
                sampleCountOut = sampleCount;
                m4xMsaaQuality = maxQualityLevel;
            }

        }
        sd.SampleDesc.Count = sampleCountOut;
        // m4xMsaaQuality是通过CheckMultisampleQualityLevels()方法获得的
        sd.SampleDesc.Quality = D3D11_STANDARD_MULTISAMPLE_PATTERN;
        debugger_main.writelog(DDEBUG, "SampleDesc.Quality= " + to_string(sd.SampleDesc.Quality));
    }
    else if (set2[0].MSAA == 1)
    {
        sampleCountOut = 4;
        sd.SampleDesc.Count = 4;
        sd.SampleDesc.Quality = 0;
    }
    // NoMSAA
    else
    {
        sampleCountOut = 1;
        sd.SampleDesc.Count = 1;
        sd.SampleDesc.Quality = 0;
    }
    
    if (FAILED(hr))
    {
        debugger_main.writelog(-1,"CheckMultisampleQualityLevels Failed.");
        return;
    }
    hr = pD3DDevice->QueryInterface(__uuidof(IDXGIDevice1), (void**)&pDxgiDevice);
    if (FAILED(hr))
    {
        debugger_main.writelog(-1,"Query D3DDevice Interface Failed.");
        return;
    }
    hr = pDxgiDevice->GetAdapter(&pDxgiAdapter);
    if (FAILED(hr))
    {
        debugger_main.writelog(-1,"GetAdapter Failed.");
        return;
    }
    hr = pDxgiDevice->SetGPUThreadPriority(5);
    if (FAILED(hr))
    {
        debugger_main.writelog(-1,"SetGPUThreadPriority Failed.");
        //return;
    }
    /*hr = pDxgiAdapter->GetDesc(&Adapter);
    if (FAILED(hr))
    {
        debugger_main.writelog(-1,"GetDesc Failed.");
        return;
    }*/
    hr = pDxgiAdapter->GetParent(IID_PPV_ARGS(&g_pDxgiFactory));
    if (FAILED(hr))
    {
        debugger_main.writelog(-1,"GetParent Failed.");
        return;
    }


    // 使用指定渲染大小
    sd.Width = orix;
    sd.Height = oriy;
    sd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    sd.Stereo = FALSE;


    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    sd.BufferCount = 1;
    sd.AlphaMode = DXGI_ALPHA_MODE_UNSPECIFIED;
    sd.Scaling = DXGI_SCALING_STRETCH;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    ZeroMemory(&fullscreenDesc, sizeof(fullscreenDesc));
    fullscreenDesc.RefreshRate.Denominator = 1;
    fullscreenDesc.RefreshRate.Numerator = 0;
    fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_PROGRESSIVE;
    if (set2[0].window_mode)
    {
        fullscreenDesc.Windowed = 1;
    }
    else
    {
        fullscreenDesc.Windowed = 0;
    }

    if (pD3DDevice == NULL)
    {
        debugger_main.writelog(-1,"pD3DDevice == 0");
        return;
    }
    hr = g_pDxgiFactory->CreateSwapChainForHwnd(
        pD3DDevice,
        hWnd,
        &sd,
        &fullscreenDesc,
        //false ? &fullscreenDesc : nullptr, 
        nullptr,
        &g_pSwapChain
    );
    if (FAILED(hr))
    {
        debugger_main.writelog(-1,"CreateSwapChainForHwnd Failed.");
        return;
    }
    hr = pDxgiDevice->SetMaximumFrameLatency(1);
    if (FAILED(hr))
    {
        debugger_main.writelog(-1, "SetMaximumFrameLatency Failed.");
        return;
    }
    hr = g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pDxgiBackBuffer));
    if (FAILED(hr))
    {
        debugger_main.writelog(-1,"GetBuffer Failed.");
        return;
    }
    hr = g_pSwapChain->GetContainingOutput(&g_output);

    if (FAILED(hr))
    {
        debugger_main.writelog(-1,"GetContainingOutput Failed.");
        return;
    }

    return;
}





HRESULT LoadBitmapFromFile(
    IWICImagingFactory* pIWICFactory,
    PCWSTR uri,
    UINT destinationWidth,
    UINT destinationHeight,
    ID2D1Bitmap** ppBitmap,
    IWICBitmapSource** ppBitmapSource
)
{
    HRESULT hr = S_OK;
    IWICBitmapDecoder* pDecoder = NULL;
    IWICBitmapFrameDecode* pSource = NULL;
    IWICFormatConverter* pConverter = NULL;
    IWICBitmapScaler* pScaler = NULL;
    if (pIWICFactory == NULL)
    {
        debugger_main.writelog(1,"pIWICFactory == 0");
        //MessageBox(NULL, "pIWICFactory ==0", "Error", 0);
    }
    hr = pIWICFactory->CreateDecoderFromFilename(
        uri,
        NULL,
        GENERIC_READ,
        WICDecodeMetadataCacheOnLoad,
        &pDecoder
    );
    if (SUCCEEDED(hr))
    {
        // Create the initial frame.
        hr = pDecoder->GetFrame(0, &pSource);
    }
    else
    {
        debugger_main.writelog(1,"Load pDecoder failed!");
        return hr;
    }
    if (SUCCEEDED(hr))
    {
        hr = pIWICFactory->CreateFormatConverter(&pConverter);
    }
    else
    {
        debugger_main.writelog(1,"Load pSource failed!");
        return hr;
    }
    // If a new width or height was specified, create an
    // IWICBitmapScaler and use it to resize the image.
    UINT originalWidth, originalHeight;
    hr = pSource->GetSize(&originalWidth, &originalHeight);
    if (destinationWidth != 0 || destinationHeight != 0)
    {
        if (SUCCEEDED(hr))
        {
            if (destinationWidth == 0)
            {
                FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
                destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
            }
            else if (destinationHeight == 0)
            {
                FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
                destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
            }
        }
    }
    else
    {
        destinationWidth = originalWidth;
        destinationHeight = originalHeight;
    }
    hr = pIWICFactory->CreateBitmapScaler(&pScaler);
    if (SUCCEEDED(hr))
    {
        hr = pScaler->Initialize(
            pSource,
            destinationWidth,
            destinationHeight,
            WICBitmapInterpolationModeHighQualityCubic
        );
    }
    else
    {
        debugger_main.writelog(1,"Scaler Initialize failed!");
        return hr;
    }
    if (SUCCEEDED(hr))
    {
        hr = pConverter->Initialize(
            pScaler,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            NULL,
            0.f,
            WICBitmapPaletteTypeMedianCut
        );
    }
    else
    {
        debugger_main.writelog(1,"Converter Initialize failed!");
        return hr;
    }
    if (SUCCEEDED(hr))
    {
        // Create a Direct2D bitmap from the WIC bitmap.
        hr = g_pD2DDeviceContext->CreateBitmapFromWicBitmap(
            pConverter,
            NULL,
            ppBitmap
        );
    }
    else
    {
        debugger_main.writelog(1,"CreateBitmapFromWicBitmap failed!");
        return hr;
    }
    if (ppBitmapSource == nullptr)
    {
        SAFE_RELEASE(pConverter);
    }
    else
    {
        *ppBitmapSource = pConverter;
    }
    SAFE_RELEASE(pDecoder);
    SAFE_RELEASE(pSource);
    SAFE_RELEASE(pScaler);
    return hr;
}



bool createShortcutBitmap(ID2D1Bitmap* dest)
{
    HRESULT hr;
    D2D1_POINT_2U point= D2D1::Point2U(0, 0);
    D2D1_RECT_U rect = D2D1::RectU(0, 0, orix, oriy);
    hr = dest->CopyFromBitmap(&point, g_pD2DTargetBimtap, &rect);
    if (FAILED(hr))
    {
        debugger_main.writelog(1,"shortcut_Bitmap CopyFromBitmap g_pD2DTargetBimtap failed!");
        return 0;
    }
    return 1;
}


bool createGaussianBlurEffectBitmap(ID2D1Bitmap* src, ID2D1Image* dest,bool interupt,float deviation)
{
    HRESULT hr;
    if (dest == NULL || src == NULL)
    {
        debugger_main.writelog(1,"invalid param in createGaussianBlurEffectBitmap()! null");
        return 0;
    }
    gaussianBlurEffect->SetInput(0, src);
    hr = gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);
    if (!SUCCEEDED(hr))
    {
        debugger_main.writelog(1,"SetValue gaussianBlurEffect OPTIMIZATION failed!");
        //quit_single = 1;
        return 0;
    }
    hr = gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_STANDARD_DEVIATION, deviation);
    if (!SUCCEEDED(hr))
    {
        debugger_main.writelog(1,"SetValue gaussianBlurEffect DEVIATION failed!");
        //quit_single = 1;
        return 0;
    }
    if (interupt)
    {
        if (main_thread_render_blocked == 0)
        {
            main_thread_render_blocked = 1;
            while (main_thread_render_blocked != 2)
            {
                Sleep(0);
                if (quit_single || normal_quit)
                {
                    return 0;
                }
            }
        }
        else
        {
            debugger_main.writelog(1,"createGaussianBlurEffectBitmap() failed! render busy");
            return 0;
        }
    }
    
    g_pD2DDeviceContext->SetTarget(dest);
    g_pD2DDeviceContext->BeginDraw();
    g_pD2DDeviceContext->Clear(D2D1::ColorF(0, 0.0f));
    g_pD2DDeviceContext->DrawImage(gaussianBlurEffect, D2D1::Point2F(0, 0), D2D1::RectF(0,0, src->GetSize().width, src->GetSize().height) , D2D1_INTERPOLATION_MODE_HIGH_QUALITY_CUBIC, D2D1_COMPOSITE_MODE_SOURCE_OVER);
    hr = g_pD2DDeviceContext->EndDraw();

    if (!SUCCEEDED(hr))
    {
        debugger_main.writelog(1,"EndDraw in createGaussianBlurEffectBitmap() failed! "+to_string(hr));
        quit_single = 1;
        Sleep(100);
        if (interupt)
        {
            main_thread_render_blocked = 0;
        }
        return 0;
    }
    g_pD2DDeviceContext->SetTarget(g_pD2DTargetBimtap);
    if (interupt)
    {
        main_thread_render_blocked = 0;
    }
    return 1;
}


void CreateD2DResource()
{
    debugger_main.writelog(DINFO, "in CreateD2DResource()",__LINE__);
    if (!g_pD2DDeviceContext)
    {
        HRESULT hr;
        //创建工厂
        hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED,
            __uuidof(ID2D1Factory1),
            reinterpret_cast<void**>(&g_pD2DFactory));
        if (FAILED(hr))
        {
            debugger_main.writelog(-1,"Create D2D factory failed!");
            quit_single = 1;
            return;
        }

        // Initialize Image Factory
        hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, __uuidof(pIWICFactory), (LPVOID*)&pIWICFactory);
        if (FAILED(hr))
        {
            writelog("Create WICImagingFactory failed!");
            quit_single = 1;
            return;
        }

        hr = DWriteCreateFactory(
            DWRITE_FACTORY_TYPE_SHARED,
            __uuidof(IDWriteFactory),
            reinterpret_cast<IUnknown**>(&g_pDWriteFactory)
        );
        if (FAILED(hr))
        {
            writelog("Create DWrite Factory failed!");
            quit_single = 1;
            return;
        }
        
        hr = g_pD2DFactory->CreateDevice(pDxgiDevice, &g_pD2DDevice);
        if (FAILED(hr))
        {
            debugger_main.writelog(-1,"Create D2D Device failed!");
            quit_single = 1;
            return;
        }
        hr = g_pD2DDevice->CreateDeviceContext(
            D2D1_DEVICE_CONTEXT_OPTIONS_NONE,
            &g_pD2DDeviceContext
        );
        if (FAILED(hr))
        {
            debugger_main.writelog(-1,"Create D2D DeviceContext failed!");
            quit_single = 1;
            return;
        }

        hr = g_pD2DDeviceContext->CreateBitmapFromDxgiSurface(
            pDxgiBackBuffer,
            &targetbitmapProperties,
            &g_pD2DTargetBimtap
        );
        
        if (FAILED(hr))
        {
            debugger_main.writelog(-1,"CreateBitmapFromDxgiSurface Failed.");
            quit_single = 1;
            return;
        }
        g_pD2DDeviceContext->SetTarget(g_pD2DTargetBimtap);

        g_pD2DDeviceContext->SetUnitMode(D2D1_UNIT_MODE_PIXELS);

        debugger_main.writelog(0,"d2d stage1 passed.");

        // Create a brush
        //通过渲染对象 创建一个固定颜色的画刷
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &g_pBrushBlack
        );
        if (FAILED(hr))
        {
            debugger_main.writelog(1,"Create brush failed!");
            quit_single = 1;
            return;
        }
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &g_pBrushQuit
        );

        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White),
            &g_pBrushWhite
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Gray),
            &g_pBrushGray
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::LightGray),
            &g_pBrushLightGray
        );

        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::SkyBlue),
            &g_pBrushBlue
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::DarkBlue),
            &g_pBrushDarkBlue
        );

        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::LightGoldenrodYellow),
            &g_pBrushLightYellow
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::LightSkyBlue),
            &g_pBrushLightBlue
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::MediumPurple),
            &g_pBrushPurple
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Yellow),
            &g_pBrushYellow
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::White),
            &g_pBrushLight
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &g_pBrushDark
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::SaddleBrown),
            &g_pBrushBrown
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::LightPink),
            &g_pBrushPink
        );
        g_pBrushLight->SetOpacity(0.5);
        g_pBrushDark->SetOpacity(0.5);
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::ForestGreen),
            &g_pBrushGreen
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::LightGreen),
            &g_pBrushLightGreen
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::FloralWhite),
            &g_pBrushFloralWhite
        ); 
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Red),
            &g_pBrushRed
        );
        /*hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &g_pBrushtext
        );*/
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::Black),
            &g_pBrushNetwork
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(1,0.5f,0),
            &g_pBrushBGSelect
        );
        hr = g_pD2DDeviceContext->CreateSolidColorBrush(
            D2D1::ColorF(D2D1::ColorF::OrangeRed),      
            &g_pBrushGameTemp
        );
        
        
        
        if (FAILED(hr))
        {
            writelog("Create brush failed!");
            quit_single = 1;
            return;
        }
        BrushRand[0] = g_pBrushGreen;
        BrushRand[1] = g_pBrushRed;
        BrushRand[2] = g_pBrushPurple;
        BrushRand[3] = g_pBrushYellow;
        BrushRand[4] = g_pBrushLightGreen;
        BrushRand[5] = g_pBrushBlue;
        BrushRand[6] = g_pBrushGreen;
        BrushRand[7] = g_pBrushGray;

        //float dashes[] = { 1 };
        if (SUCCEEDED(hr))
        {
            hr = g_pD2DFactory->CreateStrokeStyle(
                D2D1::StrokeStyleProperties(
                    D2D1_CAP_STYLE_ROUND,
                    D2D1_CAP_STYLE_ROUND,
                    D2D1_CAP_STYLE_ROUND,
                    D2D1_LINE_JOIN_ROUND,
                    10.0f,
                    D2D1_DASH_STYLE_SOLID,
                    0.0f),
                nullptr,
                0,
                &g_pStrokeStyle
            );
        }
        
        if (!SUCCEEDED(hr))
        {
            debugger_main.writelog(DERROR, "FAILED to CreateStrokeStyle", __LINE__);
        }
        
        D2D1_GRADIENT_STOP gradientStops[2];
        gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::White);
        gradientStops[0].position = 0.0f;
        gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Black);
        gradientStops[1].position = 1.0f;
        ID2D1GradientStopCollection* pGradientStops = NULL;
        // Create gradient stops collection
        hr = g_pD2DDeviceContext->CreateGradientStopCollection(
            gradientStops,
            2,
            D2D1_GAMMA_2_2,
            D2D1_EXTEND_MODE_CLAMP,
            &pGradientStops
        );
        if (FAILED(hr))
        {
            writelog("Create gradient stops collection failed!");
            quit_single = 1;
            return;
        }


        // Create a linear gradient brush to fill in the rectangle
        hr = g_pD2DDeviceContext->CreateLinearGradientBrush(
            D2D1::LinearGradientBrushProperties(
                D2D1::Point2F(0, 0),
                D2D1::Point2F(to_screen(1600), to_screen(900))),
            pGradientStops,
            &g_pLinearGradientBrush
        );

        if (FAILED(hr))
        {
            writelog("Create Linear gradient brush failed!");
            quit_single = 1;
            return;
        }
        g_pLinearGradientBrush->SetOpacity(0.4f);
        gradientStops[0].color = D2D1::ColorF(D2D1::ColorF::LightGreen);
        gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::DarkGreen);
        SAFE_RELEASE(pGradientStops)
        hr = g_pD2DDeviceContext->CreateGradientStopCollection(
            gradientStops,
            2,
            D2D1_GAMMA_2_2,
            D2D1_EXTEND_MODE_CLAMP,
            &pGradientStops
        );
        if (FAILED(hr))
        {
            writelog("Create gradient stops collection failed!");
            quit_single = 1;
            return;
        }
        hr = g_pD2DDeviceContext->CreateRadialGradientBrush(
            D2D1::RadialGradientBrushProperties(
                D2D1::Point2F(to_screen(0), to_screen(0)),
                D2D1::Point2F(to_screen(0), to_screen(0)),
                to_screen(50), to_screen(50)),
            pGradientStops,
            &g_pRadialGradientBrush
        );
        if (FAILED(hr))
        {
            writelog("Create Radial gradient brush failed!");
            quit_single = 1;
            return;
        }
        /*hr = g_pD2DDeviceContext->CreateRadialGradientBrush(
            D2D1::RadialGradientBrushProperties(
                D2D1::Point2F(0, 0),
                D2D1::Point2F(0, 0),
                to_screen(40), to_screen(40)),
            pGradientStops,
            &g_pRadialGradientBrush2
        );
        if (FAILED(hr))
        {
            writelog("Create Radial gradient brush failed!");
            quit_single = 1;
            return;
        }*/
        gradientStops[0].color = D2D1::ColorF(D2D1::ColorF(0, 0.0f));
        gradientStops[1].color = D2D1::ColorF(D2D1::ColorF::Red);
        SAFE_RELEASE(pGradientStops)
        hr = g_pD2DDeviceContext->CreateGradientStopCollection(
            gradientStops,
            2,
            D2D1_GAMMA_2_2,
            D2D1_EXTEND_MODE_CLAMP,
            &pGradientStops
        );
        if (FAILED(hr))
        {
            writelog("Create gradient stops collection failed!");
            quit_single = 1;
            return;
        }
        hr = g_pD2DDeviceContext->CreateRadialGradientBrush(
            D2D1::RadialGradientBrushProperties(
                D2D1::Point2F(to_screen(800), to_screen(450)),
                D2D1::Point2F(0, 0),
                to_screen(800), to_screen(450)),
            pGradientStops,
            &g_pRadialGradientBrush2
        );
        if (FAILED(hr))
        {
            writelog("Create Radial gradient brush failed!");
            quit_single = 1;
            return;
        }
        
        SAFE_RELEASE(pGradientStops);
        debugger_main.writelog(0,"d2d stage2 passed.");

        
        LoadFonts();
        
        
        
        if (set2[0].MSAA==2)
        {
            g_pD2DDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            g_pD2DDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_CLEARTYPE);
        }
        else if (set2[0].MSAA == 1)
        {
            g_pD2DDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
            g_pD2DDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_GRAYSCALE);
        }
        else
        {
            g_pD2DDeviceContext->SetAntialiasMode(D2D1_ANTIALIAS_MODE_ALIASED);
            g_pD2DDeviceContext->SetTextAntialiasMode(D2D1_TEXT_ANTIALIAS_MODE_ALIASED);
        }

        g_pD2DDeviceContext->SetTarget(g_pD2DTargetBimtap);
    }
    
    init_string();
    tip_used_num = rand() % 16;
    debugger_main.writelog(0,"DirectX was built.");
    thread_IO_request_verify_res = -1;
    return;
}

void LoadFonts()
{
    HRESULT hr = S_OK;
    SAFE_RELEASE(g_pTextFormat)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"得意黑",                   // Font family name
        NULL,                          // Font collection(NULL sets it to the system font collection)
        DWRITE_FONT_WEIGHT_REGULAR,    // Weight
        DWRITE_FONT_STYLE_NORMAL,      // Style
        DWRITE_FONT_STRETCH_NORMAL,    // Stretch
        to_screen(36.0f),              // Size    
        L"en-us",                      // Local
        &g_pTextFormat                 // Pointer to recieve the created object
    );
    if (FAILED(hr))
    {
        writelog("Create IDWriteTextFormat failed!");
        quit_single = 1;
        return;
    }
    g_rm.setFont("DEYIHEI", g_pTextFormat);
    g_rm.setFontIndex("DEYIHEI", 2);

    SAFE_RELEASE(g_pTextFormatLarge)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"Pacifico",                   // Font family name
        NULL,                          // Font collection(NULL sets it to the system font collection)
        DWRITE_FONT_WEIGHT_REGULAR,    // Weight
        DWRITE_FONT_STYLE_NORMAL,      // Style
        DWRITE_FONT_STRETCH_NORMAL,    // Stretch
        to_screen(144.0f),             // Size    
        L"en-us",                      // Local
        &g_pTextFormatLarge            // Pointer to recieve the created object
    );
    g_rm.setFont("Pacifico L", g_pTextFormatLarge);

    SAFE_RELEASE(g_pTextFormatLargeCN)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"得意黑",                   // Font family name
        NULL,                          // Font collection(NULL sets it to the system font collection)
        DWRITE_FONT_WEIGHT_REGULAR,    // Weight
        DWRITE_FONT_STYLE_NORMAL,      // Style
        DWRITE_FONT_STRETCH_NORMAL,    // Stretch
        to_screen(144.0f),              // Size    
        L"en-us",                      // Local
        &g_pTextFormatLargeCN                 // Pointer to recieve the created object
    );
    g_rm.setFont("DEYIHEI L", g_pTextFormatLargeCN);

    SAFE_RELEASE(g_pTextFormatL)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"得意黑",
        NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        to_screen(32.0f),
        L"en-us",
        &g_pTextFormatL
    );
    g_rm.setFont("DEYIHEI LEFT", g_pTextFormatL);

    SAFE_RELEASE(g_pTextFormat2)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"Pacifico",                   // Font family name
        NULL,                          // Font collection(NULL sets it to the system font collection)
        DWRITE_FONT_WEIGHT_REGULAR,    // Weight
        DWRITE_FONT_STYLE_NORMAL,      // Style
        DWRITE_FONT_STRETCH_NORMAL,    // Stretch
        to_screen(30.0f),              // Size    
        L"en-us",                      // Local
        &g_pTextFormat2                 // Pointer to recieve the created object
    );
    g_rm.setFont("Pacifico", g_pTextFormat2);

    SAFE_RELEASE(g_pTextFormatmini)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"Quarlow",
        NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        to_screen(50.0),
        L"en-us",
        &g_pTextFormatmini
    );
    g_rm.setFont("Quarlow", g_pTextFormatmini);

    SAFE_RELEASE(g_pTextFormatminiL)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"Quarlow",
        NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        to_screen(50.0f),
        L"en-us",
        &g_pTextFormatminiL
    );
    g_rm.setFont("Quarlow LEFT", g_pTextFormatminiL);

    SAFE_RELEASE(g_pTextFormatQuarlowLarge)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"Quarlow",
        NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        to_screen(128.0f),
        L"en-us",
        &g_pTextFormatQuarlowLarge
    );
    g_rm.setFont("Quarlow L", g_pTextFormatQuarlowLarge);

    SAFE_RELEASE(g_pTextFormatNormal)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"Public Sans",
        //L"NSimSun",
        //L"Gabriola",
        NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        to_screen(36.0f),
        L"en-us",
        &g_pTextFormatNormal
    );
    g_rm.setFont("PUBLIC", g_pTextFormatNormal);
    g_rm.setFont("default", g_pTextFormatNormal);

    SAFE_RELEASE(g_pTextFormatNormalmini)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"Public Sans",
        NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        to_screen(24.0f),
        L"en-us",
        &g_pTextFormatNormalmini
    );
    g_rm.setFont("PUBLIC M", g_pTextFormatNormalmini);

    SAFE_RELEASE(g_pTextFormatNormalLarge)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"Public Sans",
        NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        to_screen(64.0f),
        L"en-us",
        &g_pTextFormatNormalLarge
    );
    g_rm.setFont("PUBLIC L", g_pTextFormatNormalLarge);

    SAFE_RELEASE(g_pTextFormat_debug)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"Public Sans",
        NULL,
        DWRITE_FONT_WEIGHT_REGULAR,
        DWRITE_FONT_STYLE_NORMAL,
        DWRITE_FONT_STRETCH_NORMAL,
        to_screen(16.0f),
        L"en-us",
        &g_pTextFormat_debug
    );
    g_rm.setFont("DBG", g_pTextFormat_debug);

    SAFE_RELEASE(g_pTextFormatNetwork)
    hr = g_pDWriteFactory->CreateTextFormat(
        L"SocialNetworkingIcons-Rounded",                   // Font family name
        NULL,                          // Font collection(NULL sets it to the system font collection)
        DWRITE_FONT_WEIGHT_REGULAR,    // Weight
        DWRITE_FONT_STYLE_NORMAL,      // Style
        DWRITE_FONT_STRETCH_NORMAL,    // Stretch
        to_screen(50.0f),              // Size    
        L"en-us",                      // Local
        &g_pTextFormatNetwork          // Pointer to recieve the created object
    );
    g_rm.setFont("ICON", g_pTextFormatNetwork);

    g_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);           //水平居中
    g_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER); //段落居中
    g_pTextFormatL->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    g_pTextFormatmini->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    g_pTextFormatmini->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    g_pTextFormatminiL->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    g_pTextFormatQuarlowLarge->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    g_pTextFormatQuarlowLarge->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    g_pTextFormat2->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    g_pTextFormat2->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    g_pTextFormatNormal->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    g_pTextFormatNormal->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    g_pTextFormatLarge->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    g_pTextFormatLarge->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    g_pTextFormatLargeCN->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    g_pTextFormatLargeCN->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    g_pTextFormatNetwork->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_TRAILING);
    g_pTextFormatNetwork->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_FAR);
    g_pTextFormatNormalmini->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    g_pTextFormatNormalmini->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
    g_pTextFormatNormalLarge->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
    g_pTextFormatNormalLarge->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);

    g_pTextFormat_debug->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
    g_pTextFormat_debug->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

    fpsRect = D2D1::RectF(
        static_cast<FLOAT>(0),
        static_cast<FLOAT>(0),
        static_cast<FLOAT>(to_screen(150)),
        static_cast<FLOAT>(to_screen(50))
    );

    textrbRect = D2D1::RectF(
        static_cast<FLOAT>(orix - to_screen(200)),
        static_cast<FLOAT>(oriy - to_screen(100)),
        static_cast<FLOAT>(orix),
        static_cast<FLOAT>(oriy)
    );
    return;
}


void DrawBitmap_1(ID2D1Bitmap* ppBitmap, D2D_RECT_F dest, float opacity)
{
    if (ppBitmap == NULL)
    {
        writelog(2,"error in DrawBitmap_1() while param Bitmap source is null! dest= "+ to_string(dest.left) + " " + to_string(dest.top));
        normal_quit = 1;
        return;
    }
    HRESULT hr = S_OK;
    D2D_RECT_F NDEST;
    D2D1_SIZE_F size = ppBitmap->GetSize();
    UINT originalWidth, originalHeight, destinationHeight = dest.bottom - dest.top, destinationWidth = dest.right - dest.left;
    originalWidth = size.width;
    originalHeight = size.height;
    if (destinationHeight != 0 || destinationWidth != 0)    //等比例缩放
    {

        if (SUCCEEDED(hr))
        {
            if (destinationWidth == 0)
            {
                FLOAT scalar = static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight);
                destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
            }
            else if (destinationHeight == 0)
            {
                FLOAT scalar = static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth);
                destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
            }


        }
    }
    else
    {
        destinationWidth = originalWidth;
        destinationHeight = originalHeight;
    }
    dest.bottom = dest.top + destinationHeight;
    dest.right = dest.left + destinationWidth;
    // Draw bitmap
    NDEST.top = to_screen(dest.top);
    NDEST.bottom = to_screen(dest.bottom);
    NDEST.right = to_screen(dest.right);
    NDEST.left = to_screen(dest.left);
    if (set2[0].MSAA!=0)
    {
        g_pD2DDeviceContext->DrawBitmap(
            ppBitmap,
            NDEST,
            opacity,
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR
        );
    }
    else
    {
        g_pD2DDeviceContext->DrawBitmap(
            ppBitmap,
            NDEST,
            opacity,
            D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR
        );
    }
    DrawCallNum++;
    return;
}

void DrawBitmap_1(ID2D1Bitmap* ppBitmap, D2D_RECT_F dest, D2D_RECT_F src, float opacity)
{
    if (ppBitmap == NULL)
    {
        writelog(2,"error in DrawBitmap_1() while param Bitmap source is null! " + to_string(dest.left) + " " + to_string(dest.top));
        normal_quit = 1;
        return;
    }
    HRESULT hr = S_OK;
    D2D_RECT_F NDEST;
    D2D1_SIZE_F size = ppBitmap->GetSize();
    UINT originalWidth, originalHeight, destinationHeight = dest.bottom - dest.top, destinationWidth = dest.right - dest.left;
    originalWidth = size.width;
    originalHeight = size.height;
    if (destinationHeight != 0 || destinationWidth != 0)    //等比例缩放
    {
        if (SUCCEEDED(hr))
        {
            if (destinationWidth == 0)
            {
                FLOAT scalar = fabs(static_cast<FLOAT>(destinationHeight) / static_cast<FLOAT>(originalHeight));
                destinationWidth = static_cast<UINT>(scalar * static_cast<FLOAT>(originalWidth));
            }
            else if (destinationHeight == 0)
            {
                FLOAT scalar = fabs(static_cast<FLOAT>(destinationWidth) / static_cast<FLOAT>(originalWidth));
                destinationHeight = static_cast<UINT>(scalar * static_cast<FLOAT>(originalHeight));
            }
        }
    }
    else
    {
        destinationWidth = originalWidth;
        destinationHeight = originalHeight;
    }

    dest.bottom = dest.top + destinationHeight;
    dest.right = dest.left + destinationWidth;
    NDEST.top = to_screen(dest.top);
    NDEST.bottom = to_screen(dest.bottom);
    NDEST.right = to_screen(dest.right);
    NDEST.left = to_screen(dest.left);
    // Draw bitmap
    if (set2[0].MSAA!=0)
    {
        g_pD2DDeviceContext->DrawBitmap(
            ppBitmap,
            NDEST,
            opacity,
            D2D1_BITMAP_INTERPOLATION_MODE_LINEAR,
            src
        );
    }
    else
    {
        g_pD2DDeviceContext->DrawBitmap(
            ppBitmap,
            NDEST,
            opacity,
            D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
            src
        );
    }
    DrawCallNum++;
    return;
}

void DrawEffect_1(ID2D1Effect *effect)
{
    if (set2[0].MSAA == 2)
    {
        g_pD2DDeviceContext->DrawImage(effect, D2D1::Point2F(0, 0), D2D1::RectF(to_screen(0), to_screen(0), to_screen(1600), to_screen(900)), D2D1_INTERPOLATION_MODE_ANISOTROPIC, D2D1_COMPOSITE_MODE_SOURCE_OVER);
    }
    else if (set2[0].MSAA == 1)
    {
        g_pD2DDeviceContext->DrawImage(effect, D2D1::Point2F(0, 0), D2D1::RectF(to_screen(0), to_screen(0), to_screen(1600), to_screen(900)), D2D1_INTERPOLATION_MODE_LINEAR, D2D1_COMPOSITE_MODE_SOURCE_OVER);
    }
    else
    {
        g_pD2DDeviceContext->DrawImage(effect, D2D1::Point2F(0, 0), D2D1::RectF(to_screen(0), to_screen(0), to_screen(1600), to_screen(900)), D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR, D2D1_COMPOSITE_MODE_SOURCE_OVER);
    }
    DrawCallNum++;
    return;
}



wchar_t* multiByteToWideChar(const string& pKey)
{
    static int maxSize = 64;
    static wchar_t * pWCStrKey = new wchar_t[maxSize];
    const char* pCStrKey = pKey.c_str();
    //第一次调用返回转换后的字符串长度，用于确认为wchar_t*开辟多大的内存空间
    int pSize = MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, NULL, 0);
    if (pSize > maxSize)
    {
        delete[]pWCStrKey;
        maxSize = pSize;
        pWCStrKey= new wchar_t[maxSize];
    }
    //第二次调用将单字节字符串转换成双字节字符串
    MultiByteToWideChar(CP_OEMCP, 0, pCStrKey, strlen(pCStrKey) + 1, pWCStrKey, pSize);

    return pWCStrKey;
}


void DrawTextA_1(string content, IDWriteTextFormat* TextFormat, const D2D1_RECT_F rect, ID2D1Brush* brush, float opacity)
{
    if (content.empty())
    {
        return;
    }
    brush->SetOpacity(opacity);
    wchar_t* text = multiByteToWideChar(content);
    g_pD2DDeviceContext->DrawTextA(
        text,           // Text to render  
        wcslen(text),       // Text length
        TextFormat,     // Text format
        rect,    // The region of the window where the text will be rendered
        brush      // The brush used to draw the text
    );
    //delete[] text;
    brush->SetOpacity(1.0f);
    DrawCallNum++;
    return;
}

void DrawTextA_1(string content, IDWriteTextFormat* TextFormat, float x1, float y1, float x2, float y2, ID2D1Brush* brush, float opacity)
{
    brush->SetOpacity(opacity);
    wchar_t* text = multiByteToWideChar(content);
    g_pD2DDeviceContext->DrawTextA(
        text,           // Text to render  
        wcslen(text),       // Text length
        TextFormat,     // Text format
        D2D1::RectF(to_screen(x1), to_screen(y1), to_screen(x2), to_screen(y2)),    // The region of the window where the text will be rendered
        brush      // The brush used to draw the text
    );
    //delete[] text;
    brush->SetOpacity(1.0f);
    DrawCallNum++;
    return;
}

void DrawNumber_1(int content, float x1, float y1, float x2, float y2, float opacity, int style)
{
    constexpr float slength = 25, sheight = 48;
    const static D2D1_RECT_F src[10] = { D2D1::RectF(0, 0, to_screen(slength), to_screen(sheight)) ,D2D1::RectF(to_screen(slength * 1), 0, to_screen(slength * 2), to_screen(sheight)),
    D2D1::RectF(to_screen(slength * 2), 0, to_screen(slength * 3), to_screen(sheight)) ,D2D1::RectF(to_screen(slength * 3), 0, to_screen(slength * 4), to_screen(sheight)) ,
    D2D1::RectF(to_screen(slength * 4), 0, to_screen(slength * 5), to_screen(sheight)) ,D2D1::RectF(to_screen(slength * 5), 0, to_screen(slength * 6), to_screen(sheight)) ,
    D2D1::RectF(to_screen(slength * 6), 0, to_screen(slength * 7), to_screen(sheight)) ,D2D1::RectF(to_screen(slength * 7), 0, to_screen(slength * 8), to_screen(sheight)) ,
    D2D1::RectF(to_screen(slength * 8), 0, to_screen(slength * 9), to_screen(sheight)) ,D2D1::RectF(to_screen(slength * 9), 0, to_screen(slength * 10), to_screen(sheight)) };
    int digit = 0,contents[12];
    while (content > 0)
    {
        contents[digit++] = content % 10;
        content /= 10;
    }
    if (digit == 0)
    {
        digit = 1;
        contents[0] = 0;
    }
    float single_l ,single_h;
    if (style == 0)
    {
        single_h = y2 - y1;
        single_l = min((x2 - x1) / digit, single_h * 0.521f);
    }
    else
    {
        single_l = (x2 - x1) / digit;
        single_h = min(y2 - y1, single_l * 1.92f);
    }
    float s_x1, s_y1, s_x2, s_y2, endx = 0.5f * (x1 + x2) + 0.5f * digit * single_l, starty = 0.5f * (y1 + y2 - single_h);

    for (int i = 0; i < digit; i++)
    {
        s_x1 = endx - i * single_l;
        s_x2 = s_x1 + single_l;
        s_y1 = starty;
        s_y2 = starty + single_h;
        DrawBitmap_1(numberBitmaps, D2D1::RectF(s_x1, s_y1, s_x2, s_y2), src[contents[i]], opacity);
    }
    return;
}

void DrawLine_1(float x1,float y1, float x2, float y2,float width,ID2D1Brush *brush,float opacity, ID2D1StrokeStyle* strokeStyle)
{
    brush->SetOpacity(opacity);
    g_pD2DDeviceContext->DrawLine(D2D1::Point2F(to_screen(x1), to_screen(y1)), D2D1::Point2F(to_screen(x2), to_screen(y2)), brush, to_screen(width), strokeStyle);
    brush->SetOpacity(1.0f);
    DrawCallNum++;
    return;
}

void DrawRectangle_1(float x1, float y1, float x2, float y2, float width, ID2D1Brush* brush, float opacity)
{
    brush->SetOpacity(opacity);
    g_pD2DDeviceContext->DrawRectangle(D2D1::RectF(to_screen(x1), to_screen(y1), to_screen(x2), to_screen(y2)), brush,  to_screen(width));
    brush->SetOpacity(1.0f);
    DrawCallNum++;
    return;
}

void FillRectangle_1(float x1, float y1, float x2, float y2, ID2D1Brush* brush, float opacity)
{
    brush->SetOpacity(opacity);
    g_pD2DDeviceContext->FillRectangle(D2D1::RectF(to_screen(x1), to_screen(y1), to_screen(x2), to_screen(y2)), brush);
    brush->SetOpacity(1.0f);
    DrawCallNum++;
    return;
}

void FillEllipse_1(float x, float y, float radiusX, float radiusY, ID2D1Brush* brush, float opacity)
{
    brush->SetOpacity(opacity);
    g_pD2DDeviceContext->FillEllipse(D2D1::Ellipse(D2D1::Point2F(to_screen(x), to_screen(y)), to_screen(radiusX), to_screen(radiusY)), brush);
    brush->SetOpacity(1.0f);
    DrawCallNum++;
    return;
}

void DrawTriangle_1(float x1, float y1, float x2, float y2, float x3, float y3, ID2D1Brush* brush, float width, float opacity)
{
    DrawLine_1(x1, y1, x2, y2, width, brush, opacity, g_pStrokeStyle);
    DrawLine_1(x1, y1, x3, y3, width, brush, opacity, g_pStrokeStyle);
    DrawLine_1(x3, y3, x2, y2, width, brush, opacity, g_pStrokeStyle);
    return;
}

void DrawRoundedRectangle_1(float x1, float y1, float x2, float y2, float width, ID2D1Brush* brush, float opacity)
{
    brush->SetOpacity(opacity);
    g_pD2DDeviceContext->DrawRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(to_screen(x1), to_screen(y1), to_screen(x2), to_screen(y2)), to_screen((x2 - x1) * 0.05f), to_screen((y2 - y1) * 0.05f)), brush,to_screen(width));
    brush->SetOpacity(1.0f);
    DrawCallNum++;
    return;
}

void FillRoundedRectangle_1(float x1, float y1, float x2, float y2, ID2D1Brush* brush, float opacity)
{
    brush->SetOpacity(opacity);
    g_pD2DDeviceContext->FillRoundedRectangle(D2D1::RoundedRect(D2D1::RectF(to_screen(x1), to_screen(y1), to_screen(x2), to_screen(y2)), to_screen((x2 - x1) * 0.05f), to_screen((y2 - y1) * 0.05f)), brush);
    brush->SetOpacity(1.0f);
    DrawCallNum++;
    return;
}
//
//void DrawEllipse_1(D2D1_ELLIPSE* Ellipse, float width, ID2D1Brush* brush, float opacity)
//{
//    brush->SetOpacity(opacity);
//    g_pD2DDeviceContext->DrawEllipse(Ellipse, brush, to_screen(width));
//    brush->SetOpacity(1.0f);
//    DrawCallNum++;
//    return;
//}

float Calc_Triangle_Area(float x1, float y1, float x2, float y2, float x3, float y3) 
{
     return fabs((x1 * y2 - x1 * y3 - x2 * y1 + x3 * y1 + x2 * y3 - x3 * y2) *0.5f);
}

//0 for lines, 1 for rows
bool Get_Triangle_min_side(float x1, float y1, float x2, float y2, float x3, float y3,float *min_side_length)
{
    float dx = max(x1, max(x2, x3)) - min(x1, min(x2, x3)), dy = max(y1, max(y2, y3)) - min(y1, min(y2, y3));
    if ( dx < dy )
    {
        *min_side_length = dx;
        return 1;
    }
    else
    {
        *min_side_length = dy;
        return 0;
    }
}

// 待优化 优化后消耗性能取决于三角形的最小高  accuracy应大于1，越大越精确
void FillTriangle_1(float x1, float y1, float x2, float y2, float x3, float y3, ID2D1Brush* brush,float opacity,int accuracy)
{
    
    accuracy *= scale;
    //计算p3到p1-p2的高
    float stroke_width = Calc_Triangle_Area(x1, y1, x2, y2, x3, y3) / sqrtf((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    stroke_width *= 2.1f;   //magic number
    stroke_width /= accuracy;
    float target_dx1 = (x3 - x1)/ accuracy, target_dy1 = (y3 - y1)/ accuracy, target_dx2 = (x3 - x2) / accuracy, target_dy2 = (y3 - y2) / accuracy;
    //Get_Triangle_min_side(x1, y1, x2, y2, x3, y3,&stroke_width)
    if (1)
    {
        for (int i = 0; i < accuracy; i++)
        {
            DrawLine_1(x1 + (i + 0.5f) * target_dx1, y1 + (i + 0.5f) * target_dy1, x2 + (i + 0.5f) * target_dx2, y2 + (i + 0.5f) * target_dy2, stroke_width, brush, opacity);
        }
        //Draw triangle outline
        DrawLine_1(x1, y1, x2, y2, stroke_width / 2, brush, opacity);
        DrawLine_1(x1, y1, x3, y3, stroke_width / 2, brush, opacity);
        DrawLine_1(x3, y3, x2, y2, stroke_width / 2, brush, opacity);
    }
    return;
}

void DrawArc_1(float center_posx, float center_posy, float r, float angle1, float angle2, bool largeArc, ID2D1Brush* brush, float stroke_width , float opacity)
{
    ID2D1PathGeometry* arcGeometry = nullptr;
    static bool logged = 0;
    HRESULT hr = S_OK;
    D2D1_ARC_SEGMENT arc;
    ID2D1GeometrySink* pSink = NULL;
    
    arc = D2D1::ArcSegment(D2D1::Point2F(to_screen(center_posx + r * cosf(angle2)), to_screen(center_posy + r * sinf(angle2))), D2D1::SizeF(to_screen(r), to_screen(r)), angle2 - angle1, D2D1_SWEEP_DIRECTION_CLOCKWISE, largeArc? D2D1_ARC_SIZE_LARGE:D2D1_ARC_SIZE_SMALL);
    
    hr = g_pD2DFactory->CreatePathGeometry(&arcGeometry);
    if (!SUCCEEDED(hr))
    {
        debugger_main.writelog(1, "CreatePathGeometry failed in DrawArc_1() " + to_string(hr));
        return;
    }
    hr = arcGeometry->Open(&pSink); // 获取Sink对象
    if (!SUCCEEDED(hr))
    {
        debugger_main.writelog(1, "Open pSink failed in DrawArc_1() " + to_string(hr));
        return;
    }
    
    
    if (arcGeometry!= nullptr&& pSink)
    {
        pSink->BeginFigure(D2D1::Point2F(to_screen(center_posx + r * cosf(angle1)), to_screen(center_posy + r * sinf(angle1))), D2D1_FIGURE_BEGIN_HOLLOW);
        // 添加图形
        pSink->AddArc(arc);
        pSink->EndFigure(D2D1_FIGURE_END_OPEN);
        hr = pSink->Close();
        if (!SUCCEEDED(hr))
        {
            debugger_main.writelog(1, "pSink Close() failed in DrawArc_1() " + to_string(hr));
            return;
        }
        SAFE_RELEASE(pSink);
    }
    else
    {
        if (!logged)
        {
            debugger_main.writelog(1, "DrawArc_1() not prepared while arcGeometry or pSink is null ");
            logged = 1;
            return;
        }
    }
    
    brush->SetOpacity(opacity);
    g_pD2DDeviceContext->DrawGeometry(arcGeometry, brush, to_screen(stroke_width));
    brush->SetOpacity(1.0f);
    SAFE_RELEASE(arcGeometry);
    DrawCallNum++;
    return;
}

void FillOpacityMask_1(float x1, float y1, float x2, float y2, ID2D1Brush* brush, ID2D1Bitmap* mask, float opacity)
{
    brush->SetOpacity(opacity);
    g_pD2DDeviceContext->FillOpacityMask(mask, brush, D2D1::RectF(to_screen(x1), to_screen(y1), to_screen(x2), to_screen(y2)));
    DrawCallNum++;
    brush->SetOpacity(1.0f);
    return;
}


void update()   
{
    

    t_end = Clock::now();
    durnanosec = chrono::duration_cast<std::chrono::nanoseconds>(t_end - t_start).count();
    frmtm = (float)durnanosec / 1000000000;
    //g_nm.heartBeat_time -= frmtm;
    t_start = t_end;
    dur20 += durnanosec;
    nframe++;
    if (dur20 > 1000000000)  //每1s计算帧率
    {
        fps = nframe;
        nframe = 0;
        dur20 = 0;
    }
    update_target_page();
    input();

    wheel_move_value = 0;
    if (fabs(wheel_move_value_temp) > 0.1f)
    {
        wheel_move_value = wheel_move_value_temp;
        wheel_move_value_temp = 0;
    }

    return;
}








int last_page_index = 0;
int saveDeleted = 0, recorded = 0;

HANDLE hThreadLog, hThreadAudio;

void init_once()
{
    GetLocalTime(&st);
    hThreadLog = (HANDLE)_beginthreadex(NULL, 0, File_IO, NULL, 0, NULL);	//创建线程

    if (hThreadLog == 0)
    {
        MessageBoxEx(NULL, "Failed to create IO thread", "Error", MB_OK | MB_ICONSTOP, NULL);
        quit_single = 1;
        return;
    }
    srand((unsigned int)GetTickCount64());
    RECT winrc;
    SetPriorityClass(GetCurrentProcess(), ABOVE_NORMAL_PRIORITY_CLASS);
    SetProcessPriorityBoost(GetCurrentProcess(), TRUE);
    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
    GetClientRect(hWnd, &rc);
    
    if (nWidth >= 7680 && nHeight >= 4320)
    {
        orix = 3840;
        oriy = 2160;
        scale = 2.4f;
        set2[0].resolution = 6;
        set2[1].resolution = 6;
    }
    else if (nWidth >= 3840 && nHeight >= 2160)
    {
        orix = 2560;
        oriy = 1440;
        scale = 1.6f;
        set2[0].resolution = 5;
        set2[1].resolution = 5;
    }
    else if (nWidth >= 2560 && nHeight >= 1440)
    {
        orix = 1920;
        oriy = 1080;
        scale = 1.2f;
        set2[0].resolution = 4;
        set2[1].resolution = 4;
    }
    else if (nWidth >= 1920 && nHeight >= 1080)
    {
        orix = 1600;
        oriy = 900;
        scale = 1.0f;
        set2[0].resolution = 3;
        set2[1].resolution = 3;
    }
    else if (nWidth >= 1280 && nHeight >= 760)
    {
        orix = 1280;
        oriy = 720;
        scale = 0.8f;
        set2[0].resolution = 2;
        set2[1].resolution = 2;
    }
    else
    {
        orix = 800;
        oriy = 450;
        scale = 0.5f;
        set2[0].resolution = 1;
        set2[1].resolution = 1;
    }
    RECT adj;
    adj.left = 50;
    adj.top = 50;
    adj.right = orix + 50;
    adj.bottom = oriy + 50;
    int window_x= orix, window_y= oriy;
    int io_wait_num = 0;
    while (io_wait_num < 20&& !thread_IO_config_read)
    {
        //writelog("waiting IO thread...");
        Sleep(50);
        io_wait_num++;
    }

    if (!thread_IO_config_read)
    {
        thread_IO_config_read = 1;
        debugger_main.writelog(DWARNNING, "use default config.");
        debugger_main.writelog(DINFO, "target resolution: " + to_string(orix) + "x" + to_string(oriy) + " from " + to_string(nWidth) + "x" + to_string(nHeight));
    }
    else
    {
        switch (set2[0].resolution)
        {
        case 1:
            orix = 800;
            oriy = 450;
            scale = 0.5;
            break;
        case 2:
            orix = 1280;
            oriy = 720;
            scale = 0.8f;
            break;
        case 3:
            orix = 1600;
            oriy = 900;
            scale = 1.0;
            break;
        case 4:
            orix = 1920;
            oriy = 1080;
            scale = 1.2f;
            break;
        case 5:
            orix = 2560;
            oriy = 1440;
            scale = 1.6f;
            break;
        case 6:
            orix = 3840;
            oriy = 2160;
            scale = 2.4f;
            break;
        case 7:
            orix = 7680;
            oriy = 4320;
            scale = 4.8f;
            break;
        default:
            orix = 1600;
            oriy = 900;
            scale = 1.0;
            set2[0].resolution = 3;
            break;
        }

    }
    
    int limit = 2500;
    
    if (!AdjustWindowRect(&adj, WS_OVERLAPPEDWINDOW, 0))
    {
        writelog("AdjustWindowRect() failed!");
    }
    else
    {
        //writelog(to_string(adj.left) + "-" + to_string(adj.right) + " " + to_string(adj.top) + "-" + to_string(adj.bottom));
        SetWindowPos(hWnd, HWND_TOP, 0, 0, adj.right - adj.left, adj.bottom - adj.top, NULL);
    }
    
    //微调大小
    while (1)
    {
        GetWindowRect(hWnd, &winrc);
        GetClientRect(hWnd, &rc);
        if (rc.bottom - rc.top + 5 < window_y)
        {
            SetWindowPos(hWnd, HWND_TOP, 0, 0, winrc.right - winrc.left, winrc.bottom - winrc.top + 5, NULL);
            limit--;
        }
        else if (rc.bottom - rc.top - 5 > window_y)
        {
            SetWindowPos(hWnd, HWND_TOP, 0, 0, winrc.right - winrc.left, winrc.bottom - winrc.top - 5, NULL);
            limit--;
        }
        else if (rc.right - rc.left + 5 < window_x)
        {
            SetWindowPos(hWnd, HWND_TOP, 0, 0, winrc.right - winrc.left + 5, winrc.bottom - winrc.top, NULL);
            limit--;
        }
        else if (rc.right - rc.left - 5 > window_x)
        {
            SetWindowPos(hWnd, HWND_TOP, 0, 0, winrc.right - winrc.left - 5, winrc.bottom - winrc.top, NULL);
            limit--;
        }
        else if (rc.bottom - rc.top < window_y)
        {
            SetWindowPos(hWnd, HWND_TOP, 0, 0, winrc.right - winrc.left, winrc.bottom - winrc.top + 1, NULL);
            limit--;
        }
        else if (rc.bottom - rc.top > window_y)
        {
            SetWindowPos(hWnd, HWND_TOP, 0, 0, winrc.right - winrc.left, winrc.bottom - winrc.top - 1, NULL);
            limit--;
        }
        else if (rc.right - rc.left < window_x)
        {
            SetWindowPos(hWnd, HWND_TOP, 0, 0, winrc.right - winrc.left + 1, winrc.bottom - winrc.top, NULL);
            limit--;
        }
        else if (rc.right - rc.left > window_x)
        {
            SetWindowPos(hWnd, HWND_TOP, 0, 0, winrc.right - winrc.left - 1, winrc.bottom - winrc.top, NULL);
            limit--;
        }
        else
        {
            break;
        }
        if (limit <= 0)
        {
            SetWindowPos(hWnd, HWND_TOP, 0, 0, 800, 600, NULL); //safe window
            break;
        }
    }
    debugger_main.writelog(0,"Window resized @" + to_string(window_x) + "x" + to_string(window_y));

    hThreadAudio = (HANDLE)_beginthreadex(NULL, 0, ThreadPlayMusic, NULL, 0, NULL);	//创建线程
    if (hThreadAudio == 0)
    {
        writelog("Failed to create Audio thread!");
        MessageBoxEx(NULL, "Failed to create Audio thread!", "Error", MB_OK | MB_ICONSTOP, NULL);
        quit_single = 1;
        return;
    }
    SetThreadPriority(hThreadAudio, THREAD_PRIORITY_ABOVE_NORMAL);

    g_hIMC = ImmAssociateContext(hWnd, NULL);   //输入法锁定

    CreateD3DResource(hWnd);
    debugger_main.writelog(0,"Render resolution @" + to_string(orix) + "x" + to_string(oriy));

    res[0].filename = ".\\pic\\home_setting.png"; res[0].md5 = "D99A737976059CB3A2D89928A47B0C36";//textures of ui
    res[1].filename = ".\\pic\\home_exit.png"; res[1].md5 = "3B94B2D92E76B074438255DA74E7DE41";
    res[2].filename = ".\\pic\\setting_general.png"; res[2].md5 = "A502C14ED186CBB5B29ABFB01B52F405";
    res[3].filename = ".\\pic\\setting_graphics1.png"; res[3].md5 = "CB5406ECF3AF342ECCBFB1E0D5405CF7";
    res[4].filename = ".\\pic\\setting_graphics2.png"; res[4].md5 = "E930F7354F7DECE20B0831C6BADD94CD";
    res[5].filename = ".\\pic\\setting_audio1.png"; res[5].md5 = "1C4C1657E742236342C45363F991B5B4";
    res[6].filename = ".\\pic\\setting_audio2.png"; res[6].md5 = "C005637059C36F1A7397AF46927FD203";
    //res[7].filename = ".\\pic\\read_failed.png"; res[7].md5 = "E9F0465562EC31499E7B3063FC8A6B14";
    //res[8].filename = ".\\pic\\save_success1.png"; res[8].md5 = "C54900ECC61043AF96A96FE2C5484172";
    //res[9].filename = ".\\pic\\save_success2.png"; res[9].md5 = "4B7D127A76421BB5FA1C88FD51C657FF";
    res[10].filename = ".\\pic\\setting_return.png"; res[10].md5 = "0D72F585852429425AE86A77AD06A82C";
    //res[11].filename = ".\\pic\\restart_1.png"; res[11].md5 = "B61C5657C1B955AD5B4967DE3023F005";
    //res[12].filename = ".\\pic\\restart_2.png"; res[12].md5 = "3A9FA73044CBA8BE20643D38C0E61817";
    //res[11].filename = ".\\pic\\elevator.png"; res[11].md5 = "5FE65A566D8E7936E16C4A855DF4F9BA";

    res[13].filename = ".\\pic\\skin101.jpg"; res[13].md5 = "84F1386AA1EB15C56CEE2C87F0E48287";  //textures of bg
    res[14].filename = ".\\pic\\skin102.jpg"; res[14].md5 = "A5E5741227E5F5BFB0F3808C9CE4703E";
    
    //textures

    

    //res[16].filename = ".\\pic\\networkBG.png"; res[16].md5 = "C7E55A7D281F9DDA1884A0902BCAF941";
    //res[17].filename = ".\\pic\\networkBGW.png"; res[17].md5 = "91B3213F4D29D0C95F932B01452C35D4";


    /*res[18].filename = ".\\pic\\online_mode.png"; res[18].md5 = "25043C1C5124B058E249553FEACF1782";
    res[19].filename = ".\\pic\\server.png"; res[19].md5 = "B9B36D808E35B5E45AE13D0E9CAA42C3";
    res[20].filename = ".\\pic\\client.png"; res[20].md5 = "0F948A101BA4AA8598212E8179B00C53";
    res[21].filename = ".\\pic\\share.png"; res[21].md5 = "A48B4B8FACAC4C20EA5139981207999A";*/

    res[22].filename = ""; res[22].md5 = "";
    res[23].filename = "";  res[23].md5 = "";    //sprite textures
    res[24].filename = ""; res[24].md5 = "";
    res[25].filename = ""; res[25].md5 = "";
    //res[30].filename = ".\\pic\\time_black.png"; res[30].md5 = "0B0106A68002F6D4B13BBB1D33EB4EB1";
    //res[31].filename = ".\\pic\\time_white.png"; res[31].md5 = "598D32DFE1A7B3D078B8AFE075A51B82";
    /*res[26].filename = ".\\pic\\online_config.png"; res[26].md5 = "7FAF71A64CDF96AFDBDF667156EA589C";
    res[27].filename = ".\\pic\\online_waiting.png"; res[27].md5 = "4671C9511A3350807B27B241B64EFF22";
    res[28].filename = ".\\pic\\connect_failed.png"; res[28].md5 = "389BAE97BAFA1B52599BFA367756C508";
    res[29].filename = ".\\pic\\connect_success.png"; res[29].md5 = "FEDB5463C16BD53544539B8CA1369E31";

    

    res[32].filename = ".\\pic\\server_failed.png"; res[32].md5 = "21DE68455ED031BAE5C074446760E123";*/

    //res[33].filename = ".\\pic\\game\\arrow.png"; res[33].md5 = "1C36B69466FDAEDD1DD5DF9D9AEC2C08";


    //res[39].filename = ".\\music\\sys\\credit.ogg"; res[39].md5 = "8916DCB5B821E14F0C41825DFB18D430";  //bgm
    res[40].filename = ".\\music\\sys\\atmosphere_rainy.ogg"; res[40].md5 = "3789A971CCC72649E3287566A67E2ED6";

    res[41].filename = ".\\sounds\\Button1.wav"; res[41].md5 = "D00573C17D3B5639261839BC59EEF76F";    //se
    res[42].filename = ".\\sounds\\Button2.wav"; res[42].md5 = "A292398982A14804D067D21DA5C64DC5";
    res[43].filename = ".\\sounds\\save.wav"; res[43].md5 = "B0A66E74055B49F3B76A75A2A84547B6";
    res[44].filename = ".\\sounds\\load.wav"; res[44].md5 = "5AE81F2D731AD6A185DE426AA89EB766";
    res[45].filename = ".\\sounds\\gameover.wav"; res[45].md5 = "4574070CA0531CD827879C0A5DB6BE24";
    res[46].filename = ".\\sounds\\alertTick.wav"; res[46].md5 = "3A449EA4AE4BFAFFF341F36C9F885546";
    res[47].filename = ".\\sounds\\endofpage.wav"; res[47].md5 = "C772AE11BBE74516838855E6198FA852";
    //res[48].filename = ".\\sounds\\boom.wav"; res[48].md5 = "23C33D7C3889CAF5370513FBED10AE9F";

    res[49].filename = ".\\sounds\\ioerror.wav"; res[49].md5 = "F83BD1119C3DC605525D648DE963C7EC";
    res[50].filename = ".\\sounds\\pass.wav"; res[50].md5 = "F9EC142C6A49D29F0D01EF916ABCE65A";
    res[51].filename = ".\\sounds\\tick.wav"; res[51].md5 = "53C4F41AAE08974782B99A6C553E3B0C";
    res[52].filename = ".\\sounds\\loading.wav"; res[52].md5 = "83C94BFFC673E2801FE9A77DFF751549";

    /*res[53].filename = ""; res[53].md5 = "C38C4FA1C6A646B1A570F0FBE5A38898";
    res[54].filename = ""; res[54].md5 = "4B0489407711AE6EB995DBFCB34D4DF5";
    res[55].filename = ""; res[55].md5 = "9B65588A85FFA6C834B69493CC699BAC";
    res[59].filename = ""; res[59].md5 = "7EDDF5F63DAE44B74BD61AA7DDC8367F";
    res[60].filename = ""; res[60].md5 = "E0F1AF48F81C8DA9C95946F0401A7659";
    res[61].filename = ""; res[61].md5 = "1AAC02ECFD0AA8C5F382114AABFD2545";
    res[62].filename = ""; res[62].md5 = "3B9FBA764064A4454E80DC66229D5AE8";
    res[63].filename = ""; res[63].md5 = "B6BC8B8ACBEDF16F17B5980D42E96985";
    res[64].filename = ""; res[64].md5 = "72E4C1E9E437D02018065A4876F0BF8D";
    res[65].filename = ""; res[65].md5 = "25E46C336EFD7B1C28E43139A1CBB878";
    res[66].filename = ""; res[66].md5 = "90E6B9F087773540050F268BE04FB346";
    res[67].filename = ""; res[67].md5 = "CB4C08DC8A74BAF8A87AF94D4BDA5F89";
    res[68].filename = ""; res[68].md5 = "560D6F8E5329C13ED387BAEED53FF066";
    res[69].filename = ""; res[69].md5 = "4BE1F60BB1918493BE1B359AF654F755";
    res[70].filename = ""; res[70].md5 = "27DFBA82A8D18BB964924FBBBDD2286F";*/


    

    
    res[82].filename = ".\\fonts\\SocialNetworkingIcons.ttf"; res[82].md5 = "E0E2EE5102B1D3672921A3A8A8C18B44";
    res[83].filename = ".\\fonts\\Pacifico.ttf"; res[83].md5 = "6B6170FE52FB23F505B4E056FEFD2679";
    res[84].filename = ".\\fonts\\PublicSans-Regular.ttf"; res[84].md5 = "F91D98BF3813EFF4BB751A722FAFCF6E";
    res[85].filename = ".\\fonts\\Quarlow.ttf"; res[85].md5 = "114C5F3AFDD610A6A22289A2B2DDD708";
    res[86].filename = ".\\fonts\\SmileySans-Oblique.ttf"; res[86].md5 = "0F7526E2CA815E5E8FFD3BD8A1663144";

    res[89].filename = ".\\wrap_oal.dll"; res[89].md5 = "549347BCD4AACD63243D78E8F869DBB1";

    debugger_main.writelog(DINFO, "before AddFontResourceA", __LINE__);
    int tt;
    for (int i = 86; i >= 82; i--)
    {
        tt = AddFontResourceA(res[i].filename.c_str());  //添加字体文件
        //判断字体文件是否可用
        if (tt == 0)
        {
            debugger_main.writelog(1, "Load Font Resource failed! " + res[i].filename);
        }
    }
    debugger_main.writelog(DDEBUG, "after AddFontResourceA", __LINE__);
    SendMessage(HWND_BROADCAST, WM_FONTCHANGE, 0, 0);
    debugger_main.writelog(DDEBUG, "before CreateD2DResource()", __LINE__);
    CreateD2DResource();
    
    return;
}


void process_quit()
{
    for (int i = 80; i < 87; i++)
    {
        while(RemoveFontResourceA(res[i].filename.c_str()));
    }
    
    DWORD dz;
    dz = WaitForSingleObject(hThreadAudio, 200);
    switch (dz)
    {
    case WAIT_OBJECT_0:
    case WAIT_FAILED:
        break;
    case WAIT_TIMEOUT:
        MessageBox(NULL, "hThreadAudio blocked!", "Warning", MB_OK | MB_ICONWARNING);
        break;
    default:
        break;
    }
    dz = WaitForSingleObject(hThreadLog, 200);
    switch (dz)
    {
    case WAIT_OBJECT_0:
    case WAIT_FAILED:
        break;
    case WAIT_TIMEOUT:
        MessageBox(NULL, "hThreadLog blocked!", "Warning", MB_OK | MB_ICONWARNING);
        break;
    default:
        break;
    }
    return;
}


//string input_username = "defaultUser";
//float enter_notice_time = 0;
//void rend_new_user()
//{
//    static Button* okButton;
//    static LABEL* inputLabel;
//    g_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::Snow));
//    if (page_status == 0)
//    {
//        InitPage(3001);
//        g_am.playEffectSound(10);
//        g_cm.Clear();
//        okButton =g_cm.CreateButton(730, 650, 870, 720, "OK", g_pBrushBlack, g_pBrushYellow, g_pBrushLightGreen);
//        inputLabel=g_cm.CreateLabel(600, 450, 1000, 550, "Please wait while initializing", g_pBrushPink, NULL, g_pBrushBlack, NULL, 1);
//        //g_cm.CreateLabel(300, 350, 1100, 450, lan[0].enter_username, NULL, NULL, g_pBrushBlack, NULL, 3);
//        //input_expand = 1;
//        page_status = 1;
//        okButton->setClicked();
//    }
//    else if (page_status == 1)
//    {
//        enter_notice_time -= frmtm;
//        
//        if (enter_notice_time > 0)
//        {
//            inputLabel->setText("Please wait while initializing_");
//        }
//        else
//        {
//            inputLabel->setText("Please wait while initializing");
//        }
//        if (enter_notice_time < -0.5)
//        {
//            enter_notice_time = 0.5;
//        }
//
//        if (okButton->getClicked())
//        {
//            g_am.playEffectSound(9);
//            okButton->setText("loading...");
//            okButton->setBrush3(g_pBrushYellow);
//            usernameC = input_username;
//            thread_IO_request_userinit = 1;
//            page_status = 3;
//            
//        }
//        DrawTextA_1(
//            "Setup",
//            g_pTextFormatNormal,
//            D2D1::RectF(to_screen(200), to_screen(150), to_screen(600),to_screen(300)),
//            g_pBrushBlack
//        );
//    }
//    else if (page_status == 3)
//    {
//
//        if (thread_IO_request_userinit == 0)
//        {
//            okButton->setText("Done");
//            okButton->setBrush3(g_pBrushLightGreen);
//            self_restart();
//            thread_IO_request_userinit = -3;
//        }
//        else if (thread_IO_request_userinit == -1)
//        {
//            okButton->setText("Error");
//            okButton->setBrush3(g_pBrushRed);
//
//        }
//        else if (thread_IO_request_userinit == -2)
//        {
//            //Sleep(500);
//            thread_IO_request_userinit = -3;
//        }
//    }
//    return;
//}

void rendPage()
{
    g_pD2DDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::Black));
    //writelog(to_string(page_index));
    switch (page_index)
    {
    case PAGE_HOME:
        //rend_home_page();
        break;
    case PAGE_SETTING:
        //rend_setting_page();
        break;
    case PAGE_GAME:
    //case MAP_COLORING_PAGE::PAGE_INDEX:
        break;
    case PAGE_HISTORY:
        //rend_page_history();
        break;
    case PAGE_RECORD:
        //rend_page_record();
        break;
    case 1000:
    case 1001:
        //rend_start();
        break;
    case PAGE_VERIFY_FAILED:
        rend_verification_warning();
        break;
    case PAGE_REGISTER:
        //rend_new_user();
        break;
    default:
        debugger_main.writelog(DERROR, "unexpected page_index in rendPage! " + to_string(page_index), __LINE__);
        MessageBox(hWnd, "rendPage failed!", "Error", MB_OK|MB_ICONERROR);
        break;
    }
    return;
}



void rend_quit()
{
    static float quit_page_count = 0;
    if (!normal_quit)
    {
        return;
    }
    quit_page_count += frmtm;
    if (quit_page_count >= 1.0f)
    {
        quit_single = 1;
    }
    g_pBrushQuit->SetOpacity(quit_page_count);
    g_pD2DDeviceContext->FillRectangle(
        D2D1::RectF(0, 0, to_screen(1660), to_screen(960)),
        g_pBrushQuit
    );
    return;
}





void rend_verification_warning()
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



void rend_taglines(bool interupt)
{
    if (interupt)
    {
        main_thread_render_blocked = 1;
        while (main_thread_render_blocked != 2)
        {
            Sleep(0);
            if (quit_single || normal_quit)
            {
                return;
            }
        }
    }
    HRESULT hr;
    

    g_pD2DDeviceContext->SetTarget(copy_succeed_tagline);

    g_pD2DDeviceContext->BeginDraw();
    g_pD2DDeviceContext->Clear(D2D1::ColorF(0, 0.0f));
    g_pD2DDeviceContext->FillRectangle(D2D1::RectF(to_screen(0), to_screen(0), to_screen(400), to_screen(100)), g_pBrushWhite);

    DrawTextA_1(lan[0].copy_succeed, g_pTextFormat, D2D1::RectF(to_screen(0), to_screen(0), to_screen(400), to_screen(100)), g_pBrushLightGreen);

    hr = g_pD2DDeviceContext->EndDraw();
    if (!SUCCEEDED(hr))
    {
        writelog("EndDraw copy_succeed_tagline failed.");
        Sleep(200);
        quit_single = 1;
        main_thread_render_blocked = 0;
        return;
    }
    g_pD2DDeviceContext->SetTarget(copy_failed_tagline);

    g_pD2DDeviceContext->BeginDraw();
    g_pD2DDeviceContext->Clear(D2D1::ColorF(0, 0.0f));
    g_pD2DDeviceContext->FillRectangle(D2D1::RectF(to_screen(0), to_screen(0), to_screen(400), to_screen(100)), g_pBrushBlack);

    DrawTextA_1(lan[0].copy_failed, g_pTextFormat, D2D1::RectF(to_screen(0), to_screen(0), to_screen(400), to_screen(100)), g_pBrushRed);

    hr = g_pD2DDeviceContext->EndDraw();
    if (!SUCCEEDED(hr))
    {
        writelog("EndDraw copy_failed_tagline failed.");
        Sleep(200);
        quit_single = 1;
        main_thread_render_blocked = 0;
        return;
    }

    g_pD2DDeviceContext->SetTarget(io_error_tagline);
    g_pD2DDeviceContext->BeginDraw();
    g_pD2DDeviceContext->Clear(D2D1::ColorF(0, 0.0f));
    g_pD2DDeviceContext->FillRectangle(D2D1::RectF(to_screen(0), to_screen(0), to_screen(800), to_screen(100)), g_pBrushBlack);

    DrawTextA_1(lan[0].io_error, g_pTextFormat, D2D1::RectF(to_screen(0), to_screen(0), to_screen(800), to_screen(100)), g_pBrushRed);

    hr = g_pD2DDeviceContext->EndDraw();
    if (!SUCCEEDED(hr))
    {
        writelog("EndDraw io_error_tagline failed.");
        Sleep(200);
        quit_single = 1;
        main_thread_render_blocked = 0;
        return;
    }
    g_pD2DDeviceContext->SetTarget(file_corrupt_tagline);
    g_pD2DDeviceContext->BeginDraw();
    g_pD2DDeviceContext->Clear(D2D1::ColorF(0, 0.0f));
    g_pD2DDeviceContext->FillRectangle(D2D1::RectF(to_screen(0), to_screen(0), to_screen(400), to_screen(100)), g_pBrushBlack);

    DrawTextA_1(lan[0].file_corrupt, g_pTextFormat, D2D1::RectF(to_screen(0), to_screen(0), to_screen(400), to_screen(100)), g_pBrushRed);

    hr = g_pD2DDeviceContext->EndDraw();
    if (!SUCCEEDED(hr))
    {
        writelog("EndDraw file_corrupt_tagline failed.");
        Sleep(200);
        quit_single = 1;
        main_thread_render_blocked = 0;
        return;
    }

    g_pD2DDeviceContext->SetTarget(numberBitmaps);
    g_pD2DDeviceContext->BeginDraw();
    g_pD2DDeviceContext->Clear(D2D1::ColorF(0, 0.0f));
    DrawTextA_1("0123456789", g_pTextFormatminiL, D2D1::RectF(to_screen(0), to_screen(0), to_screen(256), to_screen(48)), g_pBrushLightYellow);
    hr = g_pD2DDeviceContext->EndDraw();
    if (!SUCCEEDED(hr))
    {
        writelog("EndDraw numberBitmaps failed.");
        Sleep(200);
        quit_single = 1;
        main_thread_render_blocked = 0;
        return;
    }
    if (interupt)
    {
        g_pD2DDeviceContext->SetTarget(g_pD2DTargetBimtap);
        main_thread_render_blocked = 0;
    }
    return;
}


void initgame()
{
    t_start =t_end= Clock::now();
    thread_IO_request_verify_res = 1;

    HRESULT hr;

    debugger_main.writelog(0,"Loading Resources...");


    for (int i = 0; i < 128; i++)
    {
        if (i == 127)
        {
            md5_result = 1;
        }
        else if (i == 36)
        {
            thread_IO_request_verify_res = 2;
        }
        else if (i == 72)
        {
            thread_IO_request_verify_res = 3;
        }
        if (res[i].filename != "")
        {
            if (!md5_verify(res[i].filename.c_str(), res[i].md5.c_str()))
            {
                writelog("resource hash verification failed!  " + res[i].filename);
                md5_result = -1;
                break;
            }
        }
    }
    thread_IO_request_verify_res = 4;
    //md5_result = 1;//debug
    if (md5_result == 1)
    {
        for (int i = 0; i < 128; i++)
        {
            if (res[i].filename != "")
            {
                res[i].Lfilename = stringToLPCWSTR(res[i].filename);
            }
        }
        debugger_main.writelog(0,"Loading Bitmap From File...");
        for (int i = 0; i < 36; i++)
        {
            if (res[i].filename == "")
            {
                continue;
            }
            if (i == 13 || i == 14)
            {
                if (!SUCCEEDED(LoadBitmapFromFile(pIWICFactory, res[i].Lfilename, (UINT)to_screen(2000), (UINT)to_screen(1500), &g_pD2DBimtapUI[i])))   //max size 11520x8640
                {
                    writelog("Load Bitmap From File failed!  " + res[i].filename);
                    normal_quit = 1;
                    return;
                }
            }
            else if (i == 16 || i == 17)
            {
                if (!SUCCEEDED(LoadBitmapFromFile(pIWICFactory, res[i].Lfilename, (UINT)to_screen(900), (UINT)to_screen(900), &g_pD2DBimtapUI[i])))   //max size 11520x8640
                {
                    writelog("Load Bitmap From File failed!  " + res[i].filename);
                    normal_quit = 1;
                    return;
                }
            }
            else if (!SUCCEEDED(LoadBitmapFromFile(pIWICFactory, res[i].Lfilename, 0, 0, &g_pD2DBimtapUI[i])))
            {
                writelog("Load Bitmap From File failed!  "+ res[i].filename);
                normal_quit = 1;
                return;
            }
        }
        thread_IO_request_verify_res = 5;
        

        D2D1_BITMAP_BRUSH_PROPERTIES1 BITMAP_BRUSH_PROPERTY1;
        BITMAP_BRUSH_PROPERTY1.extendModeX = D2D1_EXTEND_MODE_MIRROR;
        BITMAP_BRUSH_PROPERTY1.extendModeY = D2D1_EXTEND_MODE_MIRROR;
        if (set2[0].MSAA==2)
        {
            BITMAP_BRUSH_PROPERTY1.interpolationMode = D2D1_INTERPOLATION_MODE_ANISOTROPIC;
        }
        else if (set2[0].MSAA == 1)
        {
            BITMAP_BRUSH_PROPERTY1.interpolationMode = D2D1_INTERPOLATION_MODE_LINEAR;
        }
        else
        {
            BITMAP_BRUSH_PROPERTY1.interpolationMode = D2D1_INTERPOLATION_MODE_NEAREST_NEIGHBOR;
        }
        for (int i = 13; i < 18; i++)
        {
            if (!SUCCEEDED(g_pD2DDeviceContext->CreateBitmapBrush(g_pD2DBimtapUI[i], BITMAP_BRUSH_PROPERTY1, &g_pBitmapBrushUI[i - 1])))
            {
                writelog("Create BitmapBrush From Bitmap failed! "+ to_string(i));
                normal_quit = 1;
                return;
            }
        }
    }
    else
    {
        //normal_quit = 1;
        return;
    }

    thread_IO_request_verify_res = 6;


    hr = g_pD2DDeviceContext->CreateEffect(CLSID_D2D1GaussianBlur, &gaussianBlurEffect);
    if (!SUCCEEDED(hr))
    {
        writelog("CreateEffect gaussianBlurEffect failed!");
        normal_quit = 1;
        return;
    }

    hr = gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_OPTIMIZATION, D2D1_GAUSSIANBLUR_OPTIMIZATION_QUALITY);
    if (!SUCCEEDED(hr))
    {
        writelog("SetValue gaussianBlurEffect OPTIMIZATION failed!");
        normal_quit = 1;
        return;
    }
    hr = gaussianBlurEffect->SetValue(D2D1_GAUSSIANBLUR_PROP_BORDER_MODE, D2D1_BORDER_MODE_HARD);
    if (!SUCCEEDED(hr))
    {
        writelog("SetValue gaussianBlurEffect BORDER_MODE failed!");
        normal_quit = 1;
        return;
    }

    hr = g_pD2DDeviceContext->CreateEffect(CLSID_D2D1Opacity, &OpacityEffect);
    if (!SUCCEEDED(hr))
    {
        writelog("CreateEffectnn  OpacityEffect failed!");
        normal_quit = 1;
        return;
    }
    hr = OpacityEffect->SetValue(D2D1_OPACITY_PROP_OPACITY, 1.0f);
    if (!SUCCEEDED(hr))
    {
        writelog("SetValue OpacityEffect OPACITY failed!");
        normal_quit = 1;
        return;
    }
    
    
    debugger_main.writelog(0,"Creating Bitmap Cache...");
    
    
    
    hr = g_pD2DDeviceContext->CreateBitmap(D2D1::SizeU((UINT)to_screen(400), (UINT)to_screen(100)), nullptr, 0,
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &copy_succeed_tagline);
    if (!SUCCEEDED(hr))
    {
        writelog("CreateBitmap copy_succeed_tagline failed!");
        Sleep(200);
        quit_single = 1;
        return;
    }

    hr = g_pD2DDeviceContext->CreateBitmap(D2D1::SizeU((UINT)to_screen(400), (UINT)to_screen(100)), nullptr, 0,
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &copy_failed_tagline);

    if (!SUCCEEDED(hr))
    {
        writelog("CreateBitmap copy_failed_tagline failed!");
        Sleep(200);
        quit_single = 1;
        return;
    }
    
    hr = g_pD2DDeviceContext->CreateBitmap(D2D1::SizeU((UINT)to_screen(800), (UINT)to_screen(100)), nullptr, 0,
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &io_error_tagline);

    if (!SUCCEEDED(hr))
    {
        writelog("CreateBitmap io_error_tagline failed!");
        Sleep(200);
        quit_single = 1;
        return;
    }
    hr = g_pD2DDeviceContext->CreateBitmap(D2D1::SizeU((UINT)to_screen(400), (UINT)to_screen(100)), nullptr, 0,
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &file_corrupt_tagline);

    if (!SUCCEEDED(hr))
    {
        writelog("CreateBitmap file_corrupt_tagline failed!");
        Sleep(200);
        quit_single = 1;
        return;
    }
    
    hr = g_pD2DDeviceContext->CreateBitmap(D2D1::SizeU(to_screen(2400), to_screen(1800)), nullptr, 0,
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &gaussianBlur_Bitmap1);

    if (!SUCCEEDED(hr))
    {
        writelog("CreateBitmap gaussianBlur_Bitmap1 failed!");
        Sleep(200);
        quit_single = 1;
        return;
    }
    hr = g_pD2DDeviceContext->CreateBitmap(D2D1::SizeU(orix, oriy), nullptr, 0,
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_NONE,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &shortcut_Bitmap);

    if (!SUCCEEDED(hr))
    {
        writelog("CreateBitmap shortcut_Bitmap failed!");
        Sleep(200);
        quit_single = 1;
        return;
    }

    hr = g_pD2DDeviceContext->CreateBitmap(D2D1::SizeU(orix, oriy), nullptr, 0,
        D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET,
            D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
        &content_blur_Bitmap);

    if (!SUCCEEDED(hr))
    {
        writelog("CreateBitmap shortcut_Bitmap failed!");
        Sleep(200);
        quit_single = 1;
        return;
    }
    for (int i = 0; i < 1; i++)
    {
        hr = g_pD2DDeviceContext->CreateBitmap(D2D1::SizeU(to_screen(256), to_screen(48)), nullptr, 0,
            D2D1::BitmapProperties1(
                D2D1_BITMAP_OPTIONS_TARGET,
                D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)),
            &numberBitmaps);
        if (!SUCCEEDED(hr))
        {
            writelog("CreateBitmap numberBitmaps failed!");
            Sleep(200);
            quit_single = 1;
            return;
        }
    }
    

    thread_IO_request_verify_res = 7;
    
    //中断主线程中的渲染循环
    main_thread_render_blocked = 1;
    while (main_thread_render_blocked != 2)
    {
        Sleep(0);
        if (quit_single)
        {
            return;
        }
    }

    if (!createGaussianBlurEffectBitmap(g_pD2DBimtapUI[14], gaussianBlur_Bitmap1, 0,8.0f))
    {
        quit_single = 1;
        Sleep(200);
        main_thread_render_blocked = 0;
        return;
    }
    
    rend_taglines(0);
    
    g_pD2DDeviceContext->SetTarget(g_pD2DTargetBimtap);
    main_thread_render_blocked = 0;

    for (int i = 13; i < 18; i++)
    {
        SAFE_RELEASE(g_pD2DBimtapUI[i]);
    }
    
    g_PageManager.SetInitalPage(dynamic_cast<PAGE*>(new Start_Page()));

    g_PageManager.AddPage(dynamic_cast<PAGE*>(new Home_Page()));
    g_PageManager.AddPage(dynamic_cast<PAGE*>(new Setting_Page())); 
    g_PageManager.AddPage(dynamic_cast<PAGE*>(new New_User_Page()));
    g_PageManager.AddPage(dynamic_cast<PAGE*>(new LocalGame_Page()));
    thread_IO_request_verify_res = 8;

    return;
}



PAGE::PAGE(int Page_index_, int Page_status_, int intro_effect_index_, int BGM_switch_,string _TAG)
{
    Page_index = Page_index_, Page_status = Page_status_, intro_effect_index = intro_effect_index_, BGM_switch = BGM_switch_, TAG = _TAG;
    enable_fixed_update = 0, fixed_update_interval = 1, fixed_update_type= UPDATE_FOREGROUND;
}

PAGE::~PAGE()
{
}

bool PAGE::Init()
{
    g_cm.CleanAnimations();

    return 1;
}

void PAGE::Update()
{
    if (Page_status <= PAGE_INIT_STATUS)
    {
        //default loading 
        loadingPage.update(1);
    }
    return;
}

void PAGE::FixedUpdate()
{
}

void PAGE::Rend()
{
    if (Page_status <= PAGE_INIT_STATUS)
    {
        //default loading 
        loadingPage.rend();
    }
    return;
    
}

bool PAGE::EnterPage()
{
    if (Page_status < PAGE_INIT_STATUS)
    {
        if (!Init())
        {
            debugger_main.writelog(DWARNNING, "failed to init page");
        }
        Page_status = PAGE_INIT_STATUS;
    }
    g_cm.ClearPage();
    return 1;
}

void PAGE::SetFixedUpdate(bool enable, float interval_time, int update_type)
{
    enable_fixed_update = enable;
    fixed_update_type = update_type;
    fixed_update_interval = interval_time;
    fixed_update_timer.start_timer();
    //todo: deal with background update
    return;
}

int PAGE::getPageIndex()
{
    return Page_index;
}

int PAGE::getIntro_effect_index()
{
    return intro_effect_index;
}

string PAGE::getTAG()
{
    if (TAG.empty())
    {
        return "undefined TAG page index= " + to_string(Page_index);
    }
    else
    {
        return TAG;
    }
}

PageManager::PageManager()
{
    currentPage = nullptr;
    nextPage = nullptr;
    for (int i = 0; i < PAGE_MAX; i++)
    {
        pages[i] = nullptr;
    }
    switchingPage = 0;
    effect_wait_time = 0;
}

PageManager::~PageManager()
{
    if (currentPage != nullptr)
    {
        currentPage->ExitPage();
    }
    
    for (auto& i : pages)
    {
        if (i == nullptr)
        {
            continue;
        }

        delete i;
        i = nullptr;
    }
}

void PageManager::update()
{
    if (switchingPage)
    {
        effect_wait_time -= frmtm;
        if (effect_wait_time <= 0)
        {
            SwitchPage_implement();
        }
        debugger_main.add_output_line("PageManager switchingPage");
    }
    else
    {
        if (currentPage == nullptr)
        {
            debugger_main.add_output_line("currentPage nullptr in PageManager::update()");
            return;
        }
        if (page_index != currentPage->getPageIndex())
        {
            debugger_main.add_output_line("currentPage not match in PageManager::update() (compatible) "+to_string(page_index));
            return;
        }
        currentPage->Update();
        debugger_main.add_output_line("currentPage: "+ currentPage->getTAG());
    }

    return;
}

void PageManager::rend()
{
    if (currentPage == nullptr)
    {
        return;
        debugger_main.writelog(DERROR, "currentPage nullptr in PageManager::rend()", __LINE__);
        return;
    }
    if (page_index != currentPage->getPageIndex())
    {
        return;
    }
    currentPage->Rend();
    return;
}

void PageManager::SwitchPageTo(int page_index, float wait_time)
{
    effect_wait_time = wait_time;
    for (auto &i : pages)
    {
        if (i != nullptr && i->getPageIndex() == page_index)
        {
            SwitchPageTo(i);
            return;
        }
    }

    //target page not found
    nextPage = nullptr;
    switchingPage = 1;
    
    return;// todo: remove this line after refactor complete
    debugger_main.writelog(DDEBUG, "not found page_index in PageManager::SwitchPageTo() " + to_string(page_index), __LINE__);
    return;
}

void PageManager::SwitchPageTo(PAGE* newPage)
{
    nextPage = newPage;
    switchingPage = 1;
    if (newPage->getIntro_effect_index() == Effect::EFF_NONE)
    {
    }
    else
    {
        g_cm.CreateEffect(newPage->getIntro_effect_index(), cpos.x, cpos.y);
    }
    return;
}

void PageManager::AddPage(PAGE* Page)
{
    if (Page == nullptr)
    {
        debugger_main.writelog(DERROR, "param Page nullptr in PageManager::AddPage()", __LINE__);
        return;
    }
    for (int i = 0; i < PAGE_MAX; i++)
    {
        if (pages[i] == nullptr)
        {
            pages[i] = Page;
            return;
        }
    }
    debugger_main.writelog(DERROR, "Page Array full in PageManager::AddPage()", __LINE__);
    return;
}

void PageManager::SetInitalPage(PAGE* Page)
{
    AddPage(Page);
    /*if (!Page->EnterPage())
    {
        debugger_main.writelog(DERROR, "Failed to EnterPage in PageManager::SetInitalPage() page_index=" + to_string(currentPage->getPageIndex()), __LINE__);
    }*/
    SwitchPageTo(Page);
    return;
}

string PageManager::GetCurrentPageTag()
{
    if (currentPage == nullptr)
    {
        return "page index= " + to_string(page_index);
    }
    return currentPage->getTAG();
}

void PageManager::SwitchPage_implement()
{
    debugger_main.writelog(DDEBUG, "in PageManager::SwitchPage_implement()", __LINE__);
    switchingPage = 0;
    if (currentPage != nullptr)
    {
        if (!currentPage->ExitPage())
        {
            debugger_main.writelog(DERROR, "Failed to ExitPage in PageManager::SwitchPage_implement() current page_index=" + to_string(currentPage->getPageIndex()), __LINE__);
        }
    }
    if (nextPage == nullptr)
    {
        currentPage = nullptr;
        //debugger_main.writelog(DWARNNING, "Failed to EnterPage in PageManager::SwitchPage_implement() while nextPage nullptr page_index=" /*+ to_string(currentPage->getPageIndex())*/, __LINE__);
        return;
    }
    if (!nextPage->EnterPage())
    {
        debugger_main.writelog(DERROR, "Failed to EnterPage in PageManager::SwitchPage_implement() target page_index=" + to_string(nextPage->getPageIndex()), __LINE__);
    }
    currentPage = nextPage;
    page_index = currentPage->getPageIndex();
    page_status = 0;
    nextPage = nullptr;
    debugger_main.writelog(DDEBUG, "in PageManager::SwitchPage_implement() switching page_index=" + to_string(currentPage->getPageIndex()), __LINE__);
    return;
}

void PAGE::LOADING_PAGE::update(float timeScale)
{
    timeStamp += frmtm * timeScale;
    arc_speed1 = fabsf(sinf(timeStamp)) * MAX_SPEED + BASE_SPEED;
    arc_speed2 = fabsf(cosf(timeStamp)) * MAX_SPEED + BASE_SPEED;
    arc_angel1 += arc_speed1 * frmtm * timeScale;
    arc_angel2 += arc_speed2 * frmtm * timeScale;
    return;
}

void PAGE::LOADING_PAGE::rend()
{
    if (arc_angel1 < arc_angel2)
    {
        DrawArc_1(CENTER_POSX, CENTER_POSY, RADIUS, arc_angel1, arc_angel2, 0, g_pBrushBlack, 5);
    }
    else
    {
        DrawArc_1(CENTER_POSX, CENTER_POSY, RADIUS, arc_angel2, arc_angel1, 0, g_pBrushBlack, 5);
    }
    return;
}

PAGE::LOADING_PAGE::LOADING_PAGE()
{
    timeStamp = 0, arc_speed1 = 0, arc_speed2 = 0, arc_angel1 = 0, arc_angel2 = 0;
}
