/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 14:31:43 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/23 15:42:00 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "heap.h"

static void	ft_lst_add_back(t_heap **heap, t_heap *new)
{
	t_heap	*tmp;

	if (!heap || !new)
		return ;
	if (!*heap)
	{
		*heap = new;
		return ;
	}
	tmp = *heap;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

static void	ft_lst_add_front(t_heap **heap, t_heap *new)
{
	if (!heap || !new)
		return ;
	new->next = *heap;
	*heap = new;
}


static void	ft_lst_add_edf(t_heap **heap, t_heap *new_node)
{
	t_heap	*current;

	if (!*heap || new_node->coder->deadline < (*heap)->coder->deadline)
	{
		ft_lst_add_front(heap, new_node);
		return ;
	}
	current = *heap;
	while (current->next && current->next->coder->deadline
		<= new_node->coder->deadline)
		current = current->next;
	new_node->next = current->next;
	current->next = new_node;
}

void	ft_lstpop(t_heap **heap)
{
	t_heap	*tmp;

	if (!heap || !*heap)
		return ;
	tmp = *heap;
	*heap = (*heap)->next;
	free(tmp);
}

int	ft_heap_init(t_heap **heap, char *scheduler, t_coder *coder)
{
	t_heap	*new;

	if (!heap || !scheduler || !coder)
		return (-1);
	new = ft_calloc(1, sizeof(t_heap));
	if (!new)
		return (-1);
	new->coder = coder;
	new->next = NULL;
	if (strcmp(scheduler, "edf") == 0)
		ft_lst_add_edf(heap, new);
	else if (strcmp(scheduler, "fifo") == 0)
		ft_lst_add_back(heap, new);
	else
	{
		free(new);
		return (-1);
	}
	return (0);
}
