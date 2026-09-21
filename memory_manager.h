/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_manager.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 12:41:02 by iroh              #+#    #+#             */
/*   Updated: 2026/09/21 14:05:58 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_MANAGER_H
# define MEMORY_MANAGER_H

# include <stdlib.h>
# include "codexion.h"

typedef struct s_memory_manager
{
	t_coder		*array_coder;
	t_dongle	*array_dongle;
}	t_memory_manager;

int	memory_manager_init(t_memory_manager *manager, int nb_coder);
int	free_memory_manager(t_memory_manager *manager);

#endif // MEMORY_MANAGER_H