#include "philo.h"

short int check_params(char **argv)
{
    int i;
    int j;

    i = 1;
    while(argv[i] != NULL)
    {
        j = 0;
        while(argv[i][j] != '\0')
        {
            if(argv[i][j] < '0' || argv[i][j] > '9')
                return (1);
            j++;
        }
        i++;    
    }
    return (0);
}

int	ft_atoi(const char *str)
{
	int	ret;
	int	sign;
	int	i;

	i = 0;
	ret = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		ret = (ret * 10) + str[i] - '0';
		i++;
	}
	return (ret * sign);
}

