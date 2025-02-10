/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/22 10:00:49 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/10 12:17:57 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	error_message(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	paths = ft_split(envp[i] + 5, ':');
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	i = -1;
	free(paths);
	return (0);
}

int		count_arr_elem(char **arr)
{
	int	elem;

	elem = 0;
	while (arr[elem] != NULL)
		elem++;
	return (elem);
}

void	execute_command(char *command, t_pipe *pipes, int in, int out)
{
	char	*path;
	char	*cmd;
	char	**args;

	cmd = strdup(command);
	args = ft_split(cmd, ' ');
	path = find_path(args[0], pipes->ev);
	if (!path)
	{
		printf("This cmd %s does not exist!", args[0]);
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

void	child_process(char *cmd, t_pipe *pipes, int num_commands)
{
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
	if (pipes->i < num_commands - 1)
		close(pipes->pipe_fd[0]);
	execute_command(cmd, pipes, STDIN_FILENO, STDOUT_FILENO);
}


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

void	execute_pipe(t_shell *shell)
{
	int		num_commands;
	char	**cmd;
	pid_t	id;
	char	*clean_input;
	t_pipe	*pipes = malloc(sizeof(t_pipe));

	pipes->i = 0;
	pipes->input_fd = 0;
	clean_input = clean_string(shell->input);
	cmd = ft_split(clean_input, '|');
	num_commands = count_arr_elem(cmd);
	pipes->ev = shell->anv;
	
	while (pipes->i < num_commands)
	{
		if (pipes->i < num_commands)
		{
			if (pipe(pipes->pipe_fd) == -1)
				error_message("pipe");
		}
		id = fork();
		if (id == -1)
			error_message("fork");
		if (id == 0)
			child_process(cmd[pipes->i], pipes, num_commands);
		else
		{
			wait(NULL);
			parent_process(pipes, num_commands);
		}
		pipes->i++;
	}
}
