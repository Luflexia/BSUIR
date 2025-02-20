//bilet 2 Дано бинарное дерево. Организовать удаление любого узла(элемента)

typedef struct TreeNode{
	int data;
	TreeNode* left;
	TreeNode* right;
}TreeNode;

TreeNode* getMaxNode(TreeNode* root) {
	while (root->right) {
		root = root->right;
	}
	return root; 
}

TreeNode* deleteNode(TreeNode* root, int value) {
	if (root == NULL) {
		return root;
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
	else {
		if (root->right && root->left) {
			TreeNode* max = getMaxNode(root->left);
			root->data = max->data;
			root->left = deleteNode(root->left, max->data);
			return root;
		}
		else if (root->left) {
			TreeNode* tmp = root->left;
			free(root);
			return tmp;
		}
		else if (root->right) {
			TreeNode* tmp = root->right;
			free(root);
			return tmp;
		}
		else {
			free(root);
			return NULL;
		}
	}
}
