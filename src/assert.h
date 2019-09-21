#pragma once

#define uemb_assert(expr) if(!(expr)) { uemb_assert_impl(#expr, __FILE__, __func__, __LINE__);}

#ifdef __GNUC__
__attribute__((weak)) 
#else
__declspec( selectany )
#endif
void uemb_assert_impl( char const* expr, char const* file, char const* function, int line );