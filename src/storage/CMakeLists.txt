#
# Build configuration for the storage component, including dbadmin utility.
#

set(LIBRARY_NAME ${STORAGE_COMPONENT})

set(LIBRARY_SRCS
    db_model.cpp
)

set(LIBRARY_LIBS
    ${PROJECT_NAME}-util
    Wt::Wt
    Wt::DboSqlite3
)

include(StaticLibraryTargets)

set(TOOL_NAME "dbadmin")
set(TOOL_SRCS "main.cpp")

add_executable(${TOOL_NAME} ${TOOL_SRCS})
target_link_libraries(${TOOL_NAME} ${PROJECT_NAME}-${LIBRARY_NAME})
set_target_properties(
    ${TOOL_NAME} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}-${TOOL_NAME})
