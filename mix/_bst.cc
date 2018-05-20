#include "bst.h"
#include <iterator>
#include <boost/python.hpp>

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

template <typename K, typename V, typename C>
struct _Bst: public Bst<K,V,C> {
	
	using Node = typename Bst<K,V,C>::Node;
	using Iterator = typename Bst<K,V,C>::Iterator;
	using const_iterator = typename _Bst<K,V,C>::KVIterator;
	using iterator = typename _Bst<K,V,C>::const_iterator;
};

using namespace boost::python;
template class Bst<size_t,size_t,std::less<size_t>>;
#define tpair std::pair<size_t,size_t>
#define ttree _Bst<size_t,size_t,std::less<size_t>>

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
	
	class_<ttree>("Bst_sizet",init<>())
		.def(init<tpair>())
	    .def(init<std::vector<tpair>>())
	    
	    .def("print", &ttree::print)
	    .def("detailedPrint"  , &ttree::detailedPrint)
		
		.def("insert", &ttree::insert)
	    
	    .def("size", &ttree::size)
		.def("depth", &ttree::depth)
	    .def("avgdepth", &ttree::avgdepth)
		
	    .def("balance", &ttree::balance)
	    .def("clear", &ttree::clear)
	    .def("erase", &ttree::erase)
		
		.def("__iter__", python::iterator<ttree>())
	;
}