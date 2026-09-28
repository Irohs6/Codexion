/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/28 14:06:31 by iroh              #+#    #+#             */
/*   Updated: 2026/09/28 14:15:58 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <pthread.h>
#include "log.h"
#include "utils.h"
#include "codexion.h"

t_bool	display_log(pthread_mutex_t *mutex, t_coder *coder, const char *log)
{
	pthread_mutex_lock(mutex);
	printf("%llu %d %s\n",
		(unsigned long long)(now_ms() - coder->start_time),
		coder->id, log);
	pthread_mutex_unlock(mutex);
	return (TRUE);
}
