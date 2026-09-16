/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/16 19:18:38 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "utils.h"
#include "error.h"

int	main(int argc, char **argv)
{
	int	i;
	int error;

	if (argc == 9)
	{
		i = 1;
		while (i != argc - 1)
		{
			error = ft_is_number_valid(argv[i], i);
			if (error == 0)
				return (1);
			printf("Converted Number: %d\n", ft_convert_number(argv[i], i));
			i++;
		}
		error = ft_is_str_valid(argv[i], i);
		if (error == 0)
			return (1);
		printf ("String: %s", argv[i]);
	}
	else
		print_error(4, ERR_ARG_COUNT_MSG, 0);
		return (1);
	return (0);
}
