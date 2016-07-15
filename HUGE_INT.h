// HUGE_INT.h
// Daniel Salazar
// 2/6/14
// complete representation
// of an arbitrarily large integer
//

#ifndef HUGE_INT_H
#define HUGE_INT_H

struct Node
{
	int digit;
	Node * next;
};

class HUGE_INT
{
private:
	Node * head;
	int size;
	int printWidth; // default to size

	/* Helper Functions*/
	Node * getNodeAt(const int &position)const;
	void addNode(Node *num);
	void removeNode(const int &position);
	void pushZeros(int i);
	void setSize(const int &num);
	void clear(void);
	void setPrintWidth(const int &num); //specify print width
	int getSize()const; //return size
	bool operator==(const HUGE_INT &rhs);
	int getPrintWidth(void)const;
	HUGE_INT &powerHelper(HUGE_INT &rhs);
	void removeLeadingZeroes(void);
	HUGE_INT Karatsuba(HUGE_INT &a, HUGE_INT&b);

	/*Maintain a free list to reduce heap fragmentation*/
	static Node *free;
	Node *getNode(void);
	void freeNode(Node *p);
public:
	HUGE_INT(); //default
	HUGE_INT(HUGE_INT &num); //copy
	HUGE_INT(const unsigned long long &num); //normal
	~HUGE_INT(); //destructor
	HUGE_INT &operator=(const unsigned long long &num); //assign
	HUGE_INT &operator=(const HUGE_INT &rhs); //operator=
	HUGE_INT &operator^(const HUGE_INT &rhs);
	HUGE_INT &operator--(void);
	HUGE_INT &operator+(const HUGE_INT &rhs); //add
	HUGE_INT &operator*(const HUGE_INT &rhs); //mult
	HUGE_INT &operator-(const HUGE_INT &rhs); //sub
	HUGE_INT &operator+=(const HUGE_INT &rhs);
	HUGE_INT &operator*=(const HUGE_INT &rhs);
	HUGE_INT &operator-=(const HUGE_INT &rhs);

	void print(void); //print
	void splitAt()
};

#endif

