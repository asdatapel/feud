set(SOURCE_FILES 
    main.cpp
    AABB.cpp 
    BoundingBox.cpp
    Camera.cpp
    DebugDrawer.cpp
    Drawable.cpp
    GraphicsManager.cpp
    simplexnoise.cpp
    vec.cpp
    Mesh.cpp
    ShaderDefinition.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/include)

add_subdirectory(${PROJECT_SOURCE_DIR}/ext/nlohmann_json ${PROJECT_SOURCE_DIR}/ext/nlohmann_json)

include_directories(${PROJECT_SOURCE_DIR}/ext/glm)
include_directories(${PROJECT_SOURCE_DIR}/ext/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/ext/SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/ext/glew/lib)
link_directories(${PROJECT_SOURCE_DIR}/ext/SFML/lib)

add_executable(feud ${SOURCE_FILES})

find_package(OpenGL)

if (UNIX)
    target_link_libraries(feud ${OPENGL_LIBRARIES})
    target_link_libraries(feud GLEW.a)
    target_link_libraries(feud debug sfml-system-d.a optimized sfml-system.a)
    target_link_libraries(feud debug sfml-window-d.a optimized sfml-window.a)
    target_link_libraries(feud debug sfml-graphics-d.a optimized sfml-graphics.a)
    target_link_libraries(feud nlohmann_json::nlohmann_json)
endif()

if (WIN32)
    target_link_libraries(feud ${OPENGL_LIBRARIES})
    target_link_libraries(feud glew32s)
    target_link_libraries(feud debug sfml-system-d optimized sfml-system)
    target_link_libraries(feud debug sfml-window-d optimized sfml-window)
    target_link_libraries(feud debug sfml-graphics-d optimized sfml-graphics)
    target_link_libraries(feud nlohmann_json::nlohmann_json)
endif ()