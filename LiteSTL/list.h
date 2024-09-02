//list ver.0.1
#pragma once
#include "general_define.h"

#include<memory>
#include<exception>

namespace NAME_GENERAL_NAMESPACE 
{
	using std::allocator;
	template<typename DataType, typename Alloc = allocator<DataType>>
	class list
	{
	private:
		struct Node
		{
			DataType data;
			Node* next;
			Node* prev;
			Node(const DataType& _data) :data(_data), next(nullptr), prev(nullptr) {}
			Node(DataType&& _data) :data(std::move(_data)), next(nullptr), prev(nullptr) {}
			template<typename... Args>
			Node(Args&&... args) : data(std::forward<Args>(args)...), next(nullptr), prev(nullptr) {}
		};

		using NodeAlloc = typename Alloc::template rebind<Node>::other; // 使用rebind绑定到Node类型
		using AllocTraits = std::allocator_traits<NodeAlloc>; // 使用Node类型的分配器

		using value_type = typename AllocTraits::value_type;
		using allocator_type = Alloc;
		using size_type = typename AllocTraits::size_type;
		using difference_type = typename AllocTraits::difference_type;
		using pointer = typename AllocTraits::pointer;
		using const_pointer = typename AllocTraits::const_pointer;
		using reference = DataType&;
		using const_reference = const DataType&;

		struct Container
		{
			Node* p_head;
			Node* p_tail;
			size_type n_size;
			Container(Node* _head = nullptr, Node* _tail = nullptr, size_type _size = 0)
				:p_head(_head), p_tail(_tail), n_size(_size) {}
		};

		Container* cont;
		NodeAlloc alloc;

		template<typename Itr>
		void _append_range(Itr _begin, const Itr _end)
		{
			for (; _begin != _end; ++_begin)
			{
				Node* new_node = AllocTraits::allocate(alloc, 1);
				AllocTraits::construct(alloc, new_node, *_begin);
				if (!cont->n_size)
				{
					cont->p_head = new_node;
					cont->p_tail = new_node;
					++cont->n_size;
				}
				else
				{
					cont->p_tail->next = new_node;
					new_node->prev = cont->p_tail;
					cont->p_tail = new_node;
					++cont->n_size;
				}
			}
		}

	public:
		list() :alloc(Alloc()), cont(new Container()) {}
		template<typename Itr, typename = std::enable_if_t<std::_Is_iterator_v<Itr>>>
		list(Itr _begin, Itr _end) :alloc(Alloc()), cont(new Container())
		{
			_append_range(_begin, _end);
		}
		list(std::initializer_list<DataType> _list) :alloc(Alloc()), cont(new Container())
		{
			_append_range(_list.begin(), _list.end());
		}

		~list()
		{
			if (cont)
			{
				clear();
			}
			delete cont;
		}

		void clear()
		{
			Node* p = cont->p_head;
			while (p)
			{
				Node* next = p->next;
				AllocTraits::destroy(alloc, p);
				AllocTraits::deallocate(alloc, p, 1);
				p = next;
			}
			cont->p_head = nullptr;
			cont->p_tail = nullptr;
			cont->n_size = 0;
		}

	};
}