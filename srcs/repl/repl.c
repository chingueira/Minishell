/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:19:22 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/14 23:03:48 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

static void	parse(t_shell *shell)
{
	shell->matrix = split_input(shell->input, shell);
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

void	get_pipes(t_shell *shell)
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
			tmp_cmd->args[i++] = clean_string(tmp_token->value);
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


void	repl(t_shell *shell)
{
	while (1)
	{
		read_input(shell);
		parse(shell);
		// if (pipe_check(shell))
		// 	exec_pipe(shell);
		// else
		execute(shell);
		// debug(shell->token, shell->number_of_commands)
	}
}