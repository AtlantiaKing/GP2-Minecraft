#pragma once

// Copied from That2DGameEngine (my Programming 4 engine)

#include "Observer.h"

#include <vector>
#include <functional>

template <class T>
class Subject final
{
public:
	Subject() = default;
	~Subject();
	void AddListener(Observer<T>* observer)
	{
		m_Observers.push_back(observer);
	}
	void AddListener(const std::function<void(const T&)>& callback)
	{
		m_Functions.push_back(callback);
	}
	void RemoveListener(Observer<T>* observer)
	{
		const auto& it{ std::find(begin(m_Observers), end(m_Observers), observer) };
		if (it != end(m_Observers)) m_Observers.erase(it);
	}
	void RemoveListener(const std::function<void(const T&)>& callback)
	{
		const auto& it{ std::find(begin(m_Functions), end(m_Functions), callback) };
		if (it != end(m_Functions)) m_Functions.erase(it);
	}
	void Notify(const T& data)
	{
		for (const auto& observer : m_Observers)
		{
			observer->Notify(data);
		}
		for (const auto& function : m_Functions)
		{
			function(data);
		}
	}
private:
	std::vector<Observer<T>*> m_Observers{};
	std::vector<std::function<void(const T&)>> m_Functions{};
};

template<class T>
inline Subject<T>::~Subject()
{
	for (auto& observer : m_Observers)
	{
		observer->OnSubjectDestroy();
	}
}