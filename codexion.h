/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 15:27:00 by iroh              #+#    #+#             */
/*   Updated: 2026/09/23 15:56:13 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdint.h>
# include <pthread.h>
typedef struct s_dongle	t_dongle;

# include "config.h"

typedef struct s_coder
{
	int				id;
	int				nb_compile;
	uint64_t		last_time_compile_start;
	uint64_t		deadline;
	pthread_t		thread;
	const t_config	*config;
	t_dongle		*dongle_1;
	t_dongle		*dongle_2;
} t_coder;

struct s_dongle
{
	int				id;
	uint64_t		last_release_time;
	pthread_mutex_t	mutex;
	const t_config	*config;
};

struct	s_memory_manager;

int	init_codexion(struct s_memory_manager *manager, const t_config *config);
int	create_threads(struct s_memory_manager *manager, int nb_coder);
int	init_mutexes(struct s_memory_manager *manager, int nb_dongle);

#endif
