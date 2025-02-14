/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_error.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welepy <welepy@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 16:56:42 by welepy            #+#    #+#             */
/*   Updated: 2025/02/14 22:33:19 by welepy           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../libft.h"

void	print_error(char *error_message, void *data)
{
	ft_putstr_fd("Error: ", 2);
	ft_putendl_fd(error_message, 2);
	(void)data;
	exit(1);
}
