cmake_minimum_required(VERSION 3.16)

project(qproc LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_BINDIR "bin")
set(CMAKE_INSTALL_LIBDIR "lib")
set(CMAKE_DEBUG_POSTFIX "d")

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

add_executable(qproc_sync
    main_sync.cpp
)
add_executable(qproc_async
    main_async.cpp
)
add_executable(qproc_async2
    main_async2.cpp
)
add_executable(runme
    runme.cpp
)
target_link_libraries(qproc_sync Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(qproc_async Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(qproc_async2 Qt${QT_VERSION_MAJOR}::Core)
set_target_properties( qproc_sync qproc_async qproc_async2 runme
    PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}" )
if ( UNIX AND NOT APPLE )
    set_target_properties( qproc_sync qproc_async qproc_async2
        PROPERTIES INSTALL_RPATH "\${ORIGIN}/../lib" )
    include(GNUInstallDirs)
endif()
install(TARGETS qproc_sync
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(TARGETS qproc_async
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(TARGETS qproc_async2
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(TARGETS runme
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install( IMPORTED_RUNTIME_ARTIFACTS Qt${QT_VERSION_MAJOR}::Core
         LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
         RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
if ( UNIX AND NOT APPLE )
    install( DIRECTORY "$<TARGET_FILE_DIR:Qt${QT_VERSION_MAJOR}::Core>"
            DESTINATION ${CMAKE_INSTALL_PREFIX}
            FILES_MATCHING
            PATTERN "*libicui18n.so*"
            PATTERN "*libicuuc.so*"
            PATTERN "*libicudata.so*"
            PATTERN "pkgconfig" EXCLUDE
            PATTERN "cmake" EXCLUDE
            PATTERN "objects*" EXCLUDE )
endif()
