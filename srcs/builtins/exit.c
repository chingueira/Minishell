/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchingi <mchingi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/14 15:49:03 by mchingi           #+#    #+#             */
/*   Updated: 2025/02/15 16:34:22 by mchingi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../../inc/minihell.h"

// Claramente falta muito

void	ft_exit(t_shell *shell)
{
	printf("\nfrom builtins\n");
	free(shell);
	exit(EXIT_SUCCESS);
}