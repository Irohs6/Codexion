/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 14:39:48 by iroh              #+#    #+#             */
/*   Updated: 2026/10/02 17:10:53 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MONITORING_H
# define MONITORING_H

# include <pthread.h>
# include "utils.h"


struct	s_memory_manager;
struct	s_coder;

typedef struct s_monitoring
{
	t_bool			stop;
	pthread_mutex_t	mutex;
	pthread_t		monitor_thread;
}	t_monitoring;

t_bool	monitoring_stoped(t_monitoring *monitoring);
void	stop_monitoring(t_monitoring *monitoring);
t_bool	init_monitoring(t_monitoring *monitoring,
			struct s_memory_manager *manager);
int		check_all_deadline(struct s_coder *array_coder, int nb_coder);
t_bool	all_compiles_done(struct s_coder *array_coder, int count);

#endif // MONITORING_H