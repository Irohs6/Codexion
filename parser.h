/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gacattan <gacattan@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/14 14:27:02 by gacattan          #+#    #+#             */
/*   Updated: 2026/09/15 16:52:36 by gacattan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PARSER_H
# define PARSER_H

int		ft_is_number_valid(char *str, int arg_position);
int		ft_is_str_valid(char *str, int arg_position);
int		ft_convert_number(char *str, int arg_position);

#endif // PARSER_H