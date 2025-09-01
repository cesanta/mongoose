#!/usr/bin/env -S gawk -f
# gawk used to avoid "towc" errors seen on MacOS. env used to circumvent brew installing it wherever they like

BEGIN {
	FS="\t"
	print "{"
}
/HEALTH_DASHBOARD/ { print $2 }
END {
	print "}"
}
