#pragma once

template <typename T>
class CQSingleton
{
private :
	CQSingleton();
	virtual ~CQSingleton();
	CQSingleton(const CQSingleton&) = delete;
	CQSingleton& operator = (const CQSingleton&) = delete;

public :
	static T* GetInstance()
	{
		static T Instance;
		return &Instance;
	}
};

template<typename T>
inline CQSingleton<T>::CQSingleton()
{
}

template<typename T>
inline CQSingleton<T>::~CQSingleton()
{
}
