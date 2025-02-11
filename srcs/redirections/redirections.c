/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/10 14:25:30 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/11 12:17:12 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minishell.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/wait.h>

void	free_array(char **arr)
{
	int	i;

	i = 0;
	while (arr[i])
		free(arr[i++]);
	free(arr);
}


char	*find_path(char *cmd, char **envp)
{
	char	**paths;
	char	*path;
	int		i;
	char	*part_path;

	i = 0;
	while (envp[i] && ft_strnstr(envp[i], "PATH", 4) == 0)
		i++;
	if (!envp[i])
		return (NULL);
	paths = ft_split(envp[i] + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		part_path = ft_strjoin(paths[i], "/");
		path = ft_strjoin(part_path, cmd);
		free(part_path);
		if (access(path, F_OK) == 0)
		{
			free_array(paths);
			return (path);
		}
		free(path);
		i++;
	}
	free_array(paths);
	return (NULL);
}

void exec_command(char *command, char **env)
{
	char	*path;
	char	**args;

	args = ft_split(command, ' ');
	if (!args)
	{
		perror("split");
		exit(EXIT_FAILURE);
	}
	path = find_path(args[0], env);
	if (!path)
	{
		printf("Command not found: %s\n", args[0]);
		exit(EXIT_FAILURE);
	}
	if (execve(path, args, env) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void	redirect_input(char *cmd, char *file, char **env)
{
	int fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	exec_command(cmd, env);
}

void	redirect_output(char *cmd, char *file, char **env)
{
	int fd = open(file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	exec_command(cmd, env);
}

void	redirect_output_append(char *cmd, char *file, char **env)
{
	int fd = open(file,  O_WRONLY | O_APPEND | O_CREAT, 0644);
	if (fd == -1)
	{
		perror("open");
		exit(EXIT_FAILURE);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		exit(EXIT_FAILURE);
	}
	close(fd);
	exec_command(cmd, env);
}

// int	main(int ac, char **av, char **env)
// {
// 	pid_t id;

// 	if (ac == 4 && av[2][0] == '>') // Ensure correct syntax
// 	{
// 		id = fork();
// 		if (id == -1)
// 		{
// 			perror("fork");
// 			exit(EXIT_FAILURE);
// 		}
// 		if (id == 0) // Child process
// 			redirect_output_append(av[1], av[3], env);
// 		else // Parent process
// 			waitpid(id, NULL, 0);
// 	}
// 	else if (ac == 4 && av[2][0] == '<') // Ensure correct syntax
// 	{
// 		id = fork();
// 		if (id == -1)
// 		{
// 			perror("fork");
// 			exit(EXIT_FAILURE);
// 		}
// 		if (id == 0) // Child process
// 			redirect_input(av[1], av[3], env);
// 		else // Parent process
// 			waitpid(id, NULL, 0);
// 	}
// 	else
// 		printf("Usage: %s command > file\n", av[0]);
// 	return (0);
// }
