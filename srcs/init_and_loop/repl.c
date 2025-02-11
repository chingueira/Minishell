/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:19:22 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/11 11:55:28 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static bool	is_builtin(t_type type);

static void	read_input(t_shell *shell)
{
	shell->input = ft_strtrim(readline("minishell$ "), " ");		
	if (!shell->input)
	{
		fprintf(stderr, "Error: Empty prompt\n");
		shell->flag = false;
	}
	add_history(shell->input);						//adiciona input ao historico
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
		fprintf(stderr, "Error: Failed to parse input");
		shell->flag = false;
	}
	expand(&shell->matrix, shell->env);					//expande variaveis de ambiente
	shell->token = tokenize_matrix(shell->matrix);
	if (!shell->token)
	{
		// fprintf(stderr, "Error: Failed to tokenize input\n");
		shell->flag = false;
		return ;
	}
	identify_tokens(shell->token, shell->path);				//identifica os tokens
	token_sequence(shell->token);						//verifica a sequencia dos tokens
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

// -------------- Talvez será eliminada! ----------------/

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
	{
		printf("tá fodido\n");
		exit (0);
	}
	if (id == 0)
		if (execve(pat, options, shell->anv) == -1)
			printf("tá bem fodido!\n");
	waitpid(id, NULL, 0);
}

void	execute(t_shell *shell)
{
		t_token *tmp;

		tmp = shell->token;
		// if (is_command(tmp->value, shell->path))
		// 	exec_cmd(shell);
		// else
		// 	exec_builtins(shell);
		if (is_builtin(tmp->type))
			exec_builtins(shell);
		else if (tmp->type == COMMAND)
			exec_cmd(shell);

}

// -----------------------------------------------------/

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


static bool	pipe_check(t_shell *shell)
{
	t_token *tmp = shell->token;
	while (tmp)
	{
		if (tmp->type == PIPE)
		{
			if (tmp->next->type != COMMAND)
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

void	repl(t_shell *shell)
{
	while (1)
	{
		read_input(shell);						//leitura do prompt
		parse(shell);
		if (pipe_check(shell))
			execute_pipe(shell);
		else
			execute(shell);
		//handle signals
		// if (shell->flag)
		// else
		// printf("\n%d\n", shell->number_of_commands);
		// debug(shell->token, shell->number_of_commands);						//debugar tokens
		// printf("%s\n", shell->input);
	}
}
