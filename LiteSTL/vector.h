//vector ver.0.2
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
		struct Container
		{
			DataType* p_begin;
			DataType* p_end;
			size_t n_capacity;
			Container(DataType* _begin = nullptr, DataType* _end = nullptr, size_t _capacity = 0)
				:p_begin(_begin), p_end(_end), n_capacity(_capacity) {}
		}*container;

		using AllocTraits = std::allocator_traits<Alloc>;
		Alloc alloc;

	public:
		class const_iterator;
		class iterator;
		using reverse_iterator = std::reverse_iterator<iterator>;
		using const_reverse_iterator = std::reverse_iterator<const_iterator>;

		vector() :alloc(Alloc()), container(new Container()) {}
		vector(const size_t _size, const DataType& _val = DataType()) :alloc(Alloc()), container(new Container())
		{
			container->p_begin = AllocTraits::allocate(alloc, _size);
			container->p_end = container->p_begin;
			container->n_capacity = _size;
			for (size_t index = 0; index < _size; ++index, ++container->p_end)
			{
				AllocTraits::construct(alloc, container->p_end, _val);
			}
		}
		vector(std::initializer_list<DataType> _list) :alloc(Alloc()), container(new Container())
		{
			size_t _size = _list.size();
			container->p_begin = AllocTraits::allocate(alloc, _size);
			container->p_end = container->p_begin;
			container->n_capacity = _size;
			for (auto p = _list.begin(); p != _list.end(); ++p, ++container->p_end)
			{
				AllocTraits::construct(alloc, container->p_end, std::move(*p));
			}
		}
		template<typename Itr, typename = std::enable_if_t<std::_Is_iterator_v<Itr>>>
		vector(Itr _begin, Itr _end) :alloc(Alloc()), container(new Container())
		{
			for (auto p = _begin; p != _end; ++p)
			{
				push_back(*p);
			}
		}
		vector(const vector& other) :alloc(Alloc()), container(new Container())
		{
			size_t _size = other.size();
			container->p_begin = AllocTraits::allocate(alloc, _size);
			container->p_end = container->p_begin;
			container->n_capacity = _size;
			for (DataType* p = other.container->p_begin; p != other.container->p_end; ++p, ++container->p_end)
			{
				AllocTraits::construct(alloc, container->p_end, *p);
			}
		}
		vector(vector&& other)noexcept :alloc(other.alloc), container(other.container)
		{
			other.container = nullptr;
		}

		vector& operator=(const vector& other)
		{
			if (this != &other)
			{
				vector tmp(other);
				swap(tmp);
			}
			return *this;
		}
		vector& operator=(vector&& other)noexcept
		{
			if (this != &other)
			{
				vector tmp(std::forward<vector>(other));
				swap(tmp);
			}
			return *this;
		}

		void swap(vector& other)noexcept
		{
			std::swap(container, other.container);
			std::swap(alloc, other.alloc);
		}
		friend void swap(vector& first, vector& second)noexcept
		{
			first.swap(second);
		}

		~vector()
		{
			clear();
			if (container)
			{
				if (container->p_begin)
				{
					AllocTraits::deallocate(alloc, container->p_begin, container->n_capacity);
				}
				delete container;
			}
		}

		size_t size()const
		{
			return container->p_end - container->p_begin;
		}
		size_t capacity()const
		{
			return container->n_capacity;
		}

		void resize(const size_t _size, const DataType& _val = DataType())
		{
			size_t cur_size = container->p_end - container->p_begin;
			if (_size < cur_size)
			{
				for (DataType* p = container->p_begin + _size; p != container->p_end; p++)
				{
					AllocTraits::destroy(alloc, p);
				}
			}
			else if (_size > cur_size)
			{
				if (_size > container->n_capacity)
				{
					reserve(_size);
				}
				for (DataType* p = container->p_end; p != container->p_begin + _size; p++)
				{
					AllocTraits::construct(alloc, p, _val);
				}
			}
			container->p_end = container->p_begin + _size;
		}
		void clear()
		{
			for (DataType* p = container->p_begin; p != container->p_end; p++)
			{
				AllocTraits::destroy(alloc, p);
			}
			container->p_end = container->p_begin;
		}
		void reserve(const size_t _capacity)
		{
			if (_capacity > container->n_capacity)
			{
				DataType* new_data = AllocTraits::allocate(alloc, _capacity);
				DataType* new_end = new_data;
				for (DataType* p = container->p_begin; p != container->p_end; p++, new_end++)
				{
					AllocTraits::construct(alloc, new_end, std::move(*p));
					AllocTraits::destroy(alloc, p);
				}
				if (container->p_begin)
				{
					AllocTraits::deallocate(alloc, container->p_begin, container->n_capacity);
				}
				container->p_begin = new_data;
				container->p_end = new_end;
				container->n_capacity = _capacity;
			}
		}

		void push_back(const DataType& _data)
		{
			if (container->n_capacity == container->p_end - container->p_begin)
			{
				expand_capacity();
			}
			AllocTraits::construct(alloc, container->p_end, _data);
			++container->p_end;
		}
		template<typename... Args>
		void emplace_back(Args... args)
		{
			if (container->n_capacity == container->p_end - container->p_begin)
			{
				expand_capacity();
			}
			AllocTraits::construct(alloc, container->p_end, std::forward<Args>(args)...);
			++container->p_end;
		}

		const_iterator begin()const
		{
			return const_iterator(container->p_begin, container);
		}
		iterator begin()
		{
			return iterator(container->p_begin, container);
		}
		const_iterator cbegin()const
		{
			return const_iterator(container->p_begin, container);
		}
		const_iterator end()const
		{
			return const_iterator(container->p_end, container);
		}
		iterator end()
		{
			return iterator(container->p_end, container);
		}
		const_iterator cend()const
		{
			return const_iterator(container->p_end, container);
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

		template <typename... Args>
		iterator emplace(const_iterator _where, Args&&... args)
		{
#ifdef _DEBUG
			_STL_ASSERT(container == _where.source, "vector iterators incompatible");
#endif // _DEBUG
			if (_where.ptr == container->p_end)
			{
				emplace_back(std::forward<Args>(args)...);
				return iterator(container->p_end - 1, container);
			}
			if (container->n_capacity == container->p_end - container->p_begin)
			{
				size_t new_capacity = container->n_capacity;
				new_capacity += ((new_capacity >> 1) ? (new_capacity >> 1) : 1);
				DataType* new_begin = AllocTraits::allocate(alloc, new_capacity);
				DataType* new_end = new_begin;
				for (DataType* p = container->p_begin; p != _where.ptr; ++p, ++new_end)
				{
					AllocTraits::construct(alloc, new_end, std::move(*p));
					AllocTraits::destroy(alloc, p);
				}
				AllocTraits::construct(alloc, new_end, std::forward<Args>(args)...);
				DataType* re = new_end;
				++new_end;
				for (DataType* p = _where.ptr; p != container->p_end; ++p, ++new_end)
				{
					AllocTraits::construct(alloc, new_end, std::move(*p));
					AllocTraits::destroy(alloc, p);
				}
				AllocTraits::deallocate(alloc, container->p_begin, container->n_capacity);
				container->p_begin = new_begin;
				container->p_end = new_end;
				container->n_capacity = new_capacity;
				return iterator(re, container);
			}
			else
			{
				AllocTraits::construct(alloc, container->p_end, std::move(*(container->p_end - 1)));
				container->p_end++;
				for (DataType* p = container->p_end - 2; p != _where.ptr; --p)
				{
					*p = std::move(*(p - 1));
				}
				*(_where.ptr) = DataType(std::forward<Args>(args)...);
				return iterator(_where.ptr, container);
			}
		}
		iterator insert(const_iterator _where, std::initializer_list<DataType> _list)
		{
#ifdef _DEBUG
			_STL_ASSERT(container == _where.source, "vector iterators incompatible");
#endif // _DEBUG
			size_t list_size = _list.size();
			if (list_size > container->n_capacity - (container->p_end - container->p_begin))
			{
				size_t new_capacity = container->n_capacity;
				size_t target_capacity = list_size + (container->p_end - container->p_begin);
				while (new_capacity < target_capacity)
				{
					new_capacity += ((new_capacity >> 1) ? (new_capacity >> 1) : 1);
				}
				DataType* new_begin = AllocTraits::allocate(alloc, new_capacity);
				DataType* new_end = new_begin;
				for (DataType* p = container->p_begin; p != _where.ptr; ++p, ++new_end)
				{
					AllocTraits::construct(alloc, new_end, std::move(*p));
					AllocTraits::destroy(alloc, p);
				}
				DataType* re = new_end;
				for (const DataType* p = _list.begin(); p != _list.end(); ++p, ++new_end)
				{
					AllocTraits::construct(alloc, new_end, std::move(*p));
				}
				for (DataType* p = _where.ptr; p != container->p_end; ++p, ++new_end)
				{
					AllocTraits::construct(alloc, new_end, std::move(*p));
					AllocTraits::destroy(alloc, p);
				}
				AllocTraits::deallocate(alloc, container->p_begin, container->n_capacity);
				container->p_begin = new_begin;
				container->p_end = new_end;
				container->n_capacity = new_capacity;
				return iterator(re, container);
			}
			else
			{
				DataType* new_end = container->p_end + list_size;
				DataType* p_backward = new_end;
				for (DataType* p = container->p_end - 1; p != _where.ptr - 1; --p, --p_backward)
				{
					if (p_backward < container->p_end)
					{
						*p_backward = std::move(*p);
					}
					else
					{
						AllocTraits::construct(alloc, p_backward, std::move(*p));
					}
				}
				DataType* p_forward = _where.ptr;
				for (const DataType* p = _list.begin(); p != _list.end(); ++p, ++p_forward)
				{
					if (p_forward < container->p_end)
					{
						*p_forward = std::move(*p);
					}
					else
					{
						AllocTraits::construct(alloc, p_forward, std::move(*p));
					}
				}
				container->p_end = new_end;
				return iterator(_where.ptr, container);
			}
		}

		DataType& operator[](size_t index)
		{
#ifdef _DEBUG
			_STL_ASSERT(index < size(), "vector subscript out of range");
#endif // _DEBUG
			return *(container->p_begin + index);
		}
		const DataType& operator[](size_t index)const
		{
#ifdef _DEBUG
			_STL_ASSERT(index < size(), "vector subscript out of range");
#endif // _DEBUG
			return *(container->p_begin + index);
		}
		DataType& front()
		{
			return *(container->p_begin);
		}
		const DataType& front()const
		{
			return *(container->p_begin);
		}
		DataType& back()
		{
			return *(container->p_end - 1);
		}
		const DataType& back()const
		{
			return *(container->p_end - 1);
		}

	private:
		void expand_capacity()
		{
			if ((container->n_capacity) >> 1)reserve(container->n_capacity + ((container->n_capacity) >> 1));
			else reserve(container->n_capacity + 1);
		}
		void expand_capacity(size_t increment)
		{
			size_t tar_capacity = container->n_capacity;
			while (tar_capacity < container->n_capacity + increment)
			{
				if (tar_capacity >> 1)tar_capacity += (tar_capacity >> 1);
				else tar_capacity++;
			}
			reserve(tar_capacity);
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

		friend class vector<DataType, Alloc>;

	protected:
		pointer ptr;
		const Container* source;
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
		const_iterator(pointer _ptr, const Container* _source)noexcept :ptr(_ptr), source(_source) {}

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
				_STL_ASSERT(offset >= source->p_begin - ptr, "cannot seek vector iterator before begin");
			}

			if (offset > 0) {
				_STL_ASSERT(offset <= source->p_begin - ptr, "cannot seek vector iterator after end");
			}
#endif // _DEBUG
		}

	public:
		const reference operator*()const noexcept
		{
#ifdef _DEBUG
			_STL_ASSERT(ptr, "can't dereference value-initialized vector iterator");
			_STL_ASSERT(source, "can't dereference invalidated vector iterator");
			_STL_ASSERT(
				source->p_begin <= ptr && ptr < source->p_end, "can't dereference out of range vector iterator");
#endif // _DEBUG
			return *ptr;
		}

		const pointer operator->()const noexcept
		{
#ifdef _DEBUG
			_STL_ASSERT(ptr, "can't dereference value-initialized vector iterator");
			_STL_ASSERT(source, "can't dereference invalidated vector iterator");
			_STL_ASSERT(
				source->p_begin <= ptr && ptr < source->p_end, "can't dereference out of range vector iterator");
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
			_STL_ASSERT(ptr < source->p_end, "can't increment vector iterator past end");
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
			_STL_ASSERT(source->p_begin < ptr, "can't decrement vector iterator before begin");
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

		friend class vector<DataType, Alloc>;

	public:
		iterator() :const_iterator() {}
		/*
		iterator(const iterator& itr) :const_iterator(itr) {}
		iterator(iterator&& itr) :const_iterator(std::forward<iterator>(itr)) {}
		*/

	protected:
		iterator(pointer _ptr, Container* _source) :const_iterator(_ptr, _source) {}

	public:
		reference operator*()const noexcept
		{
			return const_cast<reference>(const_iterator::operator*());
		}
		const pointer operator->()const noexcept
		{
#ifdef _DEBUG
			_STL_ASSERT(this->ptr, "can't dereference value-initialized vector iterator");
			_STL_ASSERT(this->source, "can't dereference invalidated vector iterator");
			_STL_ASSERT(
				this->source->p_begin <= this->ptr && this->ptr < this->source->p_end, "can't dereference out of range vector iterator");
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