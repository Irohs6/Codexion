/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:56:46 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/07 15:56:49 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "memory_manager.h"
#include "error.h"

static void	ft_bzero(void *s, size_t n)
{
	size_t	i;
	char	*temp;

	i = 0;
	temp = s;
	while (i < n)
	{
		temp[i] = 0;
		i++;
	}
}

void	*ft_calloc(size_t nb_memb, size_t size)
{
	void	*ptr;
	size_t	total;

	if (size != 0 && nb_memb > (size_t)-1 / size)
	{
		print_error(ERR_ALLOC_OVERFLOW, 0);
		return (NULL);
	}
	total = nb_memb * size;
	ptr = malloc(total);
	if (!ptr)
	{
		print_error(ERR_MEMORY, 0);
		return (NULL);
	}
	ft_bzero(ptr, total);
	return (ptr);
}

t_bool	memory_manager_init(t_memory_manager *manager, int nb_coder)
{
	if (!manager)
		return (print_error(ERR_NULL, 0));
	manager->array_coder = NULL;
	manager->array_dongle = NULL;
	if (nb_coder <= 0)
		return (print_error(ERR_COUNT, 0));
	manager->array_coder = ft_calloc(nb_coder, sizeof(t_coder));
	if (!manager->array_coder)
		return (FALSE);
	manager->array_dongle = ft_calloc(nb_coder, sizeof(t_dongle));
	if (!manager->array_dongle)
	{
		free(manager->array_coder);
		manager->array_coder = NULL;
		return (FALSE);
	}
	return (TRUE);
}

t_bool	free_memory_manager(t_memory_manager *manager)
{
	if (!manager)
		return (print_error(ERR_NULL, 0));
	if (manager->array_coder)
	{
		free(manager->array_coder);
		manager->array_coder = NULL;
	}
	if (manager->array_dongle)
	{
		free(manager->array_dongle);
		manager->array_dongle = NULL;
	}
	return (TRUE);
}
