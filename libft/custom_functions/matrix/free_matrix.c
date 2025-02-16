/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_matrix.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/11 14:30:54 by marcsilv          #+#    #+#             */
/*   Updated: 2025/02/16 18:49:49 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

void	free_matrix(char **matrix)
{
	int	i;

	i = 0;
	while (matrix[i])
	{
		ft_free(&matrix[i]);
		i++;
	}
	if (matrix)
	{
		free(matrix);
		matrix = NULL;
	}
}
