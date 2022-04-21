/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: msousa <mlrcbsousa@gmail.com>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/29 15:16:34 by msousa            #+#    #+#             */
/*   Updated: 2022/04/21 11:32:37 by msousa           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "libft.h"
# include "ft_mlx.h"
# include "ft_point.h"

/* Constants */
# define WIDTH 1000
# define HEIGHT 1000

/* Enums */

/* Structs & Types */
typedef struct s_app	t_app;

struct	s_app
{
	t_image		*img;
	void		*mlx_window;
	void		*mlx;
};

/* Functions */

/* mlx */
void	draw(t_app *self);
void	set_hooks(t_app *self);

/* helpers */

/* test */


#endif
