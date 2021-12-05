file(REMOVE_RECURSE
  "libara_sm.a"
  "libara_sm.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang )
  include(CMakeFiles/ara_sm.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
