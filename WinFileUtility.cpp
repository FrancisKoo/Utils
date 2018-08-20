#include "stdafx.h"

#include "WinFileUtility.h"

#include <tchar.h>
#include <windows.h>
#include <ShellAPI.h>
#include <mmsystem.h>
#include <shlwapi.h>
#include <io.h>
#include <cstdlib>
#include <TlHelp32.h>

#include "string_lib.h"
#include "StringEncoding.h"
#include "UnitConversion.h"


#if defined(WIN32) || defined(WINCE)

// 宏定义
#define  D_PATHBUFFLENGTH 2048

int CreateDirectory_Util(const char* path, LPSECURITY_ATTRIBUTES securityAttributes) {
    return ::CreateDirectoryA(path, securityAttributes);
}

int CreateDirectory_UtilW(const wchar_t* path, LPSECURITY_ATTRIBUTES securityAttributes) {
    return ::CreateDirectoryW(path, securityAttributes);
}

// #define CreateDirectory_Util ::CreateDirectory

long long GetFileSize_Util(const char* szFileName) {
    WIN32_FILE_ATTRIBUTE_DATA data = {0};
    if (::GetFileAttributesExA(szFileName, GetFileExInfoStandard, &data) == 0)
        return 0;
    return data.nFileSizeLow;
}

long long GetFileSize_UtilW(const wchar_t* szFileName) {
    WIN32_FILE_ATTRIBUTE_DATA data = {0};
    if (::GetFileAttributesExW(szFileName, GetFileExInfoStandard, &data) == 0)
        return 0;
    return data.nFileSizeLow;
}

#else

#defined FILE_ATTRIBUTE_DIRECTORY 0x00000010

static
unsigned long GetFileAttributesW(const wchar_t* lpszFileName) {
    return 0xFFFFFFFF;
}

static
unsigned long GetFileAttributesA(const char* lpszFileName) {
    return 0xFFFFFFFF;
}

#if defined(UNICODE) || defined(_UNICODE)
#define GetFileAttributes GetFileAttributesW
#else
#define GetFileAttributes GetFileAttributesA
#endif


static
bool CreateDirectory_Util(const wchar_t* lpPath, void* param0) {

}

long long GetFileSize_Util(const wchar_t* szFileName) {
    return 0;
}

#endif // !defined(WIN32) || defined(WINCE)


NAMESPACE_DEFINE_BEGIN(Utils)


// BOOL GetDiskFreeSpace(
// LPCTSTR lpRootPathName,  //要查询的逻辑磁盘根路径，如"C:\"，传入NULL表示程序当前磁盘
// LPDWORD lpSectorsPerCluster,     //磁盘一个簇内的扇区数
// LPDWORD lpBytesPerSector,        //磁盘一个扇区内的字节数
// LPDWORD lpNumberOfFreeClusters,  //磁盘的剩余簇数
// LPDWORD lpTotalNumberOfClusters  //磁盘总簇数
// )


// BOOL GetDiskFreeSpaceExA(
// __in_opt LPCSTR lpDirectoryName,						//驱动器的名称。
// __out_opt PULARGE_INTEGER lpFreeBytesAvailableToCaller,	//用户可用的磁盘空间。
// __out_opt PULARGE_INTEGER lpTotalNumberOfBytes,			//磁盘总共的空间。
// __out_opt PULARGE_INTEGER lpTotalNumberOfFreeBytes		//磁盘空闲的空间。以上都是字节为单位。
// );

typedef BOOL(WINAPI *P_GDFSE)(LPCSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);
typedef BOOL(WINAPI *P_GDFSEW)(LPCWSTR, PULARGE_INTEGER, PULARGE_INTEGER, PULARGE_INTEGER);

typedef unsigned __int64 uint64;

bool GetDiskFreeSpace(const char* lpszDisk,
                      ByteUnit* freeBytes,
                      ByteUnit* totalBytes) {
    std::string disk = lpszDisk;
    if (disk.empty() || disk.length() < 2L) {
        return false;
    }
    disk = disk.substr(0, 2);
    BOOL  fResult;
    P_GDFSE pGetDiskFreeSpaceEx = NULL;

    pGetDiskFreeSpaceEx = (P_GDFSE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "GetDiskFreeSpaceExA");

    if (pGetDiskFreeSpaceEx) {
        unsigned __int64 i64FreeBytesToCaller,
            i64TotalBytes,
            i64FreeBytes;
        fResult = pGetDiskFreeSpaceEx(disk.c_str(),
                                      (PULARGE_INTEGER)&i64FreeBytesToCaller,
                                      (PULARGE_INTEGER)&i64TotalBytes,
                                      (PULARGE_INTEGER)&i64FreeBytes);
        if (fResult) {
            if (freeBytes) {
                *freeBytes = i64FreeBytesToCaller;
            }
            if (totalBytes) {
                *totalBytes = i64TotalBytes;
            }
            return true;
        }
    } else {
        DWORD dwSectPerClust,
            dwBytesPerSect,
            dwFreeClusters,
            dwTotalClusters;
        fResult = ::GetDiskFreeSpaceA(lpszDisk,
                                      &dwSectPerClust,		//磁盘一个簇内的扇区数
                                      &dwBytesPerSect,		//磁盘一个扇区内的字节数
                                      &dwFreeClusters,		//磁盘的剩余簇数
                                      &dwTotalClusters);	//磁盘总簇数
        if (fResult) {
            if (freeBytes) {
                *freeBytes = uint64(dwFreeClusters)
                    * uint64(dwSectPerClust)
                    * uint64(dwBytesPerSect);
            }
            if (totalBytes) {
                *totalBytes = uint64(dwTotalClusters)
                    * uint64(dwSectPerClust)
                    * uint64(dwBytesPerSect);
            }
            return true;
        }
    }
    return false;
}

bool GetDiskFreeSpaceW(const wchar_t* lpszDisk,
                       ByteUnit* freeBytes,
                       ByteUnit* totalBytes) {
    std::wstring disk = lpszDisk;
    if (disk.empty() || disk.length() < 2L) {
        return false;
    }
    disk = disk.substr(0, 2);
    BOOL  fResult;
    P_GDFSEW pGetDiskFreeSpaceEx = NULL;

    pGetDiskFreeSpaceEx = (P_GDFSEW)GetProcAddress(GetModuleHandleW(L"kernel32.dll"), "GetDiskFreeSpaceExW");

    if (pGetDiskFreeSpaceEx) {
        unsigned __int64 i64FreeBytesToCaller,
            i64TotalBytes,
            i64FreeBytes;
        fResult = pGetDiskFreeSpaceEx(disk.c_str(),
                                      (PULARGE_INTEGER)&i64FreeBytesToCaller,
                                      (PULARGE_INTEGER)&i64TotalBytes,
                                      (PULARGE_INTEGER)&i64FreeBytes);
        if (fResult) {
            if (freeBytes) {
                *freeBytes = i64FreeBytesToCaller;
            }
            if (totalBytes) {
                *totalBytes = i64TotalBytes;
            }
            return true;
        }
    } else {
        DWORD dwSectPerClust,
            dwBytesPerSect,
            dwFreeClusters,
            dwTotalClusters;
        fResult = ::GetDiskFreeSpaceW(lpszDisk,
                                      &dwSectPerClust,		//磁盘一个簇内的扇区数
                                      &dwBytesPerSect,		//磁盘一个扇区内的字节数
                                      &dwFreeClusters,		//磁盘的剩余簇数
                                      &dwTotalClusters);	//磁盘总簇数
        if (fResult) {
            if (freeBytes) {
                *freeBytes = uint64(dwFreeClusters)
                    * uint64(dwSectPerClust)
                    * uint64(dwBytesPerSect);
            }
            if (totalBytes) {
                *totalBytes = uint64(dwTotalClusters)
                    * uint64(dwSectPerClust)
                    * uint64(dwBytesPerSect);
            }
            return true;
        }
    }
    return false;
}
bool IsFileExistsW(const wchar_t* lpszFileName) {
    return (1 != IsPathOrFileW(lpszFileName)) ? false : true;
}

bool IsFileExistsA(const char* lpszFileName) {
    return (1 != IsPathOrFileA(lpszFileName)) ? false : true;
}

bool IsDirectoryExistsW(const wchar_t* lpszFileName) {
    return (0 != IsPathOrFileW(lpszFileName)) ? false : true;
}

bool IsDirectoryExistsA(const char* strFileName) {
    return (0 != IsPathOrFileA(strFileName)) ? false : true;
}

int IsPathOrFileW(const wchar_t* lpszFileName) {
    unsigned long dwFlag = GetFileAttributesW(lpszFileName);
    if (INVALID_FILE_SIZE == dwFlag)
        return -1;//不存在
    if (FILE_ATTRIBUTE_DIRECTORY & dwFlag)
        return 0;//文件夹
    return 1; //文件
}

int IsPathOrFileA(const char* lpszFileName) {
    unsigned long dwFlag = GetFileAttributesA(lpszFileName);
    if (INVALID_FILE_SIZE == dwFlag)
        return -1;//不存在
    if (FILE_ATTRIBUTE_DIRECTORY & dwFlag)
        return 0;//文件夹
    return 1; //文件
}

bool CreateFolderW(const wchar_t* szFolder) {
    if (!szFolder || !wcslen(szFolder))
        return false;

    unsigned long dwAttrib = GetFileAttributesW(szFolder);

    // already exists ?
    if (dwAttrib != INVALID_FILE_SIZE)
        return ((dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);

    // if can't create parent
    if (CreateDirectory_UtilW(szFolder, NULL)) {
        dwAttrib = 0;
    }

    return (0 == dwAttrib);
}
bool CreateFolderA(const char* szFolder) {
    if (!szFolder || !strlen(szFolder))
        return false;

    unsigned long dwAttrib = GetFileAttributesA(szFolder);

    // already exists ?
    if (dwAttrib != INVALID_FILE_SIZE)
        return ((dwAttrib & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY);

    // if can't create parent
    if (CreateDirectory_Util(szFolder, NULL)) {
        dwAttrib = 0;
    }

    return (0 == dwAttrib);
}
bool CreateDirectoryUtil(const char* lpPath) {//递归创建
    bool isSucceeded = true;

    if (!lpPath) {
        return false;
    }

    {
        char path[513] = {0};
        char* zp = 0;

        strncpy_s(path, 512, lpPath, 512);
        zp = path;
        while (*zp != '\0') {
            char* zf = strchr(zp, '\\');
            if (!zf) {
                break;
            }
            *zf = '\0';

            if (::CreateDirectoryA(path, NULL))
                isSucceeded |= true;

            *zf = '\\';
            zp = zf + 1;
        }
    }
    return isSucceeded;
}

bool CreateDirectoryUtilW(const wchar_t* lpPath) {//递归创建
    bool isSucceeded = true;

    if (!lpPath) {
        return false;
    }

    {
        wchar_t path[513] = {0};
        wchar_t* zp = 0;

        wcsncpy_s(path, 512, lpPath, 512);
        zp = path;
        while (*zp != '\0') {
            wchar_t* zf = wcschr(zp, '\\');
            if (!zf) {
                break;
            }
            *zf = '\0';

            if (::CreateDirectoryW(path, NULL))
                isSucceeded |= true;

            *zf = '\\';
            zp = zf + 1;
        }
    }
    return isSucceeded;
}

bool DeleteTree(const std::string& pathName, bool isAllowUndo) {
    int nLen = pathName.length();
    char* pPath = new char[nLen + 2];
    strcpy_s(pPath, nLen + 2, pathName.c_str());
    pPath[nLen + 1] = '\0';

    SHFILEOPSTRUCTA FileOp = {0};
    FileOp.fFlags =
        FOF_NOCONFIRMATION  //不出现确认对话框     
        | FOF_NOERRORUI     //不出现错误对话框
        | FOF_SILENT;       //静默
    if (isAllowUndo) {
        //放入回收站
        FileOp.fFlags |= FOF_ALLOWUNDO;
    }
    FileOp.pFrom = pPath;
    FileOp.pTo = NULL;      //一定要是NULL
    FileOp.wFunc = FO_DELETE;    //删除操作

    int res = SHFileOperationA(&FileOp);
    delete[]pPath;
    return res == 0;
}

//复制文件或文件夹
bool CopyFile(const std::string& to, const std::string& from) {
    int nFromLen = from.length();
    int nToLen = to.length();
    char* pFrom = new char[nFromLen + 2];
    char* pTo = new char[nToLen + 2];
    strcpy_s(pFrom, nFromLen, from.c_str());
    strcpy_s(pTo, nToLen, to.c_str());
    pFrom[nFromLen + 1] = '\0';
    pTo[nToLen + 1] = '\0';

    SHFILEOPSTRUCTA FileOp = {0};
    FileOp.fFlags =
        FOF_NOCONFIRMATION  //不出现确认对话框
        | FOF_NOCONFIRMMKDIR  //需要时直接创建一个文件夹,不需用户确定
        | FOF_SILENT;       //静默

    FileOp.pFrom = pFrom;
    FileOp.pTo = pTo;
    FileOp.wFunc = FO_COPY;
    int res = SHFileOperationA(&FileOp);
    delete[] pFrom;
    delete[] pTo;
    return res == 0;
}

//移动文件或文件夹
bool MoveFile(const std::string& to, const std::string& from) {
    int nFromLen = from.length();
    int nToLen = to.length();
    char* pFrom = new char[nFromLen + 2];
    char* pTo = new char[nToLen + 2];
    //     strcpy_s(pFrom, nFromLen, from.c_str());
    //     strcpy_s(pTo, nToLen, to.c_str());
    pFrom[nFromLen + 1] = '\0';
    pTo[nToLen + 1] = '\0';

    SHFILEOPSTRUCTA FileOp = {0};
    FileOp.fFlags =
        FOF_NOCONFIRMATION  //不出现确认对话框
        | FOF_NOCONFIRMMKDIR  //需要时直接创建一个文件夹,不需用户确定
        | FOF_SILENT;       //静默

    FileOp.pFrom = from.c_str();
    FileOp.pTo = to.c_str();
    FileOp.wFunc = FO_MOVE;
    int res = SHFileOperationA(&FileOp);
    delete[] pFrom;
    delete[] pTo;
    return res == 0;
}

//重命名文件或文件夹
bool ReNameFile(const std::string& to, const std::string& from) {
    int nFromLen = from.length();
    int nToLen = to.length();
    char* pFrom = new char[nFromLen + 2];
    char* pTo = new char[nToLen + 2];
    //strcpy_s(pFrom, nFromLen + 2, from.c_str());
    //strcpy_s(pTo, nToLen + 2, to.c_str());
    pFrom[nFromLen + 1] = '\0';
    pTo[nToLen + 1] = '\0';

    SHFILEOPSTRUCTA FileOp = {0};
    FileOp.fFlags = FOF_NOCONFIRMATION |   //不出现确认对话框
        FOF_NOCONFIRMMKDIR; //需要时直接创建一个文件夹,不需用户确定
    FileOp.pFrom = from.c_str();
    FileOp.pTo = to.c_str();
    FileOp.wFunc = FO_MOVE;
    int res = SHFileOperationA(&FileOp);
    delete[] pFrom;
    delete[] pTo;
    return res == 0;
}

long long GetFileSize(const wchar_t* szFileName) {
    return GetFileSize_UtilW(szFileName);
}

long long GetFileSize(const char* szFileName) {
    WIN32_FILE_ATTRIBUTE_DATA data = {0};
    if (::GetFileAttributesExA(szFileName, GetFileExInfoStandard, &data) == 0)
        return 0;
    return data.nFileSizeLow;
}

std::wstring GetCurrentDirectoryW() {
    wchar_t str[MAX_PATH];
    GetModuleFileNameW(NULL, str, MAX_PATH);
    std::wstring path = str;
    path = path.substr(0, path.rfind(L"\\") + 1);
    return path;
}

std::string GetCurrentDirectoryA() {
    char str[MAX_PATH];
    GetModuleFileNameA(NULL, str, MAX_PATH);
    std::string path = str;
    path = path.substr(0, path.rfind("\\") + 1);
    return path;
}

std::wstring GetFullApplicationPath() {
    wchar_t str[MAX_PATH];
    GetModuleFileNameW(NULL, str, MAX_PATH);
    return std::wstring(str);
}

std::string GetFullApplicationPathA() {
    char str[MAX_PATH];
    GetModuleFileNameA(NULL, str, MAX_PATH);
    return std::string(str);
}

std::wstring GetApplicationName(bool isWithExtension) {
    std::wstring path = GetFullApplicationPath();
    path = path.substr(path.rfind(L"\\") + 1);
    if (isWithExtension) {
        return path;
    }
    return path.substr(0, path.find(L"."));
}

std::string GetApplicationNameA(bool isWithExtension /*= false*/) {
    auto path = GetFullApplicationPathA();
    path = path.substr(path.rfind("\\") + 1);
    if (isWithExtension) {
        return path;
    }
    return path.substr(0, path.find("."));
}

//获取desktop目录路径
std::string GetDesktopPath() {
    std::string path;
    char sysDocumentPath[MAX_PATH + 1];

    ::SHGetFolderPathA(NULL, CSIDL_DESKTOPDIRECTORY, NULL, SHGFP_TYPE_CURRENT, sysDocumentPath);

    path = FixIfNotHasPathSuffix(sysDocumentPath);
    return path;
}

std::wstring GetDesktopPathW() {
    std::wstring path;
    wchar_t sysDocumentPath[MAX_PATH + 1];

    ::SHGetFolderPathW(NULL, CSIDL_DESKTOPDIRECTORY, NULL, SHGFP_TYPE_CURRENT, sysDocumentPath);

    path = FixIfNotHasPathSuffix(sysDocumentPath);
    return path;
}

//获取Document目录
std::string GetDocumentPath() {
    std::string path;
    char sysDocumentPath[MAX_PATH + 1];

    ::SHGetFolderPathA(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, sysDocumentPath);
    path = FixIfNotHasPathSuffix(sysDocumentPath);
    return path;
}

std::wstring GetDocumentPathW() {
    std::wstring path;
    wchar_t sysDocumentPath[MAX_PATH + 1];

    ::SHGetFolderPathW(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, sysDocumentPath);
    path = FixIfNotHasPathSuffix(sysDocumentPath);
    return path;
}

std::string GetFolderPathA(SystemFolder folder) {
    std::string path;
    char sysDocumentPath[MAX_PATH + 1];
    if (folder == SystemFolder::FOLDER_CSIDL_LOCALLOW_APPDATA) {
        ::SHGetFolderPathA(NULL, int(SystemFolder::FOLDER_CSIDL_APPDATA), NULL, SHGFP_TYPE_CURRENT, sysDocumentPath);
        path = sysDocumentPath;
        if (is_end_with<char>(path, "Roaming")) {
            path = path.substr(0, path.find("Roaming"));
        }
        path = FixIfNotHasPathSuffix(path);
        path += "LocalLow\\";
    } else {
        ::SHGetFolderPathA(NULL, int(folder), NULL, SHGFP_TYPE_CURRENT, sysDocumentPath);
        path = FixIfNotHasPathSuffix(sysDocumentPath);
    }
    
    return path;
}

std::wstring GetFolderPathW(SystemFolder folder) {
    std::wstring path;
    wchar_t sysDocumentPath[MAX_PATH + 1];

    ::SHGetFolderPathW(NULL, int(folder), NULL, SHGFP_TYPE_CURRENT, sysDocumentPath);
    path = FixIfNotHasPathSuffix(sysDocumentPath);
    return path;
}

inline
bool _IsHasPathSuffix(const std::wstring& path) {
    return path.back() == L'\\' || path.back() == L'/';
}

bool IsHasPathSuffix(const std::wstring& path) {
    if (path.empty()) {
        return false;
    }
    return _IsHasPathSuffix(path);
}

std::wstring FixIfNotHasPathSuffix(const std::wstring& path) {
    if (!path.empty() && !_IsHasPathSuffix(path)) {
        return path + L"\\";
    }
    return path;
}

inline
bool _IsHasPathSuffix(const std::string& path) {
    return path.back() == '\\' || path.back() == '/';
}

bool IsHasPathSuffix(const std::string& path) {
    if (path.empty()) {
        return false;
    }
    return _IsHasPathSuffix(path);
}

std::string FixIfNotHasPathSuffix(const std::string& path) {
    if (!path.empty() && !_IsHasPathSuffix(path)) {
        return path + "\\";
    }
    return path;
}

std::wstring GetExtName(const std::wstring& fileName) {
    return fileName.substr(fileName.rfind(L'.'));
}

std::string GetExtName(const std::string& fileName) {
    return fileName.substr(fileName.rfind('.'));
}

bool ChangeExtName(const std::wstring& fileName, const std::wstring& newExtName) {
    auto newFileName = fileName.substr(0, fileName.rfind(L'.')) += newExtName;
    return false;
}

std::wstring ExtractPath(const std::wstring& exeAbsolutePath) {
    std::wstring path;
    if (!exeAbsolutePath.empty()) {
        auto pos = exeAbsolutePath.rfind(L"\\");
        if (pos != std::string::npos) {
            path = exeAbsolutePath.substr(0, pos + 1);
        }
    }
    return path;
}

std::string ExtractPathA(const std::string& exeAbsolutePath) {
    std::string path;
    if (!exeAbsolutePath.empty()) {
        auto pos = exeAbsolutePath.rfind("\\");
        if (pos != std::string::npos) {
            path = exeAbsolutePath.substr(0, pos + 1);
        }
    }
    return path;
}


inline HKEY TransformKey(const std::string& key) {
    if (key == "HKEY_CLASSES_ROOT") {
        return HKEY_CLASSES_ROOT;
    } else if (key == "HKEY_CURRENT_USER") {
        return HKEY_CURRENT_USER;
    } else if (key == "HKEY_LOCAL_MACHINE") {
        return HKEY_LOCAL_MACHINE;
    } else if (key == "HKEY_USERS") {
        return HKEY_USERS;
    }
    return 0L;
}

std::string ReadRegistry(HKEY rooKey,
                         const char* subKey,
                         const char* valueName,
                         bool isWOW64_32) {
    HKEY hKey;
    char buffer[D_PATHBUFFLENGTH];

    REGSAM accessMask = KEY_QUERY_VALUE;
    if (isWOW64_32) {
        accessMask = accessMask | KEY_WOW64_32KEY;
    }
    LSTATUS res = RegOpenKeyExA(rooKey,
                                subKey,
                                0,
                                accessMask,
                                &hKey);

    if (res == ERROR_SUCCESS) {
        DWORD bufferSize = D_PATHBUFFLENGTH * sizeof(buffer[0]);
        res = RegQueryValueExA(hKey,
                               valueName,
                               NULL,
                               NULL,
                               (LPBYTE)buffer,
                               &bufferSize);
        RegCloseKey(hKey);
        if (res == ERROR_SUCCESS) {
            std::string str = buffer;
            return str;
        }
    }
    return std::string();
}

std::string ReadRegistry(const char* rooKey,
                         const char* subKey,
                         const char* valueName,
                         bool isWOW64_32) {
    return ReadRegistry(TransformKey(rooKey), subKey, valueName, isWOW64_32);
}

inline HKEY TransformKeyW(const std::wstring& key) {
    if (key == L"HKEY_CLASSES_ROOT") {
        return HKEY_CLASSES_ROOT;
    } else if (key == L"HKEY_CURRENT_USER") {
        return HKEY_CURRENT_USER;
    } else if (key == L"HKEY_LOCAL_MACHINE") {
        return HKEY_LOCAL_MACHINE;
    } else if (key == L"HKEY_USERS") {
        return HKEY_USERS;
    }
    return 0L;
}

std::wstring ReadRegistryW(HKEY rooKey,
                           const wchar_t* subKey,
                           const wchar_t* valueName,
                           bool isWOW64_32) {
    HKEY hKey;
    wchar_t buffer[D_PATHBUFFLENGTH];

    REGSAM accessMask = KEY_QUERY_VALUE;
    if (isWOW64_32) {
        accessMask = accessMask | KEY_WOW64_32KEY;
    }
    LSTATUS res = RegOpenKeyExW(rooKey,
                                subKey,
                                0,
                                accessMask,
                                &hKey);

    if (res == ERROR_SUCCESS) {
        DWORD bufferSize = D_PATHBUFFLENGTH * sizeof(buffer[0]);
        res = RegQueryValueExW(hKey,
                               valueName,
                               NULL,
                               NULL,
                               (LPBYTE)buffer,
                               &bufferSize);
        RegCloseKey(hKey);
        if (res == ERROR_SUCCESS) {
            std::wstring str = buffer;
            return str;
        }
    }
    return std::wstring();
}

std::wstring ReadRegistryW(const wchar_t* rooKey,
                           const wchar_t* subKey,
                           const wchar_t* valueName,
                           bool isWOW64_32) {
    return ReadRegistryW(TransformKeyW(rooKey), subKey, valueName, isWOW64_32);
}

void FileCopyTo(char *source, char *dis, char *searchChars, bool bcover /*= true*/) {
    _finddata_t   ffblk;
    char path[256], SourcePath[256], DisPath[256];
    sprintf(path, "%sq_*", source);
    long done = _findfirst(path, &ffblk);
    int find = 0;
    while (find == 0) {
        if (strcmp(ffblk.name, "q_")) {
            sprintf(SourcePath, "%s\\%s", source, ffblk.name);
            sprintf(DisPath, "%s\\%s", dis, ffblk.name);
            ::CopyFileA(SourcePath, DisPath, bcover);
        }
        find = _findnext(done, &ffblk);
    }
    _findclose(done);
}

bool IsProcessExists(const char* name) {
    return GetProcessIDFromName(name) != NULL;
}

bool IsProcessExistsW(const wchar_t* name) {
    return GetProcessIDFromNameW(name) != NULL;
}

DWORD GetProcessIDFromName(const char* name) {
#ifdef UNICODE
    return GetProcessIDFromNameW(Utils::UTF8ToUnicode(name).c_str());
#else
    HANDLE snapshot;
    PROCESSENTRY32 processinfo;
    processinfo.dwSize = sizeof(processinfo);
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != NULL) {
        int status = Process32First(snapshot, &processinfo);
        while (status) {
            if (strcmp(name, processinfo.szExeFile) == 0) {
                return processinfo.th32ProcessID;
            }
            status = Process32Next(snapshot, &processinfo);
        }
    }
    return NULL;
#endif
}

DWORD GetProcessIDFromNameW(const wchar_t* name) {
    HANDLE snapshot;
    PROCESSENTRY32W processinfo;
    processinfo.dwSize = sizeof(processinfo);
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot != NULL) {
        int status = Process32FirstW(snapshot, &processinfo);
        while (status) {
            if (_wcsicmp(name, processinfo.szExeFile) == 0) {
                return processinfo.th32ProcessID;
            }
            status = Process32NextW(snapshot, &processinfo);
        }
    }
    return NULL;
}

bool IsWow64() {
    typedef BOOL(WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
    LPFN_ISWOW64PROCESS fnIsWow64Process;
    BOOL bIsWow64 = FALSE;
    fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandleA("kernel32"), "IsWow64Process");
    if (NULL != fnIsWow64Process) {
        fnIsWow64Process(GetCurrentProcess(), &bIsWow64);
    }
    return bIsWow64 == TRUE;
}

ProcessorArchitecture QuerySystemProcessorArchitecture() {
    typedef void (WINAPI *fn_GetNativeSystemInfo)(LPSYSTEM_INFO);
    fn_GetNativeSystemInfo GetNativeSystemInfo = (fn_GetNativeSystemInfo)GetProcAddress(GetModuleHandleA("kernel32"), "GetNativeSystemInfo");

    SYSTEM_INFO struSysInfo;
    if (GetNativeSystemInfo) {
        GetNativeSystemInfo(&struSysInfo);
    } else {
        GetSystemInfo(&struSysInfo);
    }

    if (PROCESSOR_ARCHITECTURE_AMD64 == struSysInfo.wProcessorArchitecture
        || PROCESSOR_ARCHITECTURE_IA64 == struSysInfo.wProcessorArchitecture) {
        return ProcessorArchitecture::_64_Bit;
    }

    return ProcessorArchitecture::_32Bit;
}

void CreateIfDirectoryNotExists(const char* lpPath) {
    CreateIfDirectoryNotExistsW(Utils::MbcsToUnicode(lpPath).c_str());
}

void CreateIfDirectoryNotExistsW(const wchar_t* lpPath) {
    if (!IsDirectoryExistsW(lpPath)) {
        CreateDirectoryUtilW(lpPath);
    }
}

bool Utils::IsProcessRunningA(const char* processName) {
    return IsProcessRunningW(Utils::UTF8ToUnicode(processName).c_str());
}

bool IsProcessRunningW(const wchar_t* processName) {
    // 给系统内的所有进程拍一个快照
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return 0;
    }
    //遍历进程快照，轮流显示每个进程的信息
    PROCESSENTRY32W pe32;
    pe32.dwSize = sizeof(pe32);
    BOOL hasMore = ::Process32FirstW(hProcessSnap, &pe32);
    while (hasMore) {
        //如果该进程名在快照中存在
        if (_wcsicmp(pe32.szExeFile, processName) == 0) {
            //启动标志位置FALSE  
            return true;
        }
        hasMore = ::Process32NextW(hProcessSnap, &pe32);
    }
    ::CloseHandle(hProcessSnap);
    return false;
}

bool KillProcessByName(const std::wstring& processName) {
    //创建进程快照(TH32CS_SNAPPROCESS表示创建所有进程的快照)
    HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    //PROCESSENTRY32进程快照的结构体
    PROCESSENTRY32 pe;

    //实例化后使用Process32First获取第一个快照的进程前必做的初始化操作
    pe.dwSize = sizeof(PROCESSENTRY32);


    //下面的IF效果同:
    //if(hProcessSnap == INVALID_HANDLE_VALUE)   无效的句柄
    if (!Process32First(hSnapShot, &pe)) {
        return false;
    }

    //如果句柄有效  则一直获取下一个句柄循环下去
    while (Process32Next(hSnapShot, &pe)) {

        //pe.szExeFile获取当前进程的可执行文件名称
        std::wstring tempProcessName = pe.szExeFile;


        //比较当前进程的可执行文件名称和传递进来的文件名称是否相同
        //相同的话Compare返回0
        if (tempProcessName == processName) {
            //从快照进程中获取该进程的PID(即任务管理器中的PID)
            DWORD dwProcessID = pe.th32ProcessID;
            HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessID);
            ::TerminateProcess(hProcess, 0);
            CloseHandle(hProcess);
            return true;
        }
    }
    return false;
}

bool IsForegroundFullscreen() {
    bool bFullscreen = false;//存放当前激活窗口是否是全屏的，true表示是，false表示不是
    HWND hWnd = GetForegroundWindow();//获取当前正在与用户交互的当前激活窗口句柄

    //如果当前激活窗口不是桌面窗口，也不是控制台窗口
    if (hWnd != GetDesktopWindow() && hWnd != GetShellWindow()) {
        RECT rcApp;
        RECT rcDesk;
        GetWindowRect(hWnd, &rcApp);//获取当前激活窗口的坐标
        GetWindowRect(GetDesktopWindow(), &rcDesk);//根据桌面窗口句柄，获取整个屏幕的坐标

        //如果当前激活窗口的坐标完全覆盖住桌面窗口，就表示当前激活窗口是全屏的
        if (rcApp.left <= rcDesk.left &&
            rcApp.top <= rcDesk.top &&
            rcApp.right >= rcDesk.right &&
            rcApp.bottom >= rcDesk.bottom) {

            char szTemp[100];
            //如果获取当前激活窗口的类名成功
            if (GetClassNameA(hWnd, szTemp, sizeof(szTemp)) > 0) {
                //如果不是桌面窗口的类名，就认为当前激活窗口是全屏窗口
                if (strcmp(szTemp, "WorkerW") != 0) {
                    bFullscreen = true;
                }
            } else {
                //如果获取失败，就认为当前激活窗口是全屏窗口
                bFullscreen = true;
            }
        }
    }//如果当前激活窗口是桌面窗口，或者是控制台窗口，就直接返回不是全屏

    return bFullscreen;
}

std::string GetInstallInfomarationByGuid(const std::string& guid,
                                         InstallInfomarationFlag flag,
                                         bool isWow64) {
    char buffer[D_PATHBUFFLENGTH];
    std::string subKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
    subKey = subKey + guid + "\\";
    std::string valueName;
    switch (flag) {
    case InstallInfomarationFlag::InstallLocation:
        valueName = "InstallLocation";
        break;
    case InstallInfomarationFlag::UninstallString:
        valueName = "UninstallString";
        break;
    }
    HKEY hKey;
    auto queryFlag = isWow64 ? KEY_QUERY_VALUE | KEY_WOW64_64KEY : KEY_QUERY_VALUE | KEY_WOW64_32KEY;
    LSTATUS res = RegOpenKeyExA(HKEY_LOCAL_MACHINE,
                               subKey.c_str(),
                               0,
                               queryFlag,
                               &hKey);
    if (res == ERROR_SUCCESS) {
        DWORD bufferSize = D_PATHBUFFLENGTH * sizeof(buffer[0]);
        res = RegQueryValueExA(hKey,
                               valueName.c_str(),
                               NULL,
                               NULL,
                               (LPBYTE)buffer,
                               &bufferSize);
        if (res == ERROR_SUCCESS) {
            return std::string(buffer);
        }
        RegCloseKey(hKey);
    }
    return std::string();
}

std::wstring GetInstallInfomarationByGuidW(const std::wstring& guid,
                                           InstallInfomarationFlag flag,
                                           bool isWow64) {
    wchar_t buffer[D_PATHBUFFLENGTH];
    std::wstring subKey(L"SoftWare\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\");
    subKey += guid;
    std::wstring valueName;
    switch (flag) {
    case InstallInfomarationFlag::InstallLocation:
        valueName = L"InstallLocation";
        break;
    case InstallInfomarationFlag::UninstallString:
        valueName = L"UninstallString";
        break;
    }
    HKEY hKey;
    auto queryFlag = isWow64 ? KEY_QUERY_VALUE | KEY_WOW64_64KEY : KEY_QUERY_VALUE | KEY_WOW64_32KEY;
    LSTATUS res = RegOpenKeyExW(HKEY_LOCAL_MACHINE,
                               subKey.c_str(),
                               0,
                               queryFlag,
                               &hKey);
    if (res == ERROR_SUCCESS) {
        DWORD bufferSize = D_PATHBUFFLENGTH * sizeof(buffer[0]);
        res = RegQueryValueExW(hKey,
                               valueName.c_str(),
                               NULL,
                               NULL,
                               (LPBYTE)buffer,
                               &bufferSize);
        if (res == ERROR_SUCCESS) {
            return std::wstring(buffer);
        }
        RegCloseKey(hKey);
    }
    return std::wstring();
}

std::vector<std::string> GetCommandLine() {
    std::vector<std::string> cmds;
    LPWSTR* argv;
    int argc;
    argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);
    if (argc > 1) {
        for (int i = 0; i < argc; i++) {
            cmds.emplace_back(Utils::UnicodeToMbcs(argv[i]));
        }
    }

    LocalFree(argv);
    return cmds;
}

bool IsFolderEmpty(const char* folder) {
    using namespace std;
    WIN32_FIND_DATAA findData;
    string findPathPointPoint = folder;
    findPathPointPoint += "\\*.*";

    char removeFileName[][256] = {
        "desktop.ini"
    };

    char removeDirName[][256] = {
        "thumb",
        ".",
        ".."
    };

    HANDLE hFile = FindFirstFileA(findPathPointPoint.c_str(), &findData);
    while (hFile != INVALID_HANDLE_VALUE) {
        string fileName(findData.cFileName);
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//如果是目录
        {
            bool useless = false;
            for (int i = 0; i < sizeof(removeDirName) / 256; ++i) {//排除目录

                if (fileName == removeDirName[i]) {
                    useless = true;
                    break;
                }
            }
            if (!useless) {    //递归  搜索
                string temp = folder;
                string recursionFindPath = temp + "\\" + fileName;
                if (IsFolderEmpty(recursionFindPath.c_str())) {
                    FindClose(hFile);
                    return true;
                }
            }

        } else {
            bool useless = false;
            for (int i = 0; i < sizeof(removeFileName) / 256; ++i) {  // 排除文件

                if (fileName == removeFileName[i]) {
                    useless = true;
                    break;
                }
            }
            if (!useless) {
                FindClose(hFile);
                return true;
            }
        }

        if (!FindNextFileA(hFile, &findData)) {    //搜索结束
            FindClose(hFile);
            return false;
        }
    }
    return false;
}


void Utils::SwitchToThisWindow(HWND hwnd, BOOL fAltTab) {
    typedef void (WINAPI *PSWITCHTOTHISWINDOW) (HWND, BOOL);
    PSWITCHTOTHISWINDOW SwitchToThisWindow;
    HMODULE hUser32 = GetModuleHandle(_T("user32"));
    SwitchToThisWindow = (PSWITCHTOTHISWINDOW)GetProcAddress(hUser32, "SwitchToThisWindow");
    SwitchToThisWindow(hwnd, fAltTab);
}

NAMESPACE_DEFINE_END(Utils);
