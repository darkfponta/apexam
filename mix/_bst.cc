#include "bst.h"
#include <iterator>
#include <boost/python.hpp>

namespace python = boost::python;

// object converters
namespace {
	
	
}

using namespace boost::python;
template class Bst<size_t,size_t,std::less<size_t>>;
#define tpair std::pair<size_t,size_t>
#define ttree Bst<size_t,size_t,std::less<size_t>>

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
	;
}