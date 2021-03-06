#--------------------------------------------------------------------------------------------------
# Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
#--------------------------------------------------------------------------------------------------

# Build host unit tests.

set(LEGATO_BIN_DIR          ${LEGATO_ROOT}/build/${LEGATO_TARGET}/bin)

set(SERVICE_DIRECTORY_BIN   ${LEGATO_BIN_DIR}/serviceDirectory)
set(LOG_CTRL_DAEMON_BIN     ${LEGATO_BIN_DIR}/logCtrlDaemon)
set(CONFIG_TREE_BIN         ${LEGATO_BIN_DIR}/configTree)
set(CONFIG_TOOL_BIN         ${LEGATO_BIN_DIR}/config)


configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/configTest.sh.in
    ${EXECUTABLE_OUTPUT_PATH}/configTest.sh
    @ONLY
)


mkexe(configDropReadExe
      configDropRead
      DEPENDS legato
              ${LEGATO_ROOT}/interfaces/le_cfg.api
              configDropRead/configDropRead.c)

mkexe(configDropWriteExe
      configDropWrite
      DEPENDS legato
              ${LEGATO_ROOT}/interfaces/le_cfg.api
              configDropWrite/configDropWrite.c)


mkexe(configTestExe
      configTest
      -i ${LEGATO_ROOT}/interfaces
      DEPENDS legato
              ${LEGATO_ROOT}/interfaces/le_cfg.api
              ${LEGATO_ROOT}/interfaces/le_cfgAdmin.api
              configTest/configTest.c)


mkexe(configDelete
      configDelete
      -i ${LEGATO_ROOT}/interfaces
      DEPENDS legato
              ${LEGATO_ROOT}/interfaces/le_cfg.api
              ${LEGATO_ROOT}/interfaces/le_cfgAdmin.api
              configDelete/configDelete.c)



add_test(configTest ${EXECUTABLE_OUTPUT_PATH}/configTest.sh)


# On-target test apps.

mkapp(cfgSelfRead.adef)
mkapp(cfgSelfWrite.adef)
mkapp(cfgSystemRead.adef)
mkapp(cfgSystemWrite.adef)
