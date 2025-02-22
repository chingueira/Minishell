/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:16:24 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/22 16:03:47 by mchingi          ###   ########.fr       */
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

void	repl(t_shell *shell)
{
	while(1)
	{
		shell->input = readline("minihell> ");
		if (!shell->input)
			break ;
		add_history(shell->input);	
		parse(shell);
		// debug(shell->token, shell->num_of_cmds);
		// printf("\n");
		execute_pipe2(shell, shell->token);
	}
}

int	main(int ac, char **av, char **env)
{
	t_shell	*shell;
	
	if (ac == 1)
	{
		shell = malloc(sizeof(t_shell));
		init_shell(shell, env);
		repl(shell);
		free(shell);
	}
	else
		printf("%s do not receive argument!\n", av[0]);
	return (0);
}
