# Source files in this directory
set(HEDB_SOURCE_FILES ${HEDB_SOURCE_FILES}
    ${CMAKE_CURRENT_LIST_DIR}/extract_msb.cpp
    ${CMAKE_CURRENT_LIST_DIR}/HomCompare.cpp
    ${CMAKE_CURRENT_LIST_DIR}/operators.cpp
    ${CMAKE_CURRENT_LIST_DIR}/tfhepp_utils.cpp
)

# Add header files for installation
install(
    FILES
        ${CMAKE_CURRENT_LIST_DIR}/comparison.h
        ${CMAKE_CURRENT_LIST_DIR}/extract_msb.h
        ${CMAKE_CURRENT_LIST_DIR}/HomCompare.h
        ${CMAKE_CURRENT_LIST_DIR}/operators.h
        ${CMAKE_CURRENT_LIST_DIR}/tfhepp_utils.h
    DESTINATION
    ${HEDB_INCLUDES_INSTALL_DIR}/HEDB/comparison
)

set(HEDB_SOURCE_FILES ${HEDB_SOURCE_FILES} PARENT_SCOPE)