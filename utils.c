/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/15 14:14:31 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/15 15:20:19 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"
#include "error.h"

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
	{
		print_error(ERR_NULL, 0);
		return (0);
	}
	while (str[i])
		i++;
	return (i);
}
