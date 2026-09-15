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

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	if (!str)
		print_error(1, "NULL string provided", 0);
	while (str[i])
		i++;
	return (i);
}

void	print_error(int error_id, char *str, int arg_position)
{
	if (arg_position > 0)
		fprintf(stderr, "ERROR %d: %s ("
			"Arg position: %d)\n", error_id, str, arg_position);
	else
		fprintf(stderr, "ERROR %d: %s\n", error_id, str);
	exit (error_id);
}
