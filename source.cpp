#include<iostream>
#include<vector>
#include<ctime>
using namespace std;

class skiplist
{
	struct node
	{
		int key;
		node * right, *down;
	};

	int size;
	vector <node*> heads;

	node * createNode(int keyInsert)		// Create a new node and return
	{
		node * newNode = new node();
		newNode->down = nullptr;
		newNode->right = nullptr;
		newNode->key = keyInsert;
		return newNode;
	}

	void createListLevel()				// Create a new level in the skip list
	{
		node * newLevel = new node();
		newLevel->right = nullptr;
		if (heads.size() > 0)
		{
			newLevel->down = heads[heads.size() - 1];
		}
		else
		{
			newLevel->down = nullptr;
		}
		heads.push_back(newLevel);
	}

	int heightAtThis(node * temp)
	{
		int h = 0;
		while (temp != nullptr)
		{
			temp = temp->down;
			h++;
		}
		return h;
	}

	node * insertion(int newKey, int level)
	{
		node * newNode = createNode(newKey);
		node * a = heads[heads.size() - 1];
		node * b = a->right;
		while (true)
		{
			if (b->key > newKey)					// If key at present node is higher then the key to be inserted
			{
				while (heightAtThis(b) != level)
				{
					b = b->down;
					a = a->down;
				}
				break;
			}
			if (b->right == nullptr)			// If a level as reached to its end
			{
				if (heightAtThis(b) == level)
				{
					a = b;
					break;
				}
				b = b->down;
				a = a->down;
			}
			else                              // Move towards right in the level
			{
				a = b;
				b = b->right;
			}
		}

		if (a == b)						// Largest element is inserted in the list
		{
			a->right = newNode;
		}
		else
		{
			node * temp = a;
			a = a->right;
			while (a->key < newKey)
			{
				temp = a;
				a = a->right;
			}
			newNode->right = a;
			temp->right = newNode;
		}
		return newNode;
	}

public:

	skiplist()
	{
		createListLevel();
	}

	void insert(int newKey)
	{
		int random = 2;
		int level = 1;

		if (heads[0]->right == nullptr)					// If the skip list is empty
		{
			heads[0]->right = createNode(newKey);
			size++;
			int count = 1;
			while (random != 0)
			{
				random = rand() % 2;
				if (random == 1)
				{
					createListLevel();
					heads[count]->right = createNode(newKey);
					heads[count]->right->down = heads[count - 1]->right;
					count++;
					size++;
				}
			}
		}
		else
		{
			node * bl = insertion(newKey, 1);
			size++;
			node * bh = nullptr;
			int count = 1;             // Decides the level
			while (random != 0)
			{
				random = rand() % 2;
				if (random == 1)
				{
					size++;
					if (heads.size() < count + 1)			// New level should be created
					{
						createListLevel();
						if (count % 2 == 1 || count == 1)
						{
							bh = heads[count]->right = createNode(newKey);
							heads[count]->right->down = bl;
						}
						else
						{
							bl = heads[count]->right = createNode(newKey);
							heads[count]->right->down = bh;
						}
					}
					else             // Just insert the key in the above level 
					{
						node * temp2 = insertion(newKey, count + 1);
						if (count % 2 == 1 || count == 1)
						{
							bh = temp2;
							temp2->down = bl;
						}
						else
						{
							bl = temp2;
							temp2->down = bh;
						}
					}
					count++;
				}
			}
		}
	}

	void printall()
	{
		if (heads[0]->right == nullptr)
		{
			cout << "\n List is empty\n ";
			return;
		}

		int i = heads.size() - 1;
		while (i >= 0)
		{
			node* temp = heads[i]->right;
			while (temp != nullptr)
			{
				cout << temp->key << " ";
				temp = temp->right;
			}
			cout << endl << " ";
			i--;
		}
	}

	bool find(int keyToFind)
	{
		node * a = heads[heads.size() - 1];
		node * b = a->right;
		if (b == nullptr)	
		{
			return false;
		}
		if (b->key == keyToFind)
		{
			return true;
		}
		while (true)
		{
			if (b->key > keyToFind)
			{
				while (heightAtThis(b) != 1)
				{
					b = b->down;
					a = a->down;
				}
				break;
			}
			else if (b->key == keyToFind)
			{
				return true;
			}

			if (b->right == nullptr)
			{
				if (heightAtThis(b) == 1)
				{
					break;
				}
				b = b->down;
				a = a->down;
			}
			else
			{
				a = b;
				b = b->right;
			}
		}
		while (a->right != b)
		{
			a = a->right;          
			if (a->key == keyToFind)
			{
				return true;
			}
		}
		return false;
	}

	void remove(int rkey)
	{
		if (find(rkey) == false)
		{
			cout << "\n Number is not present\n";
			return;
		}
		node * prev = heads[heads.size() - 1];
		node * curr = heads[heads.size() - 1]->right;
		node * prev2 = nullptr;
		node * Down = nullptr;
		while (true)
		{
			if (curr->key == rkey)
			{
				break;
			}
			else if (curr->key > rkey)
			{
				prev = prev->down;
				curr = prev->right;
				if (curr->down == nullptr)
				{
					break;
				}
			}
			else					// The current key is smaller then the key to be removed 
			{
				if (curr->right == nullptr)
				{
					prev = prev->down;
					curr = curr->down;
					if (curr->down == nullptr)
					{
						break;
					}
				}
				else
				{
					prev = curr;
					curr = curr->right;
				}
			}
		}
		if (curr->key == rkey)
		{
			while (true)
			{
				if (curr->down == nullptr)
				{
					prev->right = curr->right;
					delete curr;
					size--;
					break;
				}
				Down = curr->down;
				prev2 = prev->down;

				prev->right = curr->right;
				delete curr;
				size--;

				if (heads[heads.size() - 1]->right == nullptr)          // Decrease a level by 1
				{
					heads.pop_back();
				}

				while (prev2->right != Down)
				{
					prev2 = prev2->right;
				}
				prev = prev2;           // prev now points to previous of node to be deleted
				curr = Down;
			}
		}
		else                 // Key is only present on last level
		{
			node * temp = prev;
			prev = prev->right;
			while (prev->key != rkey)
			{
				temp = prev;
				prev = prev->right;
			}
			temp->right = prev->right;
			delete prev;
			size--;
		}
	}

	void printAscending()
	{
		node *temp = heads[0]->right;
		while (temp != nullptr)
		{
			cout << temp->key << " ";
			temp = temp->right;
		}
	}

	~skiplist()
	{
		node *temp = heads[0]->right;
		if (temp != nullptr)
		{
			node *temp2 = temp->right;
			while (temp->right != nullptr)
			{
				temp2 = temp->right;
				remove(temp->key);
				temp = temp2;
			}
		}
	}

	int getsize()
	{
		return size;
	}
};

void menu()
{
	cout << "\n 1) Press 1 to insert\t 2) Press 2 to erase\n\n 3) Press 3 to search\t 4) print 4 to print skip list";
	cout << "\n\n 5) Press 5 to exit\n\n ";
}

void main()
{
	srand(time(NULL));
	skiplist obj;
	int choice;
	int n = 0;

	while (true)
	{
		menu();
		cout << "Enter your choice\t";
		cin >> choice;
		cout << endl;

		if (choice == 1)
		{
			cout << "\n Enter number to insert\t\t";
			cin >> n;
			while (obj.find(n) == true)
			{
				cout << "\n Number is already in the list.\n Enter another number\t\t";
				cin >> n;
			}
			obj.insert(n);
			cout << "\n\n List now contains the following numbers. Size is " << obj.getsize() << "\n\n ";
			cout << "---------------------------------------------------------------\n ";
			obj.printall();
			cout << "---------------------------------------------------------------";
			cout << "\n\n";
		}
		else if (choice == 2)
		{
			cout << "\n Enter number to remove\t\t";
			cin >> n;
			obj.remove(n);
			cout << "\n\n List now contains the following numbers. Size is " << obj.getsize() << "\n\n ";
			cout << "---------------------------------------------------------------\n ";
			obj.printall();
			cout << "---------------------------------------------------------------";
			cout << "\n\n";
		}
		else if (choice == 3)
		{
			cout << "\n Enter number which you want to search\t";
			cin >> n;
			if (obj.find(n) == true)
			{
				cout << "\n\n Number is present\n";
			}
			else
			{
				cout << "\n\n Number is not present\n";
			}
		}
		else if (choice == 4)
		{
			cout << "\n Which skip list do you want to print\n\n ";
			cout << "1) Press 1 to print whole skip list\t 2) Press 2 to print the last level only\n ";
			cin >> n;
			if (n == 1)
			{
				cout << endl << "\n ";
				obj.printall();
				cout << endl << "\n ";
			}
			else if (n == 2)
			{
				cout << endl << "\n ";
				obj.printAscending();
				cout << endl << "\n ";
			}
		}
		else if (choice == 5)
		{
			break;
		}
	}
	cout << endl << "\n\n ";
	system("pause");
}
