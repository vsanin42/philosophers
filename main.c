/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsanin <vsanin@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 14:48:04 by vsanin            #+#    #+#             */
/*   Updated: 2025/01/14 18:47:14 by vsanin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// check for intmax overflow is done in atoi
// check if atoi returning ERROR doesn't interfere with regular behaviour
// example: if argument 1 is perceived as ERROR, etc...
// maybe return -1 if it's problematic.
int	main(int argc, char **argv)
{
	if (check_args(argc, argv) == ERROR)
		return (ERROR);
	printf("args ok\n");
	return (0);
}
