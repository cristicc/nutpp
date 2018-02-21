#
# Build configuration for the auth component.
#

set(LIBRARY_NAME ${AUTH_COMPONENT})

set(LIBRARY_SRCS
    auth_widget.cpp
    login_session.cpp
    registration_view.cpp
    user_details_model.cpp
)

set(LIBRARY_LIBS
    ${PROJECT_NAME}-storage
)

include(StaticLibraryTargets)
