## Synopsis

This is a simple program which creates a 'template' for a doubly ended queue(similar to the templates in C++) in pure C. A deque of any type can be created without violating type safety.

## Sample Usage

Here's how a simple deque of integers can be created

```c++

Deque_DEFINE(int) 	//Defines a deque of integers. Will define class Deque_int

bool
int_less(const int &o1, const int &o2) { //a function for comparing integers
    return o1 < o2;
}

int main(int argc, char const *argv[])
{
	Deque_int d;
	Deque_int_ctor(&d,int_less); //constructor for deque of ints
	d.push_back(&d,4);
	d.dtor(&d); 	//destructor for deque of ints
	return 0;
}

```
