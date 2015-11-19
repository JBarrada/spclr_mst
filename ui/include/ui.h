#ifndef __UI_H_
#define __UI_H_

#include <stdint.h>

//enum ALIGN {
//	align_left,
//	align_right,
//};

#include <ui2.h>

typedef struct tagPOSITION {
	unsigned int x, y;
} POSITION;

//typedef struct tagSIZE {
//	unsigned int width, height;
//} SIZE;

typedef struct tagTEXT {
	POSITION pos;
	uint8_t align;
	uint8_t is_list_item;
	
	char text1[100];
	char text2[100];
	
	uint8_t color;
	uint8_t active;
	
} TEXT;

typedef struct tagITEM {
	POSITION pos;
	SIZE size;
	SIZE selectedsize;
	
	unsigned char bg;
	unsigned char active;
	unsigned char selected;
	unsigned char selectedbg;
	int fg;
	int selectedfg;
	
	TEXT header_texts[2];
	TEXT body_texts[6];
	
} ITEM;

typedef struct tagLIST {
	POSITION pos;
	SIZE size; // height is calculated
	
	unsigned char bg;
	unsigned int vpad;
	unsigned int hpad;
	
	ITEM header; // not affected by padding
	
	ITEM items[6];
	
} LIST;

typedef struct tagMENUITEM {
	POSITION pos;
	SIZE size;
	
	unsigned char fg;
	unsigned char active;
	unsigned char selected;
	unsigned char selectedfg;
	
	TEXT header_text;
	
} MENUITEM;

typedef struct tagMENU {
	POSITION pos;
	SIZE size;
	
	unsigned char bg;
	unsigned int vpad;
	unsigned int hpad;
	
	ITEM header; // not affected by padding
	
	MENUITEM items[10];
	
} MENU;

typedef struct tagCNTRLDISPLAY {
	uint8_t enter_select:1;
	uint8_t arrow_nav:1;
	uint8_t f5_refresh:1;
	uint8_t esc_back:1;
	uint8_t esc_menu:1;
} CNTRLDISPLAY;

void ui_refresh();
void ui_init();
void draw_top_bar(uint8_t swap, CNTRLDISPLAY controls);
void broadcast_msg(char *text, char big, unsigned char wait, unsigned char fg, unsigned char bg);
int text_prompt(char *text, char *buffer, int max_char);
int selection_prompt(char *text, char items[][100], int num_items, int selected);
int menu_page(MENU menu_element, char menu_items[][100], int num_items, CNTRLDISPLAY controls, uint8_t bg, unsigned int selected);

void setup_text(TEXT *text_element, char *text, POSITION pos, uint8_t color, uint8_t align);
void setup_text_list_item(TEXT *text_element, char *text1, char *text2, POSITION pos, uint8_t color, uint8_t align);
void draw_list(LIST *list, uint32_t a_x, uint32_t a_y);
void draw_menu(MENU *menu);

#endif