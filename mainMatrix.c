#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<ncurses.h>
#include<pthread.h>

//Linked list
// 0 -> 1 -> 2 -> 3 -> 4 (arrIndex/columnRef)
// ptrs to strings here
// pointer to next element

typedef struct intArrMAS {
    //We just need to store the pointer to the initialised array duh
    int arrIndex;
    char *arrList;
    struct intArrMAS *next;
} intArr_t;

void * keyListener (void *argf) {
    usleep(10000);
    char *cptr = (char *)argf;
    *cptr = getch();

    return NULL;
}

int main() {
    intArr_t *head = NULL;
    //Init
    initscr();
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    //One thread listening and one thread printing
    pthread_t keyListenThread;
    //Mallocing space for user input character
    char *userC = (char *)malloc(sizeof(char)); 

    //Screen initialisation
    keypad(stdscr, TRUE);
    
    //Welcome text and instructions after initialisation
    // -> Add later
    //Listens for first character
    
    //Listen and print
    intArr_t *temp = head;
    int print_enable = 0;
    for (int i=0; i<cols; i++) {
        pthread_create(&keyListenThread, NULL, keyListener, userC);
        //Allocate some memory for this string
        char *mallocedSect = (char *)malloc(sizeof(char)*rows);
        pthread_join(keyListenThread, NULL);
        if (33 <= *userC <= 126) {

            if (print_enable==0) print_enable=1;

            char *mallocTemp = mallocedSect;
            for (int q=0; q<rows; q++) {
                //Store the qth int in the qth section of memory - deref - this works as expected
                *mallocTemp = (char)*userC;
                //Increment memory pointer
                mvprintw(q, i+10, "%p", mallocTemp);
                refresh();
                mvprintw(q, i, "%c", *userC);
                refresh();
                mallocTemp++;
            }
            //We add pointer to linked list so it can be disposed of
            intArr_t *newElem = (intArr_t *)malloc(sizeof(intArr_t));

            newElem->arrIndex = i;
            newElem->arrList = mallocedSect;
            //Will just print mallocedSect
            mvprintw(0,42,"%p", newElem->arrList);
            
            
            if (!temp) {
                temp=newElem;
            } else {
                temp->next=newElem;
            }

            /*
            We can have a function to start the process of continuously
            printing each letter of the array. We could do this by incrementing
            set values being printed to the next pointer or continuously print
            the array incrementing the index. 
            */


        } else {
            printf("Invalid character\n");
            //Make sure not to skip a column!
            i--;
        }
        //Sort print enable here, we can use temp here too to cut on runtime
    }
    //Now at a position where we have linked list, things should be printed now since we have sorted print_enable

    //Wait... aren't all list printing processes going to have to be threaded? Is that even possible?
    refresh();
    /*
    int q = 10;
    for (; head; head=head->next) {
       mvprintw(10, q++, "%p, index:%d\n", head->arrList, head->arrIndex);
       refresh();
    }
    */
    getch();
    endwin();

    

    return 0;
}