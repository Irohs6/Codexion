/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:56:33 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/08 14:15:01 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "parser.h"
#include "codexion.h"
#include "error.h"

int	main(int argc, char **argv)
{
	t_config		config;
	pthread_mutex_t	log_mutex;
	t_bool			status;

	if (argc != 9)
	{
		print_error(ERR_ARG_COUNT, 0);
		return (EXIT_FAILURE);
	}
	if (parse(argc, argv, &config) == FALSE)
		return (EXIT_FAILURE);
	if (pthread_mutex_init(&log_mutex, NULL) != 0)
	{
		print_error(ERR_MUTEX, 0);
		return (EXIT_FAILURE);
	}
	status = run_codexion(&config, &log_mutex);
	if (pthread_mutex_destroy(&log_mutex) != 0)
		status = print_error(ERR_MUTEX_DESTROY, 0);
	if (status == FALSE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
