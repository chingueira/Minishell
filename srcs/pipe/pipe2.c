#include "../../inc/minishell.h"

void error_message(char *str) {
    perror(str);
    exit(EXIT_FAILURE);
}

void execute_command_pipe(struct s_shell *shell, int input_fd, int output_fd) {
    t_token *tmp = shell->token;
    int size = 0;
    while (tmp) {
        tmp = tmp->next;
        size++;
    }

    char **options = malloc(sizeof(char *) * (size + 1));
    tmp = shell->token;
    size = 0;
    while (tmp) {
        options[size] = tmp->value;
        tmp = tmp->next;
        size++;
    }
    options[size] = NULL;

    if (input_fd != 0) {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    if (output_fd != 1) {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }

    // Execute the command (assuming we have a simple execve for this example)
    if (execvp(options[0], options) == -1) {
        perror("Exec failed");
        exit(EXIT_FAILURE);
    }
}

void child_process(struct s_shell *shell, t_pipe *tmp) {
    int fd = open("./file", O_WRONLY | O_TRUNC | O_CREAT, 0777);
    if (fd == -1) {
        error_message("Error opening file");
    }

    // Redirect input if needed
    if (tmp->input_fd != 0) {
        dup2(tmp->input_fd, STDIN_FILENO);
        close(tmp->input_fd);
    }

    // If not the last command, pipe to next
    if (tmp->i < shell->number_of_commands - 1) {
        dup2(tmp->pipe_fd[1], STDOUT_FILENO);
        close(tmp->pipe_fd[1]);
    } else {
        // For the last command, redirect output to file
        dup2(fd, STDOUT_FILENO);
    }

    close(fd); // We can close the file descriptor now

    // Execute the command for the current token
    execute_command_pipe(shell, STDIN_FILENO, STDOUT_FILENO);
}

void parent_process(struct s_shell *shell, t_pipe *tmp) {
    // Close the pipes, and set up input for the next command
    if (tmp->input_fd != 0)
        close(tmp->input_fd);
    if (tmp->i < shell->number_of_commands - 1) {
        close(tmp->pipe_fd[1]);
        tmp->input_fd = tmp->pipe_fd[0];
    }
}

void execute_pipe(struct s_shell *shell) {
    t_pipe *tmp = malloc(sizeof(t_pipe));
    tmp->i = 0;
    tmp->input_fd = 0; // Initial input is from stdin

    // Loop through each command in the pipeline
    while (tmp->i < shell->number_of_commands) {
        if (tmp->i < shell->number_of_commands - 1) {
            if (pipe(tmp->pipe_fd) == -1) {
                error_message("Pipe failed");
            }
        }

        tmp->pid = fork();
        if (tmp->pid == -1) {
            error_message("Fork failed");
        }

        if (tmp->pid == 0) {
            // Child process
            child_process(shell, tmp);
        } else {
            // Parent process
            waitpid(tmp->pid, NULL, 0); // Wait for the child to finish
            parent_process(shell, tmp);
        }

        tmp->i++; // Move to the next command in the pipeline
    }
}