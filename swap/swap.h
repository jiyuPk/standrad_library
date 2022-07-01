#pragma once
#include <concepts>

namespace sdd
{
	template<typename T>
	concept swappable =
		std::is_move_constructible_v<std::remove_cvref_t<T>> &&
		std::is_move_assignable_v<std::remove_cvref_t<T>>;

	namespace hidden {
		template<typename T>
		constexpr void swap(T& lhs, T& rhs) noexcept {
			auto temp = std::move(lhs);
			lhs = std::move(rhs);
			rhs = std::move(temp);
		}

		struct st_swap {
			template<swappable T>
			constexpr void operator()(T& lhs, T& rhs) const noexcept {
				swap(lhs, rhs);
			}
		};
	}

	template<typename T>
	constexpr T customization_points{ };

	inline constexpr auto const& swap =
		customization_points<hidden::st_swap>;
}
