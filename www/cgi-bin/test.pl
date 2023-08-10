#!/usr/bin/perl
use CGI;

my $cgi = CGI->new();

print $cgi->header;

print << 'END';
Hello World
END
