#include "bst.h"
#include <iterator>
#include <boost/python.hpp>

namespace python = boost::python;

// object converters
namespace {
	
	
}

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
	class_<ttree>("Bst_sizet",init<>())
		.def(init<tpair>())
	    .def(init<std::vector<tpair>>())
	    
	    .def("print", &ttree::print)
	    .def("detailedPrint"  , &ttree::detailedPrint)
		
		.def("insert", &ttree::insert)
	    
	    .def("size", &ttree::size)
	    .def("avgdepth", &ttree::avgdepth)
		
	    .def("balance", &ttree::balance)
	    .def("clear", &ttree::clear)
	    .def("erase", &ttree::erase)
		
		.def("__iter__", python::iterator<ttree>())
	;
}