#include "libft.h"
#include <stdio.h>

void smart_free(void *addr)
{
	if (addr)
	{
		if (!ft_strncmp("include", (char *)addr, 8))
			printf("msg\n");
		free(addr);
		addr = NULL;
	}
}

char *smart_join(char *s1, char *s2, void *freeable)
{
	char *ret;

	ret = ft_strjoin(s1, s2);
	smart_free(freeable);
	return (ret);
}

char *smart_dup(char *s1, void *freeable)
{
	char *ret;

	ret = ft_strdup(s1);
	smart_free(freeable);
	return ret;
}

void smart_free_strs(char **strs)
{
	int i;

	i = 0;
	while (strs[i])
	{
		smart_free(strs[i]);
		i++;
	}
	smart_free(strs);
}
