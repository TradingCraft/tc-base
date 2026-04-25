#
# Base utility library
#

file(GLOB libSrcs CONFIGURE_DEPENDS *.cc)
file(GLOB libHdrs CONFIGURE_DEPENDS *.h *.hh *.hpp)

# Re-configure when HEAD or index changes so GIT_HASH stays current
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS
    ${CMAKE_SOURCE_DIR}/.git/HEAD
    ${CMAKE_SOURCE_DIR}/.git/index)

# Extract Git hash for version info
execute_process(
    COMMAND git rev-parse --short=8 HEAD
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE GIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_QUIET
)
if(NOT GIT_HASH)
    set(GIT_HASH "unknown")
endif()

# Generate version header from CMake variables
configure_file(version.hh.in ${CMAKE_CURRENT_BINARY_DIR}/version.hh @ONLY)

# Target Library name
set(tgt base)

add_library(${tgt} STATIC)

target_sources(${tgt}
    PRIVATE ${libSrcs}
  )

target_include_directories(${tgt} 
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(${tgt}
  PUBLIC
    CLI11::CLI11
    spdlog::spdlog
)

# Install the library
install(TARGETS ${tgt}
    EXPORT tc-baseTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install public headers directly in the include/ root
install(FILES ${libHdrs} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
