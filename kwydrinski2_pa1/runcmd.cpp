#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <string>
#include <fcntl.h>

#include "parser.hpp"
#include "command.hpp"

#define MAX_ALLOWED_LINES 25

int runcmd(std::vector<shell_command> shell_commands)
{
	bool execute_next = true;
	int current_index = 0;
	int previous_exit_status = 0;

	for ( shell_command commands : shell_commands ) {
		if ( current_index > 0 ) {
			next_command_mode mode = shell_commands[current_index - 1].next_mode;
			if ( mode == next_command_mode::on_success && previous_exit_status != 0 ) {
				execute_next = false;;
			} else if ( mode == next_command_mode::on_fail && previous_exit_status == 0 ) {
				execute_next = false;
			} else {
				execute_next = true;
			}
		}

		if ( !execute_next ) {
			current_index++;
			continue;
		}
		
		pid_t pid = fork();
		switch ( pid ) {
			case 0: { /* child process */ 
				istream_mode cin_mode = commands.cin_mode;
				if ( cin_mode == istream_mode::file ) {
					const char *cin_file = commands.cin_file.c_str();
					int fd;
					if ( ( fd = open( cin_file, O_RDONLY ) ) < 0 ) {
						perror(cin_file);
						exit(1);
					}
					dup2(fd, STDIN_FILENO);
					close(fd);
				}
				ostream_mode cout_mode = commands.cout_mode;
				if ( cout_mode == ostream_mode::file ) {
					const char *cout_file = commands.cout_file.c_str();
					int fd;
					if ( ( fd = open( cout_file, O_WRONLY | O_TRUNC | O_CREAT, 0644 ) ) < 0 ) {
						perror(cout_file);
						exit(1);
					}
					dup2(fd, STDOUT_FILENO);
					close(fd);
				} else if ( cout_mode == ostream_mode::append ) {
					const char *cout_file = commands.cout_file.c_str();
					int fd;
					if ( ( fd = open( cout_file, O_WRONLY | O_APPEND | O_CREAT, 0644) ) < 0 ) {
						perror(cout_file);
						exit(1);
					}
					dup2(fd, STDOUT_FILENO);
					close(fd);
				}

				std::vector<const char*> arguments;
				arguments.push_back(commands.cmd.c_str());
				for ( const auto& arg : commands.args ) {
					arguments.push_back(arg.c_str());
				}
				arguments.push_back(nullptr);
				execvp( commands.cmd.c_str(),
						const_cast<char* const*>(arguments.data()) );
				exit(1);
				}
			default: {
				int status;
				waitpid(pid, &status, 0); // reap zombies
				previous_exit_status = WEXITSTATUS(status);
				current_index++;
				break;
				 }
			case -1: {
				fprintf(stderr, "Fork Failure\n");
				return -1;
				 }	
	
		}}
	return 1;
}
