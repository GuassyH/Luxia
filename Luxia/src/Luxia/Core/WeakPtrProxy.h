#pragma once

#include <iostream>
#include <stdexcept>

// This class should make WeakPtr feel like a normal ptr, so i dont need all the boilerplate
template<typename T>
class WeakPtrProxy {
private:
	std::weak_ptr<T> weak;
public:
	WeakPtrProxy() = default;
	WeakPtrProxy(const std::weak_ptr<T>& w) : weak(w) {}
	WeakPtrProxy(const std::shared_ptr<T>& w) : weak(w) {}
	WeakPtrProxy(std::nullptr_t) : weak() {}

	WeakPtrProxy& operator=(const std::weak_ptr<T>& w) {
		weak = w;
		return *this;
	}

	WeakPtrProxy& operator=(const std::shared_ptr<T>& w) {
		weak = w;
		return *this;
	}

	WeakPtrProxy& operator=(std::nullptr_t) {
		weak.reset();
		return *this;
	}



	T* operator->() const {
		auto shared = weak.lock();
		if (!shared) {
			throw std::runtime_error("Accessing expired weak_ptr proxy");
		}
		return shared.get();
	}

	// bool operator!() const { return weak.expired(); }
	explicit operator bool() const { return !weak.expired(); }
	bool expired() const { return weak.expired(); }
	T* get() { if (!weak.expired()) return weak.lock().get(); else return nullptr; }
	std::shared_ptr<T> lock() const { return weak.lock(); }
};