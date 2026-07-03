//Билет 19  Удаление элемента из однонаправленной очереди.

struct queu {м
	int n;
	queu* next;
};

void delette(queu** head, queu** tail, int n) {
	queu* buf, * ptr;
					
	int i = 1;
	buf = (*head);
	ptr = buf->next;
	while (1) {
		if (n == 0) {
			(*head) = (*head)->next;
			free(buf);
			break;
		}
		else if (!ptr->next && n==i) {
			(*tail) = buf; 
			buf->next = nullptr;
			free(ptr);
			break;
		}
		else if (i == n) {
			buf->next = ptr->next;
			free(ptr);
			break;
		}
		i++;
		ptr = ptr->next; buf = buf->next;
	}
}
int main() {
	queu* head, * tail;
	head = tail = nullptr;

	int n;
	printf("Номер для удаления\n");
	while (!(scanf("%d", &n) && n>0)) {
		printf("error\n");
		rewind(stdin);
	}
	delette(&head, &tail, n-1);
	return 0;
}