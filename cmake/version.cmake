# Set version value for the project and validate that it matches version reported by git.

# Reference values are taken from the version.h.
# So even if the library code is decoupled from the cmake-specific project,
# source files still have proper version information.

function (regex_extract_matching_groups INPUT REGEX_STR)
    string (REGEX MATCHALL "${REGEX_STR}" _ "${INPUT}")
    if (NOT CMAKE_MATCH_0)
        return()
    endif()

    set (MATCH_GROUP_INDEX 0)
    foreach (OUTPUT_VAR ${ARGN})
        math (EXPR MATCH_GROUP_INDEX "${MATCH_GROUP_INDEX}+1")
        set (MATCH_GROUP_VALUE "${CMAKE_MATCH_${MATCH_GROUP_INDEX}}")
        set ("${OUTPUT_VAR}" "${MATCH_GROUP_VALUE}" PARENT_SCOPE)
    endforeach ()
endfunction ()


function(datastore_cpp_get_version)
    # Extract all components of the version from the datastore/version.h

    file(READ ${CMAKE_CURRENT_SOURCE_DIR}/datastore/version.h VERSION_FILE_DATA)

    foreach (VERSION_COMPONENT
            IN ITEMS
            DATASTORE_CPP_VERSION_MAJOR
            DATASTORE_CPP_VERSION_MINOR
            DATASTORE_CPP_VERSION_PATCH
            DATASTORE_CPP_VERSION_BUILD)

        regex_extract_matching_groups(
            "${VERSION_FILE_DATA}"
            "#define ${VERSION_COMPONENT} ([0-9]+)"
            "${VERSION_COMPONENT}")

        set ("${VERSION_COMPONENT}" "${${VERSION_COMPONENT}}" PARENT_SCOPE)
    endforeach ()

    set(DATASTORE_CPP_VERSION "${DATASTORE_CPP_VERSION_MAJOR}.${DATASTORE_CPP_VERSION_MINOR}.${DATASTORE_CPP_VERSION_PATCH}" PARENT_SCOPE)
endfunction()

datastore_cpp_get_version()

function(datastore_cpp_check_library_version CHECK_MODE)
## Verify that current tag matches the version

    find_program (GIT git)
    if (GIT)
        execute_process(
            COMMAND ${GIT} describe --tags
            WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
            OUTPUT_VARIABLE GIT_DESCRIBE_DATA
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        regex_extract_matching_groups(
            "${GIT_DESCRIBE_DATA}"
            "^v([0-9]+)\\.([0-9]+)\\.([0-9]+)(\\-([0-9]+).*)?"
            VERSION_FROM_GIT_DESCRIBE_MAJOR
            VERSION_FROM_GIT_DESCRIBE_MINOR
            VERSION_FROM_GIT_DESCRIBE_PATCH
            DATASTORE_CPP_VERSION_COMMIT
        )

        if (NOT (VERSION_FROM_GIT_DESCRIBE_MAJOR AND VERSION_FROM_GIT_DESCRIBE_MINOR AND VERSION_FROM_GIT_DESCRIBE_PATCH))
            message (${CHECK_MODE} "version obtained from `git describe` doesn't look like a valid version: \"${GIT_DESCRIBE_DATA}\"")
            return ()
        endif ()

      set (EXPECTED_DATASTORE_CPP_VERSION "${VERSION_FROM_GIT_DESCRIBE_MAJOR}.${VERSION_FROM_GIT_DESCRIBE_MINOR}.${VERSION_FROM_GIT_DESCRIBE_PATCH}")
      if (NOT "${EXPECTED_DATASTORE_CPP_VERSION}" STREQUAL ${DATASTORE_CPP_VERSION})
          message(${CHECK_MODE} "update DATASTORE_CPP_VERSION_ values in version.h.\n"
"git reports version as \"${GIT_DESCRIBE_DATA}\","
" hence expecting version to be ${EXPECTED_DATASTORE_CPP_VERSION}, "
" instead got ${DATASTORE_CPP_VERSION}")
      endif ()
    else ()
      message (${CHECK_MODE} "git is not found, can't verify library version")
    endif ()

    message("Version check passed: ${DATASTORE_CPP_VERSION}")
endfunction()

# datastore_cpp_check_library_version()
