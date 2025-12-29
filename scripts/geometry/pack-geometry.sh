# argument
GEOMETRY_NAME=$1

# pack geometry information
cat assets/input/geometry/$GEOMETRY_NAME.json |
grep -v '\-\-\-' | 
tr '\n' ' ' |
tr -d '[:space:]' |
head -n 1 > assets/geometry/$GEOMETRY_NAME.json
