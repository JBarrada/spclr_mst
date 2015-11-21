#include <ui2.h>
#include <stdint.h>
#include <stdbool.h>

#include <gfx.h>
#include <text.h>
#include <io.h>
#include <string.h>
#include <keyboard.h>

#include <ui.h>

#include "image/uiglyphs/check_checked.h"
#include "image/uiglyphs/check_unchecked.h"

uint32_t id_counter = 0;

BASE_UI base_ui(uint32_t ui_element) {
	switch (((uint32_t*)ui_element)[0] & 0xFF) {
		case T_TEXT_UI:
			return GET_BASE(ui_element, TEXT_UI);
			break;	
		case T_CONTAINER_UI:
			return GET_BASE(ui_element, CONTAINER_UI);
			break;
		case T_LIST_UI:
			return GET_BASE(ui_element, LIST_UI);
			break;
		case T_CHECKBOX_UI:
			return GET_BASE(ui_element, CHECKBOX_UI);
			break;
		case T_PROGRESS_UI:
			return GET_BASE(ui_element, PROGRESS_UI);
			break;
	}
}

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
		
		case A_STRETCH:
			layout->layout_left = layout->layout_space.left + layout->margin.left;
			layout->width = (WIDTH_W-layout->layout_space.right)-layout->layout_space.left - layout->margin.left - layout->margin.right;
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
			
		case A_STRETCH:
			layout->layout_top = layout->layout_space.top + layout->margin.top;
			layout->height = (HEIGHT_W-layout->layout_space.bottom)-layout->layout_space.top - layout->margin.top - layout->margin.bottom;
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

void get_element_displacement(uint32_t ui_element, SIZE *size) {
	base_ui(ui_element).update_layout(ui_element);
	LAYOUT* layout = base_ui(ui_element).layout;
	
	size->width = (layout->layout_align_h == A_STRETCH)?0:(layout->width + layout->margin.left + layout->margin.right);
	size->height = (layout->layout_align_v == A_STRETCH)?0:(layout->height + layout->margin.top + layout->margin.bottom);
}


void update_layout_text_ui(TEXT_UI *ui_element) {
	SIZE size;
	get_text_spread(ui_element->text, ui_element->format.text_size, &size);
	ui_element->layout.content_width = size.width;
	ui_element->layout.content_height = size.height;
	
	update_layout_generic(&ui_element->layout);
}
void draw_text_ui(TEXT_UI *ui_element) {
	ui_element->update_layout(ui_element);
	
	if (ui_element->format.bg != NO_COLOR)
		rectangle_filled(ui_element->layout.layout_left, ui_element->layout.layout_top, ui_element->layout.width, ui_element->layout.height, ui_element->format.bg);

	SIZE size;
	align_content(&ui_element->layout, &size);

	gfx_putstr(size.width, size.height, ui_element->text, ui_element->format.fg, ui_element->format.text_size);
}

void update_layout_container_ui(CONTAINER_UI *ui_element) {
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

	for (i=0; i<10; i++) {
		if (ui_element->content[i] != 0) {
			LAYOUT* layout = base_ui(ui_element->content[i]).layout;
			layout->layout_space.left = ui_element->layout.layout_left + ui_element->layout.padding.left;
			layout->layout_space.right = WIDTH_W - (ui_element->layout.layout_left + ui_element->layout.width) + ui_element->layout.padding.left;
			layout->layout_space.top = ui_element->layout.layout_top + ui_element->layout.padding.top;
			layout->layout_space.bottom = HEIGHT_W - (ui_element->layout.layout_top + ui_element->layout.height) + ui_element->layout.padding.bottom;
		}
	}
}
void draw_container_ui(CONTAINER_UI *ui_element) {
	ui_element->update_layout(ui_element);
	
	if (ui_element->format.bg != NO_COLOR)
		rectangle_filled(ui_element->layout.layout_left, ui_element->layout.layout_top, ui_element->layout.width, ui_element->layout.height, ui_element->format.bg);
	
	int i;
	for (i=0; i<10; i++) {
		if (ui_element->content[i] != 0) 
			base_ui(ui_element->content[i]).draw(ui_element->content[i]);
	}
}

void update_layout_list_ui(LIST_UI *ui_element) {
	ui_element->layout.content_height = 0;
	ui_element->layout.content_width = 0;
	ui_element->content_count = 0;
	
	int i;
	for (i=0; i<20; i++) {
		if (ui_element->content[i] != 0) {
			SIZE size = {0,0};
			get_element_displacement(ui_element->content[i], &size);
			ui_element->layout.content_height += size.height;
			if (size.width > ui_element->layout.content_width)
				ui_element->layout.content_width = size.width;
			ui_element->content_count++;
		}
	}
	
	if (ui_element->content_count)
		ui_element->layout.content_height += (ui_element->content_count-1)*ui_element->content_padding_v;
	
	update_layout_generic(&ui_element->layout);
	
	int16_t top_offset=0;
	for (i=0; i<20; i++) {
		if (ui_element->content[i] != 0) {
			LAYOUT* layout = base_ui(ui_element->content[i]).layout;
			layout->layout_space.left = ui_element->layout.layout_left + ui_element->layout.padding.left;
			layout->layout_space.right = WIDTH_W - (ui_element->layout.layout_left + ui_element->layout.width) + ui_element->layout.padding.left;
			layout->layout_space.top = ui_element->layout.layout_top + ui_element->layout.padding.top + top_offset;
			layout->layout_space.bottom = HEIGHT_W - (ui_element->layout.layout_top + ui_element->layout.height) + ui_element->layout.padding.bottom;
			
			top_offset += layout->height + layout->margin.top + layout->margin.bottom + ui_element->content_padding_v;
		}
	}
}
void draw_list_ui(LIST_UI *ui_element) {
	ui_element->update_layout(ui_element);
	
	if (ui_element->format.bg != NO_COLOR)
		rectangle_filled(ui_element->layout.layout_left, ui_element->layout.layout_top, ui_element->layout.width, ui_element->layout.height, ui_element->format.bg);
	
	int i;
	for (i=0; i<20; i++) {
		if (ui_element->content[i] != 0) 
			base_ui(ui_element->content[i]).draw(ui_element->content[i]);
	}
}
bool key_press_list_ui(LIST_UI *ui_element, uint16_t key) {
	bool handled = ui_element->key_press(ui_element, key);
	if (handled == false) {
		switch (key) {
			case DOWN_PRESSED:
			case RIGHT_PRESSED:
				ui_element->content_selected++; handled=true; break;
			case UP_PRESSED:
			case LEFT_PRESSED:
				ui_element->content_selected--; handled=true; break;
		}
		
		ui_element->content_selected = (ui_element->content_selected<0)?ui_element->content_count-1:ui_element->content_selected;
		ui_element->content_selected %= ui_element->content_count;
		
		
		int i;
		for (i=0; i<20; i++) {
			if (ui_element->content[i] != 0) {
				if (i == ui_element->content_selected)
					base_ui(ui_element->content[i]).select(ui_element->content[i]);
				else
					base_ui(ui_element->content[i]).deselect(ui_element->content[i]);
			}
		}
	}
	
	if (handled == false)
		handled = base_ui(ui_element->content[ui_element->content_selected])._key_press(ui_element->content[ui_element->content_selected], key);
	
	return handled;
}

void update_layout_checkbox_ui(CHECKBOX_UI *ui_element) {
	ui_element->layout.content_width = (ui_element->checked)?check_checked_width:check_unchecked_width;
	ui_element->layout.content_height = (ui_element->checked)?check_checked_height:check_unchecked_height;
	
	update_layout_generic(&ui_element->layout);
}
void draw_checkbox_ui(CHECKBOX_UI* ui_element) {
	ui_element->update_layout(ui_element);
	
	if (ui_element->format.bg != NO_COLOR)
		rectangle_filled(ui_element->layout.layout_left, ui_element->layout.layout_top, ui_element->layout.width, ui_element->layout.height, ui_element->format.bg);

	SIZE size;
	align_content(&ui_element->layout, &size);
	
	draw_glyph(size.width, size.height, ui_element->layout.content_width, ui_element->layout.content_height, (ui_element->checked)?check_checked_data:check_unchecked_data, ui_element->format.fg, 1);
}
bool key_press_checkbox_ui(CHECKBOX_UI* ui_element, uint16_t key) {
	bool handled = ui_element->key_press(ui_element, key);
	if (handled == false) {
		switch (key) {
			case SPACE_PRESSED:
				ui_element->toggle_check(ui_element);
				handled = true;
				break;
		}
	}
	return handled;
}
void toggle_check_checkbox_ui(CHECKBOX_UI* ui_element) {
	ui_element->checked = !ui_element->checked;
}

void update_layout_progress_ui(PROGRESS_UI *ui_element) {
	update_layout_generic(&ui_element->layout);
}
void draw_progress_ui(PROGRESS_UI *ui_element) {
	ui_element->update_layout(ui_element);
	
	ui_element->layout.content_width = ui_element->layout.width - ui_element->layout.padding.left - ui_element->layout.padding.right;
	ui_element->layout.content_height = ui_element->layout.height - ui_element->layout.padding.top - ui_element->layout.padding.bottom;
	
	if (ui_element->progress < 0)
		ui_element->progress = 0;
	if (ui_element->progress > 100)
		ui_element->progress = 100;
	
	
	if (ui_element->format.bg != NO_COLOR)
		rectangle_filled(ui_element->layout.layout_left, ui_element->layout.layout_top, ui_element->layout.width, ui_element->layout.height, ui_element->format.bg);
	
	float progress_pixels = (((float)ui_element->layout.content_width)/100.0)*(float)ui_element->progress;
	rectangle_filled(ui_element->layout.layout_left + ui_element->layout.padding.left, ui_element->layout.layout_top + ui_element->layout.padding.top, (int16_t)progress_pixels, ui_element->layout.content_height, ui_element->format.fg);
}

uint32_t init_text_ui(TEXT_UI *ui_element) {
	memset(ui_element, 0, sizeof(TEXT_UI));
	ui_element->id = (id_counter++ << 8) | (T_TEXT_UI & 0xFF);
	
	ui_element->select = default_pointer;
	ui_element->deselect = default_pointer;
	ui_element->key_press = default_pointer;
	ui_element->_key_press = default_pointer;
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
	
	ui_element->select = default_pointer;
	ui_element->deselect = default_pointer;
	ui_element->key_press = default_pointer;
	ui_element->_key_press = default_pointer;
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
uint32_t init_list_ui(LIST_UI *ui_element) {
	memset(ui_element, 0, sizeof(LIST_UI));
	ui_element->id = (id_counter++ << 8) | (T_LIST_UI & 0xFF);
	
	ui_element->select = default_pointer;
	ui_element->deselect = default_pointer;
	ui_element->key_press = default_pointer;
	ui_element->_key_press = key_press_list_ui;
	ui_element->update_layout = update_layout_list_ui;
	ui_element->draw = draw_list_ui;
	
	ui_element->content_padding_v = 5;
	
	ui_element->format.fg = 1;
	ui_element->format.bg = 4;
	
	ui_element->layout.w_auto = true;
	ui_element->layout.h_auto = true;
	
	ui_element->layout.padding = (THICKNESS) {5,5,5,5};
	
	ui_element->layout.layout_align_h = A_LEFT;
	ui_element->layout.layout_align_v = A_TOP;
	
	ui_element->layout.content_align_h = A_LEFT;
	ui_element->layout.content_align_v = A_TOP;
	
	return ui_element->id;
}
uint32_t init_checkbox_ui(CHECKBOX_UI *ui_element) {
	memset(ui_element, 0, sizeof(CHECKBOX_UI));
	ui_element->id = (id_counter++ << 8) | (T_CHECKBOX_UI & 0xFF);
	
	ui_element->select = default_pointer;
	ui_element->deselect = default_pointer;
	ui_element->key_press = default_pointer;
	ui_element->_key_press = key_press_checkbox_ui;
	ui_element->update_layout = update_layout_checkbox_ui;
	ui_element->draw = draw_checkbox_ui;
	
	ui_element->toggle_check = toggle_check_checkbox_ui;
	
	ui_element->checked = false;
	
	ui_element->format.fg = 1;
	ui_element->format.bg = 4;
	
	ui_element->layout.w_auto = true;
	ui_element->layout.h_auto = true;
	
	ui_element->layout.padding = (THICKNESS) {5,5,5,5};
	
	ui_element->layout.layout_align_h = A_LEFT;
	ui_element->layout.layout_align_v = A_TOP;
	
	ui_element->layout.content_align_h = A_LEFT;
	ui_element->layout.content_align_v = A_TOP;
	
	return ui_element->id;
}
uint32_t init_progress_ui(PROGRESS_UI *ui_element) {
	memset(ui_element, 0, sizeof(PROGRESS_UI));
	ui_element->id = (id_counter++ << 8) | (T_PROGRESS_UI & 0xFF);
	
	ui_element->select = default_pointer;
	ui_element->deselect = default_pointer;
	ui_element->key_press = default_pointer;
	ui_element->_key_press = default_pointer;
	ui_element->update_layout = update_layout_progress_ui;
	ui_element->draw = draw_progress_ui;

	ui_element->format.fg = 2;
	ui_element->format.bg = 4;
	
	ui_element->layout.w_auto = true;
	ui_element->layout.h_auto = false;
	
	ui_element->layout.height = 20;
	
	ui_element->layout.padding = (THICKNESS) {2,2,2,2};
	
	ui_element->layout.layout_align_h = A_STRETCH;
	ui_element->layout.layout_align_v = A_TOP;
	
	ui_element->layout.content_align_h = A_LEFT;
	ui_element->layout.content_align_v = A_TOP;
	
	return ui_element->id;
}

void default_pointer() {}
