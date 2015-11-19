#ifndef __UI2_H
#define __UI2_H

#include <stdint.h>
#include <stdbool.h>

#define NO_COLOR 0xff

// ID translation:
// 00 00 00 00
//          ^^TYPE of ui element

enum UI_ELEMENT_TYPE {
	T_TEXT_UI,
	T_CONTAINER_UI
};

enum ALIGN {
	A_LEFT,
	A_RIGHT,
	
	A_TOP,
	A_BOTTOM,
	
	A_CENTER
};

enum TEXT_SIZE {
	TS_BIG,
	TS_DEFAULT,
	TS_SMALL,
};

typedef struct tagSIZE {
	int16_t width;
	int16_t height;
} SIZE;

typedef struct tagTHICKNESS {
	int16_t left;
	int16_t right;
	int16_t top;
	int16_t bottom;
} THICKNESS;

typedef struct tagFORMAT {
	uint8_t fg;
	uint8_t bg;
	
	uint8_t fg_s;
	uint8_t bg_s;
	
	enum TEXT_SIZE text_size;
} FORMAT;

typedef struct tagLAYOUT {
	int16_t width;
	int16_t height;
	bool w_auto;
	bool h_auto;
	
	int16_t layout_left;
	int16_t layout_top;
	
	THICKNESS layout_space;
	
	THICKNESS padding;
	THICKNESS margin;
	
	enum ALIGN layout_align_h;
	enum ALIGN layout_align_v;
	
	enum ALIGN content_align_h;
	enum ALIGN content_align_v;
	
	int16_t content_width;
	int16_t content_height;
	
	int z_index;
} LAYOUT;


typedef struct tagTEXT_UI {
	uint32_t id;
	
	FORMAT format;
	LAYOUT layout;
	
	char* text;
	
	void (*update_layout)();
	void (*draw)();
} TEXT_UI;

typedef struct tagCONTAINER_UI {
	uint32_t id;
	
	FORMAT format;
	LAYOUT layout;
	
	uint32_t content[10];
	
	void (*update_layout)();
	void (*draw)();
} CONTAINER_UI;


void update_layout_generic(LAYOUT *layout);
void align_layout(LAYOUT *layout);
void align_content(LAYOUT *layout, SIZE *size);
LAYOUT* get_element_layout(uint32_t ui_element, bool update);
void draw_element(uint32_t element);
void get_element_displacement(uint32_t ui_element, SIZE *size);

void update_layout_text_ui(TEXT_UI *ui_element);
void update_layout_container_ui(CONTAINER_UI *ui_element);

void draw_text_ui(TEXT_UI *ui_element);
void draw_container_ui(CONTAINER_UI *ui_element);

uint32_t init_text_ui(TEXT_UI *ui_element);
uint32_t init_container_ui(CONTAINER_UI *ui_element);

#endif