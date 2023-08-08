#!/usr/bin/perl
use CGI;

my $cgi = CGI->new();
my %param = map { $_ => scalar $cgi->param($_) } $cgi->param();

print $cgi->header;
# print qq{PARAM:\n};
for my $k ( sort keys %param ) {
	print "<p>";
	print join ": ", $k, $param{$k};
	print "</p>\n"
}

print << 'END';
<p> Hello World </p>
END
