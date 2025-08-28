#!/usr/bin/awk -f
BEGIN {
	FS="\t"
	print "{"
}
/HEALTH_DASHBOARD/ { print $2 }
END {
	print "}"
}
