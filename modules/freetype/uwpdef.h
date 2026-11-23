// This file provides a workaround for the UWP (Universal Windows Platform)
// build of the FreeType module. In C++/CX code, "generic" is a reserved
// keyword, which can cause conflicts with the FreeType code. This header
// redefines "generic" to "freetype_generic" to avoid such errors. "generic" is
// a reserved keyword in C++/CX code this avoids the errors in the variable name
// from Freetype code
#define generic freetype_generic
