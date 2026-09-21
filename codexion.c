/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/20 16:32:40 by iroh              #+#    #+#             */
/*   Updated: 2026/09/21 16:45:51 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "memory_manager.h"

int	init_codexion(t_memory_manager *manager, const t_config *config)
{
	int	i;

	if (!manager || !config || config->number_of_coders <= 0)
		return (-1);
	if (!manager->array_coder || !manager->array_dongle)
		return (-1);
	i = 0;
	while (i < config->number_of_coders)
	{
		manager->array_coder[i].id = i + 1;
		manager->array_coder[i].config = config;
		manager->array_dongle[i].id = i + 1;
		manager->array_dongle[i].config = config;
		i++;
	}
	return (0);
}

int		start_coders(t_memory_manager *manager, int count)
{

}
