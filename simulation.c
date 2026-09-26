/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 17:24:02 by iroh              #+#    #+#             */
/*   Updated: 2026/09/26 18:55:52 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "error.h"


void	debug(t_coder *coder)
{
	printf("%llu %d is debugging\n",
		(unsigned long long)now_ms() - coder->start_time, coder->id);
	usleep(coder->config->time_to_debug * 1000);
}

void	refactor(t_coder *coder)
{
	printf("%llu %d is refactoring\n",
		(unsigned long long)now_ms() - coder->start_time, coder->id);
	usleep(coder->config->time_to_refactor * 1000);
}
