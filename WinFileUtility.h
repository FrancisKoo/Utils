#ifndef UTIL_FILEIO_H
#define UTIL_FILEIO_H

#include "utils.h"

#include <shlobj.h>
#include <string>
#include <vector>

NAMESPACE_DEFINE_BEGIN(Utils)
class ByteUnit;

enum class SystemFolder {
    FOLDER_CSIDL_DESKTOP = CSIDL_DESKTOP,           // <desktop>          
    FOLDER_CSIDL_INTERNET = CSIDL_INTERNET,         // Internet Explorer (icon on desktop)      
    FOLDER_CSIDL_PROGRAMS = CSIDL_PROGRAMS,         // Start Menu\Programs         
    FOLDER_CSIDL_CONTROLS = CSIDL_CONTROLS,         // My Computer\Control Panel        
    FOLDER_CSIDL_PRINTERS = CSIDL_PRINTERS,         // My Computer\Printers         
    FOLDER_CSIDL_PERSONAL = CSIDL_PERSONAL,         // My Documents         
    FOLDER_CSIDL_FAVORITES = CSIDL_FAVORITES,       // <user name>\Favorites         
    FOLDER_CSIDL_STARTUP = CSIDL_STARTUP,           // Start Menu\Programs\Startup         
    FOLDER_CSIDL_RECENT = CSIDL_RECENT,             // <user name>\Recent         
    FOLDER_CSIDL_SENDTO = CSIDL_SENDTO,             // <user name>\SendTo         
    FOLDER_CSIDL_BITBUCKET = CSIDL_BITBUCKET,       // <desktop>\Recycle Bin         
    FOLDER_CSIDL_STARTMENU = CSIDL_STARTMENU,       // <user name>\Start Menu        
    FOLDER_CSIDL_MYDOCUMENTS = CSIDL_MYDOCUMENTS,   // Personal was just a silly name for My Documents  
    FOLDER_CSIDL_MYMUSIC = CSIDL_MYMUSIC,           // My Music folder         
    FOLDER_CSIDL_MYVIDEO = CSIDL_MYVIDEO,           // My Videos folder         
    FOLDER_CSIDL_DESKTOPDIRECTORY = CSIDL_DESKTOPDIRECTORY, // <user name>\Desktop         
    FOLDER_CSIDL_DRIVES = CSIDL_DRIVES,             // My Computer         
    FOLDER_CSIDL_NETWORK = CSIDL_NETWORK,           // Network Neighborhood (My Network Places)      
    FOLDER_CSIDL_NETHOOD = CSIDL_NETHOOD,           // <user name>\nethood         
    FOLDER_CSIDL_FONTS = CSIDL_FONTS,               // windows\fonts          
    FOLDER_CSIDL_TEMPLATES = CSIDL_TEMPLATES,
    FOLDER_CSIDL_COMMON_STARTMENU = CSIDL_COMMON_STARTMENU,     // All Users\Start Menu        
    FOLDER_CSIDL_COMMON_PROGRAMS = CSIDL_COMMON_PROGRAMS,       // All Users\Start Menu\Programs        
    FOLDER_CSIDL_COMMON_STARTUP = CSIDL_COMMON_STARTUP,         // All Users\Startup         
    FOLDER_CSIDL_COMMON_DESKTOPDIRECTORY = CSIDL_COMMON_DESKTOPDIRECTORY, // All Users\Desktop         
    FOLDER_CSIDL_APPDATA = CSIDL_APPDATA,                       // <user name>\Application Data        
    FOLDER_CSIDL_PRINTHOOD = CSIDL_PRINTHOOD,                   // <user name>\PrintHood         

    FOLDER_CSIDL_LOCAL_APPDATA = CSIDL_LOCAL_APPDATA,           // <user name>\Local Settings\Applicaiton Data (non roaming)     

    FOLDER_CSIDL_ALTSTARTUP = CSIDL_ALTSTARTUP,                 // non localized startup        
    FOLDER_CSIDL_COMMON_ALTSTARTUP = CSIDL_COMMON_ALTSTARTUP,   // non localized common startup       
    FOLDER_CSIDL_COMMON_FAVORITES = CSIDL_COMMON_FAVORITES,

    FOLDER_CSIDL_INTERNET_CACHE = CSIDL_INTERNET_CACHE,
    FOLDER_CSIDL_COOKIES = CSIDL_COOKIES,
    FOLDER_CSIDL_HISTORY = CSIDL_HISTORY,
    FOLDER_CSIDL_COMMON_APPDATA = CSIDL_COMMON_APPDATA,         // All Users\Application Data        
    FOLDER_CSIDL_WINDOWS = CSIDL_WINDOWS,                       // GetWindowsDirectory()          
    FOLDER_CSIDL_SYSTEM = CSIDL_SYSTEM,                         // GetSystemDirectory()          
    FOLDER_CSIDL_PROGRAM_FILES = CSIDL_PROGRAM_FILES,           // C:\Program Files         
    FOLDER_CSIDL_MYPICTURES = CSIDL_MYPICTURES,                 // C:\Program Files\My Pictures        

    FOLDER_CSIDL_PROFILE = CSIDL_PROFILE,                       // USERPROFILE          
    FOLDER_CSIDL_SYSTEMX86 = CSIDL_SYSTEMX86,                   // x86 system directory on RISC      
    FOLDER_CSIDL_PROGRAM_FILESX86 = CSIDL_PROGRAM_FILESX86,     // x86 C:\Program Files on RISC      

    FOLDER_CSIDL_PROGRAM_FILES_COMMON = CSIDL_PROGRAM_FILES_COMMON, // C:\Program Files\Common         

    FOLDER_CSIDL_PROGRAM_FILES_COMMONX86 = CSIDL_PROGRAM_FILES_COMMONX86, // x86 Program Files\Common on RISC      
    FOLDER_CSIDL_COMMON_TEMPLATES = CSIDL_COMMON_TEMPLATES,     // All Users\Templates         

    FOLDER_CSIDL_COMMON_DOCUMENTS = CSIDL_COMMON_DOCUMENTS,     // All Users\Documents         
    FOLDER_CSIDL_COMMON_ADMINTOOLS = CSIDL_COMMON_ADMINTOOLS,   // All Users\Start Menu\Programs\Administrative Tools       
    FOLDER_CSIDL_ADMINTOOLS = CSIDL_ADMINTOOLS,                 // <user name>\Start Menu\Programs\Administrative Tools       

    FOLDER_CSIDL_CONNECTIONS = CSIDL_CONNECTIONS,               // Network and Dial-up Connections       
    FOLDER_CSIDL_COMMON_MUSIC = CSIDL_COMMON_MUSIC,             // All Users\My Music        
    FOLDER_CSIDL_COMMON_PICTURES = CSIDL_COMMON_PICTURES,       // All Users\My Pictures        
    FOLDER_CSIDL_COMMON_VIDEO = CSIDL_COMMON_VIDEO,             // All Users\My Video        
    FOLDER_CSIDL_RESOURCES = CSIDL_RESOURCES,                   // Resource Direcotry
    FOLDER_CSIDL_LOCALLOW_APPDATA = 0x0100                             // <user name>\Application Data\LocalLow 非系统定义
};

enum class ProcessorArchitecture {
    _32Bit,
    _64_Bit
};

//获取硬盘空间,输出字节数
bool GetDiskFreeSpace(const char* lpszDisk,             //盘符  
                      ByteUnit* freeBytes,              //剩余空间
                      ByteUnit* totalBytes = nullptr    //总空间
                      );

bool GetDiskFreeSpaceW(const wchar_t* lpszDisk,          //盘符  
                       ByteUnit* freeBytes,              //剩余空间
                       ByteUnit* totalBytes = nullptr    //总空间
                       );



//文件是否存在
bool IsFileExistsW(const wchar_t* lpszFileName);
//文件是否存在
bool IsFileExistsA(const char* lpszFileName);

//目录是否存在
bool IsDirectoryExistsW(const wchar_t* strFileName);
bool IsDirectoryExistsA(const char* strFileName);

//是文件，还是路径  0 文件夹，-1 不存在,1 文件
int IsPathOrFileW(const wchar_t* lpszFileName);
int IsPathOrFileA(const char* lpszFileName);

//创建文件夹
bool CreateFolderW(const wchar_t* szFolder);
//创建文件夹
bool CreateFolderA(const char* szFolder);
//递归创建文件夹
bool CreateDirectoryUtil(const char* lpPath);
bool CreateDirectoryUtilW(const wchar_t* lpPath);

void CreateIfDirectoryNotExists(const char* lpPath);
void CreateIfDirectoryNotExistsW(const wchar_t* lpPath);

bool DeleteTree(const std::string& pathName, bool isAllowUndo = false);

//复制文件或文件夹
bool CopyFile(const std::string& to, const std::string& from);

//移动文件或文件夹
bool MoveFile(const std::string& to, const std::string& from);

//重命名文件或文件夹
bool ReNameFile(const std::string& to, const std::string& from);

//文件大小
long long GetFileSize(const wchar_t* szFileName);

//文件大小
long long GetFileSize(const char* szFileName);

//取得当前目录
std::wstring GetCurrentDirectoryW();
std::string GetCurrentDirectoryA();
//取得带文件名的程序路径
std::wstring GetFullApplicationPath();
std::string GetFullApplicationPathA();

//取得程序名,默认不带扩展名
std::wstring GetApplicationName(bool isWithExtension = false);
std::string GetApplicationNameA(bool isWithExtension = false);

//获取desktop目录路径
std::string GetDesktopPath();
std::wstring GetDesktopPathW();

//获取Document目录
std::string GetDocumentPath();
std::wstring GetDocumentPathW();

//获取系统特殊文件夹所在位置
std::string GetFolderPathA(SystemFolder folder);
std::wstring GetFolderPathW(SystemFolder folder);

//目录是否存在 \\或/的后缀
bool IsHasPathSuffix(const std::wstring& path);
bool IsHasPathSuffix(const std::string& path);

//目录如未没有\\或/后缀 则补上
std::wstring FixIfNotHasPathSuffix(const std::wstring& path);
std::string FixIfNotHasPathSuffix(const std::string& path);

//取得扩展名
std::wstring GetExtName(const std::wstring& fileName);
std::string GetExtName(const std::string& fileName);
//改变扩展名
bool ChangeExtName(const std::wstring& fileName, const std::wstring& newExtName);

std::wstring ExtractPath(const std::wstring& exeAbsolutePath);

std::string ExtractPathA(const std::string& exeAbsolutePath);


std::string ReadRegistry(HKEY rooKey,
                         const char* subKey,
                         const char* valueName,
                         bool isWOW64_32 = true);
std::string ReadRegistry(const char* rooKey,
                         const char* subKey,
                         const char* valueName,
                         bool isWOW64_32 = true);

std::wstring ReadRegistryW(HKEY rooKey,
                          const wchar_t* subKey,
                          const wchar_t* valueName,
                          bool isWOW64_32 = true);
std::wstring ReadRegistryW(const wchar_t* rooKey,
                          const wchar_t* subKey,
                          const wchar_t* valueName,
                          bool isWOW64_32 = true);


//指定名称的进程存在,返回true
bool IsProcessExists(const char* name);
bool IsProcessExistsW(const wchar_t* name);
DWORD GetProcessIDFromName(const char* name);
DWORD GetProcessIDFromNameW(const wchar_t* name);

bool IsWow64();

ProcessorArchitecture QuerySystemProcessorArchitecture();

bool IsProcessRunningA(const char* processName);

bool IsProcessRunningW(const wchar_t* processName);

bool KillProcessByName(const std::wstring& processName);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* 函数名称：IsForegroundFullscreen
* 功能说明：判断当前正在与用户交互的当前激活窗口是否是全屏的。
* 线程安全：是
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
bool IsForegroundFullscreen();


enum class InstallInfomarationFlag {
    InstallLocation,
    UninstallString
};

enum { D_PATHBUFFLENGTH = 2048 };

std::string GetInstallInfomarationByGuid(const std::string& guid,
                                         InstallInfomarationFlag flag,
                                         bool isWow64 = false);

std::wstring GetInstallInfomarationByGuidW(const std::wstring& guid,
                                           InstallInfomarationFlag flag,
                                           bool isWow64 = false);

std::vector<std::string> GetCommandLine();

bool IsFolderEmpty(const char* folder);

//fAltTab [in]：TRUE 表示使用 Alt/Ctl+Tab 键的先后次序来切换窗口，否则设为 FALSE。
void SwitchToThisWindow(HWND hwnd, BOOL fAltTab);
NAMESPACE_DEFINE_END(Utils);

#endif //UTIL_FILEIO_H