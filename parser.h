/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iroh <iroh@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:02 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/17 15:46:18 by iroh             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSER_H
# define PARSER_H

# include "config.h"

int		ft_is_number_valid(char *str, int arg_position);
int		ft_is_str_valid(char *str, int arg_position);
int		ft_convert_number(char *str, int arg_position);
int		parse(int argc, char **argv, t_config *config);

#endif // PARSER_H