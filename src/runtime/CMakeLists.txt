if(UNIX)
    # Install the dynamic linker/loader on Linux based targets.
    set(_ld_linux_bin "/lib64/ld-linux-x86-64.so.2")
    if(NOT "${BUILD_TARGET_ARCH}" STREQUAL "x86_64")
        set(_ld_linux_bin "/lib/ld-linux.so.2")
    endif()
    get_filename_component(_ld_linux_bin_resolved "${_ld_linux_bin}" REALPATH)

    install(PROGRAMS "${_ld_linux_bin_resolved}"
        DESTINATION "${RUNTIME_COMPONENT}"
        COMPONENT ${RUNTIME_COMPONENT} RENAME "ld-linux.so.2")

    # Install helper script.
    install(PROGRAMS "run-program.sh"
        DESTINATION "${RUNTIME_COMPONENT}"
        COMPONENT ${RUNTIME_COMPONENT} RENAME "run-program")
else()
    # Install Visual C++ Redistributable for VS2015 on Windows targets.
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION "${RUNTIME_COMPONENT}/lib")
    set(CMAKE_INSTALL_SYSTEM_RUNTIME_COMPONENT "${RUNTIME_COMPONENT}")

    # As of MSVC 19 the runtime depends on the Universal CRT, which is part
    # of Windows development kit 10 and above.
    set(CMAKE_INSTALL_UCRT_LIBRARIES 1)

    # Add install rules for compiler-provided system runtime libraries.
    include(InstallRequiredSystemLibraries)

    # Install helper script.
    install(PROGRAMS "run-program.bat"
        DESTINATION "${RUNTIME_COMPONENT}"
        COMPONENT ${RUNTIME_COMPONENT})
endif()
