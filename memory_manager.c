/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_manager.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/18 12:40:55 by iroh              #+#    #+#             */
/*   Updated: 2026/09/21 14:40:36 by iroh             ###   ########.fr       */
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

static void	*ft_calloc(size_t nb_memb, size_t size)
{
	void	*ptr;
	size_t	total;

	if (size != 0 && nb_memb > (size_t)-1 / size)
	{
		print_error(6, ERR_ALLOC_OVERFLOW_MSG, 0);
		return (NULL);
	}
	total = nb_memb * size;
	ptr = malloc(total);
	if (!ptr)
	{
		print_error(6, ERR_MEMORY_MSG, 0);
		return (NULL);
	}
	ft_bzero(ptr, total);
	return (ptr);
}

int	memory_manager_init(t_memory_manager *manager, int nb_coder)
{
	if (!manager || nb_coder <= 0)
	{
		print_error(7, ERR_NULL_MSG, 0);
		return (-1);
	}
	// manager->array_coder = NULL;
	// manager->array_dongle = NULL;
	manager->array_coder = ft_calloc(nb_coder, sizeof(t_coder));
	if (!manager->array_coder)
	{
		return (-1);
	}
	manager->array_dongle = ft_calloc(nb_coder, sizeof(t_dongle));
	if (!manager->array_dongle)
	{
		free(manager->array_coder);
		manager->array_coder = NULL;
		return (-1);
	}
	return (0);
}

int	free_memory_manager(t_memory_manager *manager)
{
	if (!manager)
		return (-1);
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
	return (0);
}
