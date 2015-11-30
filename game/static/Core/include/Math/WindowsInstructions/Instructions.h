#pragma once

//This header will only be included when compiling on Windows-OS

#if !OS_WINDOWS
#error "This header file is for Windows purpose only."
#endif

#define	__Included_by_instructions_h 1
#include <Math/WindowsInstructions/__instructions.h>
#include <Math/WindowsInstructions/Accessor.h>
#include <Math/WindowsInstructions/Color.h>
#include <Math/WindowsInstructions/Conversion.h>
#include <Math/WindowsInstructions/Load.h>
#include <Math/WindowsInstructions/Matrix.h>
#include <Math/WindowsInstructions/Plane.h>
#include <Math/WindowsInstructions/Quaternion.h>
#include <Math/WindowsInstructions/Scalar.h>
#include <Math/WindowsInstructions/Store.h>
#include <Math/WindowsInstructions/Typedefs.h>
#include <Math/WindowsInstructions/Utility.h>
#include <Math/WindowsInstructions/Vector.h>
#undef  __Included_by_instructions_h

BEGINNAMESPACE



ENDNAMESPACE