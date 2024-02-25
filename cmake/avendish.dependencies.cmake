include(FetchContent)
if(NOT TARGET fmtlib::fmt)
  FetchContent_Declare(
    fmt
    GIT_REPOSITORY "https://github.com/fmtlib/fmt"
    GIT_TAG master
    GIT_PROGRESS true
  )
  FetchContent_MakeAvailable(fmt)
endif()

if(NOT TARGET concurrentqueue)
  FetchContent_Declare(
    concurrentqueue
    GIT_REPOSITORY "https://github.com/jcelerier/concurrentqueue"
    GIT_TAG master
    GIT_PROGRESS true
  )
  FetchContent_MakeAvailable(concurrentqueue)
endif()

if(NOT TARGET nlohmann_json::nlohmann_json)
  FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY "https://github.com/nlohmann/json"
    GIT_TAG master
    GIT_PROGRESS true
  )
  FetchContent_MakeAvailable(nlohmann_json)
endif()

if(NOT TARGET pantor::inja)
  set(BUILD_TESTING 0)
  set(BUILD_BENCHMARK 0)
  set(INJA_USE_EMBEDDED_JSON 0)
  FetchContent_Declare(
    pantor_inja
    GIT_REPOSITORY "https://github.com/pantor/inja"
    GIT_TAG main
    GIT_PROGRESS true
  )
  FetchContent_MakeAvailable(pantor_inja)
endif()
