#include <ui2.h>
#include <stdint.h>
#include <stdbool.h>

#include <gfx.h>
#include <text.h>
#include <io.h>
#include <string.h>

#include <ui.h>

uint32_t id_counter = 0;

void update_layout_generic(LAYOUT *layout) {
	if (layout->w_auto)
		layout->width = layout->content_width + layout->padding.left + layout->padding.right;
	if (layout->h_auto)
		layout->height = layout->content_height + layout->padding.top + layout->padding.bottom;
	
	align_layout(layout);
}

void align_layout(LAYOUT *layout) {
	switch (layout->layout_align_h) {
		case A_LEFT:
			layout->layout_left = layout->layout_space.left + layout->margin.left;
			break;
		case A_CENTER:
			layout->layout_left = layout->layout_space.left + (((WIDTH_W-layout->layout_space.right)-layout->layout_space.left)/2) - (layout->width/2);
			break;
		case A_RIGHT:
			layout->layout_left = WIDTH_W - layout->layout_space.right - layout->margin.right - layout->width;
			break;
	}
	
	switch (layout->layout_align_v) {
		case A_TOP:
			layout->layout_top = layout->layout_space.top + layout->margin.top;
			break;
		case A_CENTER:
			layout->layout_top = layout->layout_space.top + (((HEIGHT_W-layout->layout_space.bottom)-layout->layout_space.top)/2) - (layout->height/2);
			break;
		case A_BOTTOM:
			layout->layout_top = HEIGHT_W - layout->layout_space.bottom - layout->margin.bottom - layout->height;
			break;
	}
}

void align_content(LAYOUT *layout, SIZE *size) {
	switch (layout->content_align_h) {
		case A_LEFT:
			size->width = layout->layout_left + layout->padding.left;
			break;
		case A_CENTER:
			size->width = layout->layout_left + (layout->width/2) - (layout->content_width/2);
			break;
		case A_RIGHT:
			size->width = layout->layout_left + layout->width - layout->padding.right - layout->content_width;
			break;
	}
	
	switch (layout->content_align_v) {
		case A_TOP:
			size->height = layout->layout_top + layout->padding.top;
			break;
		case A_CENTER:
			size->height = layout->layout_top + (layout->height/2) - (layout->content_height/2);
			break;
		case A_BOTTOM:
			size->height =layout->layout_top + layout->height - layout->padding.bottom - layout->content_height;
			break;
	}
}

LAYOUT* get_element_layout(uint32_t ui_element, bool update) {
	switch (((uint32_t*)ui_element)[0] & 0xFF) {
		case T_TEXT_UI:
			if (update) ((TEXT_UI*)ui_element)->update_layout(ui_element);
			return &((TEXT_UI*)ui_element)->layout;
		case T_CONTAINER_UI: 
			if (update) ((CONTAINER_UI*)ui_element)->update_layout(ui_element);
			return &((CONTAINER_UI*)ui_element)->layout;
	}
}

void draw_element(uint32_t ui_element) {
	switch (((uint32_t*)ui_element)[0] & 0xFF) {
		case T_TEXT_UI:
			((TEXT_UI*)ui_element)->draw(ui_element);
			break;	
		case T_CONTAINER_UI:
			((CONTAINER_UI*)ui_element)->draw(ui_element);
			break;
	}
}

void get_element_displacement(uint32_t ui_element, SIZE *size) {
	LAYOUT* layout = get_element_layout(ui_element, true);
	
	size->width = layout->width + layout->margin.left + layout->margin.right;
	size->height = layout->height + layout->margin.top + layout->margin.bottom;
}

void update_layout_text_ui(TEXT_UI *ui_element) {
	SIZE size;
	get_text_spread(ui_element->text, ui_element->format.text_size, &size);
	ui_element->layout.content_width = size.width;
	ui_element->layout.content_height = size.height;
	
	update_layout_generic(&ui_element->layout);
}

void update_layout_container_ui(CONTAINER_UI *ui_element) {
	// update_layout of all contents
	int i;
	for (i=0; i<10; i++) {
		if (ui_element->content[i] != 0) {
			SIZE size = {0,0};
			get_element_displacement(ui_element->content[i], &size);
			if (size.width > ui_element->layout.content_width)
				ui_element->layout.content_width = size.width;
			if (size.height > ui_element->layout.content_height)
				ui_element->layout.content_height = size.height;
		}
	}
	
	
	update_layout_generic(&ui_element->layout);
	
	// re-space content
	for (i=0; i<10; i++) {
		if (ui_element->content[i] != 0) {
			LAYOUT* layout = get_element_layout(ui_element->content[i], false);
			layout->layout_space.left = ui_element->layout.layout_left + ui_element->layout.padding.left;
			layout->layout_space.right = WIDTH_W - (ui_element->layout.layout_left + ui_element->layout.width) + ui_element->layout.padding.left;
			layout->layout_space.top = ui_element->layout.layout_top + ui_element->layout.padding.top;
			layout->layout_space.right = HEIGHT_W - (ui_element->layout.layout_top + ui_element->layout.height) + ui_element->layout.padding.bottom;
		}
	}
}

void draw_text_ui(TEXT_UI *ui_element) {
	ui_element->update_layout(ui_element); // might not be needed
	
	if (ui_element->format.bg != NO_COLOR)
		rectangle_filled(ui_element->layout.layout_left, ui_element->layout.layout_top, ui_element->layout.width, ui_element->layout.height, ui_element->format.bg);

	SIZE size;
	align_content(&ui_element->layout, &size);

	gfx_putstr(size.width, size.height, ui_element->text, ui_element->format.fg, ui_element->format.text_size);
}

void draw_container_ui(CONTAINER_UI *ui_element) {
	ui_element->update_layout(ui_element);
	
	if (ui_element->format.bg != NO_COLOR)
		rectangle_filled(ui_element->layout.layout_left, ui_element->layout.layout_top, ui_element->layout.width, ui_element->layout.height, ui_element->format.bg);
	
	int i;
	for (i=0; i<10; i++) {
		if (ui_element->content[i] != 0) 
			draw_element(ui_element->content[i]);
	}
}

uint32_t init_text_ui(TEXT_UI *ui_element) {
	memset(ui_element, 0, sizeof(TEXT_UI));
	ui_element->id = (id_counter++ << 8) | (T_TEXT_UI & 0xFF);
	ui_element->update_layout = update_layout_text_ui;
	ui_element->draw = draw_text_ui;
	
	ui_element->format.fg = 1;
	ui_element->format.bg = NO_COLOR;
	
	ui_element->layout.w_auto = true;
	ui_element->layout.h_auto = true;
	
	ui_element->layout.layout_align_h = A_LEFT;
	ui_element->layout.layout_align_v = A_TOP;
	
	ui_element->layout.content_align_h = A_LEFT;
	ui_element->layout.content_align_v = A_TOP;
	
	return ui_element->id;
}

uint32_t init_container_ui(CONTAINER_UI *ui_element) {
	memset(ui_element, 0, sizeof(CONTAINER_UI));
	ui_element->id = (id_counter++ << 8) | (T_CONTAINER_UI & 0xFF);
	ui_element->update_layout = update_layout_container_ui;
	ui_element->draw = draw_container_ui;
	
	ui_element->format.fg = 1;
	ui_element->format.bg = 4;
	
	ui_element->layout.w_auto = true;
	ui_element->layout.h_auto = true;
	
	ui_element->layout.layout_align_h = A_LEFT;
	ui_element->layout.layout_align_v = A_TOP;
	
	ui_element->layout.content_align_h = A_LEFT;
	ui_element->layout.content_align_v = A_TOP;
	
	return ui_element->id;
}
