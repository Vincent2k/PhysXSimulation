# CMakeList.txt : fichier projet CMake de niveau supérieur, effectuez une configuration globale
# et incluez les sous-projets ici.
#
cmake_minimum_required (VERSION 3.8)

option(PHYSX_BUILD_TYPE "The build type of PhysX, i.e., one of {debug, checked, profile, release}" "checked")

include_directories(${PHYSX_ROOT_DIR}/include)

# Activez Rechargement à chaud pour les compilateurs MSVC si cela est pris en charge.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("PhysxSimulation")

function(add_physx_lib target buildType fileName)
    set_target_properties(${target} PROPERTIES
        IMPORTED_LOCATION "${PHYSX_ROOT_DIR}/bin/win.x86_64.vc143.md/${buildType}/${fileName}.dll"
        IMPORTED_IMPLIB   "${PHYSX_ROOT_DIR}/bin/win.x86_64.vc143.md/${buildType}/${fileName}.lib"
    )
endfunction()

function(add_static_physx_lib target buildType fileName)
    set_target_properties(${target} PROPERTIES
        IMPORTED_IMPLIB   "${PHYSX_ROOT_DIR}/bin/win.x86_64.vc143.md/${buildType}/${fileName}.lib"
    )
endfunction()

add_library(physx_core SHARED IMPORTED)
add_library(physx_foundation SHARED IMPORTED)
add_library(physx_extensions SHARED IMPORTED)
add_library(physx_pvd_sdk SHARED IMPORTED)
add_library(physx_common SHARED IMPORTED)

if(CMAKE_BUILD_TYPE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    message("Building snippet in debug configuration")
    add_compile_definitions(_DEBUG)

    add_physx_lib(physx_core debug PhysX_64)
    add_physx_lib(physx_foundation debug PhysXFoundation_64)
    add_static_physx_lib(physx_extensions debug PhysXExtensions_static_64)
    add_static_physx_lib(physx_pvd_sdk debug PhysXPvdSDK_static_64)
    add_physx_lib(physx_common debug PhysXCommon_64)
else()
    message("Building snippet in release configuration with PhysX ${PHYSX_BUILD_TYPE} configuration")
    add_compile_definitions(NDEBUG)

    add_physx_lib(physx_core release PhysX_64)
    add_physx_lib(physx_foundation release PhysXFoundation_64)
    add_static_physx_lib(physx_extensions release PhysXExtensions_static_64)
    add_static_physx_lib(physx_pvd_sdk release PhysXPvdSDK_static_64)
    add_physx_lib(physx_common release PhysXCommon_64)

endif()

add_library(PhysX INTERFACE IMPORTED)
target_link_libraries(PhysX INTERFACE
    physx_core
    physx_foundation
    physx_extensions
    physx_pvd_sdk
    physx_common
)

# Incluez les sous-projets.
add_subdirectory ("SolarSystem")
