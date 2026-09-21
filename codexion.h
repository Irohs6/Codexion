/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 15:27:00 by iroh              #+#    #+#             */
/*   Updated: 2026/09/21 16:44:00 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdint.h>
# include <pthread.h>
# include "config.h"

typedef struct s_coder
{
	int				id;
	int				nb_compile;
	uint64_t		last_time_compile_start;
	pthread_t		thread;
	const t_config	*config;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	uint64_t		last_release_time;
	pthread_mutex_t	mutex;
	const t_config	*config;
}	t_dongle;

struct	s_memory_manager;

int	init_codexion(struct s_memory_manager *manager, const t_config *config);
int	start_coders(t_memory_manager *manager, int count);

#endif
