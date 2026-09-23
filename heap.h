/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 14:33:08 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/23 15:42:36 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

# include "memory_manager.h"
# include "codexion.h"

typedef struct s_heap
{
	struct s_heap	*next;
	struct s_coder	*coder;
}	t_heap;

void	ft_lstpop(t_heap **heap);
int		ft_heap_init(t_heap **heap, char *scheduler, t_coder *coder);

#endif // HEAP_H