#pragma once
//Get�Լ�
#define GETTER(T,Member)\
 T* Get##T()\
{return Member;}

#define  SETTER(T,Member)\
void Set##T(T* Value)\
{Member = Value;}

#define LERP(prev, next, time) ((prev * (1.0f - time)) + (next * time))