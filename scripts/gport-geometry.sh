# argument
ASSET_NAME=$1

# export the file
/Applications/Blender.app/Contents/MacOS/Blender assets/blender/$ASSET_NAME.blend -b -P ./scripts/gport-geometry.py
