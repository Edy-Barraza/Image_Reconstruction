# only add PUBLIC dependencies as well
#   https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html#creating-a-package-configuration-file
include(CMakeFindDependencyMacro)

find_dependency(PNG)
find_dependency(ZLIB)

set(PNGwriter_HAVE_FREETYPE TRUE)
if(PNGwriter_HAVE_FREETYPE)
    find_dependency(Freetype)
endif()

include("${CMAKE_CURRENT_LIST_DIR}/PNGwriterTargets.cmake")

# if necessary, add COMPONENTS here
