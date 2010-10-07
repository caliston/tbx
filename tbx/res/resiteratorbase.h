
#ifndef TBX_RES_RESITERATORBASE_H
#define TBX_RES_RESITERATORBASE_H

/**
* Base class for Resource component iterators
*/
template <class T> class ResIteratorBase
{
protected:
   const T *_object;
   int _offset;
   
   ResIteratorBase(const T *object, int offset) : _object(object), _offset(offset) {};

public:
   bool operator==(const ResIteratorBase &other) const {return _object == other._object && _offset == other._offset;}
   bool operator!=(const ResIteratorBase &other) const {return _object != other._object || _offset != other._offset;}
};

#endif
