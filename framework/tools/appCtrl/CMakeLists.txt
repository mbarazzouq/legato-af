#*******************************************************************************
# Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
#
# Contributors:
#     Sierra Wireless - initial API and implementation
#*******************************************************************************

find_package(Legato REQUIRED)

mkexe(  appCtrl
            ../appCtrl
            -i "${PROJECT_SOURCE_DIR}/interfaces/supervisor"
            -i ../../c/src
            -v
        DEPENDS
            appCtrl.c
            Component.cdef
        )
