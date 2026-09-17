/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/17 15:46:46 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_H
# define ERROR_H

# include <stdio.h>

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

int	print_error(int error_id, const char *str, int arg_position);

#endif
