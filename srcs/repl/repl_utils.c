/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   repl_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 22:52:26 by welepy            #+#    #+#             */
/*   Updated: 2025/02/17 15:32:15 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	pipe_check(t_shell *shell)
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

void	execute(t_shell *shell)
{
	t_token *tmp;

	tmp = shell->token;
	if (is_builtin(tmp->type))
		exec_builtins(shell);
	else if (tmp->type == COMMAND)
		exec_cmd(shell);
	ft_free(&shell->input);

}
