#pragma once
#include <array>
#include <vector>
#include <functional>

// The size of the pool is static, provided by the template parameter.
// We can instead pass it to the constructor so we can decide the size
// the pool dynamically, but then objs_ cannot be a static array
//
template<typename T>
class ObjectPool {
public:
	// f is a lambda expression used to check if an object is in use,
	// this way we avoid assuming that T has a method with a specific
	// name.
	//
	// Ex: if we use the pool with a class A, whose method inUse is the
	// one that checks if the object is in use, we can use for f:
	//
	//     [](A *o) { return o->inUse(); }
	//
	ObjectPool(int size, std::function<bool(T*)> f) {
		inUseF_ = f;
		size_ = size;
		objs_ = new T[size_];
		for (int i = 0; i < size_; i++){
			objsPtrs_.push_back(&objs_[i]);
		}
	}

	virtual ~ObjectPool() {
		delete [] objs_;
	}

	T* getObj() {
		for (int i = 0; i < size_; i++)
			if (!inUseF_(&objs_[i])) 
				return &objs_[i]; 
		return nullptr;
	}

	const std::vector<T*>& getPool() {
		return objsPtrs_;
	}

private:
	int size_;
	std::function<bool(T*)> inUseF_;
	std::vector<T*> objsPtrs_;
	T* objs_;
};

