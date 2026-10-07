/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_manager.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:56:58 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 15:57:01 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MEMORY_MANAGER_H
# define MEMORY_MANAGER_H

# include <stdlib.h>
# include "codexion.h"

typedef struct s_memory_manager
{
	t_coder			*array_coder;
	t_dongle		*array_dongle;
	t_monitoring	*monitoring;
}	t_memory_manager;

void	*ft_calloc(size_t nb_memb, size_t size);
t_bool	memory_manager_init(t_memory_manager *manager, int nb_coder);
t_bool	free_memory_manager(t_memory_manager *manager);

#endif // MEMORY_MANAGER_H