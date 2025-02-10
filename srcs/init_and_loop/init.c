/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/16 12:18:11 by marcsilv          #+#    #+#             */
/*   Updated: 2025/01/28 15:39:30 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	init_shell(t_shell *shell, char **env)
{
	shell->token = NULL;
	shell->anv = env;
	shell->env = NULL;
	shell->matrix = NULL;
	shell->input = NULL;
	shell->path = NULL;
	shell->flag = true;
	shell->number_of_commands = 0;
	shell->env = convert_env(env);						//converte env para lista ligada
	shell->path = get_path(shell->env);					//pega o valor da variavel PATH
	shell->result = 0;
	shell->current_dir = NULL;
}
