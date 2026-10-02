/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 15:27:00 by iroh              #+#    #+#             */
/*   Updated: 2026/10/02 15:41:37 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdint.h>
# include <pthread.h>
# include <unistd.h>
# include "utils.h"
# include "config.h"
# include "heap.h"
# include "monitoring.h"

typedef struct s_dongle	t_dongle;
struct					s_memory_manager;

typedef struct s_coder
{
	int				id;
	int				nb_compile;
	t_bool			failed;
	uint64_t		last_time_compile_start;
	uint64_t		deadline;
	uint64_t		start_time;
	pthread_t		thread;
	pthread_mutex_t	*log_mutex;
	t_monitoring	*monitoring;
	const t_config	*config;
	t_dongle		*dongle_1;
	t_dongle		*dongle_2;
}	t_coder;

struct s_dongle
{
	int				id;
	t_bool			is_available;
	uint64_t		last_release_time;
	t_heap			heap;
	pthread_mutex_t	mutex;
	const t_config	*config;
};

t_bool		init_codexion(struct s_memory_manager *manager,
				const t_config *config, pthread_mutex_t *log_mutex);
t_bool		create_threads(struct s_memory_manager *manager,
				int nb_coder, pthread_mutex_t *log_mutex);
t_bool		init_mutexes(struct s_memory_manager *manager, int nb_dongle);
t_bool		register_requests(t_coder *coder);
t_bool		register_initial_requests(struct s_memory_manager *manager,
				int count);
uint64_t	now_ms(void);
void		*start_coder_thread(void *arg);

t_bool		run_codexion(const t_config *config, pthread_mutex_t *log_mutex);
t_bool		join_threads(struct s_memory_manager *manager, int count);
t_bool		destroy_mutexes(struct s_memory_manager *manager, int count);
t_bool		take_dongle(t_coder *coder);

#endif
