/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin_free.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marcsilv <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 15:02:24 by marcsilv          #+#    #+#             */
/*   Updated: 2025/01/08 15:17:53 by marcsilv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

char *ft_strjoin_free(char *s1, char *s2)
{
    size_t len1 = ft_strlen(s1);
    size_t len2 = ft_strlen(s2);
    char *result = (char *)malloc(len1 + len2 + 1);  // Allocate space for the new string

    if (!result)
        return NULL;

    // Copy first string into result
    ft_strncpy(result, s1, len1);

    // Copy second string into result
    ft_strncpy(result + len1, s2, len2 + 1);  // +1 for the null terminator

    // Free the original strings
    //free(s1);  // You can free both strings or just the one that is no longer needed
    //free(s2);

    return result;
}

