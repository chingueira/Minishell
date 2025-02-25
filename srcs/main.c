/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:16:24 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/25 10:07:41 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../inc/minihell.h"

void	debug(t_token *token, int number_of_commands)
{
	t_token	*head;

	head = token;
	while (head)
	{
		// printf("value: %s, type: %s\n", token->value, token_kind_string(token->type));
		// token = token->next;
		// if (head->type == COMMAND || head->type == ARGUMENT || head->type == VARIABLE)
		printf("%s, type: %s\n", head->value, token_kind_string(head->type));
		// else
		// 	printf("type: %s\n", token_kind_string(head->type));
		head = head->next;
	}
	printf("Number of commands: %d\n", number_of_commands);
}

void	init_shell(t_shell *shell, char **env)
{
	shell->input = NULL;
	// shell->path = NULL;
	shell->ev = env;
	shell->array = NULL;
	shell->flag = true;
	shell->num_of_cmds = 0;
	shell->env = convert_env(env);
	shell->path = get_path(shell->env);
	shell->token = NULL;
	shell->pipe = NULL;
}

void	signal_handler(int sig)
{
	if (sig == SIGINT)
	{
		rl_replace_line("", 0);
		rl_on_new_line();
		printf("┌[%s] - [%s]\n", getenv("USER"), getcwd(NULL, 0));
		rl_redisplay();
	}
}

static bool	read_input(t_shell *shell)
{
	char	*line;

	printf("┌[%s] - [%s]\n", getenv("USER"), getcwd(NULL, 0));
	line = readline("└[$] ");
	if (!line)
	{
		ft_fprintf(2, "exit\n");
		exit(0);
	}
	if (*line)
		add_history(line);
	if (all_spaces(line))
	{
		ft_free(&line);
		printf("┌[%s] - [%s]\n", getenv("USER"), getcwd(NULL, 0));
		rl_redisplay();
		return (false);
	}
	shell->input = ft_strtrim(line, " \t\n");
	ft_free(&line);
	return (true);
}

void	repl(t_shell *shell)
{
	while(1)
	{
		signal(SIGQUIT, SIG_IGN);
		signal(SIGINT, signal_handler);
		if (!read_input(shell))
			continue ;
		parse(shell);
		if (shell->flag)
			execute_pipe2(shell, shell->token);
		// debug(shell->token, shell->num_of_cmds);
		// printf("\n");
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	*shell;
	
	if (ac == 1)
	{
		shell = safe_malloc(sizeof(t_shell));
		init_shell(shell, env);
		repl(shell);
		free(shell);
	}
	else
		ft_fprintf(2, "%s do not receive argument!\n", av[0]);
	return (0);
}
