#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
	int top;
	int *data;
} marks;

typedef struct
{
	char *lastname;
	int group;
	marks *marks;
} students;

typedef struct
{
	students *students;
	struct queue *next;
} queue;

typedef struct
{
	queue *t;
	queue *h;
	int *i;
} pointers;

char *input(char *str)
{
	int i = 0;
	fflush(stdin);
	str = malloc(1 * sizeof(char));
	do
	{
		str[i++] = getchar();
		str = realloc(str, (i + 1) * sizeof(char));
		if (str == NULL)
		{
			printf("Error");
			exit(1);
		}
	} while (str[i - 1] != '\n');
	str[i - 1] = '\0';
	return str;
}

marks *addmarks(marks *stack)
{
	stack->top = 0;
	stack->data = calloc(1, sizeof(int));
	fflush(stdin);
	char tmp;
	int flag;
	do
	{
		flag = scanf("%d", &stack->data[stack->top]);
		if (!flag)
		{
			printf("\nError");
			exit(1);
		}
		scanf("%c", &tmp);
		stack->top++;
		stack->data = realloc(stack->data, (stack->top + 1) * sizeof(int));
	} while (tmp != '\n');
	return stack;
}

pointers *add(pointers *ptr)
{
	queue *tmp;
	char l = 'y';
	static queue *var = NULL;
	while (l == 'y')
	{
		if (!(tmp = calloc(1, sizeof(queue))))
		{
			printf("Error");
			exit(1);
		}
		tmp->students = calloc(1, sizeof(students));
		printf("Enter students surname: \n");
		tmp->students->lastname = input(tmp->students->lastname);
		printf("Enter students group: \n");
		scanf("%d", &tmp->students->group);
		printf("Enter students marks(ex: 5 3 6 8): \n");
		tmp->students->marks = calloc(1, sizeof(marks));
		tmp->students->marks = addmarks(tmp->students->marks);
		(*(ptr->i))++;
		if (ptr->h == NULL)
		{
			ptr->h = ptr->t = tmp;
			ptr->h->next = ptr->t->next = tmp;
			var = tmp;
		}
		else
		{
			ptr->t = tmp;
			ptr->t->next = var;
			var = ptr->t;
		}
		printf("\nContinue? y/n\n");
		fflush(stdin);
		scanf("%c", &l);
	};
	return ptr;
}

void see(pointers *ptr)
{
	queue *tmp = NULL;
	int j = 0;
	tmp = ptr->t;
	while (j < *ptr->i)
	{
		printf("\n%s - %d", tmp->students->lastname, *ptr->i - j);
		tmp = tmp->next;
		j++;
	};
	return;
}

pointers *sort(pointers *ptr)
{
	queue *tmp1 = NULL;
	queue *tmp2 = NULL;
	void *num[100];
	void *tt;
	tmp1 = ptr->t;
	int d = 0;
	int type;
	printf("\nEnter by which type to sort: \n");
	printf("1)Surname\n");
	printf("2)Group\n");
	while (1)
	{
		fflush(stdin);
		scanf("%d", &type);
		if (type > 0 && type < 3)
		{
			break;
		}
	}
	while (d < *(ptr->i))
	{
		num[d] = tmp1;
		tmp1 = tmp1->next;
		d++;
	};
	int *temp = NULL;
	for (int i = 0; i < *(ptr->i); i++)
	{
		for (int j = *(ptr->i) - 1; j > i; j--)
		{
			tmp1 = num[j];
			tmp2 = num[j - 1];
			if (type == 1)
			{
				if (strcmp(tmp1->students->lastname, tmp2->students->lastname) < 0)
				{
					tt = num[j - 1];
					temp = tmp2;
					tmp2 = tmp1;
					num[j - 1] = num[j];
					tmp1 = temp;
					num[j] = tt;
				}
			}
			if (type == 2)
			{
				if (tmp2->students->group > tmp1->students->group)
				{
					tt = num[j - 1];
					temp = tmp2;
					tmp2 = tmp1;
					num[j - 1] = num[j];
					tmp1 = temp;
					num[j] = tt;
				}
			}
		}
	}
	ptr->t = num[*ptr->i - 1];
	ptr->t->next = num[*ptr->i - 2];
	tmp1 = num[*ptr->i - 2];
	for (int i = *ptr->i - 3; i > -1; i--)
	{
		tmp1->next = num[i];
		tmp1 = tmp1->next;
	}
	ptr->h = num[0];
	ptr->h->next = num[0];
	return ptr;
}

pointers *delete (pointers *ptr)
{
	queue *tmp = NULL;
	int j = 0;
	tmp = ptr->t;
	int flag = 0;
	while (j < *ptr->i)
	{

		for (int i = 0; i < tmp->students->marks->top; i++)
		{
			if (tmp->students->marks->data[i] < 4)
			{
				flag++;
			}
		}
		if (flag >= 3)
		{
			tmp->students->group = 0;
			tmp->students->lastname = "Unknown";
			free(tmp->students->marks);
		}
		flag = 0;
		tmp = tmp->next;
		j++;
	};
	return ptr;
}

void freeptr(pointers *ptr)
{
	queue *tmp = NULL;
	queue *temp = NULL;
	int j = 0;
	tmp = ptr->t;
	while (j < *ptr->i)
	{
		free(tmp->students->lastname);
		free(tmp->students->marks->data);
		free(tmp->students->marks);
		temp = tmp->next;
		free(tmp->next);
		tmp = temp;
		j++;
	};
	free(ptr->i);
	return;
}

pointers *prog(pointers *ptr, int sel)
{
	switch (sel)
	{
	case 1:
		ptr = add(ptr);
		break;
	case 2:
		ptr = sort(ptr);
		break;
	case 3:
		ptr = delete (ptr);
		break;
	case 4:
		see(ptr);
		break;
	case 5:
		freeptr(ptr);
		exit(0);
	default:
		break;
	}
	return ptr;
}

int main()
{
	pointers *ptr = calloc(1, sizeof(pointers));
	ptr->t = NULL;
	ptr->h = NULL;
	int sel;
	ptr->i = calloc(1, sizeof(int));
	while (1)
	{
        printf("\nChoose option: \n");
        printf("1)Add student\n");
        printf("2)Sort students\n");
        printf("3)Delete information about students with 3 or more bad marks\n");
        printf("4)View all students");
        printf("5)Exit\n");
        while (1)
		{
			fflush(stdin);
			scanf("%d", &sel);
			if (sel > 0 && sel < 7)
			{
				break;
			}
			printf("Wrong number");
		}
		ptr = prog(ptr, sel);
	}
}