/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 16:21:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/30 13:49:24 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <sys/time.h>

uint64_t	now_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((uint64_t)time.tv_sec * 1000
		+ (uint64_t)time.tv_usec / 1000);
}

t_bool	wait_phase(t_coder *coder, uint64_t duration_ms)
{
	uint64_t	start;

	start = now_ms();
	while (monitoring_stoped(coder->monitoring) == FALSE)
	{
		if (now_ms() - start >= duration_ms)
			return (TRUE);
		usleep(1000);
	}
	return (FALSE);
}
