#ifndef INCLUDE_CGFINAL_XYZ_H
#define INCLUDE_CGFINAL_XYZ_H

#include <vector>
#include <cassert>

template<typename T, int Size = 3> class xyz {
private:
    T data_[Size];
public:
    explicit xyz();
    explicit xyz(std::initializer_list<T> list);

    void operator=(std::initializer_list<T> list);

    T* data();

    T& operator[](int i);
};


template<typename T, int Size>
xyz<T,Size>::xyz() {}

template<typename T, int Size>
xyz<T,Size>::xyz(std::initializer_list<T> list)
{
    std::copy(list.begin(), list.end(), data_);
}

template<typename T, int Size>
void xyz<T,Size>::operator= (std::initializer_list<T> list)
{
    std::copy(list.begin(), list.end(), data_);
}

template<typename T, int Size>
T* xyz<T,Size>::data()
{
    return data_;
}

template<typename T, int Size>
T& xyz<T,Size>::operator[](int i) {
    assert(-1 < i && i < Size);
    return data_[i];
}

#endif