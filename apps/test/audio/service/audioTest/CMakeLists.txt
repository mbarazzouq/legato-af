#*******************************************************************************
# Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
#
# Contributors:
#     Sierra Wireless - initial API and implementation
#*******************************************************************************

find_package(Legato REQUIRED)

set(APP_TARGET testAudioService)

if(${LEGATO_TARGET} STREQUAL "ar7")
    mkexe(${APP_TARGET}
                .
                -C "-g -O0"
                --verbose
                -i ${LEGATO_ROOT}/interfaces/audio
                -i ${LEGATO_ROOT}/interfaces/modemServices
            DEPENDS
                legato
                ## TODO: Remove this when the mk tools do dependency checking.
                Component.cdef
                audioTest.c
                )

    if(NOT ${LEGATO_TARGET} STREQUAL "localhost")
        add_test(${APP_TARGET} ${EXECUTABLE_OUTPUT_PATH}/${APP_TARGET})
        set_tests_properties(${APP_TARGET} PROPERTIES TIMEOUT 0)
    endif()
endif()
