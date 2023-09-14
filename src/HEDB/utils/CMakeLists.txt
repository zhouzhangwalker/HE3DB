# Source files in this directory
set(HEDB_SOURCE_FILES ${HEDB_SOURCE_FILES}
    ${CMAKE_CURRENT_LIST_DIR}/serialize.cpp
)

# Add header files for installation
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/ThreadPool.h
        ${CMAKE_CURRENT_LIST_DIR}/types.h
        ${CMAKE_CURRENT_LIST_DIR}/utils.h
        ${CMAKE_CURRENT_LIST_DIR}/function.h
        ${CMAKE_CURRENT_LIST_DIR}/multi_thread.h
        ${CMAKE_CURRENT_LIST_DIR}/serialize.h
    DESTINATION
    ${HEDB_INCLUDES_INSTALL_DIR}/HEDB/utils
)

set(HEDB_SOURCE_FILES ${HEDB_SOURCE_FILES} PARENT_SCOPE)