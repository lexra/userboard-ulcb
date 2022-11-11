SRC_URI_append = " \
	https://download.kodi.tv/demo-files/BBB/bbb_sunflower_1080p_30fps_normal.mp4;name=video6 \
	http://www.peach.themazzone.com/durian/movies/sintel-1280-stereo.mp4;name=video7 \
	https://s3.amazonaws.com/senkorasic.com/test-media/video/caminandes-llamigos/caminandes_llamigos_720p.mp4;name=video8 \
	https://github.com/andreasbotsikas/DemoVideos/raw/master/elephants_dream.mp4;name=video9 \
	https://github.com/andreasbotsikas/DemoVideos/raw/master/tears_of_steel.mp4;name=video10 \
	http://download4.dvdloc8.com/trailers/divxdigest/fantastic_four_rise_of_the_silver_surfer-trailer.zip;name=video11 \
"
SRC_URI[video6.md5sum] = "babbe51b47146187a66632daa791484b"
SRC_URI[video6.sha256sum] = "ae51005850b0ff757fe60c3dd7a12d754d3cd2397d87d939b55235e457f97658"
SRC_URI[video7.md5sum] = "a452b16a02fd18f15a480956496ad063"
SRC_URI[video7.sha256sum] = "5f662cb1e8a3f1f7584c40410612347eb48b45b04908033bae9b8ffd9573490c"
SRC_URI[video8.md5sum] = "1fd6da4fe94ded49e59392b8b4299d1e"
SRC_URI[video8.sha256sum] = "346cfbb637cca8c59dd0ee1b1b8a3b4f81691a6c5900a8114ac9f15e229b1ba5"
SRC_URI[video9.md5sum] = "c8082e075790e4081c92517306a46458"
SRC_URI[video9.sha256sum] = "9e815b3c8518bd882011ee47b571d56a106ecdd82bd7145902aefd4129da59b7"
SRC_URI[video10.md5sum] = "dbb183bd1f4daed546b5c9c5d9f20972"
SRC_URI[video10.sha256sum] = "90090f8e3c2b7a210b1e21c6480a9eab0f7b78cda4509a38259a89e907260a12"
SRC_URI[video11.md5sum] = "7d1da4d27d0a585075560199c69378a5"
SRC_URI[video11.sha256sum] = "7405599f03ab7b9d1fb0fb29209c6b13aec2f16c1bf04e66084d52267ed7d16b"

do_install_append () {
	install -d ${D}/home/root/videos
	/usr/bin/ffmpeg -i ${D}/home/root/videos/h264-bl10-fhd-30p-5m-aac-lc-stereo-124k-48000x264.mp4 -an -vcodec copy ${D}/home/root/videos/h264-bl10-fhd-30p-5m-aac-lc-stereo-124k-48000x264_new.mp4
	mv ${D}/home/root/videos/h264-bl10-fhd-30p-5m-aac-lc-stereo-124k-48000x264_new.mp4 ${D}/home/root/videos/h264-bl10-fhd-30p-5m-aac-lc-stereo-124k-48000x264.mp4

	install -d ${D}/home/root/videos/bbb
	cp -RPfv ${WORKDIR}/bbb_sunflower_1080p_30fps_normal.mp4 ${D}/home/root/videos/bbb
	/usr/bin/ffmpeg -i ${D}/home/root/videos/bbb/bbb_sunflower_1080p_30fps_normal.mp4 -an -vcodec copy ${D}/home/root/videos/bbb/bbb_sunflower_1080p_30fps_normal_new.mp4
	mv -fv ${D}/home/root/videos/bbb/bbb_sunflower_1080p_30fps_normal_new.mp4 ${D}/home/root/videos/bbb/bbb_sunflower_1080p_30fps_normal.mp4

	install -d ${D}/home/root/videos/sintel
	cp -Rpfv ${WORKDIR}/sintel-1280-stereo.mp4 ${D}/home/root/videos/sintel/sintel-1280-stereo.mp4
	/usr/bin/ffmpeg -i ${D}/home/root/videos/sintel/sintel-1280-stereo.mp4 -an -vcodec copy ${D}/home/root/videos/sintel/sintel-1280-stereo_new.mp4
	mv -fv ${D}/home/root/videos/sintel/sintel-1280-stereo_new.mp4 ${D}/home/root/videos/sintel/sintel-1280-stereo.mp4

	#install -d ${D}/home/root/videos/caminandes
	#cp -RPfv ${WORKDIR}/caminandes_llamigos_720p.mp4 ${D}/home/root/videos/caminandes
	#/usr/bin/ffmpeg -i ${D}/home/root/videos/caminandes/caminandes_llamigos_720p.mp4 -an -vcodec copy ${D}/home/root/videos/caminandes/caminandes_llamigos_720p_new.mp4
	#mv -fv ${D}/home/root/videos/caminandes/caminandes_llamigos_720p_new.mp4 ${D}/home/root/videos/caminandes/caminandes_llamigos_720p.mp4

	install -d ${D}/home/root/videos/elephantsdream
	cp -RPfv ${WORKDIR}/elephants_dream.mp4 ${D}/home/root/videos/elephantsdream/elephants-dream.mp4
	/usr/bin/ffmpeg -i ${D}/home/root/videos/elephantsdream/elephants-dream.mp4 -an -vcodec copy ${D}/home/root/videos/elephantsdream/elephants-dream_new.mp4
	mv -fv ${D}/home/root/videos/elephantsdream/elephants-dream_new.mp4 ${D}/home/root/videos/elephantsdream/elephants-dream.mp4
    
	install -d ${D}/home/root/videos/fantastic4
	cp -RPfv ${WORKDIR}/'Fantastic Four - Rise of the Silver Surfer - Trailer.mp4' ${D}/home/root/videos/fantastic4/silver-surfer.mp4
	/usr/bin/ffmpeg -i ${D}/home/root/videos/fantastic4/silver-surfer.mp4 -an -vcodec copy ${D}/home/root/videos/fantastic4/silver-surfer_new.mp4
	mv -fv ${D}/home/root/videos/fantastic4/silver-surfer_new.mp4 ${D}/home/root/videos/fantastic4/silver-surfer.mp4

	cd ${D}/home/root/videos/bbb
	ln -sf ../sintel/sintel-1280-stereo.mp4 .
	ln -sf ../fantastic4/silver-surfer.mp4 .
	ln -sf ../elephantsdream/elephants-dream.mp4 .
	cd -
}
