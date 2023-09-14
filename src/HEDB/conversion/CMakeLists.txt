# Source files in this directory
set(HEDB_SOURCE_FILES ${HEDB_SOURCE_FILES}
    ${CMAKE_CURRENT_LIST_DIR}/repack.cpp
    ${CMAKE_CURRENT_LIST_DIR}/polyeval_bsgs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/seal_util.cpp
)

# Add header files for installation
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/repack.h
        ${CMAKE_CURRENT_LIST_DIR}/polyeval_bsgs.h
        ${CMAKE_CURRENT_LIST_DIR}/conversion.h
        ${CMAKE_CURRENT_LIST_DIR}/seal_util.h
    DESTINATION
    ${HEDB_INCLUDES_INSTALL_DIR}/HEDB/conversion
)

set(HEDB_SOURCE_FILES ${HEDB_SOURCE_FILES} PARENT_SCOPE)