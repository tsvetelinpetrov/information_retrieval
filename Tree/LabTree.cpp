#include "LabTree.h"

void init_from_file(node** t, const char* filePath) {
	std::fstream file(filePath, std::ios_base::in);

	int num;
	while (file >> num)
		add_node(t, num, NULL);
}

void add_node(node** t, node_data item, node* parent) {
	node* p;
	if (*t == NULL) {
		p = (node*)malloc(sizeof(node));
		p->left = p->right = NULL;
		p->parent = parent;
		p->item = item;
		*t = p;
		return;
	}

	if (item < (*t)->item) {
		add_node(&((*t)->left), item, *t);
	}
	else {
		add_node(&((*t)->right), item, *t);
	}
}

node* find_node(node* t, node_data i) {
	if (t == NULL) return NULL;
	if (t->item == i) return t;
	if (i < t->item) {
		return find_node(t->left, i);
	}
	else {
		return find_node(t->right, i);
	}
}

node* find_min(node* t) {
	if (t == NULL) return NULL;
	node* min = t;
	while (min->left != NULL) {
		min = min->left;
	}
	return min;
}

node* find_max(node* t) {
	if (t == NULL) return NULL;
	node* max = t;
	while (max->right != NULL) {
		max = max->right;
	}
	return max;
}

void del_node(node* t, node_data item) {
	if (t == NULL) return;
	node* element = find_node(t, item);
	if (element == NULL) return;

	int hasParent = element->parent != NULL;
	int isLeft = hasParent && element->item <
		element->parent->item;

	if (element->left == NULL && element->right == NULL) { // Without childs
		if (hasParent) {
			if (isLeft) {
				element->parent->left = NULL;
			}
			else {
				element->parent->right = NULL;
			}
		}
		free(element);
	}
	else if (element->left != NULL && element->right == NULL) { // Only left
		element->left->parent = element->parent;
		if (hasParent) {
			if (isLeft) {
				element->parent->left = element->left;
			}
			else
			{
				element->parent->right = element->right;
			}
		}
		else {
			t = element->left;
		}
		free(element);
	}
	else if (element->left == NULL && element->right != NULL) { // Only right
		element->right->parent = element->parent;
		if (hasParent) {
			if (isLeft) {
				element->parent->left = element->right;
			}
			else {
				element->parent->right = element->right;
			}
		}
		else {
			t = element->right;
		}
		free(element);
	}
	else { // Two childs
		node* right_min = find_min(element->right);
		element->item = right_min->item;

		del_node(right_min, right_min->item);
	}
}

void preorder(node* t)
{
	if (t == NULL)
	{
		return;
	}
	else
	{
		preorder(t->left);
		printf("%d->", t->item);
		preorder(t->right);
	}
}

void inorder(node* t)
{
	if (t == NULL)
	{
		return;
	}
	else
	{
		printf("%d->", t->item);
		inorder(t->left);
		inorder(t->right);
	}
}

void postorder(node* t)
{
	if (t == NULL)
	{
		return;
	}
	else
	{
		postorder(t->left);
		postorder(t->right);
		printf("%d->", t->item);
	}
}

int numLevels(node* t)
{
	if (t == NULL)
	{
		return 0;
	}
	else
	{
		int ldepth = numLevels(t->left);
		int rdepth = numLevels(t->right);
		if (ldepth > rdepth)
			return ldepth + 1;
		else
			return rdepth + 1;
	}
}