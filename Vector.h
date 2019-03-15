#ifndef AISDI_LINEAR_VECTOR_H
#define AISDI_LINEAR_VECTOR_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class Vector
{
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
  size_type my_size;
  size_type my_capacity;
  pointer my_arr;

public:
  Vector()
  {
	my_size=0;
	my_capacity=0;
	my_arr=nullptr;
  }

  Vector(std::initializer_list<Type> l)
  {
	my_size=0;
	my_capacity=l.size();
	my_arr=new value_type[my_capacity];
	for(auto tmp=l.begin(); tmp!=l.end(); ++tmp)
		append(*tmp);
  }

  Vector(const Vector& other)
  {
	my_arr=new value_type[other.my_size];
	my_capacity=other.my_capacity;
	my_size=0;
	for(auto tmp=other.begin();tmp!=other.end();++tmp)
		append(*tmp);
  }

  Vector(Vector&& other)
  {
	my_arr=other.my_arr;
	my_size=other.my_size;
	my_capacity=other.my_capacity;
	other.my_arr=nullptr;
	other.my_size=0;
	other.my_capacity=0;
  }

  ~Vector()
  {
	delete []my_arr;
  }

  Vector& operator=(const Vector& other)
  {
	if(my_arr==other.my_arr && my_size==other.my_size && my_capacity==other.my_capacity)
		return *this;
	if(!isEmpty())
		erase(begin(),end());
	for(auto tmp=other.begin();tmp!=other.end();++tmp)
		append(*tmp);
	return *this;
  }

  Vector& operator=(Vector&& other)
  {
	pointer tmp=my_arr;
	my_arr=other.my_arr;
	my_size=other.my_size;
	my_capacity=other.my_capacity;
	other.my_arr=nullptr;
	other.my_size=0;
	other.my_capacity=0;
	delete []tmp;
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

  void extend()
  {
	if(my_capacity==0) my_capacity=10;
	else my_capacity=2*my_capacity;
    pointer n= new value_type[my_capacity];
	for(size_type tmp=0; tmp<my_size; ++tmp)
		n[tmp]=my_arr[tmp];
	delete [] my_arr;
	my_arr=n;
  }

  void append(const Type& item)
  {
	if(my_size+1>my_capacity)
		extend();
	my_arr[my_size]=item;
	++my_size;
  }

  void prepend(const Type& item)
  {
	if(my_arr==nullptr)
		my_arr=new value_type [1];
	if(my_size+1>=my_capacity)
		extend();
	for(size_type tmp=my_size; tmp>0; --tmp)
		my_arr[tmp]=my_arr[tmp-1];
	my_arr[0]=item;
	++my_size;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
	if(insertPosition==begin()) prepend(item);
	else if(insertPosition==end()) append(item);
	else
	{
		if(my_arr==nullptr)
			my_arr=new value_type [1];
		if(my_size+1>=my_capacity)
			extend();
		for(size_type tmp=my_size; tmp>insertPosition.i; --tmp)
			my_arr[tmp]=my_arr[tmp-1];
		my_arr[insertPosition.i]=item;
		++my_size;
	}
  }

  Type popFirst()
  {
	if(isEmpty()) throw std::out_of_range("Empty vector");
	value_type tmp=my_arr[0];
	for(size_type i=0; i<my_size-1; ++i)
		my_arr[i]= my_arr[i+1];
	--my_size;
	return tmp;
  }

  Type popLast()
  {
	if(isEmpty()) throw std::out_of_range("Empty vector");
	value_type tmp=my_arr[my_size-1];
	--my_size;
	return tmp;
  }

  void erase(const const_iterator& possition)
  {
	if(isEmpty()) throw std::out_of_range("Empty vector");
	if(possition==end()) throw std::out_of_range("an attempt to delete the element otside the vector");
	for(size_type i=possition.i; i<my_size; ++i)
		my_arr[i]= my_arr[i+1];
	--my_size;
  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
	if(isEmpty()) throw std::out_of_range("Empty vector");
	int s=lastExcluded.i-firstIncluded.i;
	if(s<0) throw std::out_of_range("last<first");
	for(size_type i=firstIncluded.i; i+s<my_size; ++i)
		{
			my_arr[i]= my_arr[i+s];
		}
		my_size=my_size-s;
		/*size_type s=0;
		s=lastExcluded.i-firstIncluded.i;
		auto  it = lastExcluded;
		for(size_type j=0 ; j<s; ++j)
        {
            erase(it-1);
        }*/
  }

  iterator begin()
  {
	iterator tmp(this,0);
	return tmp;
  }

  iterator end()
  {
	iterator tmp(this,my_size);
	return tmp;
  }

  const_iterator cbegin() const
  {
	const_iterator tmp(this,0);
	return tmp;
  }

  const_iterator cend() const
  {
	const_iterator tmp(this,my_size);
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
class Vector<Type>::ConstIterator
{
public:
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename Vector::value_type;
  using difference_type = typename Vector::difference_type;
  using pointer = typename Vector::const_pointer;
  using reference = typename Vector::const_reference;
private:
  const Vector<value_type> *ptr;
  size_t i;
  friend void Vector<Type>::erase(const const_iterator&);
  friend void Vector<Type>::erase(const const_iterator&, const const_iterator&);
  friend void Vector<Type>::insert(const const_iterator&, const Type&);
public:
  explicit ConstIterator(const Vector<Type>* tmp=nullptr,int x=0): ptr(tmp), i(x)
  {}

  reference operator*() const
  {
	if(i>=ptr->my_size) throw std::out_of_range("an attempt to get the value of the element outside the vector");
	return(ptr->my_arr[i]);
  }

  ConstIterator& operator++()
  {
	if(i>=ptr->my_size) throw std::out_of_range("an attemt to get out of vector");
	++i;
	return *this;
  }

  ConstIterator operator++(int)
  {
	if(i>=ptr->my_size) throw std::out_of_range("an attemt to get out of vector");
	auto tmp=*this;
	++i;
	return tmp;
  }

  ConstIterator& operator--()
  {
	if(i<=0) throw std::out_of_range("an attemt to get out of vector");
	--i;
	return *this;
  }

  ConstIterator operator--(int)
  {
	if(i<=0) throw std::out_of_range("an attemt to get out of vector");
	auto tmp=*this;
	--i;
	return tmp;
  }

  ConstIterator operator+(difference_type d) const
  {
	if(i+d>ptr->my_size) throw std::out_of_range("an attemt to get out of vector");
	ConstIterator tmp=*this;
	tmp.i=tmp.i+d;
	return tmp;
  }

  ConstIterator operator-(difference_type d) const
  {
	if(i-d<0) throw std::out_of_range("an attemt to get out of vector");
	ConstIterator tmp=*this;
	tmp.i=tmp.i-d;
	return tmp;
  }

  bool operator==(const ConstIterator& other) const
  {
	if(ptr==other.ptr && i==other.i) return true;
	else return false;
  }

  bool operator!=(const ConstIterator& other) const
  {
	if(ptr!=other.ptr || i!=other.i) return true;
	else return false;
  }
};

template <typename Type>
class Vector<Type>::Iterator : public Vector<Type>::ConstIterator
{
public:
  using pointer = typename Vector::pointer;
  using reference = typename Vector::reference;

  explicit Iterator(const Vector<Type>* tmp=nullptr,int x=0): ConstIterator(tmp,x)
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

#endif // AISDI_LINEAR_VECTOR_H
