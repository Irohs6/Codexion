/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 16:21:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/25 22:07:43 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "memory_manager.h"
#include "heap.h"
#include <sys/time.h>

uint64_t	now_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((uint64_t)time.tv_sec * 1000
		+ (uint64_t)time.tv_usec / 1000);
}

static t_bool	take_dongle(t_coder *coder)
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
		|| !coder->dongle_1->is_available
		|| !coder->dongle_2->is_available)
	{
		pthread_mutex_unlock(&coder->dongle_1->mutex);
		pthread_mutex_unlock(&coder->dongle_2->mutex);
		return (FALSE);
	}
	coder->dongle_1->is_available = 0;
	coder->dongle_2->is_available = 0;
	heap_pop(&coder->dongle_1->heap);
	heap_pop(&coder->dongle_2->heap);
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	pthread_mutex_unlock(&coder->dongle_2->mutex);
	return (TRUE);
}

void	compile(t_coder *coder)
{
	if (!coder)
		return ;
	coder->last_time_compile_start = now_ms();
	printf("%llu %d has taken a dongle\n%llu %d has taken a dongle\n"
		"%llu %d is compiling\n",
		(unsigned long long)(coder->last_time_compile_start
			- coder->start_time), coder->id,
		(unsigned long long)(coder->last_time_compile_start
			- coder->start_time), coder->id,
		(unsigned long long)(coder->last_time_compile_start
			- coder->start_time), coder->id);
	usleep(coder->config->time_to_compile * 1000);
	pthread_mutex_lock(&coder->dongle_1->mutex);
	pthread_mutex_lock(&coder->dongle_2->mutex);
	coder->dongle_1->is_available = 1;
	coder->dongle_2->is_available = 1;
	pthread_mutex_unlock(&coder->dongle_1->mutex);
	pthread_mutex_unlock(&coder->dongle_2->mutex);
	coder->nb_compile++;
}

void	*start_coder_thread(void *arg)
{
	t_coder		*coder;
	t_request	request;

	coder = arg;
	while (coder->nb_compile < coder->config->nb_of_cp_required)
	{
		request.coder = coder;
		request.deadline = coder->last_time_compile_start
			+ coder->config->time_to_burnout;
		heap_push(&coder->dongle_1->heap, request, coder->config->scheduler);
		heap_push(&coder->dongle_2->heap, request, coder->config->scheduler);
		while (take_dongle(coder) == FALSE)
			usleep(1000);
		compile(coder);
	}
	return (NULL);
}

void	init_start_time(t_memory_manager *manager, int count)
{
	uint64_t	start_time;
	int			i;

	start_time = now_ms();
	i = 0;
	while (i < count)
	{
		manager->array_coder[i].start_time = start_time;
		manager->array_coder[i].last_time_compile_start = start_time;
		i++;
	}
}
