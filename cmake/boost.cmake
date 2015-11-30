###---
### include the correct boost lib and include files
###
message("\tExternal: boost")

set(boost_inc_dir "${extern_root}/boost_1_56_0")
set(boost_lib_dir "${extern_root}/boost_1_56_0/vc13/lib")

set( Target_Inc ${Target_Inc} ${boost_inc_dir})
#add a new link directory for boosts auto-link feature
link_directories( ${boost_lib_dir} )