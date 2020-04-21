set(SOURCE_FILES 
    main.cpp
    AABB.cpp 
    BoundingBox.cpp
    Camera.cpp
    DebugDrawer.cpp
    GraphicsManager.cpp
    simplexnoise.cpp
    vec.cpp
    Mesh.cpp
    ShaderDefinition.cpp
)

include_directories(${PROJECT_SOURCE_DIR}/include)

include_directories(${PROJECT_SOURCE_DIR}/ext/glm)
include_directories(${PROJECT_SOURCE_DIR}/ext/glew/include)
include_directories(${PROJECT_SOURCE_DIR}/ext/SFML/include)

link_directories(${PROJECT_SOURCE_DIR}/ext/glew/lib)
link_directories(${PROJECT_SOURCE_DIR}/ext/SFML/lib)

add_executable(feud ${SOURCE_FILES})

find_package(OpenGL)

target_link_libraries(feud ${OPENGL_LIBRARIES})
target_link_libraries(feud GLEW.a)
target_link_libraries(feud sfml-system.a)
target_link_libraries(feud sfml-window.a)
target_link_libraries(feud sfml-graphics.a)


find_package(nlohmann_json 3.2.0 REQUIRED)
target_link_libraries(feud nlohmann_json::nlohmann_json)