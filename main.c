/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/15 17:00:24 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "utils.h"

int	main(int argc, char **argv)
{
	int	i;

	if (argc == 9)
	{
		i = 1;
		while (i != argc - 1)
		{
			printf("Number: %s, Answer: %d\n", argv[i],
				ft_is_number_valid(argv[i], i));
			printf("Converted Number: %d\n", ft_convert_number(argv[i], i));
			i++;
		}
		ft_is_str_valid(argv[i], i);
		printf ("String: %s", argv[i]);
	}
	else
		print_error(4, "Invalid number of arguments", 0);
	return (0);
}
