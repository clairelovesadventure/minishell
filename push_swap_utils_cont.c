#include <push_swap.h>

void	*ft_calloc(size_t count, size_t size)
{
	unsigned char	*temp;
	size_t			i;

	temp = malloc(count * size);
	if (!temp)
		return (NULL);
	i = 0;
	while (i < count * size)
	{
		temp[i] = 0;
		i++;
	}
	return ((void *)temp);
}

char	*ft_strdup(const char *src)
{
	int		org_size;
	char	*dup;
	size_t	i;

	org_size = ft_strlen(src);
	dup = (char *)malloc(sizeof(char) * (org_size + 1));
	if (!dup)
		return (NULL);
	i = 0;
	while (src[i])
	{
		dup[i] = src[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

int ft_strlen(const char *str)
{
    int i;

    i = 0;
    while (str[i])
        i++;
    return (i);
}

char	*ft_substr(char const *string, unsigned int start, size_t len)
{
	char	*sub_str;
	size_t	string_len;
	size_t	i;

	if (!string)
		return (NULL);
	string_len = ft_strlen(string);
	if (start >= string_len)
		return (ft_strdup(""));
	if (len > string_len - start)
		len = string_len - start;
	sub_str = ft_calloc(len + 1, sizeof(char));
	if (!sub_str)
		return (NULL);
	i = 0;
	while (i < len)
	{
		sub_str[i] = string[start + i];
		i++;
	}
	return (sub_str);
}