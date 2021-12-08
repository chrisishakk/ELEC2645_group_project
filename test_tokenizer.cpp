#include <iostream>
#include <string>
#include <list>
#include "terminal_control.h"
#include "line_editor.h"
#include "tokenizer.h"

std::string command_line;
unsigned int i;
int sqr_bracket_count = 0;
unsigned int index_opened;
unsigned int index_closed;
std::string command_expression;
bool run = true;
std::list<struct token_list_node> token_list_1;

void print_token_list(std::list<struct token_list_node> list)
{
	std::list<struct token_list_node>::iterator list_iter = list.begin();
	while (list_iter != list.end())
	{
		std::cout << list_iter->index << ":" << list_iter->literal_string << " -> ";
		list_iter ++;
	}
	std::cout << std::endl;
}
void print_number_token_list(std::list<struct token_list_node> list)
{
	std::list<struct token_list_node>::iterator list_iter = list.begin();
	while (list_iter != list.end())
	{
		std::cout << list_iter->index << ":" << list_iter->number << " -> ";
		list_iter ++;
	}
	std::cout << std::endl;
}


int main()
{
    try
    {
        enable_raw_mode();
		while (run)
		{
			command_line = get_line();
			for (i = 0; i < command_line.length(); i ++)
			{
				if (command_line[i] == '[')
				{
					sqr_bracket_count ++;
					if (sqr_bracket_count > 1)
					{
						std::cerr << "Error: invalid command expression. Command separators should not be nested." << std::endl;
						throw std::runtime_error("Square-bracket matching error."); 
					}
					else
					{
						index_opened = i;
					}
				}
				if (command_line[i] == ']')
				{
					sqr_bracket_count --;
					if (sqr_bracket_count < 0)
					{
						std::cerr << "Error: ']' without matching '['." << std::endl;
						throw std::runtime_error("Square-bracket matching error.");
					}
					else
					{
						index_closed = i;
						command_expression = command_line.substr((index_opened + 1), (index_closed - index_opened - 1));
						if (find_command_words(command_expression, &token_list_1, 0))
						{
							print_token_list(token_list_1);
						}
						else
						{
							std::cout << "Oops, that didn't work." << std::endl;
						}
						token_list_1.clear();
						if (find_numbers(command_expression, &token_list_1, 0))
						{
							print_number_token_list(token_list_1);
						}
						else
						{
							std::cout << "Oops, that didn't work." << std::endl;
						}
						// do some shit
						//std::cout << command_expression << std::endl;
						sqr_bracket_count = 0;
					}
				}
			}
			token_list_1.clear();
		}
        disable_raw_mode();
    }
    catch(std::runtime_error * error)
    {
        std::cerr << error->what() << std::endl;
    }
    
    return 0;
}
