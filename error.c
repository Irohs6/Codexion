/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/16 19:14:33 by iroh             ###   ########.fr       */
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

int	print_error(int error_id, const char *str, int arg_position)
{
	if (arg_position > 0)
		fprintf(stderr, "ERROR %d: %s (argument: %d: %s)\n",
         error_id, str, arg_position, argument_name(arg_position));
	else
		fprintf(stderr, "ERROR %d: %s\n", error_id, str);
	return (0);
}
