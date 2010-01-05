if(WIN32 AND NOT UNIX)
#include_directories("C:/Program Files/Microsoft DirectX SDK (November 2008)/Include")
#include_directories("C:/Program Files/Microsoft DirectX SDK (March 2009)/Include")
include_directories("C:/Program Files/Microsoft DirectX SDK (August 2009)/Include")
#link_directories("C:/Program Files/Microsoft DirectX SDK (November 2008)/Lib/x86")
#link_directories("C:/Program Files/Microsoft DirectX SDK (March 2009)/Lib/x86")
link_directories("C:/Program Files/Microsoft DirectX SDK (August 2009)/Lib/x86")

file(GLOB files "*.cpp")
add_library(Renderer_D3D9 SHARED ${files})
target_link_libraries(Renderer_D3D9 Jet d3d9 d3dx9 dxerr gdi32)

endif(WIN32 AND NOT UNIX)





