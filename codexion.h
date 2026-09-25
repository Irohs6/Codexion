/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 15:27:00 by iroh              #+#    #+#             */
/*   Updated: 2026/09/25 20:19:45 by iroh             ###   ########.fr       */
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

typedef struct s_dongle	t_dongle;
struct					s_memory_manager;

typedef struct s_coder
{
	int				id;
	int				nb_compile;
	uint64_t		last_time_compile_start;
	uint64_t		deadline;
	uint64_t		start_time;
	pthread_t		thread;
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

int			init_codexion(struct s_memory_manager *manager,
				const t_config *config);
int			create_threads(struct s_memory_manager *manager, int nb_coder);
int			init_mutexes(struct s_memory_manager *manager, int nb_dongle);
uint64_t	now_ms(void);
void		init_start_time(struct s_memory_manager *manager, int count);
void		compile(t_coder *coder);
void		*start_coder_thread(void *arg);

#endif
