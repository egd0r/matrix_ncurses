//Maybe have a bounce value array given by runtime calc?
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <ncurses.h>

#define MAX_DY 7
#define DENSITY 60
#define RADIUS 4

typedef struct CHAR_STRUCT {
    //Opacity?
    char ch;
    int x;
    int y;
    int dy;
    struct CHAR_STRUCT *next;
    struct CHAR_STRUCT *prev;
} char_n;

char_n *player;

int maxX, maxY;
char_n *head;

void add_linked(char_n **head, char_n *toAdd) {
    char_n *temp=*head, *prev=NULL;
    for (; temp; temp=temp->next) prev=temp;
    if (!prev) *head = toAdd;
    else {
        prev->next = toAdd;
        toAdd->prev = prev;
    }
}


char_n *remove_linked(char_n *toRemove) {
    char_n *ret=NULL;
    if (toRemove) {
        if (toRemove->prev) toRemove->prev->next=toRemove->next;
        if (toRemove->next) {
            toRemove->next->prev=toRemove->prev;
            ret=toRemove->next;
        }
        return ret;
    } 
}

char_n *create_letter() {
    //Generate random letter between 33 and 126 inc
    //Allocate space and return heaped
    char_n *ret = (char_n *)malloc(sizeof(char_n));
    ret->ch = (rand()% (126-33))+33;
    ret->x = rand()%maxX; //obvs depends on screen size
    ret->y = 0;
    ret->dy = (rand()%MAX_DY)+2;
    //enqueue(ret);
    add_linked(&head, ret);

    return ret;
}

int abs(int num) {
    if (num*-1>=0)
        return num*-1;
}

void move_letter(char_n *letter) {
    mvaddch(letter->y, letter->x, letter->ch);
}

void move_letters() {
    char_n *temp = head, *next;
    //Clear
    for (; temp; temp=next) {
        next=temp->next;
        if (!(abs(player->x-temp->x)<RADIUS && abs(player->y-temp->y)<(RADIUS/2)))
            move_letter(temp);
        temp->y+=temp->dy;
        if (temp->y > maxY)
            next=remove_linked(temp);
    }
}

int main() {
    srand(time(0));
    int ch;
    initscr();           /* Start curses mode        */
    getmaxyx(stdscr, maxY, maxX);
    raw();              /* Line buffering disabled  */
    keypad(stdscr, false);       /* We get F1, F2 etc..      */
    noecho();           /* Don't echo() while we do getch */
    nodelay(stdscr, true);
    curs_set(0);
    player = (char_n *)malloc(sizeof(char_n)-2*sizeof(char_n *)-sizeof(int)); //Gotta save that heap space
    player->x=0;player->y=0;player->ch='E'; //Best letter

    //Game loop
    int state=1;
    while (state) {
        clear();
        for (int i=0; i<DENSITY; i++) {
            create_letter();
        }
        usleep(30000);
        move_letters();
        char enter = getch();

        switch(enter) {
            case 'w':
                player->y--;
                break;
            case 'a':
                player->x--;
                break;
            case 's':
                player->y++;
                break;
            case 'd':
                player->x++;
                break;
            case 'q':
                state=0;
                break;
        }
        move_letter(player);
        refresh();
    }

    
    endwin();           /* End curses mode         */

    return 0;

}
