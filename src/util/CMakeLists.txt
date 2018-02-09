#
# Build configuration for the util component.
#

set(LIBRARY_NAME "util")

set(LIBRARY_SRCS
    log.cpp
    string_ops.cpp
)

set(LIBRARY_LIBS
    ${LOG4CPLUS_LIBRARIES}
)

include(StaticLibraryTargets)
