/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring_checks.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:57:08 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 15:57:10 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "monitoring.h"
#include "codexion.h"

t_bool	all_compiles_done(t_coder *array_coder, int count)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&array_coder[0].monitoring->mutex);
	while (i < count)
	{
		if (array_coder[i].nb_compile
			< array_coder[i].config->nb_of_cp_required)
		{
			pthread_mutex_unlock(&array_coder[0].monitoring->mutex);
			return (FALSE);
		}
		i++;
	}
	pthread_mutex_unlock(&array_coder[0].monitoring->mutex);
	return (TRUE);
}

int	check_all_deadline(t_coder *array_coder, int nb_coder)
{
	int	i;

	i = 0;
	pthread_mutex_lock(&array_coder[0].monitoring->mutex);
	while (i < nb_coder)
	{
		if (now_ms() >= array_coder[i].last_time_compile_start
			+ array_coder[i].config->time_to_burnout)
		{
			pthread_mutex_unlock(&array_coder[0].monitoring->mutex);
			return (array_coder[i].id);
		}
		i++;
	}
	pthread_mutex_unlock(&array_coder[0].monitoring->mutex);
	return (FALSE);
}
