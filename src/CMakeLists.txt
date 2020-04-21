set(SOURCE_FILES 
    main.cpp
    AABB.cpp 
    Block.cpp
    BlockAir.cpp
    BlockDirt.cpp
    BlockGlowstoneLamp.cpp
    BlockGrass.cpp
    Blocks.cpp
    BlockStone.cpp
    BlockWater.cpp
    BoundingBox.cpp
    Camera.cpp
    Chunk.cpp
    DebugDrawer.cpp
    GraphicsManager.cpp
    Perlin.cpp
    Player.cpp
    simplexnoise.cpp
    vec.cpp
    World.cpp
    PhysicsObj.cpp
    Box.cpp
    Mesh.cpp
    Region.cpp
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