# argument
GEOMETRY_NAME=$1

# programs
JSON_PACK=./gsdk/build/examples/json_example

# pack geometry information
$JSON_PACK assets/geometry/$GEOMETRY_NAME.ply.json |
grep -v '\-\-\-' | 
head -n 1 > assets/geometry/$GEOMETRY_NAME.json