/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:22:46 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/18 07:36:28 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

int	main(int ac, char **av, char **environment)
{
	t_shell	shell;

	if (ac != 1)
		print_error("Error: too many arguments", NULL);
	init_shell(&shell, environment);
	repl(&shell);
	clean_or_exit(&shell, true);
	(void)av;
	return (0);
}
