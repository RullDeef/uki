rm -rf include/uki.h

mkdir -p include
touch include/uki.h
cat headers/begin.h >> include/uki.h
cat headers/uki_defines.h >> include/uki.h
cat headers/uki_utils.h >> include/uki.h
cat headers/uki_input.h >> include/uki.h
cat headers/uki_table.h >> include/uki.h
cat headers/uki_menu.h >> include/uki.h
cat headers/end.h >> include/uki.h

sed "s/#include \".*\"//" -i include/uki.h
