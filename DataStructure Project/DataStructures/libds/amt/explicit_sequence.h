#pragma once

#include <libds/amt/abstract_memory_type.h>
#include <libds/amt/sequence.h>
#include <functional>
#include <iterator>

namespace ds::amt {

	template<typename BlockType>
	class ExplicitSequence :
		public Sequence<BlockType>,
		public ExplicitAMS<BlockType>
	{
	public:
		ExplicitSequence();
		ExplicitSequence(const ExplicitSequence& other);
		~ExplicitSequence() override;

		AMT& assign(const AMT& other) override;
		void clear() override;
		bool equals(const AMT& other) override;

		size_t calculateIndex(BlockType& data) override;

		BlockType* accessFirst() const override;
		BlockType* accessLast() const override;
		BlockType* access(size_t index) const override;
		BlockType* accessNext(const BlockType& block) const override;
		BlockType* accessPrevious(const BlockType& block) const override;

		BlockType& insertFirst() override;
		BlockType& insertLast() override;
		BlockType& insert(size_t index) override;
		BlockType& insertAfter(BlockType& block) override;
		BlockType& insertBefore(BlockType& block) override;

		void removeFirst() override;
		void removeLast() override;
		void remove(size_t index) override;
		void removeNext(const BlockType& block) override;
		void removePrevious(const BlockType& block) override;

	protected:
		virtual void connectBlocks(BlockType* previous, BlockType* next);
		virtual void disconnectBlock(BlockType* block);

		BlockType* first_;
		BlockType* last_;

	public:
		using DataType = decltype(BlockType().data_);

		class ExplicitSequenceIterator {
		public:
            explicit ExplicitSequenceIterator(BlockType* position);// posielame first_
			ExplicitSequenceIterator(const ExplicitSequenceIterator& other);//prvy neplatny, cize nullptr
			ExplicitSequenceIterator& operator++();//prefixovy
			ExplicitSequenceIterator operator++(int);//postfixovy, vracia kopiu cize skor ten prvy
			bool operator==(const ExplicitSequenceIterator& other) const;
			bool operator!=(const ExplicitSequenceIterator& other) const;
			DataType& operator*();//spristupni datatype

		private:
			BlockType* position_;
		};

		ExplicitSequenceIterator begin();
		ExplicitSequenceIterator end();

		using IteratorType = ExplicitSequenceIterator;
	};

	template<typename BlockType>
	using ES = ExplicitSequence<BlockType>;

	//----------

	template<typename DataType>
	struct SinglyLinkedSequenceBlock :
		public MemoryBlock<DataType>
	{

		SinglyLinkedSequenceBlock() : next_(nullptr) {}
		~SinglyLinkedSequenceBlock() { next_ = nullptr; }

		SinglyLinkedSequenceBlock<DataType>* next_;
	};
	template<typename DataType>
	using SLSBlock = SinglyLinkedSequenceBlock<DataType>;

	template<typename DataType>
	class SinglyLinkedSequence :
		public ES<SLSBlock<DataType>>
	{
	public:
		using BlockType = SinglyLinkedSequenceBlock<DataType>;
	};

	template<typename DataType>
	using SinglyLS = SinglyLinkedSequence<DataType>;

	template<typename DataType>
	class SinglyCyclicLinkedSequence :
		public SinglyLS<DataType>
	{
	};

	template<typename DataType>
	using SinglyCLS = SinglyCyclicLinkedSequence<DataType>;

	//----------

	template<typename DataType>
	struct DoublyLinkedSequenceBlock :
		public SLSBlock<DataType>
	{

		DoublyLinkedSequenceBlock() : previous_(nullptr) {}
		~DoublyLinkedSequenceBlock() { previous_ = nullptr; }

		DoublyLinkedSequenceBlock<DataType>* previous_;
	};

	template<typename DataType>
	using DLSBlock = DoublyLinkedSequenceBlock<DataType>;

	template<typename DataType>
	class DoublyLinkedSequence :
		public ES<DLSBlock<DataType>>
	{
	public:
		using BlockType = DLSBlock<DataType>;

		BlockType* access(size_t index) const override;
		BlockType* accessPrevious(const BlockType& block) const override;

		void removeFirst() override;

	protected:
		void connectBlocks(BlockType* previous, BlockType* next) override;
		void disconnectBlock(BlockType* block) override;
	};

    template<typename DataType>
	using DoublyLS = DoublyLinkedSequence<DataType>;

	template<typename DataType>
	class DoublyCyclicLinkedSequence :
		public DoublyLS<DataType>
	{
	};

    template<typename DataType>
	using DoublyCLS = DoublyCyclicLinkedSequence<DataType>;


	template<typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequence() :
		first_(nullptr),
		last_(nullptr)
	{
	}

	template<typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequence(const ExplicitSequence& other) :
	    ExplicitSequence()
	{
		assign(other);
	}

	template<typename BlockType>
    ExplicitSequence<BlockType>::~ExplicitSequence()
	{
		//04
		this->clear();
	}

	template<typename BlockType>
    AMT& ExplicitSequence<BlockType>::assign(const AMT& other)
	{
		if (this != &other)
		{
			clear();

			const ExplicitSequence<BlockType>& otherExplicitSequence = dynamic_cast<const ExplicitSequence<BlockType>&>(other);
			otherExplicitSequence.processAllBlocksForward([&](const BlockType* b)
			{
			    this->insertLast().data_ = b->data_;
			});
		}

		return *this;
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::clear()
	{
		// 04
		last_ = first_;
		while (last_ != nullptr) {
			first_ = this->accessNext(*first_);
			this->memoryManager_->releaseMemory(last_);
			last_ = first_;
		}
	}

	template<typename BlockType>
    bool ExplicitSequence<BlockType>::equals(const AMT& other)
	{
		// 04
		//ExplicitSequence<BlockType>& otherES = dynamic_cast<ExplicitSequence<BlockType>&>(other); // ak tu bude niec oine ako ES tak to bude chyba
		//ExplicitSequence<BlockType>* otherES = dynamic_cast<ExplicitSequence<BlockType>*>(&other); // ak neprejde tak tu bude nullptr
		if (this == &other)
		{
			return true;
		}

		auto* otherES = dynamic_cast<const ExplicitSequence<BlockType>*>(&other);
		if (otherES == nullptr)
		{
			return false;
		}

		if (this->size() != other.size())
		{
			return false;
		}
		//porovnavanie dvoch sekvenci ci su rovnake, cela metoda vlastne
		BlockType* thisCurrent = first_;
		BlockType* otherCurrent = otherES->first_;
		while (thisCurrent != nullptr)
		{
			if (thisCurrent->data_ == otherCurrent->data_)
			{
				thisCurrent = this->accessNext(*thisCurrent);
				otherCurrent = otherES->accessNext(*otherCurrent);
			}
			else
			{
				return false;
			}
		}
		return true;
	}

	template<typename BlockType>
    size_t ExplicitSequence<BlockType>::calculateIndex(BlockType& data)
	{
		// 04
		size_t counter = 0;
		BlockType* block = this->findBlockWithProperty([&counter, &data](BlockType* p) 
			{
				++counter;
				return &data == p;
			});
		return block == nullptr
			? INVALID_INDEX
			: counter - 1; // kvoli tomu ze vzdy to prejde aspon raz a hned prvy ma index 1 bez - 1
	}

	template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessFirst() const
	{
		return first_;
	}

	template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessLast() const
	{
		return last_;
	}

	template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::access(size_t index) const
	{
		// 04
		BlockType* result = nullptr;
		if (index < this->size()) //index >= 0 &&  toto netreba lebo size je unsigned
		{
			result = this->first_;
			for (size_t i = 0; i < index; i++)
			{
				//result = *result;
				result = this->accessNext(*result);
			}
		}
		return result;
	}

	template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessNext(const BlockType& block) const
	{
		return static_cast<BlockType*>(block.next_);
	}

	template<typename BlockType>
    BlockType* ExplicitSequence<BlockType>::accessPrevious(const BlockType& block) const
	{
		return this->findBlockWithProperty([&](BlockType* b) {return b->next_ == &block; });
	}

	template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertFirst()
	{
		// 04
		if (this->size() == 0)
		{
			first_ = last_ = this->memoryManager_->allocateMemory();
			return *first_;
		}
		else
		{
			return this->insertBefore(*first_); 
		}
	}

	template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertLast()
	{
		// 04
		if (this->size() == 0)
		{
			first_ = last_ = this->memoryManager_->allocateMemory();
			return *last_;
		}
		else
		{
			return this->insertAfter(*last_);
		}
	}

	template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insert(size_t index)
	{
		return index == 0 ? insertFirst() : index == this->size() ? insertLast() : insertAfter(*access(index - 1));
	}

	template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertAfter(BlockType& block)
	{
		// 04
		BlockType* nextBlock = this->accessNext(block);
		BlockType* newBlock = this->memoryManager_->allocateMemory();
		this->connectBlocks(&block, newBlock);
		this->connectBlocks(newBlock, nextBlock);

		if (last_ == &block)
		{
			last_ = newBlock;
		}
		return *newBlock;
	}

	template<typename BlockType>
    BlockType& ExplicitSequence<BlockType>::insertBefore(BlockType& block)
	{
		//04
		BlockType* previousBlock = this->accessPrevious(block);
		BlockType* newBlock = this->memoryManager_->allocateMemory();
		this->connectBlocks(previousBlock, newBlock);
		this->connectBlocks(newBlock, &block); 

		if (first_ == &block)
		{
			first_ = newBlock;
		}
		return *newBlock;
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::removeFirst()
	{
		// 04
		if (first_ == last_)
		{
			this->memoryManager_->releaseMemory(first_);
			first_ = last_ = nullptr;
		}
		else
		{
			BlockType* newFirst = this->accessNext(*first_);
			this->memoryManager_->releaseMemory(first_);
			first_ = newFirst;
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::removeLast()
	{
		// 04
		if (first_ == last_)
		{
			this->memoryManager_->releaseMemory(last_);
			first_ = last_ = nullptr;
		}
		else
		{
			BlockType* newLast = this->accessPrevious(*last_);
			this->memoryManager_->releaseMemory(last_);
			last_ = newLast;
			last_->next_ = nullptr;
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::remove(size_t index)
	{
		if (index == 0)
		{
			removeFirst();
		}
		else
		{
			removeNext(*access(index - 1));
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::removeNext(const BlockType& block)
	{
		// 04
		BlockType* mazanyBlok = this->accessNext(block);
		if (mazanyBlok == last_)
		{
			this->removeLast();
		}
		else
		{
			this->disconnectBlock(mazanyBlok);
			this->memoryManager_->releaseMemory(mazanyBlok);
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::removePrevious(const BlockType& block)
	{
		// 04
		BlockType* mazanyBlok = this->accessPrevious(block);
		if (mazanyBlok == first_)
		{
			this->removeFirst();
		}
		else
		{
			this->disconnectBlock(mazanyBlok);
			this->memoryManager_->releaseMemory(mazanyBlok);
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::connectBlocks(BlockType* previous, BlockType* next)
	{
		if (previous != nullptr)
		{
			previous->next_ = next;
		}
	}

	template<typename BlockType>
    void ExplicitSequence<BlockType>::disconnectBlock(BlockType* block)
	{
		connectBlocks(accessPrevious(*block), accessNext(*block));
		block->next_ = nullptr;
	}

    template <typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequenceIterator::ExplicitSequenceIterator(BlockType* position) :
		position_(position)
    {
    }

    template <typename BlockType>
    ExplicitSequence<BlockType>::ExplicitSequenceIterator::ExplicitSequenceIterator(
        const ExplicitSequenceIterator& other
	) :
		position_(other.position_)
    {
    }

    template <typename BlockType>
    auto ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator++() -> ExplicitSequenceIterator&
    {
		// 04
		position_ = static_cast<BlockType*>(position_->next_);
		return *this;
    }

    template <typename BlockType>
    auto ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator++(int) -> ExplicitSequenceIterator
    {
		ExplicitSequenceIterator tmp(*this); 
		operator++();
		//moze byt aj ++(*this)
	    return tmp;
    }

    template <typename BlockType>
    bool ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator==(const ExplicitSequenceIterator& other) const
    {
		// 04
		return position_ == other.position_;
    }

    template <typename BlockType>
    bool ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator!=(const ExplicitSequenceIterator& other) const
    {
		return !(*this == other);
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::DataType& ExplicitSequence<BlockType>::ExplicitSequenceIterator::operator*()
    {
		// 04
		return position_->data_;
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator ExplicitSequence<BlockType>::begin()
    {
		return ExplicitSequenceIterator(first_);
    }

    template <typename BlockType>
    typename ExplicitSequence<BlockType>::ExplicitSequenceIterator ExplicitSequence<BlockType>::end()
    {
		return ExplicitSequenceIterator(nullptr);
    }

    template<typename DataType>
    DLSBlock<DataType>* DoublyLinkedSequence<DataType>::access(size_t index) const
	{
		// 04
		BlockType* result = nullptr;
		if (index >= 0 && index < this->size())
		{
			if (index < this->size() / 2)
			{
				result = this->first_;
				for (size_t i = 1; i < index + 1; i++)
				{
					result = this->accessNext(*result);
				}
			}
			else 
			{
				result = this->last_;
				for (size_t i = this->size() - index; i > 1; i--)
				{			
					result = this->accessPrevious(*result);
				}
			}
		}
		return result;
	}

	template<typename DataType>
    DLSBlock<DataType>* DoublyLinkedSequence<DataType>::accessPrevious(const BlockType& block) const
	{
		return block.previous_;
	}

	template<typename DataType>
    void DoublyLinkedSequence<DataType>::removeFirst()
	{
		ExplicitSequence<BlockType>::removeFirst();

		if (ExplicitSequence<BlockType>::first_ != nullptr)
		{
			ExplicitSequence<BlockType>::first_->previous_ = nullptr;
		}
	}

	template<typename DataType>
    void DoublyLinkedSequence<DataType>::connectBlocks(BlockType* previous, BlockType* next)
	{
		ExplicitSequence<BlockType>::connectBlocks(previous, next);

		if (next != nullptr)
		{
			next->previous_ = const_cast<BlockType*>(previous);
		}
	}

	template<typename DataType>
    void DoublyLinkedSequence<DataType>::disconnectBlock(BlockType* block)
	{
		ExplicitSequence<BlockType>::disconnectBlock(block);
		block->previous_ = nullptr;
	}

}