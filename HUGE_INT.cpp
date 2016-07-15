#include "HUGE_INT.h"

//print function
#include<iostream>
#include<string>
// print function
#include<algorithm>

static HUGE_INT one(1);
static HUGE_INT zero(0);

Node * HUGE_INT::free = NULL;
Node *HUGE_INT::getNode(void)
{
	Node *p;
	if (free != NULL)
	{
		p = free;
		free = free->next;
	}
	else
		p = new Node;
	p->digit = 0;
	p->next = NULL;
	return p;
}

void HUGE_INT::freeNode(Node *p)
{
	p->next = free;
	p->digit = 0;
	free = p;
	return;
}

HUGE_INT::HUGE_INT() :head(NULL), size(0), printWidth(0)
{
}

HUGE_INT::HUGE_INT(HUGE_INT &num)
{
	// create new huge int
	this->clear();

	// walk down huge int and copy
	Node *p = num.head;
	while (p != NULL)
	{
		this->addNode(p);
		p = p->next;
	}
}// end copy constructor

HUGE_INT::HUGE_INT(const unsigned long long &num)
{
	*this = num;
}

HUGE_INT::~HUGE_INT()
{
	Node *p = head, *q;
	// Walk down list; delete Nodes
	while (p != NULL)
	{
		q = p;
		p = p->next;
		q->digit = 0;
		delete q;
	}
	p = free;
	// Walk down free list; delete Nodes
	while (p != NULL)
	{
		q = p;
		p = p->next;
		delete q;
	}
	free = NULL;
}

HUGE_INT &HUGE_INT::operator=(const unsigned long long &num)
{
	if (num == 0)
		return *this;
	// Deallocate memory of lhs
	this->clear();

	unsigned long long r = num; // remainder
	int d = 0; // digit

	// break off the one's place
	// each loop and add it to the lhs number
	while (r != 0)
	{
		d = r % 10;// get digit
		r /= 10;// reduce number by 10^1

		// create new node with digit
		Node *el = getNode();
		el->digit = d;
		this->addNode(el);
	}
	return *this;
}

HUGE_INT &HUGE_INT::operator=(const HUGE_INT &rhs)
{
	// Check for self assignment
	if (this != &rhs)
	{
		// Deallocate memory of old number
		this->clear();
		// Walk through list; copy Nodes
		for (Node *p = rhs.head; p != NULL; p = p->next)
			this->addNode(p);
	}
	return *this;
}

HUGE_INT &HUGE_INT::operator+(const HUGE_INT &rhs)
{
	// if lhs + 0
	if (rhs.getSize() == 0)
		return *this;
	// walk down both huge ints
	Node *p = this->head;
	Node *q = rhs.head;
	HUGE_INT *n = new HUGE_INT(); // sum
	int left, right, sum, digit, carry = 0;
	while ((p != NULL) || (q != NULL) || carry)
	{
		// something left to add lhs
		left = ((p == NULL) ? 0 : p->digit);
		// something left to add rhs
		right = ((q == NULL) ? 0 : q->digit);
		// add
		sum = left + right + carry;
		digit = sum % 10;
		carry = sum / 10;
		Node *d = getNode();
		d->digit = digit;
		n->addNode(d);
		// if not off end of linkedlist, make progress
		if (p != NULL) p = p->next;
		if (q != NULL) q = q->next;
	}
	return *n;
}

HUGE_INT &HUGE_INT::operator*(const HUGE_INT &rhs)
{
	Node *p, *q;
	HUGE_INT * total = new HUGE_INT(), n;
	int i, j, left, right, mult, digit, carry = 0;

	// walk through first HUGE_INT
	for (i = 0, q = rhs.head; i < rhs.getSize() && q != NULL; i++, q = q->next)
	{
		// walk through second HUGE_INT
		for (j = 0, p = this->head; j < this->getSize() && p != NULL; j++, p = p->next)
		{
			// Calculate product
			left = q->digit;
			right = p->digit;
			mult = left * right + carry;
			// Store result into a Node
			digit = mult % 10;
			carry = mult / 10;
			Node *d = getNode();
			d->digit = digit;
			n.addNode(d);
			// if there is a carry left over
			// and it is end of one portion
			// of mult, add the carry to the 
			// end of the helper huge int
			if (((j + 1) == this->getSize()) && carry)
			{
				Node *c = getNode();
				c->digit = carry;
				n.addNode(c);
				carry = 0;
			}	
		}
		// add zeros if need be
		n.pushZeros(i);
		// add helper huge int to result
		total->operator+=(n);
		// recycle helper huge int
		n.clear();
	}
	return *total;
}

HUGE_INT &HUGE_INT::operator-(const HUGE_INT &rhs)
{
	// if lhs + 0
	HUGE_INT *total = new HUGE_INT; // C
	*total = zero;
	if (this->getSize() < rhs.getSize())
	{
		return *total;
	}
	Node *p = this->head;
	Node *q = rhs.head;
	int left, right, carry = 0,  diff = 0;
	while ((p != NULL) || (q != NULL) || carry)
	{
		// A<i>
		left = ((p == NULL) ? 0 : p->digit);
		// B<i>
		right = ((q == NULL) ? 0 : q->digit);
		int diff = left - right + carry;
		if (diff < 0)
		{
			diff += 10;
			carry = -1;
		}
		else
			carry = 0;
		Node *d = getNode();
		d->digit = diff;
		total->addNode(d);
		// if not off end of linkedlist, make progress
		if (p != NULL) p = p->next;
		if (q != NULL) q = q->next;
		if (p == NULL && q == NULL && carry)
		{
			*total = zero;
			return *total;
		}
	}
	total->removeLeadingZeroes();
	return *total;
}

HUGE_INT &HUGE_INT::operator+=(const HUGE_INT &rhs)
{
	*this = *this + rhs;
	return *this;
}

HUGE_INT &HUGE_INT::operator*=(const HUGE_INT &rhs)
{
	*this = *this * rhs;
	return *this;
}

HUGE_INT &HUGE_INT::operator-=(const HUGE_INT &rhs)
{
	*this = *this - rhs;
	return *this;
}

HUGE_INT &HUGE_INT::operator^(const HUGE_INT &rhs)
{
	// n^0 = 1
	if (zero == rhs)
		return one;
	// n^1 = n
	if (one == rhs)
		return *this;
	HUGE_INT *total = new HUGE_INT;
	HUGE_INT temp;
	temp = rhs;
	*total = powerHelper(temp);
	return *total;
}

HUGE_INT &HUGE_INT::operator--(void)
{
	HUGE_INT temp;
	Node t;
	t.digit = 1;
	temp.addNode(&t);
	*this -= temp;
	return *this;
}

void HUGE_INT::print(void)
{
	// container
	std::string number = "";

	// walk through huge int
	Node * p = this->head;
	while (p != NULL) // not end of list
	{
		// convert number into a string
		// add to container
		number.insert(0, std::to_string(p->digit));
		p = p->next;
	}
	// if printWidth is bigger than the number
	if (this->getPrintWidth() > this->getSize())
	{
		int dif = this->getPrintWidth() - this->getSize();
		// pad front of number with spaces
		for (int i = 0; i < dif - 1; i++) number.insert(0, " ");
	}
	bool allZeroes = true;
	for (int i = 0; allZeroes && i < number.size(); ++i)
		if (number[i] != '0') 
			allZeroes = false;
	if (allZeroes) number = "";
	if (number.size() == 0) number = "0";
	std::cout << number << std::flush;
	return;
}

/*************** Helper Functions ******************/

void HUGE_INT::addNode(Node *num)
{

	// walk through huge int
	// till end of huge int
	// adding in reverse order
	Node * n = getNode();
	n->digit = num->digit;
	Node *p, *q;
	for (p = head, q = NULL;
		p != NULL;
		q = p, p = p->next);

	// empty list insert
	if (q == NULL)
	{
		n->next = head;
		head = n;
	}
	// end of list, insert node
	else
	{
		n->next = p;
		q->next = n;
	}

	// adjust size and printWidth
	this->setSize(this->getSize() + 1);
	this->setPrintWidth(this->getSize() + 1);
	return;
}// end addNode

void HUGE_INT::removeNode(const int &position)
{
	// check if position is within
	// the bounds of the list (1 to n)
	bool removed = (position >= 1) && (position <= this->getSize());
	if (removed)
	{

		Node *p = head, *q = NULL;

		// first digit
		if (position == 1)
		{
			head = p->next;
		}
		// some other digit
		else
		{
			q = getNodeAt(position - 1);
			p = q->next;
			q->next = p->next;
		}
		// delete all parts of node
		freeNode(p);

		// adjust size and printWidth
		this->setSize(this->getSize() - 1);
		this->setPrintWidth(this->getPrintWidth() - 1);
	}
	return;
}// end removeNode

Node * HUGE_INT::getNodeAt(const int &position)const
{
	// test for out of bounds
	// 1 to n
	if ((position >= 1) && (position <= size))
	{
		// find next node
		Node * p = head;
		for (int i = 1; i < position; ++i)
			p = p->next;
		return p;
	}
	std::cout << "getNodeAt error" << std::endl;
	return NULL;
}// end getNodeAt

void HUGE_INT::pushZeros(int i)
{
	//f(n) zeros will be added
	for (int j = 0; j < i; j++)
	{
		// insert at begining
		Node *x = getNode();
		x->digit = 0;
		x->next = this->head;
		head = x;

		// +1 to print width and size
		this->setSize(this->getSize() + 1);
		this->setPrintWidth(this->getSize() + 1);
	}
}// end pushZeros

void HUGE_INT::clear(void)
{
	while (this->getSize() != 0)removeNode(1);
}

bool HUGE_INT::operator==(const HUGE_INT &rhs)
{
	if (this->getSize() != rhs.getSize())
		return false;
	Node *p = head, *q = rhs.head;
	while (p != NULL)
	{
		if (p->digit != q->digit)
		{
			return false;
		}
		p = p->next;
		q = q->next;
	}
	return true;
}

int HUGE_INT::getSize()const
{
	return this->size;
}// end getSize

void HUGE_INT::setSize(const int &num)
{
	this->size = num;
	return;
}// end setSize

int HUGE_INT::getPrintWidth(void)const
{
	return this->printWidth;// default == size
}// end getPrintWidth

void HUGE_INT::setPrintWidth(const int &num)
{
	this->printWidth = num;// default == size
	return;
}// end setPrintWidth

HUGE_INT &HUGE_INT::powerHelper(HUGE_INT &rhs)
{
	if (rhs == one)
		return *this;
	return (*this * powerHelper(rhs - one));
}

void HUGE_INT::removeLeadingZeroes(void)
{
	Node *p, *q;
	bool allZeroes = true;
	for (p = head, q = NULL; p != NULL; p = p->next)
	{
		if (p->digit > 0)
		{
			q = p;
			allZeroes = false;
		}
		else
			allZeroes = true;
	}
	if (allZeroes && q != NULL)
	{
		while (q->next != NULL)
		{
			p = q->next;
			q->next = p->next;
			freeNode(p);
		}
	}
}

HUGE_INT HUGE_INT::Karatsuba(HUGE_INT &a, HUGE_INT&b)
{
	return NULL;
}