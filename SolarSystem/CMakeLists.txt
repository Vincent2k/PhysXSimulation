# CMakeList.txt : projet CMake pour SolarSystem, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (SolarSystem "SolarSystemMain.cpp" "rendering/Rendering.cpp" "factory/SolarSystemFactory.h" "factory/SolarSystemFactory.cpp" "entities/SolarSystem.cpp" "entities/SolarSystem.h" "entities/Planet.h" "utils/vectorutils.h" "utils/vectorutils.cpp" "graphics/TextureProvider.h" "graphics/TextureProvider.cpp" "graphics/ModelProvider.h" "graphics/ModelProvider.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SolarSystem PROPERTY CXX_STANDARD 20)
endif()

add_library(raylib SHARED IMPORTED)
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION  "${RAYLIB_ROOT_DIR}/lib/raylib.dll"
    IMPORTED_IMPLIB   "${RAYLIB_ROOT_DIR}/lib/raylibdll.lib"
)


target_link_libraries(SolarSystem
    PhysX
    raylib
    Snippet
)

if (CMAKE_IMPORT_LIBRARY_SUFFIX)
  add_custom_command(
    TARGET SolarSystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:SolarSystem> $<TARGET_FILE_DIR:SolarSystem>
    COMMAND_EXPAND_LISTS
  )
endif ()