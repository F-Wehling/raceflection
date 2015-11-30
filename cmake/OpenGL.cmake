###---
### include the correct opengl lib and include files
###
message("\tExternal: OpenGL")

find_package(OpenGL REQUIRED)

set(Target_Inc ${Target_Inc} ${OPENGL_INCLUDE_DIR})
set(Target_Lib ${Target_Lib} ${OPENGL_LIBRARIES})
