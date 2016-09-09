#include <stdlib.h>
#include <ncurses.h>
#include <panel.h>


int pause_menu(void) {
	WINDOW* window;
	PANEL* panel;
	char key;
	int position = 0;
	int end = 0;

	window = newwin(15, 40, 15, 60);
	box(window, 0, 0);
	panel = new_panel(window);
	top_panel(panel);

	mvwprintw(window, 3, 3, "%s", "Resume");
	mvwprintw(window, 5, 3, "%s", "Exit");

	while(!end) {
		mvwprintw(window, position * 2 + 3, 2, "%s", "*");
		
		update_panels();
		doupdate();
		key = getch();
		mvwprintw(window, position * 2 + 3, 2, "%s", " ");
		switch(key) {
				case 'j':
						if(position < 1) position++;
						break;
				case 'k':
						if(position > 0) position--;
						break;
				case '\n':
						end = 1;
						/*if(position == 0) return 0;*/
						/*if(position == 1) return 1;*/
						break;
		}
	}


	del_panel(panel);
	delwin(window);

	update_panels();
	doupdate();
	
	return position;
}
