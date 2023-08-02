#include "bind_common.h"





void RprError(int code)
{
	// this function may help to debug
	// should be called at any RPR error raised.
	// printf("C++ Python RprError %d\n" , code);
	return;
}



PyMalloc::PyMalloc( nb::ndarray<> a)
{
	m_data_asNdarray = nullptr;
	m_data_asString = nullptr;
	m_dtype = a.dtype();
	m_data_asNdarray = new nb::ndarray<>(a);
}
PyMalloc::PyMalloc( std::string str)
{
	m_data_asNdarray = nullptr;
	m_data_asString = nullptr;
	m_data_asString = new std::string(str);
}

void PyMalloc::Free()
{
	if ( m_data_asNdarray )
	{
		delete m_data_asNdarray; m_data_asNdarray=nullptr;
	}
	if ( m_data_asString )
	{
		delete m_data_asString; m_data_asString=nullptr;
	}
}
PyMalloc::~PyMalloc()
{
}
void* PyMalloc::GetMem(int offset)
{
	if ( m_data_asNdarray )
	{
		if ( m_dtype.bits % 8 != 0 )
			return nullptr;
		const int nbByte = (m_dtype.bits / 8);
		int8_t* data_ = &(((int8_t*)m_data_asNdarray->data())[offset*nbByte]);
		return data_;
	}
	else if ( m_data_asString )
	{
		return (void*)&( m_data_asString->c_str()[offset] );
	}
	return nullptr;
}


