/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 16:21:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/30 13:49:24 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static t_bool	cooldown_ready(t_coder *coder)
{
	uint64_t	now;

	now = now_ms();
	if (now - coder->dongle_1->last_release_time
		< (uint64_t)coder->config->dongle_cooldown)
		return (FALSE);
	if (now - coder->dongle_2->last_release_time
		< (uint64_t)coder->config->dongle_cooldown)
		return (FALSE);
	return (TRUE);
}

t_bool	take_dongle(t_coder *coder)
{
	t_request	request;
	t_request	request2;

	if (coder->dongle_1->id == coder->dongle_2->id)
		return (FALSE);
	pthread_mutex_lock(&coder->dongle_1->mutex);
	pthread_mutex_lock(&coder->dongle_2->mutex);
	request = get_heap_first(&coder->dongle_1->heap);
	request2 = get_heap_first(&coder->dongle_2->heap);
	if (request.coder != coder || request2.coder != coder
		|| coder->dongle_1->is_available == FALSE
		|| coder->dongle_2->is_available == FALSE
		|| cooldown_ready(coder) == FALSE)
	{
		pthread_mutex_unlock(&coder->dongle_1->mutex);
		pthread_mutex_unlock(&coder->dongle_2->mutex);
		return (FALSE);
	}
	coder->dongle_1->is_available = FALSE;
	coder->dongle_2->is_available = FALSE;
	heap_pop(&coder->dongle_1->heap);
	heap_pop(&coder->dongle_2->heap);
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	pthread_mutex_unlock(&coder->dongle_2->mutex);
	return (TRUE);
}
