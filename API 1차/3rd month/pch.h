#pragma once

#include "framework.h"


#include <list>
#include <algorithm>
#include <ctime>
#include <cmath>
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#ifdef _DEBUG

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifndef DBG_NEW 

#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ ) 
#define new DBG_NEW 

#endif
#endif

using namespace std;