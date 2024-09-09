include(FetchContent)

FetchContent_Declare(
  mongo-c-driver
  GIT_REPOSITORY https://github.com/mongodb/mongo-c-driver.git
  GIT_TAG        1.27.6  
)

set(ENABLE_AUTOMATIC_INIT_AND_CLEANUP OFF CACHE BOOL "" FORCE)
set(ENABLE_MONGOC OFF CACHE BOOL "" FORCE)
set(ENABLE_BSON ON CACHE BOOL "" FORCE)
set(ENABLE_STATIC ON CACHE BOOL "" FORCE)
set(ENABLE_SHARED OFF CACHE BOOL "" FORCE)
set(ENABLE_TESTS OFF CACHE BOOL "" FORCE)
set(ENABLE_EXAMPLES OFF CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(mongo-c-driver)

# Get properties and check if mongo-c-driver is populated
FetchContent_GetProperties(mongo-c-driver)
if(NOT mongo-c-driver_POPULATED)
  FetchContent_Populate(mongo-c-driver)
  add_subdirectory(${mongo-c-driver_SOURCE_DIR} ${mongo-c-driver_BINARY_DIR})
endif()
