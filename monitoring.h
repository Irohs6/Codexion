/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 14:39:48 by iroh              #+#    #+#             */
/*   Updated: 2026/10/02 15:56:08 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITORING_H
# define MONITORING_H

# include <pthread.h>
# include "utils.h"

typedef struct s_monitoring
{
	t_bool			stop;
	pthread_mutex_t	mutex;
	pthread_t		monitor_thread;
}	t_monitoring;

#endif // MONITORING_H