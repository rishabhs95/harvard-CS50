//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// space between bricks
#define SPACE 5

// velocity in X-axis
#define VX 2.0

// velocity in Y-axis
#define VY 2.0

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // initial horizontal velocity
    double velocityX = VX;
    // initial vertical velocity
    double velocityY = VY;
    
    waitForClick();
        
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        
        // check for mouse event
        GEvent event = getNextEvent(MOUSE_EVENT);
        
        // if we heard one
        if (event != NULL)
        {
            // if the event was movement
            if (getEventType(event) == MOUSE_MOVED)
            {
                // ensure paddle follows top cursor
                double x = getX(event) - WIDTH/10;
                setLocation(paddle, x, 570);
            }
        }
        // move circle along x-axis
        move(ball, velocityX, velocityY);
        
        // bounce off right edge of window
        if (getX(ball) + getWidth(ball) >= getWidth(window))
            velocityX = -velocityX;
            
        // bounce off top edge of window
        if (getY(ball) <= 0)
            velocityY = -velocityY;

        // bounce off left edge of window
        if (getX(ball) <= 0)
            velocityX = -velocityX;
        
        // collision with bottom edge
        if (getY(ball) + getHeight(ball) >= getHeight(window))
        {
            lives--;
            setLocation(ball, HEIGHT/2, WIDTH/2);
            setLocation(paddle, WIDTH/2 - getWidth(paddle)/2, 570);
            velocityX = -VX;
            velocityY = -VY;
            waitForClick();
        }
        
        // detects type of collision
        GObject object = detectCollision(window, ball);
        
        // collision with paddle    
        if (object != NULL)
        {
            if (strcmp(getType(object), "GRect") == 0)
            {
               velocityY = -velocityY;
            
               if (object != paddle)
                {
                    removeGWindow(window, object);
                    points++;   
                    bricks--;
                }
            }
        }       
        // displays Score
        updateScoreboard(window, label, points);
        
        // linger before moving again
        pause(10);
    }

    // You Lose Label Message for kicks.
    if (bricks > 0)
    {
        GLabel final = newGLabel("YOU LOSE!");
        setFont(final, "SansSerif-32");
        setColor(final, "RED");
        add(window, final);
        setLocation(final, getWidth(window)/2-getWidth(final)/2, getHeight(window)/2);
    }
    else
    {
        GLabel final = newGLabel("YOU WIN!");
        setFont(final, "SansSerif-32");
        setColor(final, "GREEN");
        add(window, final);
        setLocation(final, getWidth(window)/2-getWidth(final)/2, getHeight(window)/2);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int i, j, hspace = SPACE, vspace = SPACE + 5;
    for (i=0; i<ROWS; i++)
    {
        hspace = SPACE;
        for (j=0; j<COLS; j++)
        {
            GRect brick = newGRect(j*(WIDTH - 11*SPACE)/10 + hspace, i*10 + vspace, (WIDTH - 11*SPACE)/10, 10);
            if (i == 0)
                setColor(brick, "RED");
            if (i == 1)
                setColor(brick, "ORANGE");
            if (i == 2)
                setColor(brick, "YELLOW");
            if (i == 3)
                setColor(brick, "GREEN");
            if (i == 4)
                setColor(brick, "00FFFF");  //CYAN
            setFilled(brick, true);
            add(window, brick);
            hspace += SPACE;
        }
        vspace += SPACE;
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    // instantiate circle
    GOval circle = newGOval(0, 110, 20, 20);
    setColor(circle, "BLACK");
    setFilled(circle, true);
    add(window, circle);
    return circle;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    GRect paddle = newGRect(WIDTH/2 - WIDTH/10, 570, WIDTH/5, 10);
    setColor(paddle, "BLACK");
    setFilled(paddle, true);
    add(window, paddle);
    return paddle;
                setLocation(paddle, WIDTH/2 - getWidth(paddle)/2, 570);
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    setLocation(label, getHeight(window)/2, getWidth(window)/2);
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
