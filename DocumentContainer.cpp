#include <format>
#include "Helper.h"
#include "DocumentContainer.h"
#include "PDF.h"
#include "Font.h"

#include "ActionText.h"
#include "ActionBg.h"
#include "ActionListDot.h"
#include "ActionBorder.h"

DocumentContainer::DocumentContainer(PDF* pdf) :pdf{pdf}
{
}

DocumentContainer::~DocumentContainer() noexcept
{
}

litehtml::uint_ptr DocumentContainer::create_font(const litehtml::font_description& descr, const litehtml::document* doc, litehtml::font_metrics* fm)
{
	//感觉没啥用
	fm->font_size = descr.size;
	fm->ascent = descr.size * 0.8;
	fm->descent = descr.size * 0.2;
	fm->height = descr.size * 1.2;
	fm->draw_spaces = (descr.decoration_line != litehtml::text_decoration_line_none);
	fm->sub_shift = descr.size / 5;
	fm->super_shift = descr.size / 3;
	auto fontPath = std::format("C:\\Windows\\Fonts\\{}.ttc", descr.family);
	auto font = pdf->pdfWriter.GetFontForFile(fontPath);
	auto ptr = new Font(font,descr);
	return (litehtml::uint_ptr)ptr;
}

void DocumentContainer::delete_font(litehtml::uint_ptr hFont)
{
	//auto font = (Font*)hFont;
	//delete font;
}

bool isChinesePunctuation(const std::string& s)
{
	if (s.size() != 3) return false;

	unsigned char b1 = s[0], b2 = s[1], b3 = s[2];

	// 中文全角标点常见范围
	// U+3000 - U+303F : CJK Symbols and Punctuation
	// U+FF00 - U+FFEF : Fullwidth Forms
	return
		(b1 == 0xE3 && b2 == 0x80) ||              // 顿号、句号、书名号等
		(b1 == 0xEF && b2 == 0xBC) ||              // 全角逗号、冒号、问号、括号等
		(b1 == 0xEF && b2 == 0xBD);                // 其他全角符号
}

litehtml::pixel_t DocumentContainer::text_width(const char* text, litehtml::uint_ptr hFont)
{
	auto str = std::string(text);
	auto font = (Font*)hFont;
	if (str == " ") {
		PDFUsedFont::TextMeasures textDimensions = font->font->CalculateTextDimensions("A", font->size);
		return textDimensions.width;
	}
	else if (isChinesePunctuation(str)) {
		PDFUsedFont::TextMeasures textDimensions = font->font->CalculateTextDimensions(str, font->size);
		return textDimensions.width + 8.f;
	}
	PDFUsedFont::TextMeasures textDimensions = font->font->CalculateTextDimensions(str, font->size);
	return textDimensions.width+1.6f;//加字符间距
}

void DocumentContainer::draw_text(litehtml::uint_ptr hdc, const char* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos)
{
	auto font = (Font*)hFont;
	if (pos.y + font->size > clipY + clipH || pos.x > clipX + clipW) return;

	auto action = new ActionText(std::string(text),(Font*)hFont);
	action->color = (color.red << 16) | (color.green << 8) | (color.blue);
	action->pageIndex = (long)pos.y / pdf->viewHeight;
	action->x = pdf->edge + pos.x;
	auto y = pos.y - pdf->viewHeight * action->pageIndex;
	action->y = pdf->edge + (pdf->viewHeight - y) - font->size;
	pdf->actions.push_back(action);
}

litehtml::pixel_t DocumentContainer::pt_to_px(float pt) const
{
	return static_cast<litehtml::pixel_t>(pt * 1.333f);
}

litehtml::pixel_t DocumentContainer::get_default_font_size() const
{
    return 13.f;
}

const char* DocumentContainer::get_default_font_name() const
{
    //return "SimHei";
	return "msyh";
}

void DocumentContainer::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker)
{
	//pdf->drawListMarker(marker);
	if (marker.marker_type == litehtml::list_style_type_none) return;
	if (marker.pos.y > clipY + clipH || marker.pos.x > clipX + clipW) return;
	auto action = new ActionListDot();
	action->dotType = marker.marker_type;
	action->size = marker.pos.height/2;
	action->color = (marker.color.red << 16) | (marker.color.green << 8) | (marker.color.blue);
	action->pageIndex = (long)marker.pos.y / pdf->viewHeight;
	action->x = pdf->edge + marker.pos.x;
	auto font = (Font*)marker.font;
	action->y = pdf->height - pdf->edge - (marker.pos.y - action->pageIndex * pdf->viewHeight) + marker.pos.height - font->size * 2 / 3;
	pdf->actions.push_back(action);
}

void DocumentContainer::load_image(const char* src, const char* baseurl, bool redraw_on_ready)
{
}

void DocumentContainer::get_image_size(const char* src, const char* baseurl, litehtml::size& sz)
{
}

void DocumentContainer::draw_image(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const std::string& url, const std::string& base_url)
{
}

void DocumentContainer::draw_solid_fill(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::web_color& color)
{
	if (layer.border_box.y > clipY + clipH || layer.border_box.x > clipX + clipW) return;
	auto x = pdf->edge + layer.border_box.x;
	if (x < clipX && clipX != INT_MAX) x = clipX;
	auto ySrc = layer.border_box.y;
	if (ySrc < clipY && clipY != INT_MAX) ySrc = clipY;
	auto hSrc = layer.border_box.height;
	if (hSrc > clipH && clipH != INT_MAX) hSrc = clipH;
	auto w = layer.border_box.width;
	if (w > clipW && clipW != INT_MAX) w = clipW;
	auto pageIndex = (long)ySrc / pdf->viewHeight;
	unsigned long colorVal = (color.red << 16) | (color.green << 8) | (color.blue);
	auto flag = ySrc + hSrc > (pageIndex + 1) * pdf->viewHeight;
	if (!flag) {
		auto action = new ActionBg();
		action->pageIndex = pageIndex;
		action->color = colorVal;
		action->w = w;
		action->x = x;
		action->h = hSrc;
		action->y = pdf->edge + (pdf->viewHeight - ySrc - hSrc);
		pdf->actions.push_back(action);
		return;
	}

	while (flag) {
		auto action = new ActionBg();
		action->pageIndex = pageIndex;
		action->color = colorVal;
		action->w = w;
		action->x = x;
		action->h = (pageIndex + 1) * pdf->viewHeight - ySrc;
		action->y = pdf->edge;
		pdf->actions.push_back(action);
		hSrc = hSrc - action->h;
		ySrc = ySrc + action->h;
		pageIndex = (long)ySrc / pdf->viewHeight;
		flag = ySrc + hSrc > (pageIndex + 1) * pdf->viewHeight;
	}
	auto action = new ActionBg();
	action->pageIndex = pageIndex;
	action->color = colorVal;
	action->w = w;
	action->x = x;
	action->h = hSrc;
	action->y = pdf->edge + (pdf->viewHeight - hSrc);
	pdf->actions.push_back(action);
}

void DocumentContainer::draw_linear_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::background_layer::linear_gradient& gradient)
{
}

void DocumentContainer::draw_radial_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::background_layer::radial_gradient& gradient)
{
}

void DocumentContainer::draw_conic_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::background_layer::conic_gradient& gradient)
{
}

void DocumentContainer::draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root)
{
}

void DocumentContainer::set_caption(const char* caption)
{
}

void DocumentContainer::set_base_url(const char* base_url)
{
}

void DocumentContainer::link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el)
{
}

void DocumentContainer::on_anchor_click(const char* url, const litehtml::element::ptr& el)
{
}

void DocumentContainer::on_mouse_event(const litehtml::element::ptr& el, litehtml::mouse_event event)
{
}

void DocumentContainer::set_cursor(const char* cursor)
{
}

void DocumentContainer::transform_text(litehtml::string& text, litehtml::text_transform tt)
{
}

void DocumentContainer::import_css(litehtml::string& text, const litehtml::string& url, litehtml::string& baseurl)
{
}

void DocumentContainer::set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius)
{
	clipX = pos.x;
	clipY = pos.y;
	clipW = pos.width;
	clipH = pos.height;
}

void DocumentContainer::del_clip()
{
	clipX = INT_MAX;
	clipY = INT_MAX;
	clipW = INT_MAX;
	clipH = INT_MAX;
}

void DocumentContainer::get_viewport(litehtml::position& viewport) const
{
	viewport.width = pdf->viewWidth;
	viewport.height = pdf->viewHeight;
	viewport.x = 0;
	viewport.y = 0;
}

litehtml::element::ptr DocumentContainer::create_element(const char* tag_name, const litehtml::string_map& attributes, const std::shared_ptr<litehtml::document>& doc)
{
    return nullptr;
}

void DocumentContainer::get_media_features(litehtml::media_features& media) const
{
	media.type = litehtml::media_type_screen;
	media.width = pdf->viewWidth;
	media.height = pdf->viewHeight;
	media.device_width = pdf->viewWidth;
	media.device_height = pdf->viewHeight;
	media.color = 8;
	media.monochrome = 0;
	media.color_index = 256;
	media.resolution = 96;
}

void DocumentContainer::get_language(litehtml::string& language, litehtml::string& culture) const
{
}
