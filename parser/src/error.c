#include "error.h"

static const int max_buffer = 1000;

extern FILE *file;
static char *buffer = new char[max_buffer];
static bool eof = 0;
static int  row = 0;
static int  n_buffer = 0;
static int  l_buffer = 0;
static int  token_start = 0;
static int  token_length = 0;
static int  token_next_start = 0;

static void dump_row()
{
    if (row == 0)
    {
        fprintf(stdout, "       |");
        for (size_t i = 1; i < 71; i++)
        {
            if (i % 10 == 0)
            {
                fprintf(stdout, ":"); 
            }
            else if (i % 5 == 0)
            {
                fprintf(stdout, "+"); 
            }
            else
            {
                fprintf(stdout, ".");
            }
        }
        fprintf(stdout, "\n"); 
    }
    else 
    {
        fprintf(stdout, "%6d |%.*s", row, l_buffer, buffer);
    }
}

void print_error(char *errorstring)
{
    static char errmsg[10000];

    int start = token_start;
    int end = start + token_length - 1;

    if (eof)
    {
        fprintf(stdout, "...... !");
        for (size_t i = 0; i < l_buffer; i++)
        {
            fprintf(stdout, ".");
        }
        fprintf(stdout, "^-EOF\n");
    }
    else
    {
        fprintf(stdout, "...... !");
        for (size_t i = 1; i < start; i++)
        {
            fprintf(stdout, ".");
        }
        for (size_t i = start; i <= end; i++)
        {
            fprintf(stdout, "^");
        }
        for (size_t i = end + 1; i < l_buffer; i++)
        {
            fprintf(stdout, ".");
        }
        fprintf(stdout, "   token%d:%d\n", start, end);
    }

    fprintf(stdout, "Error: %s\n", errmsg);
}

static int getNextLine(void)
{
    int i;
    char *p;
    n_buffer = 0;
    token_start = -1;
    token_next_start = 1;
    eof = false;

    p = fgets(buffer, max_buffer, file);
    if (p == nullptr)
    {
        if (ferror(file))
        {
            return -1;
        }
        eof = true;
        return 1;
    }

    row += 1;
    l_buffer = strlen(buffer);
    dump_row();

    return 0;
}

int get_next_char(char *b, int maxBuffer)
{
    int frc;
    if (eof)
    {
        return 0;
    }

    while (n_buffer >= l_buffer)
    {
        frc = getNextLine();
        if (frc != 0)
        {
            return 0;
        }
    }

    b[0] = buffer[n_buffer];
    n_buffer += 1;

    return b[0] == 0 ? 0 : 1;
}

void begin_token(char *t)
{
    token_start = token_next_start;
    token_length = strlen(t);
    token_next_start = n_buffer;
}
