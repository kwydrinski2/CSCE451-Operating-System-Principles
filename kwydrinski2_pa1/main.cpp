#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include "command.hpp"
#include "parser.hpp"
#include "runcmd.cpp"

#define MAX_ALLOWED_LINES 25

int main(int argc, char **argv) 
{
    std::string input_line;
    std::vector<shell_command> shell_commands;

    if ( argc > 1 && argv[1] == std::string("-t")) {
	 for ( int i = 0; i < MAX_ALLOWED_LINES; i++ ) {
	    if(!std::getline(std::cin, input_line) || input_line == "exit") {
		exit(0);
	    } else {
		try {
		    shell_commands = parse_command_string(input_line);
		    runcmd(shell_commands);
		}
		catch ( const std::runtime_error& e ) {
		    std::cout << e.what() << std::endl;
	        }
	   }
	  }
    } else {
	    for ( int i = 0; i < MAX_ALLOWED_LINES; i++ ) {
		    std::cout << "osh> " << std::flush;
		    if ( !std::getline(std::cin, input_line) || input_line == "exit" ) {
			    break;
		    }
		    try { 
			    shell_commands = parse_command_string(input_line);
			    runcmd(shell_commands);
		    }
		    catch ( const std::runtime_error& e ) {
			    std::cout << "osh: " << e.what() << std::endl;
		    }
	    }
    }
    return 0;
}
