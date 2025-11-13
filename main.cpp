#include <iostream>
#include <string>
#include <litehtml/document.h>
#include <podofo/podofo.h>
#include <Windows.h>
#include "DocumentContainer.h"



int wmain(int argc, wchar_t* argv[])
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    std::wcout.imbue(std::locale("zh_CN.UTF-8"));
    std::wcout << L"Unicode控制台程序" << std::endl;
    for (int i = 0; i < argc; i++)
    {
        std::wcout << L"参数 " << i << L": " << argv[i] << std::endl;
    }


    std::stringstream ss;
    std::ifstream("demo2.html", std::ios::binary) >> ss.rdbuf();
    auto html = ss.str();
    auto docContainer = new  DocumentContainer();
    auto doc = litehtml::document::createFromString(html, docContainer);
    int best_width = doc->render(1240);
    litehtml::position clip(0, 0, 1240, 1755);
    doc->draw((litehtml::uint_ptr)nullptr, 0, 0, &clip);
    docContainer->finish();
    //794 96dpi
    //1240 150dpi
    //2480 300dpi
    //int best_width = doc->render(1240);
    //doc->render(best_width);
    //doc->render(1240);

    //HelloWorld("allen.pdf");


    std::wstring str;
    std::wcin >> str;
    return 0;
}