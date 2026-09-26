/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/17 15:48:59 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "error.h"

static const char	*argument_name(int position)
{
	if (position == 1)
		return ("number_of_coders");
	if (position == 2)
		return ("time_to_burnout");
	if (position == 3)
		return ("time_to_compile");
	if (position == 4)
		return ("time_to_debug");
	if (position == 5)
		return ("time_to_refactor");
	if (position == 6)
		return ("number_of_compiles_required");
	if (position == 7)
		return ("dongle_cooldown");
	if (position == 8)
		return ("scheduler");
	return ("unknown");
}

static const char	*argument_error(t_error error_id)
{
	if (error_id == ERR_EMPTY)
		return (ERR_EMPTY_MSG);
	if (error_id == ERR_NEGATIVE)
		return (ERR_NEGATIVE_MSG);
	if (error_id == ERR_NUMBER)
		return (ERR_NUMBER_MSG);
	if (error_id == ERR_RANGE)
		return (ERR_RANGE_MSG);
	if (error_id == ERR_SCHEDULER)
		return (ERR_SCHEDULER_MSG);
	if (error_id == ERR_MEMORY)
		return (ERR_MEMORY_MSG);
	if (error_id == ERR_NULL)
		return (ERR_NULL_MSG);
	if (error_id == ERR_ARG_COUNT)
		return (ERR_ARG_COUNT_MSG);
	if (error_id == ERR_ZERO)
		return (ERR_ZERO_MSG);
	return ("unknown error");
}

static const char	*runtime_error(t_error error_id)
{
	if (error_id == ERR_THREAD)
		return (ERR_THREAD_MSG);
	if (error_id == ERR_MUTEX)
		return (ERR_MUTEX_MSG);
	if (error_id == ERR_ALLOC_OVERFLOW)
		return (ERR_ALLOC_OVERFLOW_MSG);
	if (error_id == ERR_THREAD_JOIN)
		return (ERR_THREAD_JOIN_MSG);
	if (error_id == ERR_MUTEX_DESTROY)
		return (ERR_MUTEX_DESTROY_MSG);
	if (error_id == ERR_COND)
		return (ERR_COND_MSG);
	if (error_id == ERR_HEAP_FULL)
		return (ERR_HEAP_FULL_MSG);
	if (error_id == ERR_HEAP_EMPTY)
		return (ERR_HEAP_EMPTY_MSG);
	if (error_id == ERR_COUNT)
		return (ERR_COUNT_MSG);
	return ("unknown error");
}

t_bool	print_error(t_error error_id, int arg_position)
{
	const char	*message;

	if (error_id <= ERR_ZERO)
		message = argument_error(error_id);
	else
		message = runtime_error(error_id);
	if (arg_position > 0)
		fprintf(stderr, "ERROR %d: %s (argument: %d: %s)\n",
			error_id, message, arg_position, argument_name(arg_position));
	else
		fprintf(stderr, "ERROR %d: %s\n", error_id, message);
	return (FALSE);
}
