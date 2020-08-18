/* A quick snake game written in plain C using the ncurses library
 * Author: Pratyaksh Gautam
 * */
#include<ncurses.h>
#include<unistd.h>
#include<stdlib.h>
#include<time.h>

#define DELAY 70000
#define SIZE 1000

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
    /* seed our pseudo-random number generator with time, so that numbers generated aren't the same each time */
    srand(time(NULL));          

    /* Initialize some variables */
    int ch;         /* Store keyboard input */
    long unsigned int ticks = 15;
    long int time;
    int win_max[2];
    int egg_position[2];
    long int score = -10;
    bool still_alive = TRUE, egg_eaten = TRUE;
    char direction = 'r', egg = '@';

    /* Initialize our protagonist, Billy the Snake */
    snek billy;
    billy.length = 1;
    billy.section[0][0] = LINES/4;         /* Center the first section's y-coordinate */
    billy.section[0][0] *= 2;           /* making sure it's even */
    billy.section[0][1] = 4;          /* and it's x coordinate as well. */

    /* Our main 'game loop' */
    while( ( (ch = getch()) != KEY_F(1) ) && still_alive) {
        clear();
        getmaxyx(stdscr, win_max[0], win_max[1]);
        box(stdscr, 0, 0);          /* Draw the screen border */
        ticks--;                    /* Decrement game time* */
        mvprintw(0, 8, "TIME: %ld", (time = ticks/14));
        mvprintw(0, win_max[1] - 20, "SCORE: %ld", score);

        /* Things that have to be done if Billy just ate an egg */
        if(egg_eaten) {
            score += 10;
	    ticks += 5*14;
            billy.length += 1;
            egg_position[0] = (rand() % (win_max[0] - 4)) + 2;    /* Ensures that we don't put the egg */
            egg_position[1] = (rand() % (win_max[1] - 4)) + 2;    /* in a place inaccessible by Billy */
            egg_position[0] = (egg_position[0] / 2) * 2;        /* which we do by ensuring it is */
            egg_position[1] = (egg_position[1] / 2) * 2;        /* divisible by 2 since he moves 2 */
            egg_eaten = FALSE;                                  /* cells per tick in x axis*/
        }

        /* Print the egg */
        mvaddch(egg_position[0], egg_position[1], egg);

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
        if(billy.section[0][0] <= 0 || billy.section[0][0] >= (win_max[0] - 1) || billy.section[0][1] <= 0 || billy.section[0][1] >= (win_max[1] - 1))
            still_alive = FALSE;

        /* Check if Billy crashed into himself */
        for(int i = billy.length - 1; i > 0; i--) {
            if(billy.section[i][0] == billy.section[0][0] && billy.section[i][1] == billy.section[0][1])
                still_alive = FALSE;
        }

	/* Check if Billy ran out of time */
	if(time <= 0)
	    still_alive = FALSE;

        /* Check if Billy ate an egg */
        if(billy.section[0][0] == egg_position[0] && billy.section[0][1] == egg_position[1])
            egg_eaten = TRUE;

    /* End of game loop */
    }
    getmaxyx(stdscr, win_max[0], win_max[1]);
    mvprintw(win_max[0]/2, win_max[1]/2 - 6, "GAME OVER!");
    mvprintw(win_max[0]/2 + 1, win_max[1]/2 - 10, "Your score was: %ld", score);
    refresh();
    sleep(3);
    endwin();
}
