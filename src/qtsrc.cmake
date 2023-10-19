# Рекурсивно добавляем исходные файлы в проект
function(add_qtsources_recursive dir)
    file(GLOB_RECURSE qtsources ${dir}/*.qrc ${dir}/*.cpp ${dir}/*.c ${dir}/*.ui)
    list(APPEND all_qtsources ${qtsources})

    set(QTSOURCES ${QTSOURCES} ${qtsources} PARENT_SCOPE)

    foreach(subdir IN LISTS ARGV)
        if(IS_DIRECTORY ${dir}/${subdir})
            add_qtsources_recursive(${dir}/${subdir})
        endif()
    endforeach()
endfunction()

# Добавляем исходные файлы в проект
set(QTSOURCES "")
add_qtsources_recursive(${CMAKE_CURRENT_SOURCE_DIR}/src/APS/GUI)
