#ifndef __UI2_H
#define __UI2_H

#include <stdint.h>
#include <stdbool.h>

#define GET_BASE(ui_element, ui_type) ({ \
	BASE_UI base_ui; \
	base_ui.id = ((ui_type*)ui_element)->id; \
	base_ui.format = &((ui_type*)ui_element)->format; \
	base_ui.layout = &((ui_type*)ui_element)->layout; \
	base_ui.select = ((ui_type*)ui_element)->select; \
	base_ui.deselect = ((ui_type*)ui_element)->deselect; \
	base_ui.key_press = ((ui_type*)ui_element)->key_press; \
	base_ui._key_press = ((ui_type*)ui_element)->_key_press; \
	base_ui.update_layout = ((ui_type*)ui_element)->update_layout; \
	base_ui.draw = ((ui_type*)ui_element)->draw; \
	base_ui; \
})

#define NO_COLOR 0xff

// ID translation:
// 00 00 00 00
//          ^^TYPE of ui element

enum UI_ELEMENT_TYPE {
	T_TEXT_UI,
	T_CONTAINER_UI,
	T_LIST_UI,
	T_CHECKBOX_UI,
	T_PROGRESS_UI
};

enum ALIGN {
	A_LEFT,
	A_RIGHT,
	
	A_TOP,
	A_BOTTOM,
	
	A_CENTER,
	A_STRETCH
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

typedef struct tagBASE_UT {
	uint32_t id;
	
	FORMAT* format;
	LAYOUT* layout;

	void (*select)();
	void (*deselect)();
	bool (*key_press)();
	bool (*_key_press)();
	void (*update_layout)();
	void (*draw)();
} BASE_UI;

typedef struct tagTEXT_UI {
	uint32_t id;
	
	FORMAT format;
	LAYOUT layout;
	
	char* text;
	
	void (*select)();
	void (*deselect)();
	bool (*key_press)();
	bool (*_key_press)();
	void (*update_layout)();
	void (*draw)();
} TEXT_UI;

typedef struct tagCONTAINER_UI {
	uint32_t id;
	
	FORMAT format;
	LAYOUT layout;
	
	uint32_t content[10];
	
	void (*select)();
	void (*deselect)();
	bool (*key_press)();
	bool (*_key_press)();
	void (*update_layout)();
	void (*draw)();
} CONTAINER_UI;

typedef struct tagLIST_UI {
	uint32_t id;
	
	FORMAT format;
	LAYOUT layout;
	
	uint32_t content[20];
	uint16_t content_count;
	uint16_t content_padding_v;
	int16_t content_selected;
	
	void (*select)();
	void (*deselect)();
	bool (*key_press)();
	bool (*_key_press)();
	void (*update_layout)();
	void (*draw)();
} LIST_UI;

typedef struct tagCHECKBOX_UI {
	uint32_t id;
	
	FORMAT format;
	LAYOUT layout;
	
	bool checked;
	void (*toggle_check)();
	
	void (*select)();
	void (*deselect)();
	bool (*key_press)();
	bool (*_key_press)();
	void (*update_layout)();
	void (*draw)();
} CHECKBOX_UI;

typedef struct tagPROGRESS_UI {
	uint32_t id;
	
	FORMAT format;
	LAYOUT layout;
	
	int16_t progress;
	
	void (*select)();
	void (*deselect)();
	bool (*key_press)();
	bool (*_key_press)();
	void (*update_layout)();
	void (*draw)();
} PROGRESS_UI;

BASE_UI base_ui(uint32_t ui_element);

void update_layout_generic(LAYOUT *layout);
void align_layout(LAYOUT *layout);
void align_content(LAYOUT *layout, SIZE *size);
void get_element_displacement(uint32_t ui_element, SIZE *size);


void update_layout_text_ui(TEXT_UI *ui_element);
void draw_text_ui(TEXT_UI *ui_element);

void update_layout_container_ui(CONTAINER_UI *ui_element);
void draw_container_ui(CONTAINER_UI *ui_element);

void update_layout_container_ui(CONTAINER_UI *ui_element);
void draw_list_ui(LIST_UI *ui_element);
bool key_press_list_ui(LIST_UI *ui_element, uint16_t key);

void update_layout_checkbox_ui(CHECKBOX_UI *ui_element);
void draw_checkbox_ui(CHECKBOX_UI* ui_element);
bool key_press_checkbox_ui(CHECKBOX_UI* ui_element, uint16_t key);
void toggle_check_checkbox_ui(CHECKBOX_UI* ui_element);

void update_layout_progress_ui(PROGRESS_UI *ui_element);
void draw_progress_ui(PROGRESS_UI *ui_element);

uint32_t init_text_ui(TEXT_UI *ui_element);
uint32_t init_container_ui(CONTAINER_UI *ui_element);
uint32_t init_list_ui(LIST_UI *ui_element);
uint32_t init_checkbox_ui(CHECKBOX_UI *ui_element);
uint32_t init_progress_ui(PROGRESS_UI *ui_element);

void default_pointer();
#endif