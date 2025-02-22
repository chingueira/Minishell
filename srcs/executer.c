/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   executer.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 14:12:34 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/22 15:32:39 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/minihell.h"

void	exec_builtins(t_shell *shell)
{
	t_token *token;

	token = shell->token;
/*	if (strcmp(token->value, "echo") == 0)
		ft_echo(token);
	else */if (strcmp(token->value, "cd") == 0)
		ft_cd(token);
	else if (strcmp(token->value, "pwd") == 0)
		ft_pwd(token);
	else if (strcmp(token->value, "env") == 0)
		ft_env(shell->env, token);
	else if (strcmp(token->value, "export") == 0)
		ft_export(shell->env, token);
	else if (strcmp(token->value, "unset") == 0)
		ft_unset(shell->env, token);
	else if (strcmp(token->value, "exit") == 0)
		ft_exit(shell);
	printf("\nfrom builtins\n");
}

void	 execute_cmd_in_pipe(char *command, t_pipe *pipes, int in, int out)
{
	char	*path;
	char	*cmd;
	char	**args;

	cmd = strdup(command);
	args = ft_split(cmd, ' ');
	path = find_path(args[0], pipes->ev);
	if (!path)
	{
		printf("This cmd %s does not exist!\n", args[0]);
		exit(EXIT_FAILURE);
	}
	if (in != 0)
	{
		dup2(in, STDIN_FILENO);
		close(in);
	}
	if (out != 1)
	{
		dup2(out, STDOUT_FILENO);
		close(out);
	}
	if (execve(path, args, pipes->ev) == -1)
		error_message("execve");
}

void	execute_redirections(t_token *tokens)
{
	t_token	*tmp = tokens;
	int		fd;

	while (tmp)
	{
		if (is_lesser(tmp->type) && tmp->next) // Input redirection
		{
			fd = open(tmp->next->value, O_RDONLY);
			if (fd == -1)
				error_message(tmp->next->value);
			dup2(fd, STDIN_FILENO);
			close(fd);
		}
		else if (is_greater(tmp->type) && tmp->next) // Output redirection
		{
			fd = open(tmp->next->value, O_WRONLY | O_TRUNC | O_CREAT, 0644);
			if (fd == -1)
				error_message(tmp->next->value);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		else if (is_append(tmp->type) && tmp->next) // Append redirection
		{
			fd = open(tmp->next->value, O_WRONLY | O_APPEND | O_CREAT, 0644);
			if (fd == -1)
				error_message(tmp->next->value);
			dup2(fd, STDOUT_FILENO);
			close(fd);
		}
		tmp = tmp->next;
	}
}

// void	child_process(char *cmd, t_pipe *pipes, t_token *tokens, int num_commands)
// {
// 	execute_redirections(tokens);
	
// 	if (pipes->input_fd != 0)
// 	{
// 		dup2(pipes->input_fd, STDIN_FILENO);
// 		close(pipes->input_fd);
// 	}
// 	if (pipes->i < num_commands - 1)
// 	{
// 		dup2(pipes->pipe_fd[1], STDOUT_FILENO);
// 		close(pipes->pipe_fd[1]);
// 	}
// 	if (pipes->i < num_commands - 1)
// 		close(pipes->pipe_fd[0]);
// 	execute_command(cmd, pipes, STDIN_FILENO, STDOUT_FILENO);
// }

void	parent_process(t_pipe *pipes, int num_commands)
{
	if (pipes->input_fd != 0)
		close(pipes->input_fd);
	if (pipes->i < num_commands - 1)
	{
		close(pipes->pipe_fd[1]);
		pipes->input_fd = pipes->pipe_fd[0];
	}
}
void	child_process2(char *cmd, t_pipe *pipes, t_token *tokens, int num_commands)
{
	// Apply redirections before handling pipes
	execute_redirections(tokens);

	// Handle pipes
	if (pipes->input_fd != 0)
	{
		dup2(pipes->input_fd, STDIN_FILENO);
		close(pipes->input_fd);
	}
	if (pipes->i < num_commands - 1)
	{
		dup2(pipes->pipe_fd[1], STDOUT_FILENO);
		close(pipes->pipe_fd[1]);
	}
	close(pipes->pipe_fd[0]); // Close unused read end

	// Execute command
	execute_cmd_in_pipe(cmd, pipes, STDIN_FILENO, STDOUT_FILENO);
}

void	execute_pipe(t_shell *shell, t_token *tokens)
{
	char	**cmd;
	pid_t	id;
	char	*clean_input;
	t_pipe	*pipes = malloc(sizeof(t_pipe));

	pipes->i = 0;
	pipes->input_fd = 0;
	clean_input = clean_string(shell->input);
	cmd = ft_split(clean_input, '|');
	pipes->ev = shell->ev;

	while (pipes->i < shell->num_of_cmds)
	{
		if (pipe(pipes->pipe_fd) == -1)
			error_message("pipe");

		id = fork();
		if (id == -1)
			error_message("fork");

		if (id == 0)
		{
			child_process2(cmd[pipes->i], pipes, tokens, shell->num_of_cmds);
			exit(EXIT_SUCCESS);
		}

		wait(NULL);
		parent_process(pipes, shell->num_of_cmds);
		pipes->i++;
	}
	free(pipes);
	free_array(cmd);
}

/*
void	executer(t_shell *shell)
{
	t_token	*token;
	t_pipe	*pipes = malloc(sizeof(t_pipe));
	char **args;
	token = shell->token;
	args = cmd_args(token);

	pipes->i = 0;
	pipes->input_fd = 0;
	pipes->ev = shell->ev;
	while (pipes->i < shell-> num_of_cmds)
	{
		if (pipes->i < shell->num_of_cmds)
		{
			if (pipe(pipes->pipe_fd) == -1)
				error_message("pipe");
		}
		pipes->id = fork();
		if (pipes->id == -1)
			error_message("fork");
		if (pipes->id == 0)
			child_process(args[pipes->i], pipes, token, shell->num_of_cmds);
		else
		{
			wait(NULL);
			parent_process(pipes, shell->num_of_cmds);
		}
		pipes->i++;
	}

}
*/
