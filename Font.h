#pragma once
#include <string>
#include <PDFUsedFont.h>

class Font
{
public:
	Font() = default;
	~Font() = default;
public:
	PDFUsedFont* font;
	long size;
	std::string name;
private:

};

