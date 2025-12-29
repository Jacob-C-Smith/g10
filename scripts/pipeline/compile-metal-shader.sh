# argument
PIPELINE_NAME=$1

# compile the vertex shader
xcrun metal -c assets/pipeline/$PIPELINE_NAME/vert.metal -o assets/pipeline/$PIPELINE_NAME/vert.air

# compile the fragment shader
xcrun metal -c assets/pipeline/$PIPELINE_NAME/frag.metal -o assets/pipeline/$PIPELINE_NAME/frag.air

# link the pipeline
xcrun metallib assets/pipeline/$PIPELINE_NAME/vert.air assets/pipeline/$PIPELINE_NAME/frag.air -o assets/pipeline/$PIPELINE_NAME/$PIPELINE_NAME.metallib

printf "[gport] [pipeline] %s\n" $PIPELINE_NAME