/*
**	Command & Conquer(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* $Header:   F:\projects\c&c\vcs\code\jshell.h_v   2.16   16 Oct 1995 16:45:06   JOE_BOSTIC  $ */
/*********************************************************************************************** 
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               *** 
 *********************************************************************************************** 
 *                                                                                             * 
 *                 Project Name : Command & Conquer                                            * 
 *                                                                                             * 
 *                    File Name : JSHELL.H                                                     * 
 *                                                                                             * 
 *                   Programmer : Joe L. Bostic                                                * 
 *                                                                                             * 
 *                   Start Date : 03/13/95                                                     * 
 *                                                                                             * 
 *                  Last Update : March 13, 1995 [JLB]                                         * 
 *                                                                                             * 
 *---------------------------------------------------------------------------------------------* 
 * Functions:                                                                                  * 
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#pragma once

#include <bit>
#include <algorithm>

/*
**	Interface class to the keyboard. This insulates the game from library vagaries. Most
**	notable being the return values are declared as "int" in the library whereas C&C
**	expects it to be of KeyNumType.
*/

// Input functions — originally from wwlib32.
// TODO: Phase 2 — replace with Win32 GetAsyncKeyState / GetCursorPos.
class Keyboard 
{
	public:
		inline int Get_Key_Num(void) { return 0; };
		inline int Check_Key_Num(void) { return 0; };
		inline int To_ASCII(int _key_num) { return 0; };
		inline void Clear(void) {};
		inline void Stuff(int _key_num) {};
		inline int Down(int _key_num) { return 0; };
		inline int Mouse_X(void) { return 0; };
		inline int Mouse_Y(void) { return 0; };
};

/*
class Keyboard 
{
	public:
		static KeyNumType Get(void) {return (KeyNumType)Get_Key_Num();};
		static KeyNumType Check(void) {return (KeyNumType)Check_Key_Num();};
		static KeyASCIIType To_ASCII(KeyNumType key) {return (KeyASCIIType)KN_To_KA(key);};
		static void Clear(void) {Clear_KeyBuffer();};
		static void Stuff(KeyNumType key) {Stuff_Key_Num(key);};
		static int Down(KeyNumType key) {return Key_Down(key);};
		static int Mouse_X(void) {return Get_Mouse_X();};
		static int Mouse_Y(void) {return Get_Mouse_Y();};
};
*/

#ifdef NEVER
inline void * operator delete(void * data) 
{
	Free(data);
}

inline void * operator delete[] (void * data)
{
	Free(data);
}
#endif


/*
**	These templates allow enumeration types to have simple bitwise
**	arithmatic performed. The operators must be instatiated for the
**	enumerated types desired.
*/
/*
    TODO: Phase 1 - Constrain template with require, std::is_enum<T>
        At this state, these operators are too broad.
*/
template<class T> inline T operator ++(T & a)
{
	a = (T)((int)a + (int)1);
	return(a);
}
template<class T> inline T operator ++(T & a, int)
{
	T aa = a;
	a = (T)((int)a + (int)1);
	return(aa);
}	
template<class T> inline T operator --(T & a)
{
	a = (T)((int)a - (int)1);
	return(a);
}
template<class T> inline T operator --(T & a, int)
{
	T aa = a;
	a = (T)((int)a - (int)1);
	return(aa);
}
template<class T> inline T operator |(T t1, T t2)
{
	return((T)((int)t1 | (int)t2));
}
template<class T> inline T operator &(T t1, T t2)
{
	return((T)((int)t1 & (int)t2));
}
template<class T> inline T operator ~(T t1)
{
	return((T)(~(int)t1));
}


/*
**	The shape flags are likely to be "or"ed together and other such bitwise
**	manipulations. These instatiated operator templates allow this.
*/

/*
    TODO: Phase 1 - Convert to proper scoped enum class
	    In total, 308 call sites spotted during the last search.

*/
enum ShapeFlags_Type : int;
inline ShapeFlags_Type operator |(ShapeFlags_Type, ShapeFlags_Type);
inline ShapeFlags_Type operator &(ShapeFlags_Type, ShapeFlags_Type);
inline ShapeFlags_Type operator ~(ShapeFlags_Type);

/*
	TODO: Phase 1 - Verify the correctness of the functions with
	    the original ASM codes written in `pragma aux` blocks.
		Once the verification is done, delete ASM blocks.
*/

/*
#pragma aux Set_Bit parm [esi] [ecx] [eax] \
	modify [esi ebx] = 			\
	"mov	ebx,ecx"					\
	"shr	ebx,5"					\
	"and	ecx,01Fh"				\
	"btr	[esi+ebx*4],ecx"		\
	"or	eax,eax"					\
	"jz	ok"						\
	"bts	[esi+ebx*4],ecx"		\
	"ok:"
*/
inline void Set_Bit(void* array, int bit, int value) {
    auto* arr = static_cast<unsigned int*>(array);
    if (value)
        arr[bit >> 5] |=  (1u << (bit & 31));
    else
        arr[bit >> 5] &= ~(1u << (bit & 31));
}

/*
#pragma aux Get_Bit parm [esi] [eax] \
	modify [esi ebx] \
	value [eax]		= 				\
	"mov	ebx,eax"					\
	"shr	ebx,5"					\
	"and	eax,01Fh"				\
	"bt	[esi+ebx*4],eax"		\
	"setc	al"
*/
inline int Get_Bit(void const* array, int bit) {
    auto const* arr = static_cast<unsigned int const*>(array);
    return (arr[bit >> 5] >> (bit & 31)) & 1;
}

/*
#pragma aux First_True_Bit parm [esi] \
	modify [esi ebx] \
	value [eax]		= 				\
	"mov	eax,-32"					\
	"again:"							\
	"add	eax,32"					\
	"mov	ebx,[esi]"				\
	"add	esi,4"					\
	"bsf	ebx,ebx"					\
	"jz	again"					\
	"add	eax,ebx"
*/
inline int First_True_Bit(void const* array) {
    auto const* arr = static_cast<unsigned int const*>(array);
    int base = 0;
    while (true) {
        if (*arr) {
            return base + std::countr_zero(*arr);
        }
        ++arr;
        base += 32;
    }
}

/*
#pragma aux First_False_Bit parm [esi] \
	modify [esi ebx] \
	value [eax]		= 				\
	"mov	eax,-32"					\
	"again:"							\
	"add	eax,32"					\
	"mov	ebx,[esi]"				\
	"not	ebx"						\
	"add	esi,4"					\
	"bsf	ebx,ebx"					\
	"jz	again"					\
	"add	eax,ebx"
*/
inline int First_False_Bit(void const* array) {
    auto const* arr = static_cast<unsigned int const*>(array);
    int base = 0;
    while (true) {
        if (~(*arr)) {
            return base + std::countr_one(*arr);
        }
        ++arr;
        base += 32;
    }
}

/*
#pragma aux Bound parm [eax] [ebx] [ecx] \
	modify [eax] \
	value [eax]		= 				\
	"cmp	ebx,ecx"					\
	"jl	okorder"					\
	"xchg	ebx,ecx"					\
	"okorder: cmp	eax,ebx"		\
	"jg	okmin"					\
	"mov	eax,ebx"					\
	"okmin: cmp	eax,ecx"			\
	"jl	okmax"					\
	"mov	eax,ecx"					\
	"okmax:"
*/
inline int Bound(int original, int min, int max) {
	return std::clamp(original, min, max);
}

#ifdef NEVER
extern unsigned Bound(unsigned original, unsigned min, unsigned max);
#pragma aux Bound parm [eax] [ebx] [ecx] \
	modify [eax] \
	value [eax]		= 				\
	"cmp	ebx,ecx"					\
	"jb	okorder"					\
	"xchg	ebx,ecx"					\
	"okorder: cmp	eax,ebx"		\
	"ja	okmin"					\
	"mov	eax,ebx"					\
	"okmin: cmp	eax,ecx"			\
	"jb	okmax"					\
	"mov	eax,ecx"					\
	"okmax:"
#endif

/*
#pragma aux Fixed_To_Cardinal parm [eax] [edx] \
	modify [edx] \
	value [eax]		= 				\
	"mul	edx"						\
	"add	eax,080h"				\
	"test	eax,0FF000000h"		\
	"jz	ok"						\
	"mov	eax,000FFFFFFh"		\
	"ok:"								\
	"shr	eax,8"
*/
inline unsigned Fixed_To_Cardinal(unsigned base, unsigned fixed) {
    unsigned long long result = 
        static_cast<unsigned long long>(base) * fixed + 0x80u;
    if (result > 0x00FFFFFFu) return 0x00FFFFFFu;
    return static_cast<unsigned>(result >> 8);
}

/*
#pragma aux Cardinal_To_Fixed parm [ebx] [eax] \
	modify [edx] \
	value [eax]		= 				\
	"or	ebx,ebx"					\
	"jz	fini"						\
	"shl	eax,8"					\
	"xor	edx,edx"					\
	"div	ebx"						\
	"fini:"
*/
inline unsigned Cardinal_To_Fixed(unsigned base, unsigned cardinal) {
    if (!base) return 0;
    return static_cast<unsigned>(
        (static_cast<unsigned long long>(cardinal) << 8) / base
    );
}
