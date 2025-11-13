#include <format>
#include <iostream>
#include "DocumentContainer.h"

DocumentContainer::DocumentContainer()
{
	auto& _page = document.GetPages().CreatePage(PoDoFo::PdfPageSize::A4);
	painter.SetCanvas(_page);
	page = &_page;
}

DocumentContainer::~DocumentContainer() noexcept
{
}

void DocumentContainer::finish()
{
	painter.FinishDrawing();

	// Set some additional information on the PDF file.
	document.GetMetadata().SetCreator(PoDoFo::PdfString("HTML2PDF"));
	document.GetMetadata().SetAuthor(PoDoFo::PdfString("HTML2PDF"));
	document.GetMetadata().SetTitle(PoDoFo::PdfString("HTML2PDF"));
	document.GetMetadata().SetSubject(PoDoFo::PdfString("HTML2PDF"));
	document.GetMetadata().SetKeywords(vector<string>({ "HTML2PDF" }));

	// The last step is to close the document.
	document.Save("allen.pdf");
}

litehtml::uint_ptr DocumentContainer::create_font(const litehtml::font_description& descr, const litehtml::document* doc, litehtml::font_metrics* fm)
{
	auto fontPath = std::format("C:\\Windows\\Fonts\\{}.ttf", descr.family);
	auto& f = document.GetFonts().GetOrCreateFont(fontPath);
	auto& metrics = f.GetMetrics();
	fm->font_size = descr.size;
	fm->ascent = metrics.GetAscent();
	fm->descent = metrics.GetDescent();
	fm->height = metrics.GetCapHeight();
	fm->draw_spaces = (descr.decoration_line != litehtml::text_decoration_line_none);
	fm->sub_shift = descr.size / 5;
	fm->super_shift = descr.size / 3;
	painter.TextState.SetFont(f, descr.size);
    return (litehtml::uint_ptr)&f;
}

void DocumentContainer::delete_font(litehtml::uint_ptr hFont)
{

}

pixel_t DocumentContainer::text_width(const char* text, litehtml::uint_ptr hFont)
{
	auto f = (PoDoFo::PdfFont*)hFont;
	auto str = std::string(text);
	std::cout <<  str << "' (length: " << str.length() << ")" << std::endl;
	auto w = f->GetStringLength(str, painter.TextState);
    return static_cast<pixel_t>(w * 1.333f); //w*4.166
}

void DocumentContainer::draw_text(litehtml::uint_ptr hdc, const char* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos)
{
	auto str = std::string(text);
	std::cout << str << std::endl;
	painter.DrawText("222", 56.69, page->GetRect().Height - 56.69);
	int a = 1;
}

pixel_t DocumentContainer::pt_to_px(float pt) const
{
	return static_cast<pixel_t>(pt * 1.333f);
}

pixel_t DocumentContainer::get_default_font_size() const
{
    return 13.f;
}

const char* DocumentContainer::get_default_font_name() const
{
    return "SimHei";
}

void DocumentContainer::draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker)
{
}

void DocumentContainer::load_image(const char* src, const char* baseurl, bool redraw_on_ready)
{
}

void DocumentContainer::get_image_size(const char* src, const char* baseurl, litehtml::size& sz)
{
}

void DocumentContainer::draw_image(litehtml::uint_ptr hdc, const background_layer& layer, const std::string& url, const std::string& base_url)
{
}

void DocumentContainer::draw_solid_fill(litehtml::uint_ptr hdc, const background_layer& layer, const web_color& color)
{
}

void DocumentContainer::draw_linear_gradient(litehtml::uint_ptr hdc, const background_layer& layer, const background_layer::linear_gradient& gradient)
{
}

void DocumentContainer::draw_radial_gradient(litehtml::uint_ptr hdc, const background_layer& layer, const background_layer::radial_gradient& gradient)
{
}

void DocumentContainer::draw_conic_gradient(litehtml::uint_ptr hdc, const background_layer& layer, const background_layer::conic_gradient& gradient)
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
}

void DocumentContainer::del_clip()
{
}

void DocumentContainer::get_viewport(litehtml::position& viewport) const
{
	viewport.width = 1240;
	viewport.height = 1755;
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
	media.width = 1240;
	media.height = 1755;
	media.device_width = 1240;
	media.device_height = 1755;
	media.color = 8;
	media.monochrome = 0;
	media.color_index = 256;
	media.resolution = 96;
}

void DocumentContainer::get_language(litehtml::string& language, litehtml::string& culture) const
{
}
