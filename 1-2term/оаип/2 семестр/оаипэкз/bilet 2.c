//Дано бинарное дерево. Организовать удаление любого узла(элемента)

typedef struct TreeNode{
	int data;
	TreeNode* left;
	TreeNode* right;
}TreeNode;

// нахождение максимального узла
TreeNode* getMaxNode(TreeNode* root) {
	while (root->right) {
		root = root->right;
	}
	return root; 
}

// удаление узла по значению
TreeNode* deleteNode(TreeNode* root, int value) {
	if (root == NULL) {		// если дерево пустое
		return root;		// выход из функции
	}
	// поиск данных по значению
	if (root->data > value) {
		root->left = deleteNode(root->left, value);
		return root;
	}
	else if (root->data < value) {
		root->right = deleteNode(root->right, value);
		return root;
	}
	else {													// если данные найдены
		if (root->right && root->left) {					// если правый и левый наследник
			TreeNode* max = getMaxNode(root->left);			// нужно найти максимальный узел в левом поддереве
			root->data = max->data;							// присвоить текущему узлу данные максимального
			root->left = deleteNode(root->left, max->data);	// рекурсивно удалить найденный максимальный элемент
			return root;
		}
		else if (root->left) {								// если только левый наследник
			TreeNode* tmp = root->left;						// в переменную tmp сохраняется наследник удаляемого узла
			free(root);										// освобождается память текущего узла
			return tmp;										// возвращается указатель левый наследник 
		}
		else if (root->right) {								// аналогично, если только правый
			TreeNode* tmp = root->right;
			free(root);
			return tmp;
		}
		else {												// если нет наследников
			free(root);										// то просто очищается этот узел
			return NULL;
		}
	}
}
