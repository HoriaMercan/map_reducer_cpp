//
// Created by horia on 11/20/24.
//

#ifndef __TEMA1__SRC__HELPERS_H__
#define __TEMA1__SRC__HELPERS_H__

template<typename T>
class optional {
public:
	optional();
	optional(T &);
	bool has_value() const;
	T value() const;
private:
	bool _has_value;
	T _value;
};

template <typename T>
optional<T>::optional(): _has_value(false){}

template <typename T>
optional<T>::optional(T &t): _has_value(true), _value(t){}

template <typename T>
bool optional<T>::has_value() const {return _has_value;}

template <typename T>
T optional<T>::value() const {return static_cast<T>(_value);}

#endif /*  __TEMA1__SRC__HELPERS_H__  */
