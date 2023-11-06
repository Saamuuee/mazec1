#include "parser/ast.h"
#include "shell.h"
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

void initialize(void)
{
    /* This code will be called once at startup */
    if (prompt)
        prompt = "vush$ ";
}

void run_command(node_t *node)
{
    /* Print parsed input for testing - comment this when running the tests! */
    //print_tree(node);

    if (node->type == NODE_COMMAND) {
        char *program = node->command.program;
        char **argv = node->command.argv;
        
        // Fork a child process
        pid_t pid = fork();
        
        if (pid == 0) {
            // Child process
            // Execute the command
            execvp(program, argv);
            // If execvp returns, it must have failed
            fprintf(stderr, "Unknown command or execution failed\n");
            exit(EXIT_FAILURE);
        } else if (pid > 0) {
            // Parent process
            // Wait for the child process to complete
            int status;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status)) {
                // You can use the exit status of the child if needed
            }
        } else {
            // Fork failed
            fprintf(stderr, "Failed to fork process\n");
        }
    }

    // ... handle other node types like NODE_PIPE, NODE_REDIRECT, etc.

    if (prompt)
        prompt = "vush$ ";
}

