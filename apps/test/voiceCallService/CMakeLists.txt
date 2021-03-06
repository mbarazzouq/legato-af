#*******************************************************************************
# Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
#
# Contributors:
#     Sierra Wireless - initial API and implementation
#*******************************************************************************


find_package(Legato REQUIRED)

set(APP_TARGET testVoiceCallService)

mkexe(  ${APP_TARGET}
            voiceCallTest
            -i ${LEGATO_ROOT}/interfaces/modemServices
            -i ${LEGATO_ROOT}/interfaces/voiceCallService
            -i ${LEGATO_ROOT}/interfaces/audio
         DEPENDS
            legato
            voiceCallTest/Component.cdef
            voiceCallTest/voiceCallTest.c
            ${LEGATO_ROOT}/interfaces/modemServices/le_mdmDefs.api
    )

if(NOT ${LEGATO_TARGET} STREQUAL "localhost")
    add_test(${APP_TARGET} ${EXECUTABLE_OUTPUT_PATH}/${APP_TARGET})
    set_tests_properties(${APP_TARGET} PROPERTIES TIMEOUT 0)
endif()