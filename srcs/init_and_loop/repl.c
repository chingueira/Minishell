/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:19:22 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/14 10:50:47 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	error_message(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

static void	read_input(t_shell *shell)
{
	shell->input = ft_strtrim(readline("minishell$ "), " ");
	if (!shell->input)
	{
		ft_putstr_fd("Error: Empty prompt\n", 2);
		shell->flag = false;
	}
	add_history(shell->input);
}

static int number_of_commands(t_token *tokens)
{
	t_token *tmp;
	int	number;

	number = 0;
	tmp = tokens;
	while (tmp)
	{
		if (is_builtin_or_command(tmp->type))
			number++;
		tmp = tmp->next;
	}
	return(number);
}

static void	parse(t_shell *shell)
{
	shell->matrix = split_input(shell->input, shell);				//divide o prompt em um matriz de strings
	if (!shell->matrix)
	{
		ft_putstr_fd("Error: Failed to parse input", 2);
		shell->flag = false;
	}
	expand(&shell->matrix, shell->env);
	shell->token = tokenize_matrix(shell->matrix);
	if (!shell->token)
	{
		shell->flag = false;
		return ;
	}
	identify_tokens(shell->token, shell->path);
	token_sequence(shell->token);												//verify token sequence
	shell->number_of_commands = number_of_commands(shell->token);
	shell->flag = true;
}

char	*cmd_path(char *cmd, char *pat)
{
	char **paths;
	char *path;
	int	i;
	char *tmp;

	i = 0;
	paths = ft_split(pat, ':');
	while(paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(path, F_OK) == 0)
		{
			free_matrix(paths);
			return (path);
		}
		free(path);
		i++;
	}
	if (paths)
		free_matrix(paths);
	return(NULL);
}

void	exec_cmd(t_shell *shell)
{
	char	*pat;
	char	**options;
	t_token	*tmp = shell->token;
	int	size = 0;
	pid_t	id;

	while (tmp)
	{
		tmp = tmp->next;
		size++;
	}
	options = malloc(sizeof(char **) * (size + 1));
	tmp = shell->token;
	size = 0;
	while (tmp)
	{
		options[size++] = tmp->value;
		tmp = tmp->next;
	}
	options[size] = NULL;
	pat = cmd_path(shell->token->value, shell->path);
	if (!pat)
	{
		fprintf(stderr, "Command not found: %s\n", shell->token->value);
		free(options);
		return ;
	}
	id = fork();
	if (id == -1)
	{
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (id == 0)
	{
		if (execve(pat, options, shell->anv) == -1)
		{
			perror("execve");
			exit(EXIT_FAILURE);
		}
	}
	waitpid(id, NULL, 0);
	free(options);
}


void	exec_builtins(t_shell *shell)
{
		t_token *tmp;

		tmp = shell->token;

		if (tmp->type == ECHO)
		{
			tmp = tmp->next;
			ft_echo(tmp);
		}
		else if (tmp->type == EXIT)
		{
			tmp = tmp->next;
			ft_exit(shell, shell->token);
		}
		else if (tmp->type == PWD)
		{
			tmp = tmp->next;
			ft_pwd(shell->token);
		}
		else if (tmp->type == CD)
		{
			tmp = tmp->next;
			if (tmp)
				ft_cd(tmp);
		}
		else if (tmp->type == ENV)
		{
			tmp = tmp->next;
			ft_env(shell->env, shell->token);
		}
		else if (tmp->type == EXPORT)
		{
			tmp = tmp->next;
			ft_export(shell->env, tmp);
		}
		else if (tmp->type == UNSET)
		{
			tmp = tmp->next;
			ft_unset(shell->env, tmp);
		}
}

void	execute(t_shell *shell)
{
	t_token *tmp;

	tmp = shell->token;
	if (is_builtin(tmp->type))
		exec_builtins(shell);
	else if (tmp->type == COMMAND)
		exec_cmd(shell);

}

static bool	pipe_check(t_shell *shell)
{
	t_token *tmp = shell->token;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (!tmp->next || tmp->next->type != COMMAND)
			{
				printf("Error: Invalid pipe\n");
				return (false);
			}
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}

int	count_args(t_token *tmp_token)
{
	int	i;
	t_token	*token = tmp_token;

	i = 0;
	while (token && token->type != PIPE && token->type != COMMAND)
	{
		if (token->type == ARGUMENT || token->type == DOUBLE_QUOTE
			|| token->type == SINGLE_QUOTE)
			i++;
		token = token->next;
	}
	return (i);
}

void get_pipes(t_shell *shell)
{
	t_command *tmp_cmd;
	t_token *tmp_token;
	int i;

	shell->command_groups = malloc(sizeof(t_command));
	tmp_cmd = shell->command_groups;
	tmp_token = shell->token;
	while (tmp_token)
	{
		if (tmp_token->type == COMMAND)
		{
			tmp_cmd->command = tmp_token->value;
			tmp_cmd->args = malloc(sizeof(char *) * (count_args(tmp_token) + 1));
			i = 0;
		}
		else if (tmp_token->type == ARGUMENT || tmp_token->type == DOUBLE_QUOTE || tmp_token->type == SINGLE_QUOTE || tmp_token->type == OPTION)
			tmp_cmd->args[i++] = tmp_token->value;
		else if (tmp_token->type == PIPE)
		{
			tmp_cmd->args[i] = NULL;
			tmp_cmd->next = safe_malloc(sizeof(t_command));
			tmp_cmd = tmp_cmd->next;
			ft_memset(tmp_cmd, 0, sizeof(t_command));
			i = 0;
		}
		tmp_token = tmp_token->next;
	}
	tmp_cmd->args[i] = NULL;
	tmp_cmd->next = NULL; 
}

void	debug_commands(t_command *command_groups)
{
	t_command *tmp = command_groups;
	int i = 0;
	while (tmp)
	{
		printf("Command %d: %s\n", i, tmp->command);
		printf("Args: ");
		int j = 0;
		while (tmp->args[j])
		{
			printf("%s ", tmp->args[j]);
			j++;
		}
		printf("\n");
		tmp = tmp->next;
		i++;
	}
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

void	exec_command(t_command *cur_cmd_group, t_pipe *pipes, int in, int out)
{
	char	*path;
	char	*cmd;
	char	**args;

	cmd = strdup(cur_cmd_group->command);
	args = malloc(sizeof(char *) * (matrix_len(cur_cmd_group->args) + 2));
	args[0] = cmd;
	for (int i = 0; cur_cmd_group->args[i]; i++)
		args[i + 1] = cur_cmd_group->args[i];
	args[matrix_len(cur_cmd_group->args) + 1] = NULL;
	path = find_path(cmd, pipes->ev);
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

void	child_process(t_command *cur_cmd_group, t_pipe *pipes, int num_commands)
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
	exec_command(cur_cmd_group, pipes, STDIN_FILENO, STDOUT_FILENO);
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

void	exec_pipe(t_shell *shell)
{
	pid_t	id;
	t_command *tmp_cmd;
	t_pipe	*pipes;
	int		i = 0;

	get_pipes(shell);
	tmp_cmd = shell->command_groups;
	pipes = malloc(sizeof(t_pipe));
	pipes->input_fd = 0;
	pipes->i = 0;

	while (tmp_cmd)
	{
		if (pipes->i < shell->number_of_commands - 1) // ✅ Only create pipes if needed
		{
			if (pipe(pipes->pipe_fd) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}

		id = fork();
		if (id == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (id == 0)
			child_process(tmp_cmd, pipes, shell->number_of_commands);
		else
			parent_process(pipes, shell->number_of_commands);

		tmp_cmd = tmp_cmd->next;
		pipes->i++;
	}

	// ✅ Wait for all child processes **after** loop
	while (i++ < shell->number_of_commands)
		wait(NULL);

	free(pipes);
}

void	repl(t_shell *shell)
{
	while (1)
	{
		read_input(shell);
		parse(shell);
		if (pipe_check(shell))
			exec_pipe(shell);
		else
			execute(shell);
		//handle signals
		// if (shell->flag)
		// else
		// printf("\n%d\n", shell->number_of_commands);
		// debug(shell->token, shell->number_of_commands)
		// printf("%s\n", shell->input);
	}
}