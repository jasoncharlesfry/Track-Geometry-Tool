#ifndef MENU_H
#define MENU_H

#include "Switch.h"
#include "ST7735.h"
#include <stdint.h>

#define MENU_ENTRY()
#define NAME_LEN 42
#define SET_SIZE 42
#define MAX_CHILDREN 42

enum menu_type{list_menu, value_menu};

struct menu_elem {
	enum menu_type type;
};

struct list_menu{
	struct menu_elem elem;
	void *children[MAX_CHILDREN];
	void (*up_handler)(void);
	void (*down_handler)(void);
	void (*left_handler)(void);
	void (*right_handler)(void);
	void (*enter_handler)(void);
};
	
struct value_menu{
	struct menu_elem elem;
	uint32_t values[MAX_CHILDREN];
	void (*up_handler)(void);
	void (*down_handler)(void);
	void (*left_handler)(void);
	void (*right_handler)(void);
	void (*enter_handler)(void);
};

struct item{
	char name[NAME_LEN];
	uint32_t value;
};


struct item_menu{
	struct menu_elem elem;
	struct item sets[MAX_CHILDREN][SET_SIZE];
	void (*up_handler)(void);
	void (*down_handler)(void);
	void (*left_handler)(void);
	void (*right_handler)(void);
	void (*enter_handler)(void);
};

void Menu_init(void);

void Menu_addChild(void *parent, void *child);
void Menu_removeChild(void *parent, void *child);
void Menu_setCurrent(void *menu);
void Menu_show(void);
void Menu_refresh(void);


#endif
