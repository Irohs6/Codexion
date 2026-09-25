/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heap.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/23 14:33:08 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/25 21:21:53 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEAP_H
# define HEAP_H

# include <stdint.h>

typedef struct s_coder	t_coder;

typedef struct s_request
{
	t_coder		*coder;
	uint64_t	arrival_order;
	uint64_t	deadline;
}	t_request;

typedef struct s_heap
{
	t_request	requests[2];
	int			size;
}	t_heap;

int			heap_push(t_heap *heap, t_request request, const char *sheduler);
int			heap_pop(t_heap *heap);
t_request	get_heap_first(t_heap *heap);
#endif // HEAP_H