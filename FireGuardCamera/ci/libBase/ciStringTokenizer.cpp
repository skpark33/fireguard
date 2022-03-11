/*! \file ciStringTokenizer.C
 *
 *  Copyright �� 2003 WINCC Inc.
 *  All Rights Reserved.
 *
 *  \brief string �� delimiters �� Tokenning �Ѵ�.
 *  (Environment: OSF1 5.1A)
 *
 *  \author 	jhchoi
 *  \version
 *  \date 		2003�� 4�� 22�� 18:00
 */

#include "ciStringTokenizer.h"

//ciSET_DEBUG(10, "ciStringTokenizer");

// ciStringTokenizer Constructor
/*!
	str�� token�� �и��ϰ��� �ϴ� ���ڿ��� �����Ѵ�.
	delim  �Ķ���Ͱ� �������� ���� ��� delimiter�� "\n","\t","\r","\f" 
	,space�� default delimiter�� �Ѵ�.	
	\f		Formfeed
       	\n		Newline
       	\r		Carriage Return
       	\t		Tab
       	space
*/
ciStringTokenizer::ciStringTokenizer(const ciString& str) {
//	ciDEBUG(9,("ciStringTokenizer(ciString(%s))",str.c_str()));
	_ciStringTokenizer(str.c_str());
}

/*! ciStringTokenizer::ciStringTokenizer(const char* str)*/
ciStringTokenizer::ciStringTokenizer(const char* str) {
//	ciDEBUG(9,("ciStringTokenizer(%s)",str));
	_ciStringTokenizer(str);	
}

/*!
	str�� token�� �и��ϰ��� �ϴ� ���ڿ��� �����Ѵ�.
	delim �Ķ���ʹ� ������ token�� �и��ϱ� ���� delimiter ���ڿ��̴�.
	retDelims �Ķ���ʹ� delimiter ���ڵ� token ���� ���� �������� �����Ѵ�.
*/
ciStringTokenizer::ciStringTokenizer(const ciString& str, const ciString& delim,
						ciBoolean nullToken, ciBoolean retDelims) {
//	ciDEBUG(9,("ciStringTokenizer(ciString(%s),ciString(%s),%s,%s)",str.c_str(),
//			delim.c_str(),retDelims ? "TRUE":"FALSE", nullToken ? "TRUE":"FALSE"));
	_ciStringTokenizer(str.c_str(), delim.c_str(), nullToken, retDelims);
}

ciStringTokenizer::ciStringTokenizer(const char* str, const char* delim, 
						ciBoolean nullToken, ciBoolean retDelims) {
//	ciDEBUG(9,("ciStringTokenizer(%s,%s,%s,%s)",str,delim,
//				retDelims ? "TRUE":"FALSE", nullToken ? "TRUE":"FALSE"));					
	_ciStringTokenizer(str, delim, nullToken, retDelims);
}

//For String Delimiter
ciStringTokenizer::ciStringTokenizer(const ciString& str, const ciStringList& deliList, 
			ciBoolean nullToken, ciBoolean retDelims) {
	_ciStringTokenizer(str.c_str(), deliList, nullToken, retDelims);
}
				
ciStringTokenizer::ciStringTokenizer(const char* str, const ciStringList& deliList, 
			ciBoolean nullToken, ciBoolean retDelims)
{
	_ciStringTokenizer(str, deliList, nullToken, retDelims);
}

ciStringTokenizer::~ciStringTokenizer() {
}

void
ciStringTokenizer::_ciStringTokenizer(const char* str, const char* delim, 
						ciBoolean nullToken, ciBoolean retDelims) {
	_currentPosition = 0;
	_newPosition = -1;
	_delimsChanged = ciFalse;
	_str = str;
	_maxPosition = _str.length();
	_delimiters = delim;
	_retDelims = retDelims;
	_nullToken = nullToken;
	_strDelims = ciFalse;
	_setMaxDelimChar();
}

void
ciStringTokenizer::_ciStringTokenizer(const char* str, const ciStringList& deliList, 
						ciBoolean nullToken, ciBoolean retDelims) {
	_currentPosition = 0;
	_newPosition = -1;
	_delimsChanged = ciFalse;
	_str = str;
	_maxPosition = _str.length();
	_delimStrList = deliList;
	_retDelims = retDelims;
	_nullToken = nullToken;
	_strDelims = ciTrue;
	_setMaxDelimString();
}

/**
* Skips _delimiters starting from the specified position. If _retDelims
* is false, returns the index of the first non-delimiter character at or
* after startPosf _retDelims is true, startPos is returned.
*/
ciInt
ciStringTokenizer::_skipDelimiters(ciInt startPos) {
	ciInt position = startPos;
	
	if ( !_strDelims ) {
		if(_delimiters.empty()) {
			//ciERROR(("ERROR!!! Delimiters is Empty"));
			return -1;
		}
		while(!_retDelims && position < _maxPosition) {
			char c = _str[position];
			//if(c > _maxDelimChar || _delimiters.find(c) < 0 || _delimiters.find(c) == -1)
			if((c>_maxDelimChar) || (_delimiters.find(c)==-1) 
								 || (_delimiters.find(c)>=_delimiters.length()))
				break;
			position++;
			if (_nullToken) break; //permit null token
		}
	} else {
		if ( _delimStrList.empty() ) {
			//ciERROR(("ERROR!!! Delimiter List is Empty"));
			return -1;
		}
		ciBoolean findFlag;
		while(!_retDelims && position < _maxPosition) {
			if ( _str.substr(position, _maxDelimString.size()) > _maxDelimString ) {
				break;
			}
			findFlag = ciFalse;
			ciStringList::iterator itr;
			for ( itr = _delimStrList.begin(); itr != _delimStrList.end(); itr++ ) {
				ciInt deli_len = (*itr).size();
				if ( strncmp(_str.substr(position).c_str(), (*itr).c_str(), deli_len ) == 0 ) {
					position += deli_len - 1;
					findFlag = ciTrue;
					break;
				}
			}
			if ( !findFlag ) break;
			position++;
			if (_nullToken) break; //permit null token
		}
	}
	return position;
}
 
/**
* Set _maxDelimChar to the highest char in the delimiter set.
*/
void
ciStringTokenizer::_setMaxDelimChar() {
	if(_delimiters.empty()) {
		_maxDelimChar = '\0';
		return;
	}
	char m = '\0';
	for(ciUInt i = 0; i < _delimiters.length(); i++) {
		char c = _delimiters[i];
		if(m < c)
			m = c;
	}

	_maxDelimChar = m;
}

void
ciStringTokenizer::_setMaxDelimString() {
	if(_delimStrList.empty()) {
		_maxDelimString = "\0";
		return;
	}
	ciString a_str = "\0";
	ciStringList::iterator itr;
	for(itr = _delimStrList.begin(); itr != _delimStrList.end(); itr++) {
		
		//ciString t_str = *itr;
		if(a_str < (*itr))
			a_str = *itr;
	}
	_maxDelimString = a_str;
}

/**
* Skips ahead from startPos and returns the index of the next delimiter
* character encountered, or _maxPosition if no such delimiter is found.
*/
ciInt
ciStringTokenizer::_scanToken(ciInt startPos) {
	ciInt position = startPos;
	
	if ( !_strDelims ) { 
		while(position < _maxPosition) {
			char c = _str[position];
			if((c<=_maxDelimChar) && (_delimiters.find(c)<_delimiters.length())) 
			//if((c<=_maxDelimChar) && (_delimiters.find(c) != -1)) 
				break;
			position++;
		}
		if(_retDelims && (startPos==position)) {
			char c = _str[position];
			if((c<=_maxDelimChar) && (_delimiters.find(c)<_delimiters.length()))
			//if((c<=_maxDelimChar) && (_delimiters.find(c) != -1)) 
				position++;
		}
	} else {
		while(position < _maxPosition) {
			if ( _str.substr(position, _maxDelimString.size()) > _maxDelimString ) {
				position++;
				continue;
			}			
			ciBoolean findFlag = ciFalse;
			ciStringList::iterator itr;
			for ( itr = _delimStrList.begin(); itr != _delimStrList.end(); itr++ ) {
				ciInt deli_len = (*itr).size();
				if ( strncmp(_str.substr(position).c_str(), (*itr).c_str(), deli_len) == 0 ) {
					findFlag = ciTrue;
					break;
				}
			}
			if ( findFlag ) break;
			position++;
		}
		if ( _retDelims && (startPos==position) ) {
			if ( _str.substr(position, _maxDelimString.size()) <= _maxDelimString ) {
				ciStringList::iterator itr;
				for ( itr = _delimStrList.begin(); itr != _delimStrList.end(); itr++ ) {
					ciInt deli_len = (*itr).size();
					if ( strncmp(_str.substr(position).c_str(), (*itr).c_str(), deli_len) == 0 ) {
						position+=deli_len;
						break;
					}
				}
			}		
		}
	}
	return position;
}
		
// ciStringTokenizer::hasMoreTokens()
/*!
	������ token �� ���� �ִ� ��� true �� �����Ѵ�.
	hasMoreToken() �� true �� ��� nextToken() ���� ���� �̾Ƴ���.
*/
ciBoolean
ciStringTokenizer::hasMoreTokens() {
	_newPosition = _skipDelimiters(_currentPosition);
	if (_newPosition == -1) return false;
	
	return (_newPosition < _maxPosition);
}

// ciStringTokenizer::nextToken()
/*!
	�����ִ� token ���ڿ��� return �Ѵ�. 
	Token �� ���� ��� ""�� return �Ѵ�. 
*/
ciString
ciStringTokenizer::nextToken() {
	_currentPosition = (_newPosition>=0 && !_delimsChanged) ?
		 _newPosition : _skipDelimiters(_currentPosition);	
	if (_currentPosition == -1) return "";
	
	_delimsChanged = ciFalse;
	_newPosition = -1;
	if(_currentPosition >= _maxPosition) {
		//ciERROR(("throw NoSuchElementException"));
		return "";
	} 
	ciInt start = _currentPosition;
	_currentPosition = _scanToken(_currentPosition);
	//ciDEBUG(9,("start[%d], end[%d]",start,_currentPosition)); 
	return _str.substr(start, _currentPosition-start);
}

// ciStringTokenizer::nextToken()
/*!
    delimiter �� delim ���� �����Ͽ�
    �����ִ� token ���ڿ��� return �Ѵ�.
*   Token �� ���� ��� ""�� return �Ѵ�.
*/
ciString
ciStringTokenizer::nextToken(const char* delim) {
	if ( _strDelims ) {
		_strDelims = ciFalse;
		//_maxDelimString.clear();
		_maxDelimString="";
		_delimStrList.clear();
	} 
	_delimiters = delim;
	_delimsChanged = ciTrue;
	_setMaxDelimChar();

	return nextToken();
}

// ciStringTokenizer::nextToken()
/*!
	delimiter �� delim ���� �����Ͽ� 
	�����ִ� token ���ڿ��� return �Ѵ�. 
	Token �� ���� ��� ""�� return �Ѵ�. 
*/
ciString
ciStringTokenizer::nextToken(const ciString& delim) {
	return nextToken(delim.c_str());
}

ciString
ciStringTokenizer::nextToken(const ciStringList& deliList) {
	if ( !_strDelims ) {
		_strDelims = ciTrue;
		_maxDelimChar = '\0';
		_delimiters = "";
		//_delimiters.clear();
	} 
	_delimStrList = deliList;
	_delimsChanged = ciTrue;
	_setMaxDelimString();
	
	return nextToken();
}

// ciStringTokenizer::nextToken(ciInt n)
/*!
	�����ִ� token ���ڿ��� i ��° 
	���ڿ��� return �Ѵ�. 
	Token �� ���� ��� ""�� return �Ѵ�. 	
*/
ciString
ciStringTokenizer::nextToken(ciInt n) {
	ciString tStr;
	for(ciInt i = 0; i < n-1; i++) {
		tStr = nextToken();
		if( tStr.c_str() == "") {
			return "";
		}
	}
	return nextToken();
}

ciString
ciStringTokenizer::restTokens() {
	ciInt start = _currentPosition;
	_currentPosition = _maxPosition;
	return _str.substr(start, _currentPosition - start);
}

// ciStringTokenizer::nextToken(ciString& out)
/*!
	�����ִ� token ���ڿ��� return �Ѵ�. 
	Token �� ���� ��� ""�� return �Ѵ�. 
*/
void
ciStringTokenizer::nextToken_r(ciString& out) {
	_currentPosition = (_newPosition>=0 && !_delimsChanged) ?
		 _newPosition : _skipDelimiters(_currentPosition);	
	if (_currentPosition == -1) {
		out = "";
		return;
	}
	
	_delimsChanged = ciFalse;
	_newPosition = -1;
	if(_currentPosition >= _maxPosition) {
		//ciERROR(("throw NoSuchElementException"));
		out = "";
		return;
	} 
	ciInt start = _currentPosition;
	_currentPosition = _scanToken(_currentPosition);
	out = _str.substr(start, _currentPosition-start);
}

// ciStringTokenizer::nextToken(const char* delim, ciString& out)
/*!
    delimiter �� delim ���� �����Ͽ�
    �����ִ� token ���ڿ��� return �Ѵ�.
*   Token �� ���� ��� ""�� return �Ѵ�.
*/
void
ciStringTokenizer::nextToken_r(const char* delim, ciString& out) {
	if ( _strDelims ) {
		_strDelims = ciFalse;
		//_maxDelimString.clear();
		_maxDelimString="";
		_delimStrList.clear();		
	} 
	_delimiters = delim;
	_delimsChanged = ciTrue;
	_setMaxDelimChar();

	nextToken_r(out);
}

// ciStringTokenizer::nextToken(ciString& delim, ciString& out)
/*!
	delimiter �� delim ���� �����Ͽ� 
	�����ִ� token ���ڿ��� return �Ѵ�. 
	Token �� ���� ��� ""�� return �Ѵ�. 
*/
void
ciStringTokenizer::nextToken_r(const ciString& delim, ciString& out) {
	nextToken_r(delim.c_str(), out);
}

void
ciStringTokenizer::nextToken_r(const ciStringList& deliList, ciString& out) {
	if ( !_strDelims ) {
		_strDelims = ciTrue;
		_maxDelimChar = '\0';
		//_delimiters.clear();		
		_delimiters="";		
	} 
	_delimStrList = deliList;
	_delimsChanged = ciTrue;
	_setMaxDelimString();

	nextToken_r(out);
}

// ciStringTokenizer::nextToken(ciInt n, ciString& out)
/*!
	�����ִ� token ���ڿ��� i ��° 
	���ڿ��� return �Ѵ�. 
	Token �� ���� ��� ""�� return �Ѵ�. 	
*/
void
ciStringTokenizer::nextToken_r(ciInt n, ciString& out) {
	ciString tStr;
	for(ciInt i = 0; i < n-1; i++) {
		nextToken_r(tStr);
		if( tStr == "" ) {
			out = "";
			return;
		}
	}
	nextToken_r(out);
}

void
ciStringTokenizer::restTokens_r(ciString& out) {
	ciInt start = _currentPosition;
	_currentPosition = _maxPosition;
	out = _str.substr(start, _currentPosition - start);
}

// ciStringTokenizer::countTokens()
/*!
	Token �� ���� return �Ѵ�.
*/
ciInt
ciStringTokenizer::countTokens() {
	ciInt count = 0;
	ciInt currpos = _currentPosition;
	while(currpos < _maxPosition) {
		currpos = _skipDelimiters(currpos);
		if (currpos == -1) return 0;
		
		if(currpos >= _maxPosition)
			break;
		currpos = _scanToken(currpos);
		count++;
	}
	return count;
}
