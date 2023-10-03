#pragma once

#ifdef _DEBUG

namespace g3d::impl {

//void GLog(const char* format, ...);

void CheckGLError(const char* file,
							   unsigned int line,
							   const char* expression);

} // namespace g3d::impl

#define glCheck(expr) do{expr; g3d::impl::CheckGLError(__FILE__,__LINE__,#expr);} while(false)

#else

#define glCheck(expr) (expr)

#endif
