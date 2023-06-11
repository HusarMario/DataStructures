#pragma once

#include <libds/amt/abstract_memory_type.h>
#include <libds/amt/sequence.h>

namespace ds::amt {

	template<typename DataType>
	class ImplicitSequence :
		public Sequence<MemoryBlock<DataType>>,
		public ImplicitAMS<DataType>
	{
	public:
		using BlockType = MemoryBlock<DataType>;

		ImplicitSequence();
		ImplicitSequence(size_t capacity, bool initBlocks);
		ImplicitSequence(const ImplicitSequence<DataType>& other);
		~ImplicitSequence() override;

		size_t calculateIndex(MemoryBlock<DataType>& block) override;

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
		void removeNext(const MemoryBlock<DataType>& block) override;
		void removePrevious(const MemoryBlock<DataType>& block) override;

		void reserveCapacity(size_t capacity);

		virtual size_t indexOfNext(size_t currentIndex) const;
		virtual size_t indexOfPrevious(size_t currentIndex) const;

	public:
		class ImplicitSequenceIterator {
		public:
			ImplicitSequenceIterator(ImplicitSequence<DataType>* sequence, size_t index);
			ImplicitSequenceIterator(const ImplicitSequenceIterator& other);
			ImplicitSequenceIterator& operator++();
			ImplicitSequenceIterator operator++(int);
			bool operator==(const ImplicitSequenceIterator& other) const;
			bool operator!=(const ImplicitSequenceIterator& other) const;
			DataType& operator*();

		private:
			ImplicitSequence<DataType>* sequence_;
			size_t position_;
		};

		ImplicitSequenceIterator begin();
		ImplicitSequenceIterator end();

		using IteratorType = ImplicitSequenceIterator;
	};

	template<typename DataType>
	using IS = ImplicitSequence<DataType>;

	//----------

	template<typename DataType>
	class CyclicImplicitSequence : public IS<DataType>
	{
	public:
		CyclicImplicitSequence();
		CyclicImplicitSequence(size_t initSize, bool initBlocks);

		size_t indexOfNext(size_t currentIndex) const override;
		size_t indexOfPrevious(size_t currentIndex) const override;
	};

	template<typename DataType>
	using CIS = CyclicImplicitSequence<DataType>;

	//----------

	template<typename DataType>
    ImplicitSequence<DataType>::ImplicitSequence()
	{
	}

	template<typename DataType>
    ImplicitSequence<DataType>::ImplicitSequence(size_t initialSize, bool initBlocks):
		ImplicitAMS<DataType>(initialSize, initBlocks)
	{
	}

	template<typename DataType>
    ImplicitSequence<DataType>::ImplicitSequence(const ImplicitSequence<DataType>& other):
		ImplicitAMS<DataType>::ImplicitAbstractMemoryStructure(other)
	{
	}

	template<typename DataType>
    ImplicitSequence<DataType>::~ImplicitSequence()
	{
	}

	template<typename DataType>
    size_t ImplicitSequence<DataType>::calculateIndex(BlockType& block)
	{
		// 03
		return this->getMemoryManager()->calculateIndex(block);
	}

	template<typename DataType>
    MemoryBlock<DataType>* ImplicitSequence<DataType>::accessFirst() const
	{
		// 03
		if (this->size() > 0) {
			return &(this->getMemoryManager()->getBlockAt(0));
		}
		else {
			return nullptr;
		}
	}

	template<typename DataType>
    MemoryBlock<DataType>* ImplicitSequence<DataType>::accessLast() const
	{
		// 03
		const size_t size_ = this->size();
		if (size_ > 0) {
			return &(this->getMemoryManager()->getBlockAt(size_ - 1));
		}
		else {
			return nullptr;
		}
	}

	template<typename DataType>
    MemoryBlock<DataType>* ImplicitSequence<DataType>::access(size_t index) const
	{
		// 03
		if (index >= 0 && index < this->size()) {
			return &(this->getMemoryManager()->getBlockAt(index));
		}
		else {
			return nullptr;
		}
	}

	template<typename DataType>
    MemoryBlock<DataType>* ImplicitSequence<DataType>::accessNext(const MemoryBlock<DataType>& block) const
	{
		// 03
		size_t index = indexOfNext(this->getMemoryManager()->calculateIndex(block));
		if (index < this->size()) {
			return &(this->getMemoryManager()->getBlockAt(index));
		}
		else {
			return nullptr;
		}
	}

	template<typename DataType>
    MemoryBlock<DataType>* ImplicitSequence<DataType>::accessPrevious(const MemoryBlock<DataType>& block) const
	{
		// 03
		size_t index = indexOfPrevious(this->getMemoryManager()->calculateIndex(block));
		if (index < this->size()) {
			return &(this->getMemoryManager()->getBlockAt(index));
		}
		else {
			return nullptr;
		}
	}

	template<typename DataType>
    MemoryBlock<DataType>& ImplicitSequence<DataType>::insertFirst()
	{
		// 03
		return *this->getMemoryManager()->allocateMemoryAt(0);
	}

	template<typename DataType>
    MemoryBlock<DataType>& ImplicitSequence<DataType>::insertLast()
	{
		// 03
		return *this->getMemoryManager()->allocateMemory();
	}

	template<typename DataType>
    MemoryBlock<DataType>& ImplicitSequence<DataType>::insert(size_t index)
	{
		// 03
		return *this->getMemoryManager()->allocateMemoryAt(index);
	}

	template<typename DataType>
    MemoryBlock<DataType>& ImplicitSequence<DataType>::insertAfter(BlockType& block)
	{
		// 03
		return *this->getMemoryManager()->allocateMemoryAt(this->getMemoryManager()->calculateIndex(block) + 1);
	}

	template<typename DataType>
    MemoryBlock<DataType>& ImplicitSequence<DataType>::insertBefore(BlockType& block)
	{
		// 03
		return *this->getMemoryManager()->allocateMemoryAt(this->getMemoryManager()->calculateIndex(block));

	}

	template<typename DataType>
    void ImplicitSequence<DataType>::removeFirst()
	{
		// 03
		this->getMemoryManager()->releaseMemoryAt(0);
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::removeLast()
	{
		// 03
		this->getMemoryManager()->releaseMemory();
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::remove(size_t index)
	{
		// 03
		this->getMemoryManager()->releaseMemoryAt(index);
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::removeNext(const MemoryBlock<DataType>& block)
	{
		// 03
		this->getMemoryManager()->releaseMemoryAt(indexOfNext(this->getMemoryManager()->calculateIndex(block)));
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::removePrevious(const MemoryBlock<DataType>& block)
	{
		// 03
		this->getMemoryManager()->releaseMemoryAt(indexOfPrevious(this->getMemoryManager()->calculateIndex(block)));
	}

	template<typename DataType>
    void ImplicitSequence<DataType>::reserveCapacity(size_t capacity)
	{
		// 03
		this.getMemoryManager()->changeCapacity(capacity);
	}

	template<typename DataType>
    size_t ImplicitSequence<DataType>::indexOfNext(size_t currentIndex) const
	{
		// 03
		if (currentIndex >= this->size() - 1) {
			return -1;
		}
		else {
			return currentIndex + 1;
		}
	}

	template<typename DataType>
    size_t ImplicitSequence<DataType>::indexOfPrevious(size_t currentIndex) const
	{
		// 03
		if (currentIndex <= 0) {
			return -1;
		}
		else {
			return currentIndex - 1;
		}
	}

    template <typename DataType>
    ImplicitSequence<DataType>::ImplicitSequenceIterator::ImplicitSequenceIterator
        (ImplicitSequence<DataType>* sequence, size_t index) :
		    sequence_(sequence),
            position_(index)
    {
    }

    template <typename DataType>
    ImplicitSequence<DataType>::ImplicitSequenceIterator::ImplicitSequenceIterator
        (const ImplicitSequenceIterator& other) :
		    sequence_(other.sequence_),
            position_(other.position_)
    {
    }

    template <typename DataType>
    auto ImplicitSequence<DataType>::ImplicitSequenceIterator::operator++() -> ImplicitSequenceIterator&
    {
		// TODO 03
		++position_;
		return *this;
    }

    template <typename DataType>
    auto ImplicitSequence<DataType>::ImplicitSequenceIterator::operator++(int) -> ImplicitSequenceIterator
    {
		ImplicitSequenceIterator tmp(*this);
	    operator++();
	    return tmp;
    }

    template <typename DataType>
    bool ImplicitSequence<DataType>::ImplicitSequenceIterator::operator==(const ImplicitSequenceIterator& other) const
    {
		// 03
		return (sequence_ == other.sequence_ && position_ == other.position_);
    }

    template <typename DataType>
    bool ImplicitSequence<DataType>::ImplicitSequenceIterator::operator!=(const ImplicitSequenceIterator& other) const
    {
		return !(*this == other);
    }

    template <typename DataType>
    DataType& ImplicitSequence<DataType>::ImplicitSequenceIterator::operator*()
    {
		// 03
		return sequence_->access(position_)->data_;
    }

    template <typename DataType>
    auto ImplicitSequence<DataType>::begin() -> ImplicitSequenceIterator
    {
		return ImplicitSequenceIterator(this, 0);
    }

    template <typename DataType>
    auto ImplicitSequence<DataType>::end() -> ImplicitSequenceIterator
    {
		return ImplicitSequenceIterator(this, ImplicitAbstractMemoryStructure<DataType>::size());
    }

    template<typename DataType>
    CyclicImplicitSequence<DataType>::CyclicImplicitSequence():
		IS<DataType>()
	{
	}

	template<typename DataType>
    CyclicImplicitSequence<DataType>::CyclicImplicitSequence(size_t initCapacity, bool initBlocks):
		IS<DataType>(initCapacity, initBlocks)
	{
	}

	template<typename DataType>
    size_t CyclicImplicitSequence<DataType>::indexOfNext(size_t currentIndex) const
	{
		// 03
		size_t size_ = this->size();
		if (size_ != 0) {
			if (currentIndex >= size_ - 1) {
				return 0;
			}
			else {
				return currentIndex + 1;
			}
		}
		else {
			return -1;
		}
	}

	template<typename DataType>
    size_t CyclicImplicitSequence<DataType>::indexOfPrevious(size_t currentIndex) const
	{
		// 03
		size_t size_ = this->size();
		if (size_ != 0) {
			if (currentIndex <= 0) {
				return size_ -1;
			}
			else {
				return currentIndex - 1;
			}
		}
		else {
			return -1;
		}
	}

}