
find_package(Boost REQUIRED COMPONENTS system filesystem )

# Check if Boost was found
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    
    # Add your executable
    add_executable(prototype prototype.cc)
    # add_executable(MyExecutable main.cpp) # Replace with your source files
    
    # Link Boost libraries (shared)
    target_link_libraries(prototype Boost::system Boost::filesystem)
endif()