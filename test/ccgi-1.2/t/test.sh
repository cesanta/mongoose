#! /bin/sh

PATH=/bin:/usr/bin

# Regression tests for the CGI library
# Usage: ./test.sh [ -r ]
# The -r option causes the script to edit itself and renumber
# the tests in case any are added or removed.

# renumber the tests in this script

renumber() {
    cp $0 $0.old &&
    awk '/^TEST=[0-9].* ######/ {
        printf("TEST=%d  ########################################\n", ++i);
        next }
        { print }' $0.old > $0 &&
    /bin/rm $0.old
    exit
}

check () {
    echo
    if diff -c expected result; then
        echo "Test $TEST success"
    else
        echo "Test $TEST failure"
    fi
}

test "X$1" = X-r && renumber

TEST=1  ########################################

# Testing CGI_get_query

Q='one=This+is+a+test&two=%22Bob+Smith%22+%3Cbob%40gmail.com%3E&'
Q=$Q'varnovalue&eq=un=encoded=eq==&one=second+value&the+end'

QUERY_STRING=$Q ./test CGI_get_query > result 2>&1

cat > expected <<'E-O-F'
one [0] >>This is a test<<
one [1] >>second value<<
two [0] >>"Bob Smith" <bob@gmail.com><<
varnovalue [0] >><<
eq [0] >>un=encoded=eq==<<
the end [0] >><<
E-O-F

check

TEST=2  ########################################

# Testing CGI_get_post

CONTENT_TYPE=application/x-www-form-urlencoded \
CONTENT_LENGTH=60 \
./test CGI_get_post > result 2>&1 <<'E-O-F'
one=This+is+a+test&two=%22Bob+Smith%22+%3Cbob%40gmail.com%3E
E-O-F

cat > expected <<'E-O-F'
one [0] >>This is a test<<
two [0] >>"Bob Smith" <bob@gmail.com><<
E-O-F

check

TEST=3  ########################################

# Testing CGI_get_post  multipart/form-data

CONTENT_TYPE='multipart/form-data; boundary=----12345' \
./test CGI_get_post > result 2>&1 <<'E-O-F'
------12345
Content-Disposition: form-data; name="textvar1"


------12345
Content-Disposition: form-data; name="doublevar"

Text data here
------12345
Content-Disposition: form-data; name="cbvar1"

cb #1
------12345
Content-Disposition: form-data; name="cbvar1"

cb #3
------12345
Content-Disposition: form-data; name="rbvar1"

rb #3
------12345
Content-Disposition: form-data; name="selvar1"

selval1
------12345
Content-Disposition: form-data; name="selvar1"

selval3
------12345
Content-Disposition: form-data; name="selvar1"

selval4
------12345
Content-Disposition: form-data; name="selvar1"

selval7
------12345
Content-Disposition: form-data; name="textarea1"

Initial Line 1
Initial Line 2
Added Line
Another Line
------12345
Content-Disposition: form-data; name="doublevar"

OK
------12345
Content-Disposition: form-data; name="hidden"

This is three
lines of hidden
form data
------12345--
E-O-F

cat > expected <<'E-O-F'
textvar1 [0] >><<
doublevar [0] >>Text data here<<
doublevar [1] >>OK<<
cbvar1 [0] >>cb #1<<
cbvar1 [1] >>cb #3<<
rbvar1 [0] >>rb #3<<
selvar1 [0] >>selval1<<
selvar1 [1] >>selval3<<
selvar1 [2] >>selval4<<
selvar1 [3] >>selval7<<
textarea1 [0] >>Initial Line 1
Initial Line 2
Added Line
Another Line<<
hidden [0] >>This is three
lines of hidden
form data<<
E-O-F

check

TEST=4  ########################################

# Testing CGI_get_cookie

HTTP_COOKIE='cookie1=cookie1data; cookie2=cookie2data; cookie3=more-stuff;' \
./test CGI_get_cookie > result 2>&1

cat > expected <<'E-O-F'
cookie1 [0] >>cookie1data<<
cookie2 [0] >>cookie2data<<
cookie3 [0] >>more-stuff<<
E-O-F

check

TEST=5  ########################################

# Testing upload

/bin/rm -f cgi-upload-??????
CONTENT_TYPE='multipart/form-data; boundary=----12345' \
./test upload > result 2> result <<'E-O-F'
------12345
Content-Disposition: form-data; name="var1"

Value # 1
------12345
Content-Disposition: form-data; name="upload"; filename="input"

This is a test uploaded file
Here is the second line.
The quick brown fox jumped over the lazy dog
(or was that a hog?)
Now is the time for
all good men
to come to the aid of
their party!

------12345--
E-O-F

F=` echo ./cgi-upload-?????? `

cat $F >> result

/bin/rm -f $F

cat > expected << E-O-F
var1 [0] >>Value # 1<<
upload [0] >>$F<<
upload [1] >>input<<
This is a test uploaded file
Here is the second line.
The quick brown fox jumped over the lazy dog
(or was that a hog?)
Now is the time for
all good men
to come to the aid of
their party!
E-O-F

check

TEST=6  ########################################

# Testing CGI_add_var

./test CGI_add_var one 'first value' two 'value # 2' \
    three '3rd-var' two 200% > result 2>&1

cat > expected <<'E-O-F'
one [0] >>first value<<
two [0] >>value # 2<<
two [1] >>200%<<
three [0] >>3rd-var<<
E-O-F

check

TEST=7  ########################################

# Testing CGI_encode_varlist

./test CGI_encode_varlist one 'first value' two 'value # 2' \
    three '3rd-var' two 200% > result 2>&1

cat > expected <<'E-O-F'
one [0] >>first value<<
two [0] >>value # 2<<
two [1] >>200%<<
three [0] >>3rd-var<<
one=first+value&two=value+%23+2&two=200%25&three=3rd-var
E-O-F

check

TEST=8  ########################################

# Testing CGI_encode_query

./test CGI_encode_query one 'first value' two 'value # 2' \
    three '3rd-var' two 200% > result 2>&1

cat > expected <<'E-O-F'
one=first+value&two=value+%23+2&three=3rd-var&two=200%25
one [0] >>first value<<
two [0] >>value # 2<<
two [1] >>200%<<
three [0] >>3rd-var<<
E-O-F

check

TEST=9  ########################################

# Testing CGI_encode_url

./test CGI_encode_url '/etc/passwd holds Unix(TM) accounts.' > result 2>&1

cat > expected <<'E-O-F'
%2Fetc%2Fpasswd+holds+Unix%28TM%29+accounts.
/etc/passwd holds Unix(TM) accounts.
E-O-F

check


TEST=10  ########################################

# Testing CGI_encode_entity

./test CGI_encode_entity \
'if (i > 20 && i < 200) {
  fputs("OK\n", stdout);
}' > result 2>&1
cat > expected <<'E-O-F'
if (i &gt; 20 &amp;&amp; i &lt; 200) {&#10;  fputs(&quot;OK\n&quot;, stdout);&#10;}
E-O-F

check

TEST=11  ########################################

# Testing CGI_encode_base64

./test CGI_encode_base64 'Now is the time for the quick brown fox, ...' \
    > result 2>&1
cat > expected <<'E-O-F'
Tm93IGlzIHRoZSB0aW1lIGZvciB0aGUgcXVpY2sgYnJvd24gZm94LCAuLi4=
Now is the time for the quick brown fox, ...
E-O-F

check

TEST=12  ########################################

# Testing CGI_encode_hex

./test CGI_encode_hex 'Now is the time for the quick brown fox, ...' \
    > result 2>&1
cat > expected <<'E-O-F'
4E6F77206973207468652074696D6520666F722074686520717569636B2062726F776E20666F782C202E2E2E
Now is the time for the quick brown fox, ...
E-O-F

check

TEST=13  ########################################

# Testing CGI_encrypt

./test CGI_encrypt 'Now is the time for the quick brown fox, ...' \
    > result 2>&1
cat > expected <<'E-O-F'
enc len = 120
dec len = 44 Now is the time for the quick brown fox, ...
E-O-F

check


# clean up

/bin/rm -f result expected cgi-upload-??????
