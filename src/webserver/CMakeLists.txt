#
# Build configuration for the webserver component.
#

set(WEBSERVER_SRCS
    email_validator.cpp
    main.cpp
    nutpp_ui.cpp
    patient_form_model.cpp
    patient_form_view.cpp
    patient_list_view.cpp
    patient_view.cpp
    settings.cpp
)

set(WEBSERVER_LIBS
    ${PROJECT_NAME}-auth
    Wt::HTTP
)

add_executable(${WEBSERVER_COMPONENT} ${WEBSERVER_SRCS})
target_link_libraries(${WEBSERVER_COMPONENT} ${WEBSERVER_LIBS})
set_target_properties(
    ${WEBSERVER_COMPONENT} PROPERTIES OUTPUT_NAME ${PROJECT_NAME}-webserver)

# Prepare the start script.
if(UNIX)
#    set(APP_START_SCRIPT "start-${WEBSERVER_COMPONENT}.sh")
#    configure_file("scripts/start-simulator.sh.in" "${APP_START_SCRIPT}" @ONLY)
else()
#    set(APP_START_SCRIPT "start-${WEBSERVER_COMPONENT}.bat")
#    configure_file("scripts/start-simulator.bat.in" "${APP_START_SCRIPT}" @ONLY)
endif()

# Install component.
install(TARGETS ${WEBSERVER_COMPONENT} DESTINATION "${WEBSERVER_COMPONENT}/bin"
    COMPONENT ${WEBSERVER_COMPONENT})

install(DIRECTORY "../conf" DESTINATION "${WEBSERVER_COMPONENT}"
    COMPONENT ${WEBSERVER_COMPONENT})

#install(PROGRAMS "${CMAKE_CURRENT_BINARY_DIR}/${APP_START_SCRIPT}"
#    DESTINATION "."
#    COMPONENT ${WEBSERVER_COMPONENT})

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
