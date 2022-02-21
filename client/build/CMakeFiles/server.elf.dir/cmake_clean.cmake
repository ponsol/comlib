file(REMOVE_RECURSE
  "bootloader/bootloader.bin"
  "bootloader/bootloader.elf"
  "bootloader/bootloader.map"
  "config/sdkconfig.cmake"
  "config/sdkconfig.h"
  "flash_project_args"
  "project_elf_src_esp32.c"
  "server.bin"
  "server.map"
  "CMakeFiles/server.elf.dir/project_elf_src_esp32.c.obj"
  "CMakeFiles/server.elf.dir/project_elf_src_esp32.c.obj.d"
  "project_elf_src_esp32.c"
  "server.elf"
  "server.elf.pdb"
)

# Per-language clean rules from dependency scanning.
foreach(lang C)
  include(CMakeFiles/server.elf.dir/cmake_clean_${lang}.cmake OPTIONAL)
endforeach()
