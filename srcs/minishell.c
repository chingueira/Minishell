/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marcsilv@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 13:22:46 by marcsilv          #+#    #+#             */
/*   Updated: 2025/01/16 14:19:26 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/minishell.h"

int	main(int ac, char **av, char **environment)
{
	t_shell	shell;

	if (ac != 1)
		print_error("Error: too many arguments", NULL);
	init_shell(&shell, environment);					//inicializar variaveis da struct shell
	repl(&shell);								//loop infinito para ler input e tokenizar
	//clean_and_exit(&shell);
	(void)av;								//para evitar warning
	return (0);
}
