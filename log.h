/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   log.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/07 15:56:22 by gacattan          #+#    #+#             */
/*   Updated: 2026/10/08 14:18:26 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOG_H
# define LOG_H

# include "codexion.h"

# define MSG_DONGLE "has taken a dongle"
# define MSG_COMPILE "is compiling"
# define MSG_DEBUG "is debugging"
# define MSG_REFACTOR "is refactoring"
# define MSG_BURNOUT "burned out"

t_bool	display_log(pthread_mutex_t *mutex, t_coder *coder, const char *log);
t_bool	display_compile_log(pthread_mutex_t *mutex, t_coder *coder);
#endif