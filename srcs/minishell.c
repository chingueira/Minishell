/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:22:46 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/13 14:37:55 by welepy           ###   ########.fr       */
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
	//clean_and_exit(&shell);
	(void)av;
	return (0);
}
