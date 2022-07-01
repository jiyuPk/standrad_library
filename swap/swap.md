# sdd::swap

```cpp
template<typename T>
constexpr void swap(T& lhs, T& rhs) noexcept;
```

## Parameter 

**lhs** , **rls** - the value to be swapped

###  Type requirements

T must the requirement of **std::is_move_constructible** and **std::is_move_assignable**

## Example

```cpp
#include <iostream>
#include "swap.h"

class A {
public:
	A(int a, int b) : a{ a }, b{ b } {}

	A(A&&) = default;
	A& operator=(A&&) = default;

	friend void swap(A& lhs, A& rhs) {
		std::cout << "custom swap function\n";
		auto temp{ std::move(lhs) };
		lhs = std::move(rhs);
		rhs = std::move(temp);
	}

	int a;
	int b;
};

std::ostream& operator<<(std::ostream& os, const A& a) {
	return os << a.a << " " << a.b;
}

int main() {
	A p{ 1, 2 };
	A q{ 3, 4 };

	sdd::swap(p, q); // will call custom swap function
	std::cout << p << '\n' << q << '\n';

	int n{ 1 }, m{ 2 };

	sdd::swap(n, m);
	std::cout << n << ' ' << m << '\n';
}
```

### Output
```
custom swap function
3 4
1 2
2 1
```
