# Download md4c
FetchContent_Declare(
    md4c
    PREFIX "external/md4c"
    GIT_REPOSITORY "https://github.com/mity/md4c.git"
    GIT_TAG master
    TIMEOUT 10
)

FetchContent_MakeAvailable(md4c)

FetchContent_GetProperties(md4c
	SOURCE_DIR MD4C_SOURCE_DIR
)
set(MD4C_INCLUDE_DIR ${SOURCE_DIR}/src)