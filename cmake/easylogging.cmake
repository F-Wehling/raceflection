##
## Easylogging++
## Load
##
message("\tExternal: easylogging++")
set(easylogging_dir "${extern_root}/easylogging++")

set( easylogging_inc_dir "${easylogging_dir}/include")

#include_directories(${easylogging_inc_dir})
set( Target_Inc ${Target_Inc} ${easylogging_inc_dir})
