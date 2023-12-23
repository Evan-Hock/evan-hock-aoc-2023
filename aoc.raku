#!/usr/bin/env raku


sub MAIN($lang, $day, $part) {
    # language-specific build options
    my $COMPILER;
    my %OPTIONS;
    my $EXTENSION = $lang;
    my $OUTPUT-SPECIFIER = '-o';
    # my $DEBUGGER;
    # my $DEBUG-SPECIFIER = '-g';

    # apply language config if it exists
    try EVALFILE "$*SPEC.curdir()/$lang/cfg.raku";

    my $solution-dir = "$*SPEC.curdir()/$lang/$day/$part";

    # get solution-specific config if it exists
    try EVALFILE "$solution-dir/cfg.raku";

    my $solution = $solution-dir.IO.dir(test => /.* ".$EXTENSION" $/).head;
    my $input = "$*SPEC.curdir()/input/$day.txt";

    # assume interpreted language if no compiler is specified by config
    without $COMPILER {
        run-aoc-program $input, $solution;
        return;
    }

    # assume compiled from here
    
    my $dirtydir = "$*SPEC.curdir()/temp";
    $dirtydir.IO.mkdir;

    # build the program
    my $build-product = "$dirtydir/aoc-main";
    run $COMPILER, %OPTIONS.kv.grep(* !~~ Bool), $OUTPUT-SPECIFIER, $build-product, $solution;

    run-aoc-program $input, $build-product;

    LEAVE try $dirtydir.IO.&rm;
}


#| This function recursively removes a directory and all of its children,
#| much like the beloved C<rm -r> program.
sub rm(IO::Path $dir) {
    return True unless $dir.e;

    fail "$dir must be a directory" unless $dir.d;

    for eager $dir.dir -> $entry {
        if !$entry.l && $entry.d {
            $entry.&rm;
        } else {
            $entry.unlink;
        }
    }

    $dir.rmdir
}


sub run-aoc-program(Str $input, Str $program) {
    my $in = $input.IO.open:r orelse .fail;

    run $program, :$in;

    LEAVE try $in.close;
}
