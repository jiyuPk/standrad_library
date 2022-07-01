#pragma once
#include <memory>
#include <ostream>

namespace sdd {
	using namespace std;

	template <typename T>
	class vec {
	public:
		vec() = default;
		~vec();
		void allocate_front(size_t amount);
		void allocate_back(size_t amount);
		void resolve_empty_indices();
		void erase(size_t start_idx, size_t end_idx);
		void push_back(T item);
		size_t size() const;

		friend ostream& operator<< <T>(ostream& os, vec<T>& that);

		void operator<<(int amount);
		void operator>>(int amount);
		T operator[](int index) const;
		T operator[](size_t index) const;
	private:
		size_t _size{ 0 };
		size_t capacity{ 0 };
		size_t first_item_idx{ 0 };
		T* data{ nullptr };
		bool* bitmask{ nullptr };
	};

	template <typename T>
	vec<T>::~vec() {
		delete[] data;
		delete[] bitmask;
	}

	template <typename T>
	void vec<T>::allocate_front(size_t amount) {
		size_t old_capacity{ capacity };
		capacity += amount;
		T* new_data{ new T[capacity]() };
		bool* new_bitmask = { new bool[capacity](false) };
		for (size_t i{ 0 }; i < old_capacity; i++) {
			size_t new_idx{ i + amount };
			new_data[new_idx] = data[i];
			new_bitmask[new_idx] = bitmask[i];
		}
		delete[] data;
		delete[] bitmask;
		data = new_data;
		bitmask = new_bitmask;
		first_item_idx += amount;
	}

	template <typename T>
	void vec<T>::allocate_back(size_t amount) {
		size_t old_capacity{ capacity };
		capacity += amount;
		T* new_data{ new T[capacity]() };
		bool* new_bitmask{ new bool[capacity](false) };
		for (size_t i{ 0 }; i < old_capacity; i++) {
			new_data[i] = data[i];
			new_bitmask[i] = bitmask[i];
		}
		delete[] data;
		delete[] bitmask;
		data = new_data;
		bitmask = new_bitmask;
	}

	template <typename T>
	void vec<T>::resolve_empty_indices() {
		size_t resolved_count = -1;
		bool has_empty_idx_before = false;
		for (size_t i = first_item_idx; i < capacity; i++) {
			if (!bitmask[i]) continue;
			resolved_count++;
			size_t idx = first_item_idx + resolved_count;
			if (has_empty_idx_before) {
				data[idx] = data[i];
				bitmask[idx] = bitmask[i];
				bitmask[i] = false;
			}
			has_empty_idx_before = !bitmask[idx + 1];
		}
		_size -= resolved_count;
	}

	template <typename T>
	void vec<T>::push_back(T item) {
		size_t idx{ first_item_idx + _size };
		data[idx] = item;
		bitmask[idx] = true;
		_size++;
	}

	template <typename T>
	void vec<T>::erase(size_t start_idx, size_t end_idx) {
		for (size_t i{ start_idx }; i < end_idx; i++) {
			bitmask[i] = false;
		}
	}

	template <typename T>
	size_t vec<T>::size() const {
		return _size;
	}

	template <typename T>
	void vec<T>::operator<<(int amount) {
		size_t end{ first_item_idx + _size };
		for (size_t i{ first_item_idx }; i < end; i++) {
			size_t target_idx{ i - amount };
			data[target_idx] = data[i];
			bitmask[target_idx] = bitmask[i];
		}
		for (size_t i{ end - amount }; i < end; i++) {
			data[i] = T();
			bitmask[i] = false;
		}
		first_item_idx -= amount;
	}

	template <typename T>
	void vec<T>::operator>>(int amount) {
		size_t end_idx{ first_item_idx + _size - 1 };
		for (size_t start_idx{ end_idx }, minus{ 0 }; minus < _size; minus++) {
			size_t i{ start_idx - minus };
			size_t target_idx{ i + amount };
			data[target_idx] = data[i];
			bitmask[target_idx] = bitmask[i];
		}
		for (size_t i{ first_item_idx }; i < first_item_idx + amount; i++) {
			data[i] = T();
			bitmask[i] = false;
		}
		first_item_idx += amount;
	}

	template <typename T>
	T vec<T>::operator[](int index) const {
		return data[first_item_idx + ((_size + index) % _size)];
	}
	template <typename T>
	T vec<T>::operator[](size_t index) const {
		return data[first_item_idx + index];
	}

	template <typename T>
	ostream& operator<<(ostream& os, vec<T>& that) {
		os << '[';
		size_t end{ that.first_item_idx + that._size };
		for (size_t i{ that.first_item_idx }; i < end; i++) {
			os << that.data[i];
			if (end - i != 1) os << ", ";
		}
		os << ']';
		return os;
	}
}
