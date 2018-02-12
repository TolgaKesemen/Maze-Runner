#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

struct
{
    char data;
    int marked;
} maze[30][30];                                                     /* array of maze */

int i = 0, j = 0;                                                   /* borders of maze */
char keys[25];                                                      /* array of keys */
char *key = keys;

int size = 1;                                                       /* size of stack */
char *stack;                                                        /* pointer of stack */
int top = -1;                                                       /* top of stack */

int mazeRunner(int, int);
void unmarkAll();
int isfull();
int isempty();
void push(char);
char pop();


/* This programs takes one input file as text format and in it there should be a maze like defined in the report */
/* Program looking for a true path that starts from S and ends at E */
/* Finally it open a file named "path.txt" and write path to in it */

int main(int argc, char **argv)
{
    stack = (char*) malloc(size * sizeof(char));                    /* stack size is 1 */

    char const *const f = argv[1];                                  /* argv[1] = input.txt */
    FILE *file = fopen(f, "r");
    static char line[64];

    /* below code is reading file line by line and store informations in maze array */

    while(fgets(line, sizeof(line), file))
    {
        j = 0;
        while((int) line[j] != 0)
        {
            maze[i][j].data = line[j];
            j++;
        }
        i++;
    }
    fclose(file);

    int control = 0;                                                /* for control of is there a solution path */

    int m = 0;
    while(m<j)                                                      /* looking for starting point of maze */
    {
        if(maze[0][m].data == 'S')
        {
            break;
        }
        m++;
    }
    control = mazeRunner(0, m);                                     /* calling search function with starting point of maze */

    FILE *fptr = fopen("path.txt", "w");

    if(control == 1)                                                /* if path is found, then stack will be printed to path.txt */
    {
        char road[(top+1)];
        int n = top;
        while(!isempty())
        {
            road[n] = pop();
            n--;
        }
        top = 0;
        n = sizeof(road);
        fprintf(fptr, "Start ");
        while(top<n)
        {
            fprintf(fptr, "%c ", road[top]);
            top++;
        }
        fprintf(fptr, "Exit");
    }
    else                                                            /* if path not found, then an error message will be printed to path.txt */
    {
        fprintf(fptr, "ERROR!!!! No path found!");
    }
    free(stack);
    return 0;
}

/* This function searching for solution path */

int mazeRunner(int x, int y)
{
    int control = 0;

    /* looking east of current position in the maze */
    if((maze[x][y+1].data != '1') && (maze[x][y+1].marked != 1) && (y < (j-1)) && (control != 1))
    {
        /* if east of current position is exit then pushes the way to stack, mark it and changes control to 1 */
        if(maze[x][y+1].data == 'E')
        {
            push('E');
            maze[x][y+1].marked = 1;
            control = 1;
        }

            /* if east of current position is 0 then pushes the way to stack, mark it and function calls itself */
        else if(maze[x][y+1].data == '0')
        {
            push('E');
            maze[x][y+1].marked = 1;
            control = mazeRunner(x, (y+1));

            /* if solution path is not found pop the way and look other ways */
            if(control != 1)
            {
                pop();
            }
        }

            /* if east of current position is lower case then takes it to array of keys, */
            /* pushes the way and key to stack, then function calls itself */
        else if(islower((int) maze[x][y+1].data))
        {
            *key = maze[x][y+1].data;
            key++;
            push('E');
            push(maze[x][y+1].data);
            maze[x][y+1].marked = 1;
            control = mazeRunner(x, (y+1));

            /* if solution path is not found pop the way and push the way back to key, */
            /* unmark all cells those are marked before and call mazerunner with position of key */
            if(control != 1)
            {
                push('W');
                unmarkAll();
                maze[x][y+1].marked = 1;
                control = mazeRunner(x, y);
            }
        }

            /* if east of current position is upper case looks for is there a key that matches */
        else if(isupper((int) maze[x][y+1].data))
        {
            int n = 0;
            while(n<25)
            {
                /* if key is found, push the way and door to stack, mark current position and call function itself */
                if(((int) keys[n]) == tolower((int) maze[x][y+1].data))
                {
                    push('E');
                    push(maze[x][y+1].data);
                    maze[x][y+1].marked = 1;
                    control = mazeRunner(x, (y+1));

                    /* if path is not found pop the way and door from stack and exit from while loop */
                    if(control != 1)
                    {
                        pop();
                        pop();
                    }
                    break;
                }
                n++;
            }
        }
    }

    /* looking south of current position in the maze */
    if((maze[x+1][y].data != '1') && (maze[x+1][y].marked != 1) && (x < (i-1))  && (control != 1))
    {
        /* if south of current position is exit then pushes the way to stack, mark it and changes control to 1 */
        if(maze[x+1][y].data == 'E')
        {
            push('S');
            maze[x+1][y].marked = 1;
            control = 1;
        }

            /* if south of current position is 0 then pushes the way to stack, mark it and function calls itself */
        else if(maze[x+1][y].data == '0')
        {
            push('S');
            maze[x+1][y].marked = 1;
            control = mazeRunner((x+1), y);

            /* if solution path is not found pop the way and look other ways */
            if(control != 1)
            {
                pop();
            }
        }

            /* if south of current position is lower case then takes it to array of keys, */
            /* pushes the way and key to stack, then function calls itself */
        else if(islower((int) maze[x+1][y].data))
        {
            *key = maze[x+1][y].data;
            key++;
            push('S');
            push(maze[x+1][y].data);
            maze[x+1][y].marked = 1;
            control = mazeRunner((x+1), y);

            /* if solution path is not found pop the way and push the way back to key, */
            /* unmark all cells those are marked before and call mazerunner with position of key */
            if(control != 1)
            {
                push('N');
                unmarkAll();
                maze[x+1][y].marked = 1;
                control = mazeRunner(x, y);
            }
        }
            /* if south of current position is upper case looks for is there a key that matches */
        else if(isupper((int) maze[x+1][y].data))
        {
            int n = 0;
            while(n<25)
            {
                /* if key is found, push the way and door to stack, mark current position and call function itself */
                if(((int) keys[n]) == tolower((int) maze[x+1][y].data))
                {
                    push('S');
                    push(maze[x+1][y].data);
                    maze[x+1][y].marked = 1;
                    control = mazeRunner((x+1), y);

                    /* if path is not found pop the way and door from stack and exit from while loop */
                    if(control != 1)
                    {
                        pop();
                        pop();
                    }
                    break;
                }
                n++;
            }
        }
    }

    /* looking west of current position in the maze */
    if((maze[x][y-1].data != '1') && (maze[x][y-1].marked != 1) && (y != 0)  && (control != 1))
    {
        /* if west of current position is exit then pushes the way to stack, mark it and changes control to 1 */
        if(maze[x][y-1].data == 'E')
        {
            push('W');
            maze[x][y-1].marked = 1;
            control = 1;
        }

            /* if west of current position is 0 then pushes the way to stack, mark it and function calls itself */
        else if(maze[x][y-1].data == '0')
        {
            push('W');
            maze[x][y-1].marked = 1;
            control = mazeRunner(x, (y-1));

            /* if solution path is not found pop the way and look other ways */
            if(control != 1)
            {
                pop();
            }
        }

            /* if west of current position is lower case then takes it to array of keys, */
            /* pushes the way and key to stack, then function calls itself */
        else if(islower((int) maze[x][y-1].data))
        {
            *key = maze[x][y-1].data;
            key++;
            push('W');
            push(maze[x][y-1].data);
            maze[x][y-1].marked = 1;
            control = mazeRunner(x, (y-1));

            /* if solution path is not found pop the way and push the way back to key, */
            /* unmark all cells those are marked before and call mazerunner with position of key */
            if(control != 1)
            {
                push('E');
                unmarkAll();
                maze[x][y-1].marked = 1;
                control = mazeRunner(x, y);
            }
        }

            /* if west of current position is upper case looks for is there a key that matches */
        else if(isupper((int) maze[x][y-1].data))
        {
            int n = 0;
            while(n<25)
            {
                /* if key is found, push the way and door to stack, mark current position and call function itself */
                if(((int) keys[n]) == tolower((int) maze[x][y-1].data))
                {
                    push('W');
                    push(maze[x][y-1].data);
                    maze[x][y-1].marked = 1;
                    control = mazeRunner(x, (y-1));

                    /* if path is not found pop the way and door from stack and exit from while loop */
                    if(control != 1)
                    {
                        pop();
                        pop();
                    }
                    break;
                }
                n++;
            }
        }
    }

    /* looking north of current position in the maze */
    if((maze[x-1][y].data != '1') && (maze[x-1][y].marked != 1) && (x != 0)  && (control != 1))
    {
        /* if north of current position is exit then pushes the way to stack, mark it and changes control to 1 */
        if(maze[x-1][y].data == 'E')
        {
            push('N');
            maze[x-1][y].marked = 1;
            control = 1;
        }

            /* if north of current position is 0 then pushes the way to stack, mark it and function calls itself */
        else if(maze[x-1][y].data == '0')
        {
            push('N');
            maze[x-1][y].marked = 1;
            control = mazeRunner((x-1), y);

            /* if solution path is not found pop the way and look other ways */
            if(control != 1)
            {
                pop();
            }
        }

            /* if north of current position is lower case then takes it to array of keys, */
            /* pushes the way and key to stack, then function calls itself */
        else if(islower((int) maze[x-1][y].data))
        {
            *key = maze[x-1][y].data;
            key++;
            push('N');
            push(maze[x-1][y].data);
            maze[x-1][y].marked = 1;
            control = mazeRunner((x-1), y);

            /* if solution path is not found pop the way and push the way back to key, */
            /* unmark all cells those are marked before and call mazerunner with position of key */
            if(control != 1)
            {
                push('S');
                unmarkAll();
                maze[x-1][y].marked = 1;
                control = mazeRunner(x, y);
            }
        }

            /* if north of current position is upper case looks for is there a key that matches */
        else if(isupper((int) maze[x-1][y].data))
        {
            int n = 0;
            while(n<25)
            {
                /* if key is found, push the way and door to stack, mark current position and call function itself */
                if(((int) keys[n]) == tolower((int) maze[x-1][y].data))
                {
                    push('N');
                    push(maze[x-1][y].data);
                    maze[x-1][y].marked = 1;
                    control = mazeRunner((x-1), y);

                    /* if path is not found pop the way and door from stack and exit from while loop */
                    if(control != 1)
                    {
                        pop();
                        pop();
                    }
                    break;
                }
                n++;
            }
        }
    }
    return control;
}

/* This function unmark all marked cells in the maze */
void unmarkAll()
{
    int x = 0, y = 0;
    while(x<i)
    {
        y = 0;
        while(y<j)
        {
            maze[x][y].marked = 0;
            y++;
        }
        x++;
    }
}

/* This function is looking for is stack full, if it returns 1 otherwise it returns 0 */
int isfull()
{
    if(top == (size-1))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/* This function is looking for is stack empty, if it returns 1 otherwise it returns 0 */
int isempty()
{
    if(top == -1)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void push(char data)
{
    if(!isfull())
    {
        top++;
        *(stack + top) = data;
    }
    else
    {
        size = size + 1;
        stack = realloc(stack, size);
        top++;
        *(stack + top) = data;
    }
}

char pop()
{
    char data;
    if(!isempty())
    {
        data = *(stack + top);
        top--;
        return data;
    }
}