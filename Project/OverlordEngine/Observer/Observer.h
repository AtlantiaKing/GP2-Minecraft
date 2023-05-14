#pragma once

// Copied from That2DGameEngine (my Programming 4 engine)

template <typename T>
class Observer
{
public:
	virtual ~Observer() = default;
	virtual void Notify(const T& data) = 0;
	virtual void OnSubjectDestroy() {};
};

