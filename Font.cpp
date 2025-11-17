
#include <format>
#include "Font.h"

Font::Font(PDFUsedFont* font,const litehtml::font_description& descr) :font{font},size { (long)descr.size }, weight{ descr.weight }
{
}

Font::~Font()
{
	delete font;
}
