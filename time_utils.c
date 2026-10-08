/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:58:08 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/08 13:51:47 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/time.h>
#include "codexion.h"

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
		usleep(100);
	}
	return (FALSE);
}
