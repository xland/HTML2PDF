#include <iostream>
#include <chrono>
#include <string>
#include <Windows.h>
#include "PDF.h"

std::string wstringToString(const std::wstring& wstr)
{
    LPCWSTR pwszSrc = wstr.c_str();
    int nLen = WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, NULL, 0, NULL, NULL);
    if (nLen == 0) return(std::string(""));

    char* pszDst = new char[nLen];
    if (!pszDst) return(std::string(""));

    WideCharToMultiByte(CP_ACP, 0, pwszSrc, -1, pszDst, nLen, NULL, NULL);
    std::string str(pszDst);
    delete[] pszDst;
    pszDst = NULL;
    return(str);
}

int wmain(int argc, wchar_t* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::wcout.imbue(std::locale("zh_CN.UTF-8"));
    if (argc < 2) {
        std::wcout << L"请输入要处理的HTML文件路径，支持相对目录" << std::endl;
    }
    std::wstring htmlPath{ argv[1] };
    auto pathStr = wstringToString(htmlPath);

    auto pdf = std::make_unique<PDF>();
    auto start = std::chrono::high_resolution_clock::now();
	pdf->start(pathStr);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duration = end - start;
    std::wcout << L"程序运行时间: " << duration.count() << L" 毫秒" << std::endl;
    return 0;
}