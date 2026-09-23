/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:20 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/23 16:10:00 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <string.h>

#include "parser.h"
#include "config.h"
#include "error.h"
#include "codexion.h"
#include "memory_manager.h"
#include "heap.h"

static int	check(int condition, const char *name)
{
	if (!condition)
	{
		printf("[FAIL] %s\n", name);
		return (1);
	}
	printf("[ OK ] %s\n", name);
	return (0);
}

static void	free_heap(t_heap **heap)
{
	while (heap && *heap)
		ft_lstpop(heap);
}

static int	test_parser(char **argv, t_config *config)
{
	char	*invalid_argv[9];

	if (check(parse(9, argv, config) == 1,
			"parser accepte une configuration valide"))
		return (1);
	memcpy(invalid_argv, argv, sizeof(invalid_argv));
	invalid_argv[8] = (char *)"invalid";
	return (check(parse(9, invalid_argv, config) == -1,
			"parser refuse un scheduler invalide"));
}

static int	test_heap(t_memory_manager *manager)
{
	t_heap	*heap;

	heap = NULL;
	manager->array_coder[0].deadline = 30;
	manager->array_coder[1].deadline = 10;
	if (check(ft_heap_init(&heap, "fifo", &manager->array_coder[0]) == 0,
			"heap accepte FIFO"))
		return (1);
	if (check(ft_heap_init(&heap, "fifo", &manager->array_coder[1]) == 0
			&& heap->coder == &manager->array_coder[0],
			"heap conserve l'ordre FIFO"))
		return (1);
	free_heap(&heap);
	if (check(ft_heap_init(&heap, "edf", &manager->array_coder[0]) == 0,
			"heap accepte EDF"))
		return (1);
	if (check(ft_heap_init(&heap, "edf", &manager->array_coder[1]) == 0
			&& heap->coder == &manager->array_coder[1],
			"heap trie EDF par deadline"))
		return (1);
	if (check(ft_heap_init(&heap, "unknown", &manager->array_coder[0]) == -1,
			"heap refuse un scheduler invalide"))
		return (1);
	free_heap(&heap);
	return (0);
}

static void	join_and_destroy(t_memory_manager *manager, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(manager->array_coder[i].thread, NULL);
		pthread_mutex_destroy(&manager->array_dongle[i].mutex);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_config			config;
	t_memory_manager	manager;
	int					failed;

	if (argc != 9)
		return (print_error(4, ERR_ARG_COUNT_MSG, 0), 1);
	failed = test_parser(argv, &config);
	if (check(memory_manager_init(NULL, config.number_of_coders) == -1,
			"memory manager refuse NULL"))
		failed = 1;
	if (memory_manager_init(&manager, config.number_of_coders) == -1)
		return (1);
	if (check(init_codexion(&manager, &config) == 0,
			"codexion initialise mutexes et threads"))
		failed = 1;
	else
	{
		if (test_heap(&manager))
			failed = 1;
		join_and_destroy(&manager, config.number_of_coders);
	}
	free_memory_manager(&manager);
	printf("%s\n", failed ? "Tests en echec." : "Tous les tests sont passes.");
	return (failed);
}