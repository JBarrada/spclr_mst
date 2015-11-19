#include <ui.h>
#include <io.h>
#include <gfx.h>
#include <font.h>
#include <stdint.h>
#include <string.h>
#include <keyboard.h>
#include "image/spclr_logo.h"

#include "image/cntrldisplay/enter_select.h"
#include "image/cntrldisplay/arrow_nav.h"
#include "image/cntrldisplay/f5_refresh.h"
#include "image/cntrldisplay/esc_back.h"
#include "image/cntrldisplay/esc_menu.h"

void draw_controls(CNTRLDISPLAY controls) {
	uint32_t x_pos = WIDTH_W;
	if (controls.enter_select) {
		x_pos -= enter_select_width + 5;
		draw_glyph(x_pos, 5, enter_select_width, enter_select_height, enter_select_bitmap, 0, 1);
	}
	if (controls.arrow_nav) {
		x_pos -= arrow_nav_width + 5;
		draw_glyph(x_pos, 5, arrow_nav_width, arrow_nav_height, arrow_nav_bitmap, 0, 1);
	}
	if (controls.f5_refresh) {
		x_pos -= f5_refresh_width + 5;
		draw_glyph(x_pos, 5, f5_refresh_width, f5_refresh_height, f5_refresh_bitmap, 0, 1);
	}
	if (controls.esc_back) {
		x_pos -= esc_back_width + 5;
		draw_glyph(x_pos, 5, esc_back_width, esc_back_height, esc_back_bitmap, 0, 1);
	}
	if (controls.esc_menu) {
		x_pos -= esc_menu_width + 5;
		draw_glyph(x_pos, 5, esc_menu_width, esc_menu_height, esc_menu_bitmap, 0, 1);
	}
}

void broadcast_msg(char *text, char big, unsigned char wait, unsigned char fg, unsigned char bg) {
	int text_width = (big)?get_str_width_big(text):get_str_width(text);
	int text_height = (big)?25:16;
	
	uint32_t absolute_x = (WIDTH_W/2) - (text_width/2) - 10;
	uint32_t absolute_y = (HEIGHT_W/2) - (text_height/2) - 10;
	
	ashes_palette_dim();
	
	rectangle_filled(absolute_x, absolute_y, text_width + (2*10), text_height + (2*10), bg);
	
	if (big)
		putstr_big(absolute_x + 10, absolute_y + 10, text, fg);
	else
		putstr(absolute_x + 10, absolute_y + 10, text, fg);
	
	swap_buffers();
	
	if (wait) {
		uint16_t key;
		while (1) {
			key = get_key();
			switch (key) {
				case ENTER_PRESSED:
				case ESC_PRESSED:
					ashes_palette();
					return;
			}
		}
	}
}

void draw_selection_prompt(char *text, char items[][100], int num_items, unsigned int selected) {
	uint32_t prompt_px_width = get_str_width(text);

	int i, item_width, selection_width = 0;
	for (i=0; i<num_items; i++) {
		item_width = get_str_width_big(items[i]);
		selection_width = (item_width+10>selection_width)?item_width+10:selection_width;
	}
	
	int box_height = 10 + 16 + 10 + 27 + 10;
	int box_width = ((prompt_px_width+20)>((selection_width*num_items)+10+(10*num_items)))?(prompt_px_width+20):((selection_width*num_items)+10+(10*num_items));
	
	uint32_t absolute_x = (WIDTH_W/2) - (box_width/2);
	uint32_t absolute_y = (HEIGHT_W/2) - (box_height/2);
	
	rectangle_filled(absolute_x, absolute_y, box_width, box_height, 4+8);

	putstr((WIDTH_W/2) - (prompt_px_width/2) , absolute_y + 10, text, 1+8);
	
	absolute_x += (box_width/2) - (((selection_width*num_items)+10+(10*num_items))/2) + 10;
	for (i=0; i<num_items; i++) {
		item_width = get_str_width_big(items[i]);
		if (i==selected)
			rectangle_filled(absolute_x, absolute_y + 26 + 10, selection_width, 27, 1+8);
		putstr_big(absolute_x + (selection_width/2) - (item_width/2), absolute_y + 26 + 10 + 1, items[i], (i==selected)?4+8:3+8);
		absolute_x += 10 + selection_width;
	}

	swap_buffers();
}
int selection_prompt(char *text, char items[][100], int num_items, int selected) {
	capture();
	ashes_palette_dim();
	
	char key;
	while (1) {
		restore();
		draw_selection_prompt(text, items, num_items, selected);

		key = get_key();
		switch (key) {
			case DOWN_PRESSED:
			case RIGHT_PRESSED:
				selected++; break;
			case UP_PRESSED:
			case LEFT_PRESSED:
				selected--; break;
			
			case ENTER_PRESSED:
				restore();
				ashes_palette();
				return selected;
			case ESC_PRESSED:
				restore();
				ashes_palette();
				return -1;
		}
		selected %= num_items;
		selected = (selected<0)?num_items-1:selected;
	}

}

void draw_text_prompt(char *text, char *buffer, int max_char) {
	int buf_loc = strlen(buffer);
	
	char prompt[100];
	memset(prompt, 0 ,100);
	sstring(prompt, "%s (%d:%d)", text, buf_loc, max_char);
	
	uint32_t prompt_px_width = get_str_width(prompt);
	uint32_t buffer_px_width = get_str_width_big(buffer);
	
	int box_height = 10 + 16 + 10 + 27 + 10;
	int box_width = ((prompt_px_width+20)>(buffer_px_width+20+10+6))?(prompt_px_width+20):(buffer_px_width+20+10+6);
	
	uint32_t absolute_x = (WIDTH_W/2) - (box_width/2);
	uint32_t absolute_y = (HEIGHT_W/2) - (box_height/2);
	
	rectangle_filled(absolute_x, absolute_y, box_width, box_height, 3+8);
	rectangle_filled(absolute_x+10, absolute_y+26 + 10, box_width - 20, 27, 4+8);
		
	putstr((WIDTH_W/2) - (prompt_px_width/2) , absolute_y + 10, prompt, 1+8);
	
	putstr_big(absolute_x+14, absolute_y+26 + 10 + 1, buffer, 1+8);
	putstr_big(absolute_x+14 + buffer_px_width, absolute_y+26 + 10 + 1, "_", 2+8);
	
	swap_buffers();
}
int text_prompt(char *text, char *buffer, int max_char) {
	capture();
	ashes_palette_dim();

	int buf_loc = strlen(buffer);
	while (1) {
		restore();
		draw_text_prompt(text, buffer, max_char);
		
		char current_key = get_char();
		switch (current_key) {
			case '\0':
				continue;
			case '\n':
				buffer[buf_loc] = '\0';
				restore();
				ashes_palette();
				return 1;
			case '\x1b':
				restore();
				ashes_palette();
				return 0;
			case '\b':
				if (buf_loc > 0) buffer[--buf_loc] = '\0';
				break;
			default:
				if (buf_loc<max_char) buffer[buf_loc++] = current_key;
				break;
		}
	}
}

int menu_page(MENU menu_element, char menu_items[][100], int num_items, CNTRLDISPLAY controls, uint8_t bg, unsigned int selected) {
	memset(&menu_element, 0, sizeof(MENU));
	menu_element.bg = bg;
	menu_element.vpad = menu_element.hpad = 10;
	
	int i;
	for (i=0; i<num_items; i++) {
		MENUITEM *item = &menu_element.items[i];
		item->size.height = 25;
		item->fg = 1;
		item->selectedfg = 2;
		item->active = 1;
		item->selected = (i==selected)?1:0;
		setup_text(&item->header_text, menu_items[i], (POSITION){0,0}, 1, 0);
	}

	uint16_t key;
	while (1) {
		draw_top_bar(0, controls);
		draw_menu(&menu_element);
		swap_buffers();
		
		key = get_key();
		switch (key&0xff) {
			case DOWN_PRESSED:
			case RIGHT_PRESSED:
				selected++; break;
			case UP_PRESSED:
			case LEFT_PRESSED:
				selected--; break;
			case ENTER_PRESSED:
				return selected;
			case ESC_PRESSED:
				if (controls.esc_back || controls.esc_menu)
					return -1;
				break;
			
			default:
				continue;
		}
		selected = (selected<0)?num_items-1:selected;
		selected %= num_items;
		for (i=0;i<num_items;i++)
			menu_element.items[i].selected = (i==selected);
	}
}

void draw_top_bar(uint8_t swap, CNTRLDISPLAY controls) {
	rectangle_filled(0, 0, 640, 30, 1);
	rectangle_filled(0, 30, 640, 480-30, 0);
	draw_glyph(3, 7, spclr_logo_width, spclr_logo_height, spclr_logo, 0, 0);
	
	draw_controls(controls);
	
	if (swap) swap_buffers();
}

int draw_text(TEXT *text_element, uint32_t a_x, uint32_t a_y) {
	int str1_width = get_str_width(text_element->text1);
	int str2_width = get_str_width(text_element->text2);
	
	int text1_x = (text_element->align==1)?(WIDTH_W-text_element->pos.x-str1_width):(a_x+text_element->pos.x);
	int text2_x = WIDTH_W-text_element->pos.x-str2_width;
	int text_y = a_y+text_element->pos.y;
	
	putstr(text1_x, text_y, text_element->text1, text_element->color);
	
	if (text_element->is_list_item) {
		putstr(text2_x, text_y, text_element->text2, text_element->color);
		line(text1_x+str1_width+4, text_y+9, text2_x-6, text_y+9, text_element->color);
	}

	return (text_element->pos.y + 16);
}

int draw_text_big(TEXT *text_element, uint32_t a_x, uint32_t a_y) {
	int str1_width = get_str_width_big(text_element->text1);
	int str2_width = get_str_width_big(text_element->text2);
	
	int text1_x = (text_element->align==1)?(WIDTH_W-text_element->pos.x-str1_width):(a_x+text_element->pos.x);
	int text2_x = WIDTH_W-text_element->pos.x-str2_width;
	int text_y = a_y+text_element->pos.y;
	
	putstr_big(text1_x, text_y, text_element->text1, text_element->color);
	
	if (text_element->is_list_item) {
		putstr_big(text2_x, text_y, text_element->text2, text_element->color);
		line(text1_x+str1_width+6, text_y+13, text2_x-8, text_y+13, text_element->color);
		line(text1_x+str1_width+6, text_y+14, text2_x-8, text_y+14, text_element->color);
	}

	return (text_element->pos.y + 16);
}

unsigned int get_item_height(ITEM *item) {
	if (item->active)
		return (item->selected)? item->selectedsize.height:item->size.height;
	else
		return 0;
}
unsigned int get_item_displacement(ITEM *item) {
	if (item->active)
		return item->pos.y + ((item->selected)? item->selectedsize.height:item->size.height);
	else
		return 0;
}

int draw_item(ITEM *item, uint32_t a_x, uint32_t a_y) {
	int i;
	
	uint32_t absolute_x = a_x + item->pos.x;
	uint32_t absolute_y = a_y + item->pos.y;
	
	rectangle_filled(absolute_x, absolute_y, item->size.width, get_item_height(item), (item->selected)?item->selectedbg:item->bg);
	
	for (i=0; i<2; i++) {
		if (item->header_texts[i].active)
			if (item->fg != -1)
				item->header_texts[i].color = (item->selected)?item->selectedfg:item->fg;

			draw_text(&item->header_texts[i], absolute_x, absolute_y);
	}
	
	if (item->selected) {
		for (i=0; i<6; i++) {
			if (item->body_texts[i].active)
				draw_text(&item->body_texts[i], absolute_x, absolute_y);
		}
	}
	return get_item_displacement(item);
}

void draw_list(LIST *list, uint32_t a_x, uint32_t a_y) {
	int i;
	
	list->size.height = list->vpad + get_item_displacement(&list->header);
	for (i=0; i<6; i++) 
		list->size.height += get_item_displacement(&list->items[i]) + ((list->items[i].active)?list->vpad:0);
	
	uint32_t absolute_x = a_x + list->pos.x;
	uint32_t absolute_y = a_y + list->pos.y;
	rectangle_filled(absolute_x, absolute_y, list->size.width, list->size.height, list->bg);
	
	if (list->header.active)
		absolute_y += draw_item(&list->header, absolute_x, absolute_y);
	
	absolute_x += list->hpad;
	absolute_y += list->vpad;
	for (i=0; i<6; i++) {
		if (list->items[i].active)
			absolute_y += list->vpad + draw_item(&list->items[i], absolute_x, absolute_y);
	}
}


int draw_menuitem(MENUITEM *item, uint32_t a_x, uint32_t a_y) {
	uint32_t absolute_x = a_x + item->pos.x;
	uint32_t absolute_y = a_y + item->pos.y;
	
	item->header_text.color = (item->selected)?item->selectedfg:item->fg;

	draw_text_big(&item->header_text, absolute_x, absolute_y);
		
	return item->pos.y + item->size.height;
}

void draw_menu(MENU *menu) {
	int i;

	uint32_t height = menu->vpad;
	uint32_t width=0, text_width;
	for (i=0; i<10; i++) {
		if (menu->items[i].active) {
			height += menu->vpad + menu->items[i].size.height;
			text_width = get_str_width_big(menu->items[i].header_text.text1);
			if ((text_width + (2*menu->hpad)) > width) {
				width = text_width + (2*menu->hpad);
			} 
		}
	}
	
	uint32_t absolute_x = menu->pos.x + (WIDTH_W/2) - (width/2);
	uint32_t absolute_y = menu->pos.y + (HEIGHT_W/2) - (height/2);
	rectangle_filled(absolute_x, absolute_y, width, height, menu->bg);
	
	if (menu->header.active)
		absolute_y += draw_item(&menu->header, absolute_x, absolute_y);
	
	absolute_x += menu->hpad;
	absolute_y += menu->vpad;
	for (i=0; i<10; i++) {
		if (menu->items[i].active)
			absolute_y += menu->vpad + draw_menuitem(&menu->items[i], absolute_x, absolute_y);
	}
}

void setup_text(TEXT *text_element, char *text, POSITION pos, uint8_t color, uint8_t align) {
	text_element->pos = pos;
	text_element->align = align;
	text_element->is_list_item = 0;
	
	memset(text_element->text1, 0, 100);
	memcpy(text_element->text1, text, strlen(text));
	
	text_element->color = color;
	text_element->active = 1;
}

void setup_text_list_item(TEXT *text_element, char *text1, char *text2, POSITION pos, uint8_t color, uint8_t align) {
	text_element->pos = pos;
	text_element->align = align;
	text_element->is_list_item = 1;
	
	memset(text_element->text1, 0, 100);
	memcpy(text_element->text1, text1, strlen(text1));
	memset(text_element->text2, 0, 100);
	memcpy(text_element->text2, text2, strlen(text2));
	
	text_element->color = color;
	text_element->active = 1;
}

void ui_refresh() {

}

void ui_init() {

}
