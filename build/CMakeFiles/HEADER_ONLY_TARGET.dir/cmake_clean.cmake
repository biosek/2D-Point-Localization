FILE(REMOVE_RECURSE
  "libHEADER_ONLY_TARGET.pdb"
  "libHEADER_ONLY_TARGET.a"
)

# Per-language clean rules from dependency scanning.
FOREACH(lang)
  INCLUDE(CMakeFiles/HEADER_ONLY_TARGET.dir/cmake_clean_${lang}.cmake OPTIONAL)
ENDFOREACH(lang)
