/*
*                    GNU AFFERO GENERAL PUBLIC LICENSE
*                       Version 3, 19 November 2007
*
*    Copyright (c) 2023  Tyler Parret True
*
*    This program is free software: you can redistribute it and/or modify
*    it under the terms of the GNU Affero General Public License as published
*    by the Free Software Foundation, either version 3 of the License, or
*    (at your option) any later version.
*
*    This program is distributed in the hope that it will be useful,
*    but WITHOUT ANY WARRANTY; without even the implied warranty of
*    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*    GNU Affero General Public License for more details.
*
*    You should have received a copy of the GNU Affero General Public License
*    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
* @Authors
*     Tyler Parret True (OwlHowlinMornSky) <mysteryworldgod@outlook.com>
*/
#include "glCheck.h"

#ifdef _DEBUG

//#include <cstdarg>
//#include <cstring>
//#include <cctype>
//#include <cstdio>
//#define WIN32_LEAN_AND_MEAN
//#include <Windows.h>

#include <GL/glew.h>
#include <SFML/OpenGL.hpp>

#include <string>

#define GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION 0x0506

namespace {

wchar_t output[4096];

} // namespace

namespace g3d::impl {

/*
void GLog(const char* format, ...) {
	char buf[4096];
	char* p = buf;
	va_list args;
	int n;

	va_start(args, format);
	n = vsnprintf(p, sizeof buf - 3, format, args); // buf-3 is room for CR/LF/NUL

	int len = (int)strlen(buf);
	buf[len] = '\n';
	buf[len + 1] = '\0';

	vprintf(buf, args);
	va_end(args);

	p += (n < 0) ? sizeof buf - 3 : n;

	while (p > buf && p[-1] >= 0 && p[-1] <= 255 && isspace(p[-1]))
		*--p = '\0';

	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

	for (unsigned int i = 0; i < strlen(buf); i++)
		output[i] = (wchar_t)buf[i];
	output[strlen(buf)] = L'\0';

#ifdef _MSC_VER
	OutputDebugStringW(output);
#endif
	return;
}*/

void CheckGLError(const char* file, unsigned int line, const char* expression) {
	// Get the last error
	GLenum errorCode = glGetError();

	if (errorCode != GL_NO_ERROR) {
		std::string fileString = file;
		std::string error = "Unknown error";
		std::string description = "No description";

		// Decode the error code
		switch (errorCode) {
		case GL_INVALID_ENUM:
			error = "GL_INVALID_ENUM";
			description = "An unacceptable value has been specified for an enumerated argument.";
			break;
		case GL_INVALID_VALUE:
			error = "GL_INVALID_VALUE";
			description = "A numeric argument is out of range.";
			break;
		case GL_INVALID_OPERATION:
			error = "GL_INVALID_OPERATION";
			description = "The specified operation is not allowed in the current state.";
			break;
		case GL_STACK_OVERFLOW:
			error = "GL_STACK_OVERFLOW";
			description = "This command would cause a stack overflow.";
			break;
		case GL_STACK_UNDERFLOW:
			error = "GL_STACK_UNDERFLOW";
			description = "This command would cause a stack underflow.";
			break;
		case GL_OUT_OF_MEMORY:
			error = "GL_OUT_OF_MEMORY";
			description = "There is not enough memory left to execute the command.";
			break;
		case GLEXT_GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "GL_INVALID_FRAMEBUFFER_OPERATION";
			description = "The object bound to FRAMEBUFFER_BINDING is not \"framebuffer complete\".";
			break;
		default:;
		}

		// Log the error
		//GLog("An internal OpenGL call failed in %s (%d).\nExpression:\t%s\nError Description:\t%s\n%s\n", fileString.substr(fileString.find_last_of("\\/") + 1).c_str(), line, expression, error.c_str(), description.c_str());
#ifdef _MSC_VER
		printf_s("G3D: An internal OpenGL call failed in %s(%d).\n",
				 fileString.substr(fileString.find_last_of("\\/") + 1).c_str(),
				 line);
		printf_s("Expression:\n   %s\nError description:\n   %s\n   %s\n\n",
				 expression,
				 error.c_str(),
				 description.c_str());
#else
		printf("G3D: An internal OpenGL call failed in %s(%d).\n",
				 fileString.substr(fileString.find_last_of("\\/") + 1).c_str(),
				 line);
		printf("Expression:\n   %s\nError description:\n   %s\n   %s\n\n",
				 expression,
				 error.c_str(),
				 description.c_str());
#endif
	}
	return;
}


} // namespace g3d::impl

#endif // _DEBUG
