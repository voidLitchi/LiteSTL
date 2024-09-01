//vector ver.0.1
#pragma once
#include "general_define.h"

#include<memory>
#include<exception>

namespace NAME_GENERAL_NAMESPACE
{
	using std::allocator;//my allocator has not been implemented yet.
	template<typename DataType, typename Alloc = allocator<DataType>>
	class vector
	{
	private:
		DataType* p_data;
		size_t n_size;
		size_t n_capacity;
		using AllocTraits = std::allocator_traits<Alloc>;
		Alloc alloc;

	public:
		class const_iterator;
		class iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		vector() :p_data(nullptr), n_size(0), n_capacity(0), alloc(Alloc()) {}
		vector(const size_t _size, const DataType& _val = DataType()) :n_size(_size), n_capacity(_size), alloc(Alloc())
		{
			p_data = AllocTraits::allocate(alloc, _size);
			for (size_t i = 0; i < _size; ++i)
			{
				AllocTraits::construct(alloc, p_data + i, _val);
			}
		}
		vector(std::initializer_list<DataType> _list) :alloc(Alloc())
		{
			size_t _size = _list.size();
			p_data = AllocTraits::allocate(alloc, _size);
			n_capacity = _size;
			size_t offset = 0;
			for (auto p = _list.begin(); p != _list.end(); ++p, ++offset)
			{
				AllocTraits::construct(alloc, p_data + offset, *p);
			}
			n_size = _size;
		}
		template<typename Itr, typename = std::enable_if_t<std::_Is_iterator_v<Itr>>>
		vector(Itr _begin, Itr _end) :p_data(nullptr), n_size(0), n_capacity(0), alloc(Alloc())
		{
			for (auto p = _begin; p != _end; ++p)
			{
				push_back(*p);
			}
		}
		vector(const vector& other) :alloc(Alloc())
		{
			p_data = AllocTraits::allocate(alloc, other.n_size);
			n_capacity = other.n_size;
			for (size_t i = 0; i < other.n_size; ++i)
			{
				AllocTraits::construct(alloc, p_data + i, other.p_data + i);
			}
			n_size = other.n_size;
		}
		vector(vector&& other)noexcept :p_data(other.p_data), n_size(other.n_size), n_capacity(other.n_capacity), alloc(other.alloc)
		{
			other.p_data = nullptr;
		}

		~vector()
		{
			clear();
			if (p_data)
			{
				AllocTraits::deallocate(alloc, p_data, n_capacity);
			}
		}

		size_t size()const
		{
			return n_size;
		}
		size_t capacity()const
		{
			return n_capacity;
		}

		void resize(const size_t _size, const DataType& _val = DataType())
		{
			if (_size < n_size)
			{
				for (size_t i = _size; i < n_size; ++i)
				{
					AllocTraits::destroy(alloc, p_data + i);
				}
			}
			else if (_size > n_size)
			{
				if (_size > n_capacity)
				{
					reserve(_size);
				}
				for (size_t i = n_size; i < _size; i++)
				{
					AllocTraits::construct(alloc, p_data + i, _val);
				}
			}
			n_size = _size;
		}
		void clear()
		{
			for (size_t i = 0; i < n_size; ++i)
			{
				AllocTraits::destroy(alloc, p_data + i);
			}
			n_size = 0;
		}
		void reserve(const size_t _capacity)
		{
			if (_capacity > n_capacity)
			{
				DataType* new_data = AllocTraits::allocate(alloc, _capacity);
				for (size_t i = 0; i < n_size; ++i)
				{
					AllocTraits::construct(alloc, new_data + i, std::move(p_data[i]));
					AllocTraits::destroy(alloc, p_data + i);
				}
				if (p_data)
					AllocTraits::deallocate(alloc, p_data, n_capacity);
				p_data = new_data;
				n_capacity = _capacity;
			}
		}

		void push_back(const DataType& _data)
		{
			if (n_capacity == n_size)
			{
				expand_capacity();
			}
			AllocTraits::construct(alloc, p_data + n_size, _data);
			++n_size;
		}
		template<typename... Args>
		void emplace_back(Args... args)
		{
			if (n_capacity == n_size)
			{
				expand_capacity();
			}
			AllocTraits::construct(alloc, p_data + n_size, std::forward<Args>(args)...);
			++n_size;
		}

		const_iterator begin()const
		{
			return const_iterator(p_data, this);
		}
		iterator begin()
		{
			return iterator(p_data, this);
		}
		const_iterator cbegin()const
		{
			return const_iterator(p_data, this);
		}
		const_iterator end()const
		{
			return const_iterator(p_data + n_size, this);
		}
		iterator end()
		{
			return iterator(p_data + n_size, this);
		}
		const_iterator cend()const
		{
			return const_iterator(p_data + n_size, this);
		}
		const_reverse_iterator rbegin()const
		{
			return const_reverse_iterator(end());
		}
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}
		const_reverse_iterator crbegin()const
		{
			return const_reverse_iterator(end());
		}
		const_reverse_iterator rend()const
		{
			return const_reverse_iterator(begin());
		}
		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}
		const_reverse_iterator crend()const
		{
			return const_reverse_iterator(begin());
		}

		DataType& operator[](size_t index)
		{
#ifdef _DEBUG
			_STL_ASSERT(index < n_size, "vector subscript out of range");
#endif // _DEBUG
			return *(p_data + index);
		}
		const DataType& operator[](size_t index)const
		{
#ifdef _DEBUG
			_STL_ASSERT(index < n_size, "vector subscript out of range");
#endif // _DEBUG
			return *(p_data + index);
		}

	private:
		void expand_capacity()
		{
			if (n_capacity / 2)reserve(n_capacity + n_capacity / 2);
			else reserve(n_capacity + 1);
		}

	};

	template<typename DataType, typename Alloc>
	class vector<DataType, Alloc>::const_iterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = DataType;
		using difference_type = std::ptrdiff_t;
		using pointer = DataType*;
		using reference = DataType&;

		using container_type = vector<DataType, Alloc>;
		friend class container_type;

	protected:
		pointer ptr;
		const container_type* source;

	public:
		const_iterator()noexcept :ptr(nullptr), source(nullptr) {}
		/*
		const_iterator(const const_iterator& itr)noexcept :ptr(itr.ptr), source(itr.source) {}
		const_iterator(const_iterator&& itr)noexcept :ptr(itr.ptr), source(itr.source)
		{
			itr.ptr = nullptr;
			itr.source = nullptr;
		}
		*/

	protected:
		//This constructor has been hidden to user in our implementation, which is diffrent from the std implementation.
		const_iterator(pointer _ptr, const container_type* _source)noexcept :ptr(_ptr), source(_source) {}

		void compatible_check(const const_iterator& other)const noexcept
		{
#ifndef _DEBUG
			(void) other;
#else
			_STL_ASSERT(source == other.source, "vector iterators incompatible");
#endif // _DEBUG
		}
		void offset_check(difference_type offset)const noexcept
		{
#ifndef _DEBUG
			(void) offset;
#else
			_STL_ASSERT(offset == 0 || ptr, "cannot seek value-initialized vector iterator");
			_STL_ASSERT(offset == 0 || source, "cannot seek invalidated vector iterator");
			if (offset < 0) {
				_STL_ASSERT(offset >= source->p_data - ptr, "cannot seek vector iterator before begin");
			}

			if (offset > 0) {
				_STL_ASSERT(offset <= source->p_data - ptr, "cannot seek vector iterator after end");
			}
#endif // _DEBUG
		}

	public:
		const reference operator*()const noexcept
		{
#ifdef _DEBUG
			_STL_VERIFY(ptr, "can't dereference value-initialized vector iterator");
			_STL_VERIFY(source, "can't dereference invalidated vector iterator");
			_STL_VERIFY(
				source->p_data <= ptr && ptr < source->p_data + source->n_size, "can't dereference out of range vector iterator");
#endif // _DEBUG
			return *ptr;
		}

		const pointer operator->()const noexcept
		{
#ifdef _DEBUG
			_STL_VERIFY(ptr, "can't dereference value-initialized vector iterator");
			_STL_VERIFY(source, "can't dereference invalidated vector iterator");
			_STL_VERIFY(
				source->p_data <= ptr && ptr < source->p_data + source->n_size, "can't dereference out of range vector iterator");
#endif // _DEBUG
			return ptr;
		}

		bool operator==(const const_iterator& other)const noexcept
		{
			compatible_check(other);
			return ptr == other.ptr;
		}
		bool operator!=(const const_iterator& other)const noexcept
		{
			return !operator==(other);
		}
		
		const_iterator& operator++()noexcept
		{
#ifdef _DEBUG
			_STL_ASSERT(ptr, "can't increment value-initialized vector iterator");
			_STL_ASSERT(source, "can't increment invalidated vector iterator");
			_STL_ASSERT(ptr < source->p_data + source->n_size, "can't increment vector iterator past end");
#endif // _DEBUG
			ptr++;
			return *this;
		}
		const_iterator operator++(int)noexcept
		{
			const_iterator re = *this;
			operator++();
			return re;
		}

		const_iterator& operator--()noexcept
		{
#ifdef _DEBUG
			_STL_ASSERT(ptr, "can't decrement value-initialized vector iterator");
			_STL_ASSERT(source, "can't decrement invalidated vector iterator");
			_STL_ASSERT(source->p_data < ptr, "can't decrement vector iterator before begin");
#endif // _DEBUG
			ptr--;
			return *this;
		}
		const_iterator operator--(int)noexcept
		{
			const_iterator re = *this;
			operator--();
			return re;
		}

		bool operator>(const const_iterator& other)const noexcept
		{
			compatible_check(other);
			return ptr > other.ptr;
		}
		bool operator<(const const_iterator& other)const noexcept
		{
			compatible_check(other);
			return ptr < other.ptr;
		}
		bool operator>=(const const_iterator& other)const noexcept
		{
			compatible_check(other);
			return ptr >= other.ptr;
		}
		bool operator<=(const const_iterator& other)const noexcept
		{
			compatible_check(other);
			return ptr <= other.ptr;
		}

		const_iterator& operator+=(difference_type offset)noexcept
		{
			offset_check(offset);
			ptr += offset;
			return *this;
		}
		const_iterator operator+(difference_type offset)const noexcept
		{
			const_iterator re = *this;
			re += offset;
			return re;
		}
		const_iterator& operator-=(difference_type offset)noexcept
		{
			offset_check(-offset);
			ptr -= offset;
			return *this;
		}
		const_iterator operator-(difference_type offset)const noexcept
		{
			const_iterator re = *this;
			re -= offset;
			return re;
		}
		difference_type operator-(const const_iterator& other)const noexcept
		{
			compatible_check(other);
			return other.ptr - ptr;
		}

		const reference operator[](const difference_type offset)const noexcept
		{
			return *(*this + offset);
		}

	};

	template<typename DataType, typename Alloc>
	class vector<DataType, Alloc>::iterator :public const_iterator
	{
	public:
		using iterator_category = std::random_access_iterator_tag;
		using value_type = DataType;
		using difference_type = std::ptrdiff_t;
		using pointer = DataType*;
		using reference = DataType&;

		using container_type = vector<DataType, Alloc>;
		friend class container_type;

	public:
		iterator() :const_iterator() {}
		/*
		iterator(const iterator& itr) :const_iterator(itr) {}
		iterator(iterator&& itr) :const_iterator(std::forward<iterator>(itr)) {}
		*/

	protected:
		iterator(pointer _ptr, container_type* _source) :const_iterator(_ptr, _source) {}

	public:
		reference operator*()const noexcept
		{
			return const_cast<reference>(const_iterator::operator*());
		}
		const pointer operator->()const noexcept
		{
#ifdef _DEBUG
			_STL_VERIFY(this->ptr, "can't dereference value-initialized vector iterator");
			_STL_VERIFY(this->source, "can't dereference invalidated vector iterator");
			_STL_VERIFY(
				this->source->p_data <= this->ptr && this->ptr < this->source->p_data + this->source->n_size, "can't dereference out of range vector iterator");
#endif // _DEBUG
			return this->ptr;
		}
		
		iterator& operator++()noexcept
		{
			const_iterator::operator++();
			return *this;
		}
		iterator operator++(int)noexcept
		{
			iterator re = *this;
			const_iterator::operator++();
			return re;
		}
		
		iterator& operator--()noexcept
		{
			const_iterator::operator--();
			return *this;
		}
		iterator operator--(int)noexcept
		{
			iterator re = *this;
			const_iterator::operator--();
			return re;
		}

		iterator& operator+=(difference_type offset)noexcept
		{
			const_iterator::operator+=(offset);
			return *this;
		}
		iterator operator+(difference_type offset)const noexcept
		{
			iterator re = *this;
			re += offset;
			return re;
		}
		iterator& operator-=(difference_type offset)noexcept
		{
			const_iterator::operator-=(offset);
			return *this;
		}
		//using const_iterator::operator-;
		difference_type operator-(const iterator& other)const noexcept
		{
			return const_iterator::operator-(other);
		}
		iterator operator-(difference_type offset)const noexcept
		{
			iterator re = *this;
			re -= offset;
			return re;
		}

		reference operator[](const difference_type offset)const noexcept
		{
			return const_cast<reference>(const_iterator::operator[](offset));
		}

	};
}