/*! \file ciPointer.h
 *
 *  Copyright �� 2003 WINCC Inc.
 *  All Rights Reserved.
 *
 *  \brief Memory leak�� �����ϱ� ���� smart safe pointer�� �����Ѵ�.
 *  	�ϴ� assigned�� pointer�� ciPointer�� �������� ������ 
 *  	ciPointer�� life cycle�� ������  �Ҹ��� ȣ��� �ڵ����� delete �Ͽ� 
 *  	memory leak�� �����Ѵ�. CORBA�� _var pointer�� ������ ����ϴ�. 
 *  (Environment: OSF1 5.1A)
 *
 *  \author 	jhchoi
 *  \version
 *  \date 		2003�� 4�� 22�� 18:00
 */

#ifndef _ciPointer_h_
#define _ciPointer_h_

#include "ciListType.h"

template <class OBJ>
class ciPointer {
public:
	ciPointer();
	ciPointer(OBJ *p_obj);	
   	~ciPointer();
   
	// OBJ* this()	{ return _obj; }
	void operator=(const OBJ *p_obj);
	void operator=(const ciPointer& ptr);

private: 
	OBJ* _obj;
};

#ifdef COP_CI_EXPORTS
template <class OBJ>
ciPointer<OBJ>::ciPointer() {
	_obj = NULL;
}

template <class OBJ>
ciPointer<OBJ>::ciPointer(OBJ *p_obj) {
	if ( _obj != NULL ) delete _obj;
	_obj = p_obj;
}

template <class OBJ>
ciPointer<OBJ>::~ciPointer() {
	if ( _obj != NULL ) delete _obj;
	_obj = NULL;
}

template <class OBJ>
void
ciPointer<OBJ>::operator=(const OBJ *p_obj) {
	if ( _obj != NULL ) delete _obj;
	_obj = p_obj;
}

template <class OBJ>
void
ciPointer<OBJ>::operator=(const ciPointer& ptr) {
	if ( _obj != NULL ) delete _obj;
	_obj = ptr;
}
#endif //COP_CI_EXPORTS

#endif //_ciPointer_h_
