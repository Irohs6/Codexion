/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:32:40 by iroh              #+#    #+#             */
/*   Updated: 2026/09/20 16:56:11 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	init_codexion(t_memory_manager *manager, const t_config *config)
{
	if (!manager || !config)
		return (-1);
	manager->array_coder->compile_required = config->nb_of_cp_required;
	manager->array_coder->time_to_burnout = config->time_to_burnout;
	manager->array_coder->time_to_compile = config->time_to_compile;
	manager->array_coder->time_to_debug = config->time_to_debug;

	return (0);
}