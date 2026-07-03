//Билет 17 Рекурсивно создать двунаправленное кольцо


struct ring { 
	int n; 
	ring* left;
	ring* right;
};

void create(ring** head, int n) {   
								
	if (n == 0) { 
		return;
	}
	ring* s;
	s = (ring*)calloc(1, sizeof(ring));
	printf("Введите число\n");
	while (!scanf("%d", &s->n)) {
							
		rewind(stdin);
		printf("Error\n");
	}
	
	s->left = s->right = nullptr;
	if (!(*head)) {
		s->left = s->right = s;
		(*head) = s;
	}
	else {// если кольцо уже есть
		s->left = (*head)->left;
								
		(*head)->left->right = s;
		s->right = (*head);
							
							
		(*head)->left = s; 
	}
	create(head, n - 1);					
}

int main() {
	ring* head;
	head = nullptr;
	create(&head, 5);
      return 0;
}