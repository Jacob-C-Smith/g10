# Ask for superuser up front
sudo ls > /dev/null

# Create the log file
rm logs/g10_server.txt;

# Kill any docker containers
sudo docker kill $(sudo docker ps -q) > /dev/null

# Build the docker image
sudo docker build -t g10_server . > logs/docker_build_output.txt 2> logs/docker_build_output.txt

# Start the server
sudo docker logs --details -f $(sudo docker run -it -d g10_server) >& "logs/g10_server.txt" &
