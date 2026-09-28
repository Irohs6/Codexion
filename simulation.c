/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/26 17:24:02 by iroh              #+#    #+#             */
/*   Updated: 2026/09/26 18:55:52 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "log.h"

void	debug(t_coder *coder)
{
	display_log(coder->log_mutex, coder, MSG_DEBUG);
	usleep(coder->config->time_to_debug * 1000);
}

void	refactor(t_coder *coder)
{
	display_log(coder->log_mutex, coder, MSG_REFACTOR);
	usleep(coder->config->time_to_refactor * 1000);
}
