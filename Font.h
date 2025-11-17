#pragma once
#include <string>
#include <PDFUsedFont.h>
#include <litehtml/document.h>

class PDF;
class Font
{
public:
	Font(PDFUsedFont* font,const litehtml::font_description& descr);
	~Font();
public:
	PDFUsedFont* font;
	long size;
	int weight;
	float half;
private:

};

