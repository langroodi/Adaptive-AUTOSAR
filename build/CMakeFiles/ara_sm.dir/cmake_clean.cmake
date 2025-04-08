file(REMOVE_RECURSE
  "bin/libara_sm.pdb"
  "lib/libara_sm.so"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/ara_sm.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
