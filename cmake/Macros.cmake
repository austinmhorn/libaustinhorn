# Custom function for building an example
function( build_example SAMPLE_NAME SOURCES )

    add_executable( ${SAMPLE_NAME} ${SOURCES} )
    target_link_libraries( ${SAMPLE_NAME} PRIVATE austinhorn::austinhorn )

    install(
        TARGETS ${SAMPLE_NAME}
        RUNTIME DESTINATION "${SHARE_PATH}/examples" COMPONENT examples
    )

    install(
        FILES ${SOURCES}
        DESTINATION "${SHARE_PATH}/examples" COMPONENT examples
    )
endfunction()

