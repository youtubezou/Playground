#ifndef DYNAMICARRAY_H
#define DYNAMICARRAY_H

template <class T>
class DynamicArray
{
public:
    DynamicArray();
    DynamicArray(int size);
    virtual ~DynamicArray();

    DynamicArray<T>(const DynamicArray<T>&) = default;
    DynamicArray& operator =(const DynamicArray<T>&) = default;

    bool append(T item);
    bool truncate();
    void clear();
    int length() const;
    bool isEmpty() const;
    const T& operator [](int i) const;
    T &operator [](int i);

private:
    T* _data;
    int _nData;
    int _arraySize;
};

template <class T>
DynamicArray<T>::DynamicArray() :
    _data(nullptr), _nData(0), _arraySize(4)
{
    _data = new T[4];
}

template <class T>
DynamicArray<T>::DynamicArray(int size) :
    _data(nullptr), _nData(0), _arraySize(size)
{
    _data = new T[size];
}

template <class T>
DynamicArray<T>::~DynamicArray()
{
    delete[] _data;
}

template <class T>
bool DynamicArray<T>::append(T item)
{
    if (_nData == _arraySize)
    {
        _arraySize <<= 1;
        T* temp = _data;
        if (!(_data = new T[_arraySize]))
        {
            return false;
        }
        for (int i = 0; i < _nData; ++i)
        {
            _data[i] = temp[i];
        }
        delete[] temp;
    }
    _data[_nData++] = item;
    return true;
}

template <class T>
bool DynamicArray<T>::truncate()
{
    if (_nData != _arraySize)
    {
        T* temp = _data;
        _arraySize = _nData;
        if (!(_data = new T[_arraySize]))
        {
            return false;
        }
        for (int i = 0; i < _nData; ++i)
        {
            _data[i] = temp[i];
        }
        delete[] temp;
    }
    return true;
}

template <class T>
void DynamicArray<T>::clear()
{
    _nData = 0;
}

template <class T>
int DynamicArray<T>::length() const
{
    return _nData;
}

template <class T>
bool DynamicArray<T>::isEmpty() const
{
    return _nData == 0;
}

template <class T>
const T& DynamicArray<T>::operator [](int i) const
{
    return _data[i];
}

template <class T>
T& DynamicArray<T>::operator [](int i)
{
    return _data[i];
}

#endif // DYNAMICARRAY_H
