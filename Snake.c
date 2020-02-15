#include<ncurses.h>
#include<unistd.h>

#define DELAY 70000
#define SIZE 100

typedef struct snake{
    int section[SIZE][2];
    int length;
} snek;

int main()
{
    /* Initialise ncurses mode */
    initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(FALSE);

    /* Initialize some variables */
    int ch;         /* Store keyboard input */
    long unsigned int ticks = 0;
    int time, max_y, max_x;
    bool still_alive = TRUE;
    char direction = 'u';

    /* Initialize our protagonist, Billy the Snake */
    snek billy;
    billy.length = 1;
    billy.section[0][0] = LINES/2;         /* Center the first section's y-coordinate */
    billy.section[0][1] = COLS/2;          /* and it's x coordinate as well. */

    /* Our main 'game loop' */
    while( ( (ch = getch()) != KEY_F(1) ) && still_alive) {
        clear();
        getmaxyx(stdscr, max_y, max_x);
        box(stdscr, 0, 0);          /* Draw the screen border */
        ticks++;                    /* Increment game time* */
        mvprintw(1, 3, "Time: %d", (time = ticks/10));
        billy.length = time;
        /* Display Billy's current position in the map */
        for(int i = 0; i < billy.length; i++) {
            mvaddch(billy.section[i][0], billy.section[i][1], 'O'); 
        }

        /* Update the screen */
        refresh(); 
        usleep(DELAY);

        /* Billy's body sections will follow his head */
        for(int i = billy.length - 1; i > 0; i--) {
            billy.section[i][0] = billy.section[i-1][0];
            billy.section[i][1] = billy.section[i-1][1];
        }
      
        /* Update the direction Billy is travelling in */
        switch(ch){
            case KEY_DOWN:      if(direction != 'u')    direction = 'd';    break;
            case KEY_UP:        if(direction != 'd')    direction = 'u';    break;
            case KEY_LEFT:      if(direction != 'r')    direction = 'l';    break;
            case KEY_RIGHT:     if(direction != 'l')    direction = 'r';    break;
        }

        /* Update the coordinate's of Billy's head */
        switch(direction){
            case 'u':       billy.section[0][0]--;  break;
            case 'd':       billy.section[0][0]++;  break;
            case 'l':       billy.section[0][1]-=2;  break;
            case 'r':       billy.section[0][1]+=2;  break;
        }

        /* Check if Billy crashed into the walls */
        if(billy.section[0][0] <= 0 || billy.section[0][0] >= max_y || billy.section[0][1] <= 0 || billy.section[0][1] >= max_x)
            still_alive = FALSE;

    /* Check if Billy crashed into himself */
        for(int i = billy.length - 1; i > 0; i--) {
            if(billy.section[i][0] == billy.section[0][0] && billy.section[i][1] == billy.section[0][1])
                still_alive = FALSE;
        }
    }
    mvprintw(3, 10, "GAME OVER!");
    sleep(1);
    endwin();
}
