#ifndef JWT_CPP_GLAZE_TRAITS_H
#define JWT_CPP_GLAZE_TRAITS_H

#include "glaze/glaze.hpp"
#include "jwt-cpp/jwt.h"

namespace jwt {
	/**
	 * \brief Namespace containing all the json_trait implementations for a jwt::basic_claim.
	*/
	namespace traits {
		/// basic_claim's JSON trait implementation for glaze
		struct glaze {
			using value_type = glz::json_t;
			using object_type = value_type::object_t;
			using array_type = value_type::array_t;
			using string_type = std::string;
			using number_type = double;
			using integer_type = int64_t;
			using boolean_type = bool;

			static bool is_integer(double value) { return std::trunc(value) == value; }

			static jwt::json::type get_type(const value_type& val) {
				using jwt::json::type;

				if (val.is_object()) return type::object;
				if (val.is_array()) return type::array;
				if (val.is_string()) return type::string;
				if (val.is_number() && is_integer(val.get_number())) return type::integer;
				if (val.is_number()) return type::number;
				if (val.is_boolean()) return type::boolean;

				throw std::logic_error("invalid type");
			}

			static object_type as_object(const value_type& val) {
				if (!val.is_object()) throw std::bad_cast();
				return val.get_object();
			}

			static array_type as_array(const value_type& val) {
				if (!val.is_array()) throw std::bad_cast();
				return val.get_array();
			}

			static string_type as_string(const value_type& val) {
				if (!val.is_string()) throw std::bad_cast();
				return val.get_string();
			}

			static number_type as_number(const value_type& val) {
				if (!val.is_number()) throw std::bad_cast();
				return val.get_number();
			}

			static integer_type as_integer(const value_type& val) {
				if (!val.is_number()) throw std::bad_cast();
				return static_cast<integer_type>(val.get_number());
			}

			static boolean_type as_boolean(const value_type& val) {
				if (!val.is_boolean()) throw std::bad_cast();
				return val.get_boolean();
			}

			static bool parse(value_type& val, string_type str) {
				auto parsed = glz::read_json<value_type>(str);

				if (parsed.has_value()) {
					val = parsed.value();
					return true;
				}

				return false;
			}

			static string_type serialize(const value_type& val) { return val.dump().value(); }
		};
	} // namespace traits
} // namespace jwt

#endif
