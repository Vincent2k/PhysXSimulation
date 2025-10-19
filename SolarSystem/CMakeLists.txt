# CMakeList.txt : projet CMake pour SolarSystem, incluez la source et définissez
# la logique spécifique au projet ici.
#

# Ajoutez une source à l'exécutable de ce projet.
add_executable (SolarSystem "SolarSystem.cpp" "SolarSystem.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET SolarSystem PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(SolarSystem
    PhysX
)

if (CMAKE_IMPORT_LIBRARY_SUFFIX)
  add_custom_command(
    TARGET SolarSystem POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:SolarSystem> $<TARGET_FILE_DIR:SolarSystem>
    COMMAND_EXPAND_LISTS
  )
endif ()