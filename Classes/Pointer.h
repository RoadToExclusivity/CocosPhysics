#pragma once

template <typename T>
class Pointer
{
public:
	Pointer(T *node)
		:m_ptr(node)
	{
		if (m_ptr)
		{
			m_ptr->retain();
		}
	}

	~Pointer()
	{
		Clear();
	}

	Pointer(const Pointer& other)
		:m_ptr(other.m_ptr)
	{
		if (m_ptr)
		{
			m_ptr->retain();
		}
	}

	Pointer& operator=(const Pointer& other)
	{
		if (std::addressof(other) != this)
		{
			Clear();

			m_ptr = other.m_ptr;
			if (m_ptr)
			{
				m_ptr->retain();
			}
		}

		return *this;
	}

	T& operator*() const
	{
		return *m_ptr;
	}

	T* operator->() const
	{
		return m_ptr;
	}

	operator T*() const
	{
		return m_ptr;
	}

	explicit operator bool() const
	{
		return m_ptr != nullptr;
	}

	void Clear()
	{
		if (m_ptr)
		{
			m_ptr->release();
			m_ptr = nullptr;
		}
	}

private:
	T* m_ptr;
};