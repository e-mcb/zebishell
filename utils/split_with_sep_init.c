#include "../includes/minishell.h"

void	init_splitter(t_splitter *splitter)
{
	splitter->i = 0;
	splitter->start = 0;
	splitter->count = 0;
	splitter->capacity = 4;
}
