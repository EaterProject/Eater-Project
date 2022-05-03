#pragma once
//GetÇÔ¼ö
#define GETTER(T,Member)\
 T* Get##T()\
{return Member;}

#define  SETTER(T,Member)\
void Set##T(T* Value)\
{Member = Value;}

