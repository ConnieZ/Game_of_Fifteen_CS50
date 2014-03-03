/**
 * fifteen.c
 *
 * Computer Science 50
 * Problem Set 3
 *
 * Implements the Game of Fifteen (generalized to d x d).
 *
 * Usage: ./fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [MIN,MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */
 
#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// board's minimal dimension
#define MIN 3

// board's maximal dimension
#define MAX 9

// board, whereby board[i][j] represents row i and column j
int board[MAX][MAX];

// board's dimension
int d;

// number representing quantity of tiles on the board for use in functions and iterating through
int reverse_order;

// variables storing row and column of blank tile
int blank_row;
int blank_col;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);
void save(void);

int main(int argc, string argv[])
{
    // greet player
    greet();

    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: ./fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < MIN || d > MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            MIN, MIN, MAX, MAX);
        return 2;
    }

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // saves the current state of the board (for testing)
        save();

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = GetInt();

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep for animation's sake
        usleep(500000);
    }

    // that's all folks
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1,
 * (i.e., fills board with values but does not actually print them),
 * whereby board[i][j] represents row i and column j.
 */
void init(void)
{
    reverse_order = d*d - 1;
    // fill the board with numbers
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            board[i][j] = reverse_order;
            reverse_order --;
            //the empty tile place will be represented by zero
        }
    }
    // if the board has an odd # of tiles, we will swap 1 and 2
    if (d%2 == 0)
    {
        board[d-1][d-2] = 2;
        board[d-1][d-3] = 1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    // print the tiles of the board
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            //the empty tile place will be recognized as zero and printed as underscore
            if (board[i][j] == 0)
            {
                if (d > 3)
                    printf("__");
                else 
                    printf(" _");
                blank_row = i;
                blank_col = j;
                printf(" ");
            }    
            else if (board[i][j] < 10)
            {
                printf("%2d", board[i][j]);
                printf(" ");
            }
            else
            {
                printf("%d", board[i][j]);
                printf(" ");
            }   
        }
        printf("\n");
    } 
}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false. 
 */
bool move(int tile)
{
    // first we check if the tile is actually on the board and is greater than 0
    if (tile < d*d && tile > 0)
    {
        // implement linear search through the array to find the tile's location and move if found
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                if (board[i][j] == tile)
                {
                    // we found the tile, what's its location?
                    if (i == blank_row && j == blank_col - 1)
                    {
                        //it's to the left of the blank tile
                        board[blank_row][blank_col] = tile;
                        board[i][j] = 0;
                        break;
                    }
                    else if (i == blank_row && j == blank_col + 1)
                    {
                        //it's to the right of the blank tile
                        board[blank_row][blank_col] = tile;
                        board[i][j] = 0;
                        break;
                    }
                    else if (i == blank_row - 1 && j == blank_col)
                    {    
                        //it's on top of the blank tile
                        board[blank_row][blank_col] = tile;
                        board[i][j] = 0;
                        break;
                    }
                    else if (i == blank_row + 1 && j == blank_col)
                    {
                        //it's below the blank tile
                        board[blank_row][blank_col] = tile;
                        board[i][j] = 0;
                        break;
                    }
                    else
                        //an illegal move
                        break;
                }
            }
        }
        //checking if any moves were made, if none - return false to ask for a legal move
        if (board[blank_row][blank_col] != 0)
            return true;
        else
            return false;    
    }
    // the tile is not on the board, return false to ask for a legal move
    else
        return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration), 
 * else false.
 */
bool won(void)
{
    reverse_order = d*d - 1;
    bool checker;
    // iterating over the board to check if the tiles are in correct order
    for (int i = 0; i < d; i++)
    {
        for (int j = 0; j < d; j++)
        {
            if (board[i][j] == reverse_order - (reverse_order-1))
            {    
                reverse_order --;
                checker = true;
            }
            else
            {
                checker = false;
                break;
            }
        }
    }
    if (checker == true)
        return true;
    else
        return false;
}

/**
 * Saves the current state of the board to disk (for testing).
 */
void save(void)
{
    // log
    const string log = "log.txt";

    // delete existing log, if any, before first save
    static bool saved = false;
    if (!saved)
    {
        unlink(log);
        saved = true;
    }

    // open log
    FILE* p = fopen(log, "a");
    if (p == NULL)
    {
        return;
    }

    // log board
    fprintf(p, "{");
    for (int i = 0; i < d; i++)
    {
        fprintf(p, "{");
        for (int j = 0; j < d; j++)
        {
            fprintf(p, "%i", board[i][j]);
            if (j < d - 1)
            {
                fprintf(p, ",");
            }
        }
        fprintf(p, "}");
        if (i < d - 1)
        {
            fprintf(p, ",");
        }
    }
    fprintf(p, "}\n");

    // close log
    fclose(p);
}
