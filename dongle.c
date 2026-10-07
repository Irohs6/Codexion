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

static uint64_t	cooldown_remaining(const t_config *config, t_dongle *dongle_1,
	t_dongle *dongle_2)
{
	uint64_t	last_release;
	uint64_t	elapsed;

	last_release = dongle_1->last_release_time;
	if (dongle_2->last_release_time > last_release)
		last_release = dongle_2->last_release_time;
	elapsed = now_ms() - last_release;
	if (elapsed >= (uint64_t)config->dongle_cooldown)
		return (0);
	return ((uint64_t)config->dongle_cooldown - elapsed);
}

static t_bool	cooldown_ready(t_coder *coder)
{
	if (cooldown_remaining(coder->config, coder->dongle_1, coder->dongle_2) > 0)
		return (FALSE);
	return (TRUE);
}

uint64_t	get_cooldown_wait(t_coder *coder)
{
	t_request	request_1;
	t_request	request_2;
	uint64_t	wait_time;

	wait_time = 0;
	pthread_mutex_lock(&coder->dongle_1->mutex);
	pthread_mutex_lock(&coder->dongle_2->mutex);
	request_1 = get_heap_first(&coder->dongle_1->heap);
	request_2 = get_heap_first(&coder->dongle_2->heap);
	if (request_1.coder == coder && request_2.coder == coder
		&& coder->dongle_1->is_available == TRUE
		&& coder->dongle_2->is_available == TRUE)
		wait_time = cooldown_remaining(coder->config, coder->dongle_1,
				coder->dongle_2);
	pthread_mutex_unlock(&coder->dongle_2->mutex);
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	return (wait_time);
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
