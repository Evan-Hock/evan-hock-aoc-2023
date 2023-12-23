#!/usr/bin/env raku

sub MAIN($lang, $day, $part, :$ext? is copy) {
    # language-specific build options
    my ($COMPILER, %OPTIONS);

    # by default, extension is the same as language
    $ext //= $lang;

    # apply language config if it exists
    try EVALFILE "$*SPEC.curdir()/$lang/cfg.raku";

    my $solution-dir = "$*SPEC.curdir()/$lang/$day/$part";

    # get solution-specific config if it exists
    try EVALFILE "$solution-dir/cfg.raku";

    my $solution = $solution-dir.IO.dir(test => /.* ".$ext" $/).head; # fetch the solution for the given part
    my $input = "$*SPEC.curdir()/input/$day.txt"; # fetch the input for the given part

    # assume interpreted language if no compiler is specified by config
    without $COMPILER {
        run-aoc-program $input, $solution;
        return;
    }

    # create a temporary directory to house the build products
    my $dirtydir = "$*SPEC.curdir()/temp";
    $dirtydir.IO.mkdir;

    # build the program
    my $build-product = "$dirtydir/aoc-main";
    run $COMPILER, %OPTIONS.kv.grep(* !~~ Bool), '-o', $build-product, $solution;

    # run the build product
    run-aoc-program $input, $build-product;

    LEAVE try $dirtydir.IO.&rm;
}


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
    my $input-fh = $input.IO.open:r orelse .fail;

    run $program, :in($input-fh);

    LEAVE try $input-fh.close;
}
