#pragma once
#include <litehtml/document_container.h>

class PDF;
class DocumentContainer : public litehtml::document_container
{
public:
	DocumentContainer(PDF* pdf);
	~DocumentContainer() noexcept override;
	virtual litehtml::uint_ptr	create_font(const litehtml::font_description& descr, const litehtml::document* doc, litehtml::font_metrics* fm);
	virtual void				delete_font(litehtml::uint_ptr hFont);
	virtual litehtml::pixel_t   text_width(const char* text, litehtml::uint_ptr hFont);
	virtual void				draw_text(litehtml::uint_ptr hdc, const char* text, litehtml::uint_ptr hFont, litehtml::web_color color, const litehtml::position& pos);
	virtual litehtml::pixel_t   pt_to_px(float pt) const;
	virtual litehtml::pixel_t   get_default_font_size() const;
	virtual const char*         get_default_font_name() const;
	virtual void				draw_list_marker(litehtml::uint_ptr hdc, const litehtml::list_marker& marker);
	virtual void				load_image(const char* src, const char* baseurl, bool redraw_on_ready);
	virtual void				get_image_size(const char* src, const char* baseurl, litehtml::size& sz);
	virtual void				draw_image(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const std::string& url, const std::string& base_url);
	virtual void				draw_solid_fill(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::web_color& color);
	virtual void				draw_linear_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::background_layer::linear_gradient& gradient);
	virtual void				draw_radial_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::background_layer::radial_gradient& gradient);
	virtual void				draw_conic_gradient(litehtml::uint_ptr hdc, const litehtml::background_layer& layer, const litehtml::background_layer::conic_gradient& gradient);
	virtual void				draw_borders(litehtml::uint_ptr hdc, const litehtml::borders& borders, const litehtml::position& draw_pos, bool root);

	virtual	void				set_caption(const char* caption);
	virtual	void				set_base_url(const char* base_url);
	virtual void				link(const std::shared_ptr<litehtml::document>& doc, const litehtml::element::ptr& el);
	virtual void				on_anchor_click(const char* url, const litehtml::element::ptr& el);
	virtual bool				on_element_click(const litehtml::element::ptr& /*el*/) { return false; };
	virtual void				on_mouse_event(const litehtml::element::ptr& el, litehtml::mouse_event event);
	virtual	void				set_cursor(const char* cursor);
	virtual	void				transform_text(litehtml::string& text, litehtml::text_transform tt);
	virtual void				import_css(litehtml::string& text, const litehtml::string& url, litehtml::string& baseurl);
	virtual void				set_clip(const litehtml::position& pos, const litehtml::border_radiuses& bdr_radius);
	virtual void				del_clip();
	virtual void				get_viewport(litehtml::position& viewport) const;
	virtual litehtml::element::ptr	create_element(const char* tag_name,
		const litehtml::string_map& attributes,
		const std::shared_ptr<litehtml::document>& doc);

	virtual void				get_media_features(litehtml::media_features& media) const;
	virtual void				get_language(litehtml::string& language, litehtml::string& culture) const;
	virtual litehtml::string	resolve_color(const litehtml::string& /*color*/) const { return litehtml::string(); }
private:
	PDF* pdf;
};

