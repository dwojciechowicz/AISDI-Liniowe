#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
struct Node
{
	Node* next;
	Node* prev;
	Type data;
	Node() {next=nullptr; prev=nullptr;}
	Node(const Type& object) :data(object) {next=nullptr; prev=nullptr;}
	~Node() {next=nullptr; prev=nullptr;}
};

public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;
 private:
	Node* first;
	Node* last;
	size_type my_size;
 public:
 LinkedList()
  {
	first=new Node;
	last=new Node;
	first->next=last;
	last->prev=first;
	my_size=0;
  }

  LinkedList(std::initializer_list<Type> l): LinkedList()
  {
	//my_size=0;
	for(auto tmp=l.begin(); tmp!=l.end(); ++tmp)
		append(*tmp);
  }

  LinkedList(const LinkedList& other): LinkedList()
  {
	//my_size=0;
	for(auto tmp=other.begin(); tmp!=other.end(); ++tmp)
		append(*tmp);
  }

  LinkedList(LinkedList&& other)
  {
	first=other.first;
	last=other.last;
	my_size=other.my_size;
	other.first=nullptr;
	other.last=nullptr;
	other.my_size=0;
  }

  ~LinkedList()
  {
	while(my_size>0)
		erase(begin());
	delete first;
	delete last;
	//my_size=0;
  }

  LinkedList& operator=(const LinkedList& other)
  {
	if(this==&other)
		return *this;
	if(!isEmpty())
		erase(begin(),end());
	for(auto tmp=other.begin(); tmp!=other.end(); ++tmp)
		append(*tmp);
	return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {
	if(my_size>0)
		erase(begin(),end());
	first=other.first;
	last=other.last;
	my_size=other.my_size;
	other.first=nullptr;
	other.last=nullptr;
	other.my_size=0;
	return *this;
  }

  bool isEmpty() const
  {
	if(my_size==0) return true;
	else return false;
  }

  size_type getSize() const
  {
	return my_size;
  }

  void append(const Type& item)
  {
	Node* tmp=new Node(item);
	tmp->prev=last->prev;
	last->prev->next=tmp;
	last->prev=tmp;
	tmp->next=last;
	++my_size;
  }

  void prepend(const Type& item)
  {
	Node* tmp=new Node(item);
	tmp->next=first->next;
	first->next->prev=tmp;
	tmp->prev=first;
	first->next=tmp;
	++my_size;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
    Node* tmp=new Node(item);
	if(insertPosition==begin()) prepend(item);
	else if(insertPosition==end()) append(item);
	else
	{
        tmp->next=insertPosition.ptr->prev->next;
		tmp->prev=insertPosition.ptr->prev;
		insertPosition.ptr->prev->next=tmp;
		insertPosition.ptr->prev=tmp;
		++my_size;
	}
  }

  Type popFirst()
  {
	Type tmp=*(begin());
	erase (begin());
	return tmp;
  }

  Type popLast()
  {
	if(isEmpty()) throw std::out_of_range("the list is empty");
	Type tmp=*(--end());
	erase (--end());
	return tmp;
  }

  void erase(const const_iterator& possition)
  {
	if(isEmpty()) throw std::out_of_range("the list is empty");
	if(possition==end()) throw std::out_of_range("an attempt to delete the sentinel");
	possition.ptr->prev->next=possition.ptr->next;
	possition.ptr->next->prev=possition.ptr->prev;
	delete possition.ptr;
	--my_size;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
	/*for(auto tmp=firstIncluded;tmp!=lastExcluded;)
		{
			auto i=++tmp;
			erase(tmp);
			tmp=i;
		}*/
    size_type s = 0;
    for (auto tmp = firstIncluded; tmp != lastExcluded ;++tmp) ++s;
    auto tmp = lastExcluded;
    for(size_type j=0 ; j<s; ++j)
    {
    erase(tmp-1);
    }
  }

  iterator begin()
  {
	iterator tmp(first->next);
	return tmp;
  }

  iterator end()
  {
	iterator tmp(last);
	return tmp;
  }

  const_iterator cbegin() const
  {
	const_iterator tmp(first->next);
	return tmp;
  }

  const_iterator cend() const
  {
	const_iterator tmp(last);
	return tmp;
  }

  const_iterator begin() const
  {
	return cbegin();
  }

  const_iterator end() const
  {
	return cend();
  }
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;
private:
  Node* ptr;
  friend void LinkedList<Type>::erase(const const_iterator& possition);
  friend void LinkedList<Type>::erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded);
  friend void LinkedList<Type>::insert(const const_iterator& insertPosition, const Type& item);
public:
  explicit ConstIterator(Node* pointer): ptr(pointer)
  {}

  reference operator*() const
  {
	if (ptr->prev==nullptr || ptr->next==nullptr) throw std::out_of_range("an attempt to get the value of sentinel");
	return(ptr->data);
  }

  ConstIterator& operator++()
  {
	if(ptr->next==nullptr) throw std::out_of_range("an attempt to get out of the list");
	ptr=ptr->next;
	return *this;
  }

  ConstIterator operator++(int)//postinkrementacja
  {
	if(ptr->next==nullptr) throw std::out_of_range("an attempt to get out of the list");
	auto tmp=*this;
	ptr=ptr->next;
	return tmp;
  }

  ConstIterator& operator--()
  {
	if(ptr->prev->prev==nullptr) throw std::out_of_range("an attempt to get to sentinel");//nie pozwalam wejsc na wartownika
	ptr=ptr->prev;
	return *this;
  }

  ConstIterator operator--(int)//postdekrementacja
  {
	if(ptr->prev->prev==nullptr) throw std::out_of_range("an attempt to get to sentinel");
	auto tmp=*this;
	ptr=ptr->prev;
	return tmp;
  }

  ConstIterator operator+(difference_type d) const
  {
	auto tmp=*this;
	if(d>0)
	{
		for(difference_type i = 0 ; i<d ;++i)
        {
            ++tmp;
        //if(tmp.ptr == nullptr) throw std::out_of_range("an attempt to get out of the list");
        }
	}
	else if(d==0) return tmp;
	else
	{
		for (difference_type i = d ; i<0 ; ++i)
        {
            --tmp;
            //if(tmp.ptr == nullptr) throw std::out_of_range("an attempt to get to the sentinel");
        }
	}
	return tmp;
  }

  ConstIterator operator-(difference_type d) const
  {
	auto tmp=*this;
	if(d>0)
	{
		for (difference_type i = 0 ; i<d ; ++i)
        {
            --tmp;
            //if(tmp.ptr == nullptr) throw std::out_of_range("an attempt to get to the sentinel");
        }
	}
	else if(d==0) return tmp;
	else
	{
		for(difference_type i = 0 ; i<-d ;++i)
        {
            ++tmp;
        //if(tmp.ptr == nullptr) throw std::out_of_range("an attempt to get out of the list");
        }
	}
	return tmp;
  }

  bool operator==(const ConstIterator& other) const
  {
	if(this->ptr==other.ptr) return true;
	else return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
	if(this->ptr!=other.ptr) return true;
	else return false;
  }
};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(Node* ptr=nullptr): ConstIterator(ptr)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
	ConstIterator::operator++();
	return *this;
  }

  Iterator operator++(int)
  {
	auto result = *this;
	ConstIterator::operator++();
	return result;
  }

  Iterator& operator--()
  {
	ConstIterator::operator--();
	return *this;
  }

  Iterator operator--(int)
  {
	auto result = *this;
	ConstIterator::operator--();
	return result;
  }

  Iterator operator+(difference_type d) const
  {
	return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
	return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
	return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
