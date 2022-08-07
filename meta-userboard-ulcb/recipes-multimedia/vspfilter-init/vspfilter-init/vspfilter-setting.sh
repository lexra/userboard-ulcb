#!/bin/sh

MATCH=0
file="/etc/gstvspfilter.conf"

[ ! -e $file ] && exit 0
current_input=`cat $file | head -n 1 | cut -d'=' -f 2`
current_output=`cat $file | tail -n 1 | cut -d'=' -f 2`

[ $(ls /dev/ | grep media | wc -l) -eq 0 ] && exit 0
for media_device in `ls /dev/media*`
do
	error=`media-ctl -d ${media_device} -e "fe9a0000.vsp rpf.0 input" | awk '{print $1}'`
	if [ $error != "Entity" ]
	then
		input=`media-ctl -d $media_device -e "fe9a0000.vsp rpf.0 input"`
		output=`media-ctl -d $media_device -e "fe9a0000.vsp wpf.0 output"`
		MATCH=1
		break
	fi
done
[ 0 -eq $MATCH ] && exit 0

sed -i "s|$current_input|$input|g" $file
sed -i "s|$current_output|$output|g" $file
