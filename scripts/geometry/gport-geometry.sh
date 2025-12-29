# argument
ASSET_NAME=$1

# export the file
/Applications/Blender.app/Contents/MacOS/Blender \
assets/blender/$ASSET_NAME.blend \
-b \
--addons gport \
-P ./scripts/geometry/gport-geometry.py
