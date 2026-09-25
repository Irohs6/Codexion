/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 14:31:43 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/25 22:07:13 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string.h>

#include "heap.h"

int	heap_push(t_heap *heap, t_request request, const char *scheduler)
{
	t_request		temp;

	if (!heap || heap->size >= 2)
		return (-1);
	heap->requests[heap->size] = request;
	heap->size++;
	if (strcmp(scheduler, "edf") == 0)
	{
		if (heap->size == 2
			&& heap->requests[0].deadline > heap->requests[1].deadline)
		{
			temp = heap->requests[0];
			heap->requests[0] = heap->requests[1];
			heap->requests[1] = temp;
		}
	}
	return (0);
}

int	heap_pop(t_heap *heap)
{
	if (!heap || heap->size == 0)
		return (-1);
	if (heap->size == 2)
		heap->requests[0] = heap->requests[1];
	heap->size--;
	return (0);
}

t_request	get_heap_first(t_heap *heap)
{
	if (!heap || heap->size == 0)
		return ((t_request){0});
	return (heap->requests[0]);
}
