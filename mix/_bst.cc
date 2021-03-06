#include "bst.h"
#include <iterator>
#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

namespace python = boost::python;

// object converters
namespace {	
// Converts a std::pair instance to a Python tuple.
template <typename T1, typename T2>
struct std_pair_to_tuple
{
    static PyObject* convert(std::pair<T1, T2> const& p)
    {
		return python::incref(
			python::make_tuple(p.first, p.second).ptr());
    }
    static PyTypeObject const *get_pytype () {return &PyTuple_Type; }
};


template <typename T1, typename T2>
struct pair_from_python_tuple
{
	pair_from_python_tuple()
	{
		python::converter::registry::push_back(
        &convertible,
        &construct,
        python::type_id<pair<T1,T2>>());
    }
	
	// Determine if obj_ptr can be converted in a std::pair
    static void* convertible(PyObject* obj_ptr)
    {
        if (PyTuple_CheckExact(obj_ptr))
		    if (PyTuple_Size(obj_ptr)==2)
				return obj_ptr;
		return nullptr;
    }
 
    // Convert obj_ptr into a std::pair
    static void construct(
    PyObject* obj_ptr,
    boost::python::converter::rvalue_from_python_stage1_data* data)
    {
		typedef python::converter::rvalue_from_python_storage<std::pair<T1,T2>>
																	storage_type;
		void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;

		new (storage) std::pair<T1,T2>(tupletopair( obj_ptr ));
		data->convertible = storage;

    }
	
	struct pytuple_to_pair_wrapper {
		python::tuple params;
		pytuple_to_pair_wrapper(const python::tuple& _params):params(_params){}

		std::pair<T1,T2> extract() {
			return std::make_pair( (static_cast<T1>(python::extract<T1>(params[0]))) ,
									(static_cast<T2>(python::extract<T2>(params[1]))) );
		}
	};
	
	static std::pair<T1,T2> tupletopair(PyObject* obj_ptr) {
		python::tuple tup(python::borrowed(obj_ptr));
		pytuple_to_pair_wrapper wrapper(tup);
		std::pair<T1,T2> pair = wrapper.extract();
		return pair;
	}

};

template <typename T>
struct vector_from_python_list
{
	vector_from_python_list()
    {
		python::converter::registry::push_back(
        &convertible,
        &construct,
        python::type_id<vector<T>>());
    }
 
    // Determine if obj_ptr can be converted in a std::vector
    static void* convertible(PyObject* obj_ptr)
    {
      return PyList_Check(obj_ptr) ? obj_ptr : nullptr;
    }
 
    // Convert obj_ptr into a std::vector
    static void construct(
    PyObject* obj_ptr,
    python::converter::rvalue_from_python_stage1_data* data)
    {
		namespace python = boost::python;
		// Object is a borrowed reference, so create a handle indicting it is
		// borrowed for proper reference counting.
		python::handle<> handle(python::borrowed(obj_ptr));

		// Obtain a handle to the memory block that the converter has allocated
		// for the C++ type.
		typedef python::converter::rvalue_from_python_storage<std::vector<T>>
																	storage_type;
		void* storage = reinterpret_cast<storage_type*>(data)->storage.bytes;

		typedef python::stl_input_iterator<typename std::vector<T>::value_type>
																		iterator;
																		
		// Allocate the C++ type into the converter's memory block, and assign
		// its handle to the converter's convertible variable.  The C++
		// container is populated by passing the begin and end iterators of
		// the python object to the container's constructor.
		new (storage) std::vector<T>(
			iterator(python::object(handle)), // begin
			iterator());                      // end
		data->convertible = storage;

    }
 
};


// exception
template <class T>
void explainException(const T& x) {
	PyErr_SetString(PyExc_UserWarning, x.what()); 
};
template void explainException<NotFoundException>(const NotFoundException&);
template void explainException<InvalidRangeException>(const InvalidRangeException&);
template void explainException<InvalidLeafException>(const InvalidLeafException&);

template <class T>
void throwException() {	throw T(); };


template <typename K, typename V, typename C>
struct _Bst: public Bst<K,V,C> {

	using Node = typename Bst<K,V,C>::Node;
	using Bst<K,V,C>::Bst;
	using Bst<K,V,C>::print;
	using Bst<K,V,C>::detailedPrint;
	using Bst<K,V,C>::addSubTree;
	using Bst<K,V,C>::addSubTreeBalanced;
	using Bst<K,V,C>::insert;
	using Bst<K,V,C>::size;
	using Bst<K,V,C>::depth;
	using Bst<K,V,C>::avgdepth;
	using Bst<K,V,C>::balance;
	using Bst<K,V,C>::clear;
	using Bst<K,V,C>::erase;
	using Bst<K,V,C>::checkBalanced;
	
	
	void bst_setitem(K key, V value)
	{
		(*this)[key] = value;
	}
	
	V bst_getitem(K key)  // can't have size_t& in python...
	{
		return (*this)[key];
	}
		
	class KVIterator;
	using const_iterator = typename _Bst<K,V,C>::KVIterator;
	using iterator = typename _Bst<K,V,C>::const_iterator;

	KVIterator begin() {
		if (!Bst<K,V,C>::root)
			return KVIterator{ nullptr };
		Node* temp = Bst<K,V,C>::root.get();
		Bst<K,V,C>::ffwd(temp);
		return KVIterator{ temp };
	}
	KVIterator end() { return KVIterator{ nullptr }; }
};

template <typename K, typename V, class C>
class _Bst<K, V, C>::KVIterator 
	:	public Bst<K, V, C>::Iterator,
		public std::iterator<std::forward_iterator_tag, const KVpair>
{
protected:
	using parent = typename Bst<K, V, C>::Iterator;
	using N = typename parent::N;
public:
	using parent::Iterator; // the ctor
	KVIterator operator++() {
		return parent::operator++();
		}
	KVIterator operator++(int) {
		KVIterator v(*this); parent::operator++();
		return v;
		}
	const KVpair& operator*() const { return parent::operator*().getpair(); }
};
};

using namespace boost::python;
template class Bst<size_t,size_t,std::less<size_t>>;
#define tpair std::pair<size_t,size_t>
#define ttree _Bst<size_t,size_t,std::less<size_t>>

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(depth_overload0, depth, 0, 0)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(checkbalanced_overload0, checkBalanced, 0, 0)
BOOST_PYTHON_MODULE(_bst) {
	
	to_python_converter<
        std::pair<size_t,size_t>,
        std_pair_to_tuple<size_t,size_t>,
        true //std_pair_to_tuple has get_pytype
        >();
	pair_from_python_tuple<size_t,size_t>();
	vector_from_python_list<tpair>();
	
	class_<std::vector<tpair>>("std_vector_uintpair")
		.def(vector_indexing_suite<std::vector<tpair>>())
    ;
	
	register_exception_translator<NotFoundException>(explainException<NotFoundException>);
	def("NotFoundException", throwException<NotFoundException>);
	register_exception_translator<InvalidRangeException>(explainException<InvalidRangeException>);
	def("InvalidRangeException", throwException<InvalidRangeException>);
	register_exception_translator<InvalidLeafException>(explainException<InvalidLeafException>);
	def("InvalidLeafException", throwException<InvalidLeafException>);
	
	class_<ttree>("Bst_sizet",init<>())
		.def(init<tpair>())
	    .def(init<std::vector<tpair>>())
	    
	    .def("print", &ttree::print)
	    .def("detailedPrint"  , &ttree::detailedPrint)
		
		.def("addSubTree", &ttree::addSubTree)
		.def("addSubTreeBalanced", &ttree::addSubTreeBalanced)
		.def("insert", &ttree::insert)
	    
	    .def("size", &ttree::size)
	    .def("depth", (size_t(ttree::*)())0, depth_overload0())
	    .def("avgdepth", &ttree::avgdepth)
	    
	    .def("balance", &ttree::balance)
	    .def("clear", &ttree::clear)
	    .def("erase", &ttree::erase)
	    
	    .def("checkBalanced", (size_t(ttree::*)())0, checkbalanced_overload0())
		.def("__iter__", python::iterator<ttree>())
		
		.def("__getitem__", &ttree::bst_getitem)
        .def("__setitem__", &ttree::bst_setitem)
	;
	
};
