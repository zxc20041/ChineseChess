#pragma once 
#include <string>
#include <cmath>
#include <cstring>
#include <chrono>
#include <Windows.h>
#include <imm.h>  
#include <unordered_map>
#include<conio.h>
#include <io.h>
#include <shlobj_core.h>
#include <process.h>
#include<array>
#include <vector>
#include <fstream>
#include <d3d11.h>
#include <d2d1_2.h> //支持win8.1及以上的系统
#include <d2d1effects_2.h>  //支持win10及以上的系统
#include <dxgi1_6.h>
#include <dwrite.h>
#include <wincodec.h>
#include<dinput.h>

#include <openssl/md5.h>


//布局源分辨率1600x900

constexpr int NO_HASH_CHECK = 0;
// 12 KB buffers
constexpr int BUFFER_SIZE = 12288;
constexpr const char* VERSION = "1.0.0.1";
constexpr const char* VERSION_SHORT = "Ver 1.0.0";

constexpr const char* PRODUCT_NAME = "Chinese Chess";
constexpr int BUTTON_MAX = 128;
constexpr int LABEL_MAX = 128;
constexpr int EFFECT_MAX = 4096;
constexpr int ANIME_MAX = 128;
constexpr int BOX_MAX = 1024;
constexpr float PI = 3.14159265F;

constexpr int PAGE_HOME = 0;
constexpr int PAGE_SETTING = 1;
constexpr int PAGE_GAME = 2;
constexpr int PAGE_HISTORY = 5;
constexpr int PAGE_RECORD = 6;
constexpr int PAGE_CONNECT = 7;
constexpr int PAGE_REGISTER = 3001;
constexpr int PAGE_VERIFY_FAILED = 2001;
constexpr int PAGE_INTRO = 1000;



#define SAFE_RELEASE(P) if(P){P->Release() ; P = NULL ;}    //安全释放对象
#define cleanStrBuff(STR) CleanStrBuff(STR,sizeof(STR)/sizeof(STR[0]))
#define ReadFile(filename,buf) ReadFile_1(filename,buf,sizeof(buf)/sizeof(buf[0]))
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)  //用来检测按键的点击事件
typedef std::chrono::high_resolution_clock Clock;
using namespace std;
extern SYSTEMTIME st;

//退出  
extern bool quit_single;

//正常退出
extern bool normal_quit;

extern IDXGIOutput* g_output;
//用户名
extern string usernameC;

//md5资源校验结果
extern int md5_result;
//页面序号
extern int page_index;
//页面状态
extern int page_status;
//重启信号
extern bool self_restarted;

extern int loading_progress_show;
extern int ed_stage;
extern bool long_credit;
extern bool clicking,rightclick;
extern bool keyw, keya, keys ,keyd;
extern bool keyTab, keyESC, keyComb, keySpace;
extern float wheel_move_value;

//日志文件路径
extern const char* filename_dbg;
//日志文件缓冲区
extern string filebuf1[64];

//鼠标位置
extern D2D1_POINT_2F cpos;

//帧时间(秒)
extern float frmtm;
//渲染比率
extern float scale;

extern int tip_used_num;

struct Resource
{
    LPCWSTR Lfilename = L"";
    string filename = "";
    string md5 = "";
};
extern Resource res[128];


extern string tips[16];

extern HWND hWnd, FocusWindow;


class Box  
{
private:
    //启用
    bool active;
    float x1, x2, y1, y2;
    //聚焦
    bool focus;
    //单击
    bool clicked;
    bool click_status;
    bool rightclick_status;
    //右键
    bool right_clicked;
    float clicked_time, rightclicked_time;
    bool doubleclick_status;
    //双键
    bool double_clicked;
    bool lr_click_status_right;
public:
    Box(float x1,float y1,float x2,float y2);
    void check();
    void deactivate();
    void activate();
    void reset_click_status();
    bool getFocus();
    bool getClickStatus();
    bool getClicked();
    void setClicked();
    bool getRightClicked();
    bool getDoubleClicked();
    void setBounds(float x1, float y1, float x2, float y2);
};
//点击区域

constexpr int UI_TYPE_BUTTON_DYNAMIC = 0;
constexpr int UI_TYPE_BUTTON_STATIC = 1;
constexpr int UI_TYPE_BUTTON_STATIC2 = 2;
constexpr int UI_TYPE_BUTTON_RECT_STATIC = 3;
class LABEL
{
protected:
    
    //启用
    bool show;
    
    int ui_type;
    //坐标
    float x1, x2, y1, y2;
    
    string text;
    int TextFormat;
    
    //外框画笔
    ID2D1SolidColorBrush* Brush1;
    //填充画笔
    ID2D1SolidColorBrush* Brush2;
    //文字画笔
    ID2D1SolidColorBrush* Brush3;
    
    //贴图
    ID2D1Bitmap* Bitmap;
public:
    LABEL(float x1, float y1, float x2, float y2, string text,
        ID2D1SolidColorBrush* Brush1,
        ID2D1SolidColorBrush* Brush2,
        ID2D1SolidColorBrush* Brush3,
        ID2D1Bitmap* Bitmap,
        int TextFormat);
    void setShow(bool show);
    //void getShow();
    void rend();
    virtual void setBound(float x1, float y1, float x2, float y2);
    void setText(string text);
    void setBrush1(ID2D1SolidColorBrush* Brush);
    void setBrush2(ID2D1SolidColorBrush* Brush);
    void setBrush3(ID2D1SolidColorBrush* Brush);
    void setBitmap(ID2D1Bitmap* Bitmap);
    void setUItype(int ui_type);

    void getBound(float* x1, float* y1, float* x2, float* y2);
    string getText();
};

class Button : public LABEL
{
private:
    //启用
    bool active;

    //按钮原坐标
    float ori_x1, ori_x2, ori_y1, ori_y2;

    //按钮填充初始画笔
    ID2D1SolidColorBrush* Brush2_ori;

    int pos_move_status;

    Box *box;

    //be called in update() method
    void ui_update();

public:
    Button(float x1, float y1, float x2, float y2, string text,
        ID2D1SolidColorBrush* Brush1,
        ID2D1SolidColorBrush* Brush2,
        ID2D1SolidColorBrush* Brush3,
        ID2D1Bitmap* Bitmap=NULL,
        int TextFormat=2);

    ~Button();

    void deactivate();
    void activate();
    void setBrush2(ID2D1SolidColorBrush* Brush);
    bool getFocus();
    bool getClicked();
    bool getClickStatus();
    void setClicked();
    void update();

    void rend();

    void setBound(float x1, float y1, float x2, float y2);
};


class Effect
{
private:
    bool active = 0;
    int index = 0;
    float time = 0;
    float posx = 0, posy = 0;
public:
    Effect(int index, float posx, float posy);
    void update();
    void rend();
    bool getActive();

    constexpr static int BLACK_SWITCH = 5;
    constexpr static int WHITE_SWITCH = 8;
    constexpr static int WHITE_CIRCLE_SWITCH = 4;
    constexpr static int BLUR_SWITCH = 9;
    constexpr static int EFF_NONE = -1;
};




class Animation
{
    bool active;
    bool repeat;
    int type;
    int curnum;
    int num;
    int destl;
    int destw;
    int length;
    int width;
    int destx;
    int desty;
    float stime;
    float ctime;

    float delta_x1, delta_x2, delta_y1, delta_y2;
    ID2D1Bitmap* bitmap;
public:
    Animation(int type, int destx, int desty, int destw, int destl);
    Animation(ID2D1Bitmap* bitmap, int src_length, int src_height, int src_dx,int src_dy,int src_dw,int src_dh,int num,float stime);
    void update();
    void rend();

    //destx,desty should be center pos
    void rend(float destx, float desty, float destw, float desth, bool direction,float opacity = 1.0f);
    bool getActive();
};


class ComponentManager
{
private:
    int Label_num, Button_num, Effect_num, Anime_num;
    int managed_Label_num, managed_Button_num;
    LABEL* labels[LABEL_MAX];
    array<weak_ptr<LABEL>, BUTTON_MAX> managed_labels;
    Button* buttons[BUTTON_MAX];
    array<weak_ptr<Button>, BUTTON_MAX> managed_buttons;
    Effect* effects[EFFECT_MAX];
    Animation* anime[ANIME_MAX];
public:
    ComponentManager();

    ~ComponentManager();

    //LABEL* CreateLabel(float x1, float y1, float x2, float y2, string text, ID2D1SolidColorBrush* Brush1, ID2D1SolidColorBrush* Brush2, ID2D1SolidColorBrush* Brush3, ID2D1Bitmap* Bitmap = NULL, int TextFormat = 2);

    //Button* CreateButton(float x1, float y1, float x2, float y2, string text, ID2D1SolidColorBrush* Brush1, ID2D1SolidColorBrush* Brush2, ID2D1SolidColorBrush* Brush3, ID2D1Bitmap* Bitmap = NULL, int TextFormat = 2);

    Effect* CreateEffect(int index, float posx=0.f, float posy=0.f);

    Animation* CreateAnimation(int type, int destx, int desty, int destw, int destl);

    void AddLabel(weak_ptr<LABEL> label);

    void AddButton(weak_ptr<Button> button);


    void CleanEffects();

    void CleanAnimations();

    void Clear();

    void ClearPage();

    void hideAll();

    void update();

    void rend();
};
extern ComponentManager g_cm;

struct TEXT_RESOURCE
{
    int lines;
    string* content;
    TEXT_RESOURCE() { lines = 0, content = nullptr; }
    ~TEXT_RESOURCE() { if (content != nullptr) delete[]content; }
};


struct FONT_DESC
{
    int fontSize=20;
    string FontFamilyName;
};

struct TEXTURE_DESC
{
    int width = 0;
    int height = 0;
    bool save_Bitmap = 1;
    bool save_WicBitmapSource = 0;
    bool save_Brush = 0;
};

struct TEXT_DESC
{
    int lines_max = 64;
    bool separatedByLines = 1;
};

struct RESOURCE_INFO;

class ResourceManager
{
public:
    constexpr static int SINGLE_LOADING = 1;
    constexpr static int SINGLE_IDLE = 0;
    constexpr static int SINGLE_FAILED = -1;
    enum class ResourceType
    {
        Resource_Texture = 0,
        Resource_Audio = 1,
        Resource_Font = 2,
        Resource_Text = 3,
        Resource_Handle = 4
    };
    ResourceManager();
    ~ResourceManager();
    void AddResource(string aliasName, string filePath, ResourceType type, string md5, TEXTURE_DESC texture_desc = TEXTURE_DESC(), FONT_DESC font_desc = FONT_DESC(), TEXT_DESC text_desc= TEXT_DESC(),void*func=0);

    void LoadAll(); //async
    void ReleaseAll();
    void FinishLoading();   //sync
    bool GetFinishLoading();    //async
    void AddThread(string aliasName, void* func);


    ID2D1Bitmap* getTexture(string aliasName);
    ID2D1BitmapBrush1* getBrush(string aliasName);
    int getAudioIndex(string aliasName);
    IDWriteTextFormat* getFont(string aliasName);
    int getFontIndex(string aliasName);
    TEXT_RESOURCE* getText(string aliasName);
    void setFontIndex(string aliasName, int index);
    void setFont(string aliasName, IDWriteTextFormat* font);

    atomic<int> loadAllSignal,releaseAllSignal;
    bool LoadAll_implementation();
    void releaseAll_implementation();

private:
    constexpr static int TEXT_MAX = 128;
    constexpr static int RES_MAX = 128;
    constexpr static int FILE_IDLE = 0;
    constexpr static int FILE_VERIFY_REQ = 1;
    constexpr static int FILE_VERIFY_SUCCEED = 2;
    constexpr static int FILE_VERIFY_FAILED = 3;
    
    unordered_map<string, ID2D1Bitmap*> textureMap;
    unordered_map<string, IWICBitmapSource*> WICtextureMap;
    unordered_map<string, ID2D1BitmapBrush1*> brushMap;
    unordered_map<string, int> audioMap;
    unordered_map<string, IDWriteTextFormat*> fontMap;
    unordered_map<string, int> fontIndexMap;
    unordered_map<string, TEXT_RESOURCE*> textMap;
    unordered_map<string, HANDLE> handleMap;

    TEXT_RESOURCE* texts[TEXT_MAX];

    RESOURCE_INFO* res_info[RES_MAX];


    int res_num;

    atomic<int> fileOperationSignal;
    

    bool verifyFile(RESOURCE_INFO* res);
    bool md5_verify_implementation(string filename, string md5);
    
    
    
    //todo: add text resolver
};

extern ResourceManager g_rm;

struct RESOURCE_INFO
{
    string aliasName;
    string filePath;
    ResourceManager::ResourceType type;
    string md5;
    FONT_DESC fontDesc;
    TEXTURE_DESC textureDesc;
    TEXT_DESC textDesc;
    void* funcAddr;
    bool loaded;
    RESOURCE_INFO(string name, string path, ResourceManager::ResourceType type, string md5hash, TEXTURE_DESC texture_desc, FONT_DESC font_desc, TEXT_DESC text_desc, void* func=0)
    {
        aliasName = name, filePath = path, this->type = type, md5 = md5hash, loaded = 0;
        fontDesc = font_desc;
        textureDesc = texture_desc;
        textDesc = text_desc;
        funcAddr = func;
    }
    constexpr static TEXTURE_DESC DEFAULT_Bitmap_WicBitmap = { 0, 0, 1 };
    constexpr static TEXTURE_DESC DEFAULT_Bitmap = { 0,0,0 };
};


class TIMER
{
private:
    Clock::time_point start;
    Clock::time_point end;
public:
    void start_timer();
    float stop_timer();
};

class PAGE
{
public:
    PAGE(int Page_index, int Page_status, int intro_effect_index, int BGM_switch = 0, string TAG = "");
    virtual ~PAGE();

    virtual bool Init();
    virtual void Update();
    virtual void FixedUpdate();
    virtual void Rend() = 0;

    virtual bool EnterPage();
    virtual bool ExitPage() = 0;

    void SetFixedUpdate(bool enable, float interval_time=0.02f,int update_type= UPDATE_FOREGROUND);  //time in sec

    int getPageIndex();
    int getIntro_effect_index();

    string getTAG();
protected:
    int BGM_switch;
    int intro_effect_index;
    int Page_index;
    int Page_status;

    bool enable_fixed_update;
    int fixed_update_type;
    float fixed_update_interval;
    TIMER fixed_update_timer;
    string TAG;

    constexpr static int UPDATE_FOREGROUND = 0;
    constexpr static int UPDATE_BACKGROUND = 1;

    constexpr static int PAGE_CREATED_STATUS = -1;
    constexpr static int PAGE_INIT_STATUS = 0;
    constexpr static int PAGE_PREPARED_STATUS = 1;

    constexpr static int HOME_PAGE_INDEX = 0;
    class LOADING_PAGE
    {
    public:
        void update(float timeScale);
        void rend();
        LOADING_PAGE();
    private:
        constexpr static float MAX_SPEED = 6, CENTER_POSX = 800, CENTER_POSY = 500, RADIUS = 50, BASE_SPEED = 1;
        float arc_angel1, arc_angel2;
        float arc_speed1, arc_speed2;
        float timeStamp;
    };
    LOADING_PAGE loadingPage;
};

class PageManager
{
public:
    PageManager();
    ~PageManager();
    void update();
    void rend();
    void SwitchPageTo(int page_index, float wait_time = 0.4f);
    void SwitchPageTo(PAGE* newPage);
    void AddPage(PAGE* Page);

    void SetInitalPage(PAGE* Page);

    string GetCurrentPageTag();
private:
    constexpr static int PAGE_MAX = 128;
    bool switchingPage;

    float effect_wait_time;

    PAGE* currentPage;
    PAGE* nextPage;
    PAGE* pages[PAGE_MAX];

    void SwitchPage_implement();
};

extern PageManager g_PageManager;


struct Read_info
{
    int active = 0;
    string opponent = "";
    int avatar = 0;
    int result = 0;
    string time = "";

    string date = "";

    string win_num = "";
    string gamedata = "";

    //for record
    string time_online = "";
    string win_num_online = "";
    string match_num = "";
    string match_num_online = "";
};

extern Read_info record;

class Letter
{
private:
    static int num, homepage_text_type;
    int index_num;
    float posx = 0;
    float posy = 0;
    float target_posx = 0;
    float target_posy = 0;
    float speedx = 0;
    float speedy = 0;
    float opacity = 0.4f;
    bool effect = 0;//0 for dark,1 for light
    float effect_level = 0;
    bool effect_stage = 0;
    float effect_wait_time = 0;
    string sLetter;
public:
    Letter(char single_char);
    void reset();
    void update();
    void rend();
};



struct credit_string
{

    string ori = "";
    float time = 0;
    float load_char_time = 0;
    int charnum = 0;
    int font = 0;
    int pos = 0;
};

constexpr int logbuf_length = 256;
namespace debugger 
{
    constexpr static int DERROR = 2, DWARNNING = 1, DINFO = 0, DDEBUG = 3;
}
class debug_ex
{
public:
    
    string logbuf[logbuf_length];
    void add_output_continuous(string content);
    void add_output_space(string content);
    void add_output_line(string content);
    void add_tagline(string content);
    void reset();
    void rend();
    void set_osd_enable(bool enable);
    debug_ex(bool enable);
    void writelog(int type,string content, int line = 0);
    void setSurfix(string surfix);
private:
    string on_screen_output_content;
    string tagline_content[16];
    float tagline_time_remain[16];
    bool osd_enable;
    string log_surfix;
    //string log_filename;
};

extern debug_ex debugger_main;


class Drawable
{
public:
    virtual void update(float timeScale) = 0;
    virtual void rend() = 0;

private:

};



//写入日志
void writelog(string content);
void writelog(int type,string content);


//读取文件，装入filebuf
int ReadFile_1(const char* filename, string filebuf[],int max_num);

//从filebuf写入文件
bool WriteFile(const char* filename, string filebuf[] , int max_num = 64);

//验证资源文件的完整性
bool md5_verify(const char* filename, const char* expected_md5);

//验证存档的完整性
bool file_private_verify(string filename);

//生成存档的校验文件
void certfile(string filename);

//初始化未创建的存档文件
void fileinit();

//初始化用户文件，在注册时调用
void userinit();

//更新 history 和 account记录，在对局结束时调用
//void game_record();

//读取指定history文件
//void game_read_history();

//读取account文件
void game_read_record();

//更新AllUsersProfile
void update_profile();

//控件部分


//重置页面状态
void InitPage(int index);


//类型转换
//string tcharToChar(TCHAR* buffer);

void Wchar_tToString(string& szDst, wchar_t* wchar);

LPCWSTR stringToLPCWSTR(string orig);

//清空string数组
void CleanStrBuff(string buf[], int size);

//将字符串包入中括号
string addFormat(string ori);

//获取当前时间戳
string getTimeStr();

//获取截短时间(单位：秒)
string getTimeDigit(float time);

//获取截短时间(单位：分)
string getTimeDigit_minute(float time_sec);

//计算字符串md5
string md5sum(string& str);

//判断字符串是否全为数字
bool AllisNum(string str);

//将位置坐标转换为渲染坐标
inline float to_screen(float ori)
{
    return ori * scale;
}


//文件读写线程
unsigned __stdcall File_IO(LPVOID lpParameter);


//载入tips文字
void init_string();

//初始化窗口,DirectX设备和内部资源
void init_once();

//校验外部资源并准备DirectX资源
void initgame();

//载入字体
void LoadFonts();

//强制退出(响应quit_single)
void process_quit();

//获取剪切板文字内容
string getClipBoard();

//设置剪切板文字内容
bool setClipBoard(string content);


//程序重启
void self_restart();

//设定跳转的目标页面和目标页面状态
void set_target_page(int page_index, int page_status, float delay_time);

//set_target_page()实现
void update_target_page();


//渲染部分

extern ID2D1DeviceContext1* g_pD2DDeviceContext;
extern IDXGISwapChain1* g_pSwapChain;
extern IWICImagingFactory* pIWICFactory;

extern ID2D1SolidColorBrush* g_pBrushBlack;	//brush
extern ID2D1SolidColorBrush* g_pBrushQuit;
extern ID2D1SolidColorBrush* g_pBrushGray;
extern ID2D1SolidColorBrush* g_pBrushLightGray;
extern ID2D1SolidColorBrush* g_pBrushBlue;
extern ID2D1SolidColorBrush* g_pBrushDarkBlue;
extern ID2D1SolidColorBrush* g_pBrushLightBlue;
extern ID2D1SolidColorBrush* g_pBrushYellow;
extern ID2D1SolidColorBrush* g_pBrushLightYellow;
extern ID2D1SolidColorBrush* g_pBrushFloralWhite;
extern ID2D1SolidColorBrush* g_pBrushGreen;
extern ID2D1SolidColorBrush* g_pBrushLightGreen;
//extern ID2D1SolidColorBrush* g_pBrushtext;
extern ID2D1SolidColorBrush* g_pBrushRed;
extern ID2D1SolidColorBrush* g_pBrushPurple;
extern ID2D1SolidColorBrush* g_pBrushBrown;
extern ID2D1SolidColorBrush* BrushRand[8];
extern ID2D1SolidColorBrush* g_pBrushBGSelect;
extern ID2D1SolidColorBrush* g_pBrushLight;
extern ID2D1SolidColorBrush* g_pBrushDark;
extern ID2D1SolidColorBrush* g_pBrushPink;
extern ID2D1SolidColorBrush* g_pBrushWhite;
extern ID2D1SolidColorBrush* g_pBrushNetwork;
extern ID2D1SolidColorBrush* g_pBrushGameTemp;
extern ID2D1LinearGradientBrush* g_pLinearGradientBrush;
extern ID2D1RadialGradientBrush* g_pRadialGradientBrush;
extern ID2D1RadialGradientBrush* g_pRadialGradientBrush2;
extern ID2D1RadialGradientBrush* g_pRadialGradientBrush3;

extern ID2D1BitmapBrush1* g_pBitmapBrushUI[20];

//extern ID2D1PathGeometry* pathGeometry[8];
//extern ID2D1GeometryRealization* networkGeometry[12];

extern IDWriteTextFormat* g_pTextFormat;
extern IDWriteTextFormat* g_pTextFormatLargeCN;
extern IDWriteTextFormat* g_pTextFormatLarge;
extern IDWriteTextFormat* g_pTextFormatL;
extern IDWriteTextFormat* g_pTextFormatmini;
extern IDWriteTextFormat* g_pTextFormatminiL;
extern IDWriteTextFormat* g_pTextFormatQuarlowLarge;
extern IDWriteTextFormat* g_pTextFormat2;
extern IDWriteTextFormat* g_pTextFormatNormal;
extern IDWriteTextFormat* g_pTextFormatNormalmini;
extern IDWriteTextFormat* g_pTextFormatNormalLarge;
extern IDWriteTextFormat* g_pTextFormatNetwork;
extern IDWriteTextFormat* g_pTextFormat_debug;
extern IDWriteTextFormat* g_pTextFormatGame1;
extern IDWriteTextFormat* g_pTextFormatGame2;

extern ID2D1StrokeStyle* g_pStrokeStyle;




extern ID2D1Bitmap1* g_pD2DTargetBimtap;
extern ID2D1Bitmap* g_pD2DBimtapUI[64];
//extern ID2D1Bitmap1* startSceneBitmapChess_Black, * startSceneBitmapChess_White;
//extern ID2D1Bitmap1* networkBG_base;
extern ID2D1Effect* gaussianBlurEffect;
extern ID2D1Effect* OpacityEffect;
extern ID2D1Bitmap1* gaussianBlur_Bitmap1;
extern ID2D1Bitmap1* shortcut_Bitmap;
extern ID2D1Bitmap1* content_blur_Bitmap;


extern ID2D1Bitmap1* copy_succeed_tagline, * copy_failed_tagline;
extern ID2D1Bitmap1* io_error_tagline, * file_corrupt_tagline;
extern ID2D1Bitmap1* numberBitmaps;




//extern RECT rc;
extern D2D1_RECT_F fpsRect, textrbRect;
//extern RECT windowrect;
//MSAA样本数
extern UINT sampleCountOut;
//每帧DrawCall次数
extern int DrawCallNum;
//帧率
extern int fps;
//渲染分辨率
extern int orix, oriy;
//屏幕宽度
extern int nWidth;  
//屏幕高度
extern int nHeight; 
//extern bool  isloadfiles, isloading, fullSrcState;
//是否接收键盘输入
extern bool input_expand;
//键盘输入值
extern char expand_input_key;

//窗口大小改变时获取窗口大小
void OnSize();

void RebuildRenderContext();

//接受鼠标和键盘输入
void input();

void input_wheel(short value);

//渲染提示标语缓存图片
void rend_taglines(bool interupt);

//载入图片
HRESULT LoadBitmapFromFile(
    IWICImagingFactory* pIWICFactory,
    PCWSTR uri,
    UINT destinationWidth,
    UINT destinationHeight,
    ID2D1Bitmap** ppBitmap, //out
    IWICBitmapSource** ppBitmapSource = nullptr //out,opt
);

//截取屏幕快照，返回1为成功
bool createShortcutBitmap(ID2D1Bitmap* dest);

//创建高斯模糊缓存图片
bool createGaussianBlurEffectBitmap(ID2D1Bitmap* src, ID2D1Image* dest, bool interupt, float deviation);

//Draw a complete picture
void DrawBitmap_1(ID2D1Bitmap* ppBitmap, D2D_RECT_F dest, float opacity);

//Draw part of the picture
void DrawBitmap_1(ID2D1Bitmap* ppBitmap, D2D_RECT_F dest, D2D_RECT_F src, float opacity);

//Draw effect to fullscreen
void DrawEffect_1(ID2D1Effect* effect);

//Draw SpriteSheet
//void DrawSpriteSheet();

//Draw Text
void DrawTextA_1(string content, IDWriteTextFormat* TextFormat, const D2D1_RECT_F rect, ID2D1Brush* brush, float opacity = 1.0f);
void DrawTextA_1(string content, IDWriteTextFormat* TextFormat, float x1,float y1,float x2,float y2, ID2D1Brush* brush, float opacity = 1.0f);

//input positive int content
//style 0 for fillY, 1 for fillX
void DrawNumber_1(int content, float x1, float y1, float x2, float y2, float opacity = 1.0f, int style = 0);

//Draw Line
void DrawLine_1(float x1, float y1, float x2, float y2, float width, ID2D1Brush* brush, float opacity=1.0f, ID2D1StrokeStyle* strokeStyle=0);


void DrawRectangle_1(float x1, float y1, float x2, float y2, float width, ID2D1Brush* brush, float opacity = 1.0f);

void FillRectangle_1(float x1, float y1, float x2, float y2, ID2D1Brush* brush, float opacity = 1.0f);

void DrawRoundedRectangle_1(float x1, float y1, float x2, float y2, float width, ID2D1Brush* brush, float opacity);

void FillRoundedRectangle_1(float x1, float y1, float x2, float y2, ID2D1Brush* brush, float opacity);

//Fill Ellipse
void FillEllipse_1(float x, float y, float radiusX, float radiusY, ID2D1Brush* brush, float opacity=1.0f);

void DrawTriangle_1(float x1, float y1, float x2, float y2, float x3, float y3, ID2D1Brush* brush,float width=2.0f, float opacity = 1.0f);

void FillTriangle_1(float x1, float y1, float x2, float y2, float x3, float y3, ID2D1Brush* brush, float opacity=1.0f, int accuracy=16);

//开销过大，少用
//确保 angle1 < angle2
void DrawArc_1(float center_posx, float center_posy, float r, float angle1, float angle2, bool largeArc, ID2D1Brush* brush,  float stroke_width=2.0f, float opacity=1.0f);


void FillOpacityMask_1(float x1, float y1, float x2, float y2, ID2D1Brush* brush, ID2D1Bitmap* mask,float opacity=1.0f);


//update button ui, frametime, input status
void update();


//渲染对应内容


//执行页面逻辑并渲染其它内容
void rendPage();

void rend_quit();


void rend_verification_warning();

//各个页面逻辑及内容已移至其它文件




//main application loop
void render();


extern int last_page_index;
extern int saveDeleted, recorded;
