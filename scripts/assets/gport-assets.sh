# argument
ASSET_PATH=$1

# iterate through each blender file
for file in $ASSET_PATH/blender/*.blend; do

    BLENDER_FILE=$(
        printf "${file##*/}" |
        awk -F'.' '{print $1}'
    )

    printf "file: %s\n" $BLENDER_FILE

done