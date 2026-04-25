#
# Base utility library
#

file(GLOB libSrcs CONFIGURE_DEPENDS *.cc)
file(GLOB libHdrs CONFIGURE_DEPENDS *.h *.hh *.hpp)

# Target Library name
set(tgt base)

add_library(${tgt} STATIC)

target_sources(${tgt}
    PRIVATE ${libSrcs}
  )

target_include_directories(${tgt} 
  PUBLIC
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
