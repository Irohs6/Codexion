/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 15:27:00 by iroh              #+#    #+#             */
/*   Updated: 2026/09/20 16:40:29 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <stdint.h>
# include <pthread.h>
# include "memory_manager.h"
# include "config.h"

typedef struct s_coder
{
	int			id;
	int			nb_compile;
	int			compile_required;
	uint64_t	time_to_compile;
	uint64_t	time_to_refactor;
	uint64_t	time_to_debug;
	uint64_t	time_to_burnout;
	uint64_t	last_time_compile_start;
	pthread_t	thread;
}	t_coder;

typedef struct s_dongle
{
	int				id;
	uint64_t		last_release_time;
	uint64_t		cooldown;
	pthread_mutex_t	mutex;
}	t_dongle;

int	init_codexion(t_memory_manager *manager, const t_config *config);

#endif
