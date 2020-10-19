# Configure FairVersion.h
 # ------------------------------

 Find_Package(Git)

 If(GIT_FOUND AND EXISTS "${SOURCE_DIR}/.git")
   Execute_Process(COMMAND ${GIT_EXECUTABLE} describe
                   OUTPUT_VARIABLE ENSARROOT_GIT_VERSION
                   OUTPUT_STRIP_TRAILING_WHITESPACE
                   WORKING_DIRECTORY ${SOURCE_DIR}
                  )
   Execute_Process(COMMAND ${GIT_EXECUTABLE} log -1 --format=%cd
                   OUTPUT_VARIABLE ENSARROOT_GIT_DATE
                   OUTPUT_STRIP_TRAILING_WHITESPACE
                   WORKING_DIRECTORY ${SOURCE_DIR}
                  )
   Message(STATUS "ENSARRoot Version - ${ENSARROOT_GIT_VERSION} from - ${ENSARROOT_GIT_DATE}")
   Configure_File(${SOURCE_DIR}/cmake/scripts/ENSARRootVersion.h.tmp ${BINARY_DIR}/ENSARRootVersion.h @ONLY)

 Else()
   Configure_File(${SOURCE_DIR}/cmake/scripts/ENSARRootVersion.h.default ${BINARY_DIR}/ENSARRootVersion.h COPYONLY)
 EndIf()
