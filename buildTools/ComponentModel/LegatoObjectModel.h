//--------------------------------------------------------------------------------------------------
/**
 *  @file LegatoObjectModel.h
 *
 * Core header of the Component Model.
 *
 * Client code need only include this file to get all object model definitions.
 *
 * The object model looks like this: (note: '*' = "multiple")
 *
 * @verbatim

System --+--> Binds List --*--> User-to-User Bind
         |
         +--> App List --*--+
                            |
 +--------------------------+
 |
 V
App --+--> File Imports List --*---------+
      |                                  +---------------------------------------------------+
      +--> Included Files List --*-------+                                                   |
      |                                                                      Api <-+         |
      +--> Component --*--> Component --+--> Sources List                          |         |
      |    List                 ^       |                                Library <-+         |
      |                         |       +--> Library List                          |         |
      |                         |       |                                          |         |
      |                         |       +--> Imported Interfaces Map --*--+        |         |
      |                         |       |                                 +--> Interface <---|--+
      |                         |       +--> Exported Interfaces Map --*--+                  |  |
      |                         |       |                                                    |  |
      |                         |       +--> Included Files List --*--+                      |  |
      |                         |       |                             +--> File Mapping <----+  |
      |                         |       +--> File Imports List --*----+                         |
      |                         |       |                                                       |
      |                         |       +--> Pools Map (name, pool size)                        |
      |                         |       |                                                       |
      |                         |       +--> Config Item Map (path, value)                      |
      |                         |                                                               |
      |                         +---------------------------+                                   |
      |                                                     |                                   |
      +--> Executables --*--> Executable                    |                                   |
      |    List                 ^  |                        |                                   |
      |                         |  |    Component           |                                   |
      |                         |  +--> Instance --*--> Component                               |
      |                         |        List           Instance                                |
      |                         |                           |                                   |
      |                         |                           +--> Imported Interfaces Map --*----+
      |                         |                           |                                   |
      |                         |                           +--> Exported Interfaces Map --*----+
      |                         |
      |                         +---------------------------------------------------------+
      |                                                                                   |
      |                                                                                   |
      +--> Process Environment List --*--> Process Environment --> Process List --*--> Process
      |
      +--> Internal API Binds List --*--> Internal API Bind
      |
      +--> External API Binds List --*--> Exe-to-User API Bind

@endverbatim
 *
 *  Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
 */
//--------------------------------------------------------------------------------------------------

#ifndef LEGATO_OBJECT_MODEL_INCLUDE_GUARD
#define LEGATO_OBJECT_MODEL_INCLUDE_GUARD


#include <stdexcept>
#include <string>
#include <list>
#include <map>
#include <set>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Exception.h"  // Basic string exception type.
#include "Limit.h"      // Classes for various limits settings.
#include "Library.h"
#include "Api.h"
#include "Permissions.h"
#include "ConfigItem.h"
#include "MemoryPool.h"
#include "FilePath.h"    // Helper functions related to file paths.
#include "FileMapping.h"
#include "Interface.h"
#include "IpcBinding.h"
#include "Component.h"
#include "ComponentInstance.h"
#include "Executable.h"
#include "Process.h"
#include "ProcessEnvironment.h"
#include "App.h"
#include "ProgrammingLanguage.h"  // Types and functions related to programming languages.
#include "System.h"
#include "BuildParams.h"    // Build parameters collected from the command line.


#endif  // LEGATO_OBJECT_MODEL_INCLUDE_GUARD
