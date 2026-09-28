/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 19:03:49 by iroh              #+#    #+#             */
/*   Updated: 2026/09/28 14:06:21 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

t_bool	cooldown_ready(t_coder *coder)
{
	uint64_t	now;

	now = now_ms();
	if (now - coder->dongle_1->last_release_time
		< (uint64_t)coder->config->dongle_cooldown)
		return (FALSE);
	if (now - coder->dongle_2->last_release_time
		< (uint64_t)coder->config->dongle_cooldown)
		return (FALSE);
	return (TRUE);
}
