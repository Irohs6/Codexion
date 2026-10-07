/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:57:25 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 16:01:16 by gacattan         ###   ########.fr       */
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
	pthread_mutex_t	resource_mutex;
	pthread_cond_t	resource_cond;
}	t_monitoring;

t_bool	monitoring_stoped(t_monitoring *monitoring);
void	stop_monitoring(t_monitoring *monitoring);
t_bool	init_monitoring(t_monitoring *monitoring,
			struct s_memory_manager *manager);
int		check_all_deadline(struct s_coder *array_coder, int nb_coder);
t_bool	all_compiles_done(struct s_coder *array_coder, int count);

#endif // MONITORING_H