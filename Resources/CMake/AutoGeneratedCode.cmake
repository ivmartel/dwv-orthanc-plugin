set(AUTOGENERATED_DIR "${CMAKE_CURRENT_BINARY_DIR}/AUTOGENERATED")
set(AUTOGENERATED_SOURCES)

file(MAKE_DIRECTORY ${AUTOGENERATED_DIR})
include_directories(${AUTOGENERATED_DIR})

macro(EmbedResources)
  # Convert a semicolon separated list to a whitespace separated string
  set(SCRIPT_OPTIONS)
  set(SCRIPT_ARGUMENTS)
  set(DEPENDENCIES)
  set(IS_PATH_NAME false)

  # Loop over the arguments of the function
  foreach(arg ${ARGN})
    # Extract the first character of the argument
    string(SUBSTRING "${arg}" 0 1 FIRST_CHAR)
    if (${FIRST_CHAR} STREQUAL "-")
      # If the argument starts with a dash "-", this is an option to
      # EmbedResources.py
      list(APPEND SCRIPT_OPTIONS ${arg})
    else()
      if (${IS_PATH_NAME})
        list(APPEND SCRIPT_ARGUMENTS "${arg}")
        list(APPEND DEPENDENCIES "${arg}")
        set(IS_PATH_NAME false)
      else()
        list(APPEND SCRIPT_ARGUMENTS "${arg}")
        set(IS_PATH_NAME true)
      endif()
    endif()
  endforeach()

  set(TARGET_BASE "${AUTOGENERATED_DIR}/EmbeddedResources")
  add_custom_command(
    OUTPUT
    "${TARGET_BASE}.h"
    "${TARGET_BASE}.cpp"
    COMMAND 
    ${PYTHON_EXECUTABLE}
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources/EmbedResources.py"
    ${SCRIPT_OPTIONS}
    "${AUTOGENERATED_DIR}/EmbeddedResources"
    ${SCRIPT_ARGUMENTS}
    DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/Resources/EmbedResources.py"
    ${DEPENDENCIES}
    )

  list(APPEND AUTOGENERATED_SOURCES
    "${AUTOGENERATED_DIR}/EmbeddedResources.cpp"
    ) 
endmacro()
