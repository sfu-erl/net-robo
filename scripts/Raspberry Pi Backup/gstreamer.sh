raspivid -t 0 -vf -hf -h 720 -w 1280 -fps 25 -b 2000000 -o - | gst-launch-1.0 -v fdsrc ! h264parse ! flvmux ! rtmpsink location=\"rtmp://192.168.0.104/live/test live=1\"
