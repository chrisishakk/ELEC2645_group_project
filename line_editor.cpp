//some important ANSII escape codes
#include <iostream>
#include <string>

//vvvv Received control codes vvvv
#define LEFT_ARROW      "\x1B[D"
#define RIGHT_ARROW     "\x1B[C"
#define UP_ARROW        "\x1B[A"
#define DOWN_ARROW      "\x1B[B"

#define HOME            "\x1B[H"
#define END             "\x1B[F"

#define PAGE_UP         "\x1B[5~"
#define PAGE_DOWN       "\x1B[6~"

#define INSERT          "\x1B[2~"
#define DELETE          "\x1B[3~"

//vvvv Delivered control codes vvvv
#define BACKSPACE       "\x08"
#define ERASE_IN_LINE   "\x1B[K"
#define ERASE_TO_EOL    "\x1B[K"
#define ERASE_LINE      "\x1B[2K"
#define CURSOR_HOME     "\x1B[0;0H"
#define CURSOR_POS(x, Y)    "\x1B[%d;%dH", (X), (Y)
#define LINE_HOME       "\x1B[0G" // horizontal move absolute

char * prompt = "lineEd: ";
unsigned int prompt_length = 8;

void LineEd_set_prompt(const char * custom_prompt)
{
    prompt = (char *)custom_prompt;
    unsigned int i = 0;
    while (prompt[i] != '\0') i ++;
    prompt_length = i - 1; //check this
}

enum states {NORMAL, ESC_SEQ, CTRL_SEQ};

states state = NORMAL;

std::string command_line;
unsigned int line_position = 0;
unsigned int cursor_column = 0;
unsigned int cursor_row = 0;

char in_char;

void visual_erase()
{
    std::cout << ERASE_LINE;
    std::cout << LINE_HOME; 
}

void visual_re_print()
{
    visual_erase();
    std::cout << command_line;
}

std::string get_line()
{
    while ((in_char = getchar()))
    {
        switch (state)
        {
            case NORMAL:
            if ((in_char > 0x1F && in_char < 0x7F) || in_char == 0x09)
            {
                command_line.insert(line_position, {in_char});
                line_position ++;
            }
            else
            {
                switch (in_char)
                {
                    // For some reason backspace sends character 127 (delete) instead of 8 (backspace).
                    // Can't seem to change this so making both do the same thing.
                    case 0x08:
                    case 0x7F:
                    line_position --;
                    command_line.erase(line_position);
                    break;

                    case 0x0A:
                    case 0x0D:
                    return command_line;
                    break;

                    case 0x03:
                    return 0;
                    break;

                    default:
                    printf("%c", in_char);
                    break;
                }
            }
            break;

            case ESC_SEQ:

            break;

            case CTRL_SEQ:

            break;
        }

        visual_re_print();
    }
}
