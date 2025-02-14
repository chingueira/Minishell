/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:18:11 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/14 23:19:29 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_shell(t_shell *shell, char **env)
{
	shell->token = NULL;
	shell->anv = env;
	shell->env = NULL;
	shell->matrix = NULL;
	shell->input = NULL;
	shell->path = NULL;
	shell->flag = true;
	shell->command_groups = NULL;
	shell->number_of_commands = 0;
	shell->env = convert_env(env);
	shell->path = get_path(shell->env);
	shell->result = 0;
	shell->current_dir = NULL;
}
