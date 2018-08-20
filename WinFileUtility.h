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
    FOLDER_CSIDL_LOCALLOW_APPDATA = 0x0100                             // <user name>\Application Data\LocalLow ��ϵͳ����
};

enum class ProcessorArchitecture {
    _32Bit,
    _64_Bit
};

//��ȡӲ�̿ռ�,����ֽ���
bool GetDiskFreeSpace(const char* lpszDisk,             //�̷�  
                      ByteUnit* freeBytes,              //ʣ��ռ�
                      ByteUnit* totalBytes = nullptr    //�ܿռ�
                      );

bool GetDiskFreeSpaceW(const wchar_t* lpszDisk,          //�̷�  
                       ByteUnit* freeBytes,              //ʣ��ռ�
                       ByteUnit* totalBytes = nullptr    //�ܿռ�
                       );



//�ļ��Ƿ����
bool IsFileExistsW(const wchar_t* lpszFileName);
//�ļ��Ƿ����
bool IsFileExistsA(const char* lpszFileName);

//Ŀ¼�Ƿ����
bool IsDirectoryExistsW(const wchar_t* strFileName);
bool IsDirectoryExistsA(const char* strFileName);

//���ļ�������·��  0 �ļ��У�-1 ������,1 �ļ�
int IsPathOrFileW(const wchar_t* lpszFileName);
int IsPathOrFileA(const char* lpszFileName);

//�����ļ���
bool CreateFolderW(const wchar_t* szFolder);
//�����ļ���
bool CreateFolderA(const char* szFolder);
//�ݹ鴴���ļ���
bool CreateDirectoryUtil(const char* lpPath);
bool CreateDirectoryUtilW(const wchar_t* lpPath);

void CreateIfDirectoryNotExists(const char* lpPath);
void CreateIfDirectoryNotExistsW(const wchar_t* lpPath);

bool DeleteTree(const std::string& pathName, bool isAllowUndo = false);

//�����ļ����ļ���
bool CopyFile(const std::string& to, const std::string& from);

//�ƶ��ļ����ļ���
bool MoveFile(const std::string& to, const std::string& from);

//�������ļ����ļ���
bool ReNameFile(const std::string& to, const std::string& from);

//�ļ���С
long long GetFileSize(const wchar_t* szFileName);

//�ļ���С
long long GetFileSize(const char* szFileName);

//ȡ�õ�ǰĿ¼
std::wstring GetCurrentDirectoryW();
std::string GetCurrentDirectoryA();
//ȡ�ô��ļ����ĳ���·��
std::wstring GetFullApplicationPath();
std::string GetFullApplicationPathA();

//ȡ�ó�����,Ĭ�ϲ�����չ��
std::wstring GetApplicationName(bool isWithExtension = false);
std::string GetApplicationNameA(bool isWithExtension = false);

//��ȡdesktopĿ¼·��
std::string GetDesktopPath();
std::wstring GetDesktopPathW();

//��ȡDocumentĿ¼
std::string GetDocumentPath();
std::wstring GetDocumentPathW();

//��ȡϵͳ�����ļ�������λ��
std::string GetFolderPathA(SystemFolder folder);
std::wstring GetFolderPathW(SystemFolder folder);

//Ŀ¼�Ƿ���� \\��/�ĺ�׺
bool IsHasPathSuffix(const std::wstring& path);
bool IsHasPathSuffix(const std::string& path);

//Ŀ¼��δû��\\��/��׺ ����
std::wstring FixIfNotHasPathSuffix(const std::wstring& path);
std::string FixIfNotHasPathSuffix(const std::string& path);

//ȡ����չ��
std::wstring GetExtName(const std::wstring& fileName);
std::string GetExtName(const std::string& fileName);
//�ı���չ��
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


//ָ�����ƵĽ��̴���,����true
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
* �������ƣ�IsForegroundFullscreen
* ����˵�����жϵ�ǰ�������û������ĵ�ǰ������Ƿ���ȫ���ġ�
* �̰߳�ȫ����
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

//fAltTab [in]��TRUE ��ʾʹ�� Alt/Ctl+Tab �����Ⱥ�������л����ڣ�������Ϊ FALSE��
void SwitchToThisWindow(HWND hwnd, BOOL fAltTab);
NAMESPACE_DEFINE_END(Utils);

#endif //UTIL_FILEIO_H