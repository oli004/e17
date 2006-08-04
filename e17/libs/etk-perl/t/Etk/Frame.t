use Test::More 'no_plan'; # tests => 1;
use Etk;

my $b = Etk::Frame->new("frame");

ok( defined $b, 	"Frame new()");
ok( $b->isa("Etk::Frame"),	"Class Check");

is($b->LabelGet(), "frame", 	"Label get");

$b->LabelSet("moo");
is($b->LabelGet(), "moo", 	"Label set");

