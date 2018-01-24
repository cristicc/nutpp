##
# Contains utility functions to help assemble a standalone bundle application.
#
# The implementation is an extension of the standard module BundleUtilities
# and allows the user to create a fully portable application by including all
# the library dependencies, even those part of the system, in a single bundle
# package.
#
# Copyright (C) 2018  Cristian Ciocaltea
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#
# @author Cristian Ciocaltea <cristian.ciocaltea@gmail.com>
##

#
# This is a project-specific hook of gp_item_default_embedded_path(),
# defined in GetPrerequisites module, to avoid placing the dependent
# libraries in the application binary folder.
#
function(gp_item_default_embedded_path_override item default_embedded_path_var)
  if("${APP_RUNTIME_DIR}" STREQUAL "")
    message(FATAL_ERROR
      "Please set 'APP_RUNTIME_DIR' before including 'BundleUtilitiesEx'!")
  endif()

  set(${default_embedded_path_var}
    "${CMAKE_INSTALL_PREFIX}/${APP_RUNTIME_DIR}/lib" PARENT_SCOPE)
endfunction()

#
# This is a project-specific hook of gp_resolved_file_type(), defined in
# GetPrerequisites module, to override the decision on whether a library
# is local or not. It is needed to mark external (non system) libraries as
# as local in order to make verify_bundle_prerequisites() succeed.
#
function(gp_resolved_file_type_override file type_var)
  if(NOT "${${type_var}}" STREQUAL "local" AND NOT "${${type_var}}" STREQUAL "system")
    message(STATUS "Marking '${file}' of type '${${type_var}}' as 'local'")
    set(${type_var} "local" PARENT_SCOPE)
  endif()
endfunction()

#
# Import the base package.
#
include(BundleUtilities)

#
# Extends get_bundle_keys with a new parameter:
# <exclude_system> must be 0 or 1 indicating whether to include or
# exclude "system" prerequisites.
#
function(get_bundle_keys_ex app libs dirs exclude_system keys_var)
  set(${keys_var} PARENT_SCOPE)

  get_bundle_and_executable("${app}" bundle executable valid)
  if(valid)
    # Always use the exepath of the main bundle executable for @executable_path
    # replacements:
    #
    get_filename_component(exepath "${executable}" PATH)

    # But do fixups on all executables in the bundle:
    #
    get_bundle_all_executables("${bundle}" exes)

    # Set keys for main executable first:
    #
    set_bundle_key_values(${keys_var} "${executable}" "${executable}" "${exepath}" "${dirs}" 0)

    # Get rpaths specified by main executable:
    #
    get_item_key("${executable}" executable_key)
    set(main_rpaths "${${executable_key}_RPATHS}")

    # For each extra lib, accumulate a key as well and then also accumulate
    # any of its prerequisites. (Extra libs are typically dynamically loaded
    # plugins: libraries that are prerequisites for full runtime functionality
    # but that do not show up in otool -L output...)
    #
    foreach(lib ${libs})
      set_bundle_key_values(${keys_var} "${lib}" "${lib}" "${exepath}" "${dirs}" 0 "${main_rpaths}")

      set(prereqs "")
      get_prerequisites("${lib}" prereqs ${exclude_system} 1 "${exepath}" "${dirs}" "${main_rpaths}")
      foreach(pr ${prereqs})
        set_bundle_key_values(${keys_var} "${lib}" "${pr}" "${exepath}" "${dirs}" 1 "${main_rpaths}")
      endforeach()
    endforeach()

    # For each executable found in the bundle, accumulate keys as we go.
    # The list of keys should be complete when all prerequisites of all
    # binaries in the bundle have been analyzed.
    #
    foreach(exe ${exes})
      # Main executable is scanned first above:
      #
      if(NOT "${exe}" STREQUAL "${executable}")
        # Add the exe itself to the keys:
        #
        set_bundle_key_values(${keys_var} "${exe}" "${exe}" "${exepath}" "${dirs}" 0 "${main_rpaths}")

        # Get rpaths specified by executable:
        #
        get_item_key("${exe}" exe_key)
        set(exe_rpaths "${main_rpaths}" "${${exe_key}_RPATHS}")
      else()
        set(exe_rpaths "${main_rpaths}")
      endif()

      # Add each prerequisite to the keys:
      #
      set(prereqs "")
      get_prerequisites("${exe}" prereqs ${exclude_system} 1 "${exepath}" "${dirs}" "${exe_rpaths}")
      foreach(pr ${prereqs})
        set_bundle_key_values(${keys_var} "${exe}" "${pr}" "${exepath}" "${dirs}" 1 "${exe_rpaths}")
      endforeach()
    endforeach()

    # Propagate values to caller's scope:
    #
    set(${keys_var} ${${keys_var}} PARENT_SCOPE)
    foreach(key ${${keys_var}})
      set(${key}_ITEM "${${key}_ITEM}" PARENT_SCOPE)
      set(${key}_RESOLVED_ITEM "${${key}_RESOLVED_ITEM}" PARENT_SCOPE)
      set(${key}_DEFAULT_EMBEDDED_PATH "${${key}_DEFAULT_EMBEDDED_PATH}" PARENT_SCOPE)
      set(${key}_EMBEDDED_ITEM "${${key}_EMBEDDED_ITEM}" PARENT_SCOPE)
      set(${key}_RESOLVED_EMBEDDED_ITEM "${${key}_RESOLVED_EMBEDDED_ITEM}" PARENT_SCOPE)
      set(${key}_COPYFLAG "${${key}_COPYFLAG}" PARENT_SCOPE)
      set(${key}_RPATHS "${${key}_RPATHS}" PARENT_SCOPE)
      set(${key}_RDEP_RPATHS "${${key}_RDEP_RPATHS}" PARENT_SCOPE)
    endforeach()
  endif()
endfunction()

#
# Extends fixup_bundle with a new parameter:
# <exclude_system> must be 0 or 1 indicating whether to include or
# exclude "system" prerequisites.
#
function(fixup_bundle_ex app libs dirs exclude_system)
  message(STATUS "fixup_bundle_ex")
  message(STATUS "  app='${app}'")
  message(STATUS "  libs='${libs}'")
  message(STATUS "  dirs='${dirs}'")
  message(STATUS "  exclude_system='${exclude_system}'")

  get_bundle_and_executable("${app}" bundle executable valid)
  if(valid)
    get_filename_component(exepath "${executable}" PATH)

    message(STATUS "fixup_bundle_ex: preparing...")
    get_bundle_keys_ex("${app}" "${libs}" "${dirs}" ${exclude_system} keys)

    message(STATUS "fixup_bundle_ex: copying...")
    list(LENGTH keys n)
    math(EXPR n ${n}*2)

    set(i 0)
    foreach(key ${keys})
      math(EXPR i ${i}+1)
      if(${${key}_COPYFLAG})
        message(STATUS "${i}/${n}: copying '${${key}_RESOLVED_ITEM}'")
      else()
        message(STATUS "${i}/${n}: *NOT* copying '${${key}_RESOLVED_ITEM}'")
      endif()

      set(show_status 0)
      if(show_status)
        message(STATUS "key='${key}'")
        message(STATUS "item='${${key}_ITEM}'")
        message(STATUS "resolved_item='${${key}_RESOLVED_ITEM}'")
        message(STATUS "default_embedded_path='${${key}_DEFAULT_EMBEDDED_PATH}'")
        message(STATUS "embedded_item='${${key}_EMBEDDED_ITEM}'")
        message(STATUS "resolved_embedded_item='${${key}_RESOLVED_EMBEDDED_ITEM}'")
        message(STATUS "copyflag='${${key}_COPYFLAG}'")
        message(STATUS "")
      endif()

      if(${${key}_COPYFLAG})
        set(item "${${key}_ITEM}")
        if(item MATCHES "[^/]+\\.framework/")
          copy_resolved_framework_into_bundle("${${key}_RESOLVED_ITEM}"
            "${${key}_RESOLVED_EMBEDDED_ITEM}")
        else()
          copy_resolved_item_into_bundle("${${key}_RESOLVED_ITEM}"
            "${${key}_RESOLVED_EMBEDDED_ITEM}")
        endif()
      endif()
    endforeach()

    message(STATUS "fixup_bundle_ex: fixing...")
    foreach(key ${keys})
      math(EXPR i ${i}+1)
      if(APPLE)
        message(STATUS "${i}/${n}: fixing up '${${key}_RESOLVED_EMBEDDED_ITEM}'")
        fixup_bundle_item("${${key}_RESOLVED_EMBEDDED_ITEM}" "${exepath}" "${dirs}")
      else()
        message(STATUS "${i}/${n}: fix-up not required on this platform '${${key}_RESOLVED_EMBEDDED_ITEM}'")
      endif()
    endforeach()

    message(STATUS "fixup_bundle_ex: cleaning up...")
    clear_bundle_keys(keys)

    message(STATUS "fixup_bundle_ex: verifying...")
    verify_app("${app}" ${exclude_system})
  else()
    message(SEND_ERROR "error: fixup_bundle: not a valid bundle")
  endif()

  message(STATUS "fixup_bundle_ex: done")
endfunction()
