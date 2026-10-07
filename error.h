/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 15:55:43 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include <stdio.h>
# include "utils.h"

# define ERR_ARG_COUNT_MSG "invalid number of arguments"
# define ERR_EMPTY_MSG "empty string provided"
# define ERR_NEGATIVE_MSG "negative numbers are not allowed"
# define ERR_NUMBER_MSG "invalid number"
# define ERR_RANGE_MSG "number out of range"
# define ERR_SCHEDULER_MSG "expected fifo or edf"
# define ERR_MEMORY_MSG "memory allocation failed"
# define ERR_THREAD_MSG "thread creation failed"
# define ERR_MUTEX_MSG "mutex initialization failed"
# define ERR_COND_MSG "condition variable initialization failed"
# define ERR_ZERO_MSG "zero is not allowed"
# define ERR_ALLOC_OVERFLOW_MSG "allocation size overflow"
# define ERR_NULL_MSG "null pointer provided"

# define ERR_THREAD_JOIN_MSG "thread join failed"
# define ERR_MUTEX_DESTROY_MSG "mutex destruction failed"
# define ERR_HEAP_FULL_MSG "request heap is full"
# define ERR_HEAP_EMPTY_MSG "request heap is empty"
# define ERR_COUNT_MSG "resource count must be positive"
# define MSG_BURNOUT "burned out"

typedef enum e_error
{
	ERR_EMPTY = 1,
	ERR_NEGATIVE = 2,
	ERR_NUMBER = 3,
	ERR_RANGE = 4,
	ERR_SCHEDULER = 5,
	ERR_MEMORY = 6,
	ERR_NULL = 7,
	ERR_ARG_COUNT = 8,
	ERR_ZERO = 9,
	ERR_THREAD = 10,
	ERR_MUTEX = 11,
	ERR_ALLOC_OVERFLOW = 12,
	ERR_THREAD_JOIN = 13,
	ERR_MUTEX_DESTROY = 14,
	ERR_COND = 15,
	ERR_HEAP_FULL = 16,
	ERR_HEAP_EMPTY = 17,
	ERR_COUNT = 18
}	t_error;

t_bool	print_error(t_error error_id, int arg_position);

#endif
