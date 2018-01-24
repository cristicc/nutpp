# Build Webserver application component.
set(COMPONENT_SRCS
    main.cpp
)

set(COMPONENT_LIBS
    ${LIBWT_LIBRARIES}
)

include_directories(.)

add_executable(${WEBSERVER_COMPONENT} ${COMPONENT_SRCS})
target_link_libraries(${WEBSERVER_COMPONENT} ${COMPONENT_LIBS})

# Prepare the start script.
if(UNIX)
    set(APP_START_SCRIPT "start-${WEBSERVER_COMPONENT}.sh")
    configure_file("scripts/start-simulator.sh.in" "${APP_START_SCRIPT}" @ONLY)
else()
    set(APP_START_SCRIPT "start-${WEBSERVER_COMPONENT}.bat")
    configure_file("scripts/start-simulator.bat.in" "${APP_START_SCRIPT}" @ONLY)
endif()

# Install component.
install(TARGETS ${WEBSERVER_COMPONENT} DESTINATION "${WEBSERVER_COMPONENT}/bin"
    COMPONENT ${WEBSERVER_COMPONENT})

install(DIRECTORY "../conf" DESTINATION "${WEBSERVER_COMPONENT}"
    COMPONENT ${WEBSERVER_COMPONENT})

install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/${APP_START_SCRIPT}"
    DESTINATION "."
    COMPONENT ${WEBSERVER_COMPONENT})

# Install runtime.
install(
    CODE "
        set(CMAKE_MODULE_PATH \"${CMAKE_MODULE_PATH}\")
        set(APP_RUNTIME_DIR \"${RUNTIME_COMPONENT}\")
        include(BundleUtilitiesEx)
        if(UNIX)
            fixup_bundle_ex(\"${CMAKE_CURRENT_BINARY_DIR}/${WEBSERVER_COMPONENT}\" \"\" \"\" 0)
        else()
            fixup_bundle_ex(\"${CMAKE_CURRENT_BINARY_DIR}/${WEBSERVER_COMPONENT}.exe\" \"\" \"\" 1)
        endif()
    "
    COMPONENT ${RUNTIME_COMPONENT}
)
