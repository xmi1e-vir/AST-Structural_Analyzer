
typedef struct node {
	int deepth;
	unsigned long long num;
	void * pinfo;
	struct node * left;
	struct node * right;
	struct node * root;
} Tree;

Tree * addIt(Tree* nod, unsigned long long number);
Tree * findIt(Tree * nod, unsigned long long val);




void rebalance_right(Tree *rot_nod, Tree *end_nod)
{
	Tree *ini1_nod, *end1_nod;
	Tree *ini2_nod, *end2_nod;
	Tree *mid_nod;
	int cmp, endRight;

	if (end_nod->deepth - rot_nod->deepth == 0)
	{
		assert(rot_nod == end_nod);
		return;
	}

	if (end_nod->deepth - rot_nod->deepth == 1)
	{
		assert(rot_nod->right == end_nod);
		return;
	}

	if (end_nod->deepth - rot_nod->deepth == 2)
	{
		assert(rot_nod->right == end_nod->root);
		rot_nod->right->deepth = rot_nod->deepth + 1;
		return;
	}

	endRight = end_nod->deepth + 1;
	ini1_nod = rot_nod->right;
	end2_nod = end_nod;

	int aux = (rot_nod->deepth + end_nod->deepth);

	if (aux % 2 != 0)
	{
		aux++;
	}
	aux /= 2;
	
	mid_nod = rot_nod;
	cmp = rot_nod->deepth;

	while (cmp != aux)
	{
		mid_nod = mid_nod->right;
		cmp++;
		endRight--;
	}

	end1_nod = mid_nod->root;
	ini2_nod = mid_nod->right;
	end1_nod->right = NULL;

	rot_nod->right = mid_nod;
	mid_nod->root = rot_nod;
	mid_nod->left = ini1_nod;
	mid_nod->right = ini2_nod;
	ini1_nod->root = mid_nod;
	ini2_nod->root = mid_nod;


	mid_nod->deepth = rot_nod->deepth + 1;
	ini1_nod->deepth = mid_nod->deepth + 1;
	ini2_nod->deepth = mid_nod->deepth + 1;

	aux = cmp - rot_nod->deepth;

	end1_nod->deepth = rot_nod->deepth + aux;
	rebalance_right(ini1_nod, end1_nod);

	end2_nod->deepth = endRight;
	rebalance_right(ini2_nod, end2_nod);

}


Tree *addIt(Tree *nod, unsigned long long number)
{
	int deep_level = 0;
	int left_bal = 0;
	int right_bal = 0;
	Tree* middle = nod;
	Tree* nod_bal_right = NULL;
	Tree* nod_bal_left = NULL;


	if (nod == NULL) {
		nod = (Tree*)malloc(sizeof(Tree));
		if (nod == NULL) {
			return NULL;
		}
		nod->deepth = 0;
		nod->num = number;
		nod->left = NULL;
		nod->root = NULL;
		nod->right = NULL;
		nod->pinfo = NULL; 
		return nod;
	}

	while (middle != NULL)
	{
		if (number > middle->num)
		{
			left_bal = 0;
			
			if (middle->left == NULL)
			{
				if ((right_bal == 0) && (middle != nod))
				{
					nod_bal_right = middle;
					right_bal = 1;
				}
				else
				{
					right_bal++;
				}
			}
			else
			{
				right_bal = 0;
			}


			if (middle->right == NULL)
			{
				
				Tree *right = addIt(NULL, number); 
				right->root = middle;
				middle->right = right;
				right->deepth = deep_level + 1;

				if (right_bal >= MAX_RECURSIVE) 
				{
					rebalance_right(nod_bal_right, right);
				}

		

				return right;
			}

			
			middle = middle->right;
			deep_level++;
		}
		else if (number < middle->num)
		{
		
			left_bal = 0;
			right_bal = 0;
			if (middle->left == NULL)
			{
				
				Tree *left = addIt(NULL, number);
				
				middle->left = left;
				left->root = middle;
				left->deepth = deep_level + 1;

				return left;
			}

		
			middle = middle->left;
			deep_level++;
		}
		else
		{
			return middle;
		}
	}

	
	assert(1 == 0);

	return NULL;
}

Tree *findIt(Tree *where, unsigned long long val)
{
	if (where == NULL)
	{
		return NULL;
	}

	if (where->num == val)
	{
		return where;
	}

	while (where != NULL)
	{
		if (val > where->num)
		{
			where = where->right;
		}
		else if (val < where->num)
		{
			where = where->left;
		}
		else
		{
			return where;
		}
	}

	return NULL;
}

void actInOrder(Tree *nod, void action(Tree *))
{
	Tree *aux = nod;

	if (nod == NULL)
	{
		return;
	}

	maxOnLeft:
	{
		while (nod->left != NULL)
		{
			nod = nod->left;
		}
		
		action(nod);			

		if (nod->right != NULL)
		{
			nod = nod->right;
			goto maxOnLeft;
		}

		while (nod->root != NULL)
		{
			aux = nod->root;

			if (aux->root == NULL)
			{
				return;     
			}

			if (aux->left == nod) 
			{
				action(aux);
				if (aux->right != NULL) 
				{
					nod = aux->right;
					goto maxOnLeft;
				}

			}
			nod = aux;
		}
		return;               

	}
}


void freeAllTree(Tree *root, void freeMe(Tree *))
{
	Tree *aux;
	Tree *nod = root;

	if (nod == NULL)
	{
		return;
	}


	run_max_on_left:
	{
		while (nod->left != NULL)
		{
			nod = nod->left;
		}

		if (nod->right != NULL)
		{
			nod = nod->right;
			goto run_max_on_left;
		}

		while (nod->root != NULL)
		{
			aux = nod->root;
			if (aux->left == nod)
			{
				freeMe(aux->left);
				aux->left = NULL;
				if (aux->right != NULL)
				{
					nod = aux->right;
					goto run_max_on_left;
				}
			}
			else if (aux->right == nod)
			{
				assert(aux->left == NULL);
				freeMe(aux->right);
				aux->right = NULL;
			}
			else
			{
				assert(0);
			}
			nod = aux;
		}
		if (nod == root)
		{
			freeMe(root);
			return;
		}

	}
}


int counter = 0;

void freeMeImplementation(Tree *t)
{
	if (t != NULL)
	{
		if (t->pinfo != NULL)
		{
			free(t->pinfo);
		}
		free(t);
	}
	counter--;
}

void printImplementation(Tree *t)
{
	printf(" %.2lld", t->num);
}

void printInDeepthImplementation(Tree *t)
{
	for (int i = 0; i < t->deepth; i++)
	{
		printf("    .");
	}
	printf(" %.2lld\n", t->num);
}



void counterImplementation(Tree *t)
{
	counter++;
}

void printRandomic(void)
{
	for (int i = 0; i < 1; i++)
	{
		Tree* root = NULL;
		int length = (MAX_RECURSIVE + 1) * 2;
		int val = length;

		root = addIt(NULL, val);
		printf("\nValues:  %.2d", val);

		for (int i = 0; i < length; i++)
		{

			val = 2 * i + 1;
			addIt(root, val);
			printf(" %.2d", val);
		}

		printf("\nInOrder:");
		actInOrder(root->left, printImplementation);
		printImplementation(root);
		actInOrder(root->right, printImplementation);

		counter = 1;
		actInOrder(root->left, counterImplementation);
		actInOrder(root->right, counterImplementation);
		printf("\nTotal of nodes is : %d ", counter);

		printf("\n================================== Print In Deepth =======================================");
		printf("\n012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789\n");
		actInOrder(root->left, printInDeepthImplementation);
		printInDeepthImplementation(root);
		actInOrder(root->right, printInDeepthImplementation);


		freeAllTree(root, freeMeImplementation);
		printf("\nNumber of nodes after free : %d ", counter);

		assert(counter == 0);
	}
}

int oddNumbers[MAX_SIZE_TREE / 2];
int odd_len = MAX_SIZE_TREE / 2;

void print_lins(int factor)
{
	char oddLeaf[MAX_SIZE_TREE+1];
	char evenLeaf[MAX_SIZE_TREE + 1];
	
	char fmt[] = "                                                                                                                                            ";

	int x = factor*4;
	assert((x+4) < sizeof(fmt));

	fmt[x+2] = 0;
	x = (x / 2) - 1;
	fmt[x++] = '%';
	fmt[x++] = '.';
	fmt[x++] = '2';
	fmt[x++] = 'd';

	for (int i = 0; i < odd_len/factor; i++)
	{
		printf(fmt, factor * oddNumbers[i]);
	}
	printf("\n");

	if (odd_len == factor)
	{
		return;
	}

	
	for (int i = 0; i < (4*factor); i++) 
	{
		if (i < (2 * factor)) {
			if (i == (2 * factor - 1)) {
				oddLeaf[i] = '/';
			} else {
				oddLeaf[i] = '_';
			}
			evenLeaf[i] = ' ';
		}
		else 
		{
			oddLeaf[i] = ' ';
			if (i == (2 * factor)) {
				evenLeaf[i] = '\\';
			} else {
				evenLeaf[i] = '_';
			}
		}
	}
	evenLeaf[(4 * factor)] = 0;
	oddLeaf[(4 * factor)] = 0;



	for (int i = 0; i < odd_len / factor; i++)
	{
		if (i % 2)
		{
			printf("%s", oddLeaf);
		}
		else
		{
			printf("%s", evenLeaf);
		}
	}
	printf("\n");

}

void print_staticaly()
{
	system("cls");


	for (int i = 0; i < odd_len; i++)
	{
		oddNumbers[i] = 2 * i + 1;
	}

	int bit = 1;

	while (bit < MAX_SIZE_TREE)
	{
		print_lins(bit);
		bit = bit << 1;
	}


	printf("\n");

}

int main(int argc, char ** argv)
{
	print_staticaly();
	printRandomic();

	return (EXIT_SUCCESS);
}