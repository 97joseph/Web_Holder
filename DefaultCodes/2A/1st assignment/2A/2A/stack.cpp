template <typename T>
class Node
{
private:
	T data;
	Node<T>* next;

public:

	Node()
	{
		data = 0;
		next = NULL;
	}

	Node(T value)
	{
		data = value;
		next = NULL;
	}

	Node <T>* getNext()
	{
		return next;
	}

	void setnext(Node<T>* ptr)
	{
		next = ptr;
	}

	T getData()
	{
		return data;
	}

	void setData(T value)
	{
		data = value;
	}

};


////////////////////////////////////////////////

template <typename T>
class Stack
{
	Node<T>* top;

public:
	Stack()
	{
		top = NULL;
	}

	Stack(Node<T>* ptr)
	{
		top = ptr;
	}

	Node <T>* getTop()
	{
		return top->getData();
	}

	void setTop(Node<T>* ptr)
	{
		top = ptr;
	}

	bool isEmpty()
	{
		if (top == NULL)
		{
			return true;
		}
		else
			return false;
	}

	void push(T value)
	{
		Node<T>* temp = new Node <T>(value);

		if (top == NULL)
		{
			top = temp;
		}
		else
		{
			temp->setnext(top);
			top = temp;
		}

	}

	int length() {
		Node<T>* current = top;
		int count = 0;
		while (current != NULL) {
			current = current->getNext();
			count++;
		}
		return count * 1;
	}
	bool pop()
	{

		if (top == NULL)
		{
			return false;
		}
		else
		{
			Node<T>* temp = top;
			top = top->getNext();
			delete temp;

			return true;
		}
	}
	T returner(int i) {
		int count = 0;
		Node<T>* current = top;
		while (count < i) {
			current = current->getNext();
			count++;
		}
		return current->getData();
	}
	T peek()
	{
		if (isEmpty())
			return NULL;
		else
			return top->getData();
	}
};

