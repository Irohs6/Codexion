/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 14:31:43 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 15:57:50 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "error.h"
#include "memory_manager.h"

static t_bool	push_requests(t_coder *coder, t_request request)
{
	if (coder->dongle_1->heap.size >= 2 || coder->dongle_2->heap.size >= 2)
		return (print_error(ERR_HEAP_FULL, 0));
	if (heap_push(&coder->dongle_1->heap, request,
			coder->config->scheduler) == FALSE)
		return (FALSE);
	if (coder->dongle_1 != coder->dongle_2)
		return (heap_push(&coder->dongle_2->heap, request,
				coder->config->scheduler));
	return (TRUE);
}

t_bool	register_requests(t_coder *coder)
{
	t_request	request;
	t_bool		status;

	if (!coder || !coder->dongle_1 || !coder->dongle_2)
		return (print_error(ERR_NULL, 0));
	request.coder = coder;
	request.arrival_order = 0;
	request.deadline = coder->last_time_compile_start
		+ coder->config->time_to_burnout;
	pthread_mutex_lock(&coder->dongle_1->mutex);
	if (coder->dongle_1 != coder->dongle_2)
	{
		pthread_mutex_lock(&coder->dongle_2->mutex);
		status = push_requests(coder, request);
		pthread_mutex_unlock(&coder->dongle_2->mutex);
	}
	else
		status = push_requests(coder, request);
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	return (status);
}

t_bool	register_initial_requests(t_memory_manager *manager, int count)
{
	int	parity;
	int	i;

	if (!manager || !manager->array_coder)
		return (print_error(ERR_NULL, 0));
	parity = 0;
	while (parity < 2)
	{
		i = parity;
		while (i < count)
		{
			if (manager->array_coder[i].config->nb_of_cp_required > 0
				&& register_requests(&manager->array_coder[i]) == FALSE)
				return (FALSE);
			i += 2;
		}
		parity++;
	}
	return (TRUE);
}
