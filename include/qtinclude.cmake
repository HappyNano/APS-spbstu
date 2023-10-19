# Рекурсивно добавляем исходные файлы в проект
function(add_qtinclude_recursive dir)
    file(GLOB_RECURSE qtinclude ${dir}/*.hpp ${dir}/*.h)
    list(APPEND all_qtinclude ${qtinclude})

    set(QTINCLUDE ${QTINCLUDE} ${qtinclude} PARENT_SCOPE)

    foreach(subdir IN LISTS ARGV)
        if(IS_DIRECTORY ${dir}/${subdir})
            add_qtinclude_recursive(${dir}/${subdir})
        endif()
    endforeach()
endfunction()

# Добавляем исходные файлы в проект
set(QTINCLUDE "")
add_qtinclude_recursive(${CMAKE_CURRENT_SOURCE_DIR}/include/APS/GUI)
