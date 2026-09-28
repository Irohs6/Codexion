/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   requests.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 14:31:43 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/28 13:40:52 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "error.h"

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
		pthread_mutex_lock(&coder->dongle_2->mutex);
	status = push_requests(coder, request);
	if (coder->dongle_1 != coder->dongle_2)
		pthread_mutex_unlock(&coder->dongle_2->mutex);
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	return (status);
}
