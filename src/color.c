#include <stdio.h>
#include <stdlib.h>
#define ANSI_COLOR_RED            "\x1b[31m"
#define ANSI_COLOR_GREEN          "\x1b[32m"
#define ANSI_COLOR_YELLOW         "\x1b[33m"
#define ANSI_COLOR_BLUE           "\x1b[34m"
#define ANSI_COLOR_MAGENTA        "\x1b[35m"
#define ANSI_COLOR_CYAN           "\x1b[36m"
#define ANSI_COLOR_RESET          "\x1b[0m"
#define ANSI_SET_CURSOR_AT_START  "\x1b[0;0H"
	typedef enum{
		defaultColor = 0,
		red,
		green,
		yellow,
		blue
	} color_t;

	void setCursor(){
		printf(ANSI_SET_CURSOR_AT_START);
	}

	void setColor(color_t color){
		if(color == 0) printf(ANSI_COLOR_RESET);
		if(color == 1) printf(ANSI_COLOR_RED);
		if(color == 2) printf(ANSI_COLOR_GREEN);
		if(color == 3) printf(ANSI_COLOR_YELLOW);
		if(color == 4) printf(ANSI_COLOR_BLUE);
	}
		
int main(){
        int i=0;
        while(1){
                char key = 0;
                setCursor();
                printf("\n \n \n \n \n \n \n \n \n");
                if(i==0) setColor(red);
                printf("%-30s" ,"New game \n");
                setColor(defaultColor);
                if(i==1) setColor(red);
                printf("%-30s" , "Options \n");
                setColor(defaultColor);
                if(i==2) setColor(red);
                printf("%-30s" , "Quit \n");
                setColor(defaultColor);
                printf("\n \n \n \n \n \n \n \n \n");
                key = getc(stdin);
                printf("%c", key);
                if((key == 'w') && (i>0)) i--;
                if((key == 's') && (i<2)) i++;
        }
        return 0;
}

	
