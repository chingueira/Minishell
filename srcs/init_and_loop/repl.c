/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:19:22 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/13 21:36:50 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	is_builtin(t_type type);
void	exec_builtins(t_shell *shell);
void	exec_p(t_shell *shell);

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
		if (tmp->type == COMMAND || is_builtin(tmp->type))
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
			return (path);
		free(path);
		i++;
	}
	free(paths);
	return(NULL);
}

void	exec_cmd(t_shell *shell)
{
	char *pat;
	char	**options;
	t_token	*tmp = shell->token;
	int	size = 0;
	while (tmp)
	{
		tmp = tmp->next;
		size++;
	}
	options = malloc(sizeof(char **) * size + 1);
	tmp = shell->token;
	size = 0;
	while (tmp)
	{
		options[size] = tmp->value;
		tmp = tmp->next;
		size++;
	}
	options[size] = NULL;
	tmp = shell->token;
	pat = cmd_path(tmp->value, shell->path);
	pid_t id = fork();
	if (id == -1)
		exit (0);
	if (id == 0)
		if (execve(pat, options, shell->anv) == -1)
			printf("execve error!\n");
	waitpid(id, NULL, 0);
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

static bool	is_builtin(t_type type)
{
	return (type == ECHO || type == CD || type == PWD ||
		type == EXPORT || type == UNSET || type == ENV || type == EXIT);
}


/*static bool	pipe_check(t_shell *shell)
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
}*/

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
			tmp_cmd->next = malloc(sizeof(t_command));
			tmp_cmd = tmp_cmd->next;
			tmp_cmd->next = NULL;
			tmp_cmd->command = NULL;
			tmp_cmd->args = NULL;
			tmp_cmd->path = NULL;
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

void exec_p(t_shell *shell)
{
	int i = 0;
	int pipe_fds[2];
	int prev_pipe = -1;
	pid_t pid;
	t_command *cmd = shell->command_groups;

	while (cmd)
	{
		if (cmd->next)
		{
			if (pipe(pipe_fds) == -1)
			{
				perror("pipe");
				exit(EXIT_FAILURE);
			}
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		else if (pid == 0)
		{
			if (prev_pipe != -1)
			{
				dup2(prev_pipe, STDIN_FILENO);
				close(prev_pipe);
			}
			if (cmd->next)
			{
				dup2(pipe_fds[1], STDOUT_FILENO);
				close(pipe_fds[1]);
				close(pipe_fds[0]);
			}
			execvp(cmd->command, cmd->args);
			perror("execvp");
			exit(EXIT_FAILURE);
		}
		if (prev_pipe != -1) 
			close(prev_pipe);
		if (cmd->next)
			close(pipe_fds[1]);
		prev_pipe = pipe_fds[0];
		cmd = cmd->next;
	}
	for (i = 0; i < shell->number_of_commands; i++)
		wait(NULL);
}

void	exec_comamnd(char *command, t_pipe *pipes, int in, int out)
{
	
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
void	exec_pipe(t_shell *shell)
{
	int	i = 0;
	char	*clean_input;
	t_command	*tmp_cmd = shell->command_groups;
	pid_t	id;
	
	t_pipe	*pipes = malloc(sizeof(t_pipe));
	pipes->input_fd = 0;
	clean_input = clean_string(shell->input);
	while (tmp_cmd)
	{
		if (tmp_cmd->next)
		{
			if (pipe(pipes->pipe_fd) == -1)
				error_message("pipe");
		}
		id = fork();
		if (id == -1)
			error_message("fork");
		child_process(tmp_cmd->command, pipes, shell->number_of_commands);
		else
		{
			wait(NULL);
			parent_process(pipes, shell->number_of_commands);
		}
		tmp_cmd = tmp_cmd->next;
		pipes->i++;
	}
}

void	repl(t_shell *shell)
{
	while (1)
	{
		read_input(shell);
		parse(shell);
		// if (pipe_check(shell))
		// 	exec_pipes(shell);
		// else
		// 	execute(shell);
		// get_pipes(shell);
		debug_commands(shell->command_groups);
		exec_p(shell);
		//handle signals
		// if (shell->flag)
		// else
		// printf("\n%d\n", shell->number_of_commands);
		// debug(shell->token, shell->number_of_commands)
		// printf("%s\n", shell->input);
	}
}