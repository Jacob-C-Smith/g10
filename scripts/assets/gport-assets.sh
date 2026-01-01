# argument
ASSET_PATH=$1

# iterate through each blender file
for file in $ASSET_PATH/blender/*.blend; do

    BLENDER_FILE=$(
        printf "${file##*/}" |
        awk -F'.' '{print $1}'
    )
    BLENDER_PATH=$(printf "%s/blender/%s.blend" $ASSET_PATH $BLENDER_FILE)

    printf "file: %s\n" $BLENDER_FILE
    printf "path: %s\n" $BLENDER_PATH

    /Applications/Blender.app/Contents/MacOS/Blender \
        $BLENDER_PATH \
        -b \
        --addons gport \
        -P ./scripts/assets/gport.py

done